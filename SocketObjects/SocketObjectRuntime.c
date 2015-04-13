//
//  SocketObjectRuntime.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/10/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "SocketObjectRuntime.h"
#include "SocketObject_Private.h"
#include "csapp.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>

#include "Classes.h"

/////////////////////////////////////////////////////////////////////////////
struct Class {
    const char *name;
    ListNode *propertyList;
    ListNode *methodList;
    Class superClass;
};

typedef struct InstanceProperty InstanceProperty;
struct InstanceProperty {
    Selector setter;
    Selector getter;
};

typedef struct InstanceMethod InstanceMethod;
struct InstanceMethod {
    Selector selector;
    Method method;
};

in_port_t portNumber = 9001;
in_port_t nextPortNumber(){
    if (portNumber >= 10000) portNumber = 9001;
    return portNumber++;
}

static ListNode *classList = NULL;

const char *getClassName(Class class){
    return class->name;
}

Selector setterForGetter(Selector getter){
    char *setter = heapstrcat("set", getter);
    setter[3] = toupper(setter[3]);
    return setter;
}

void msg_listen(SocketObject *obj);
void *_msg_listen(void *arg);

void msg_recieve(SocketObject *obj);

InstanceProperty *getPropertyWithSelector(Class class, Selector selector);

/////////////////////////////////////////////////////////////////////////////

void initialize_runtime(){
    Classes_init();
}

/////////////////////////////////////////////////////////////////////////////
SocketObject *allocInstance(Class class) {
    SocketObject *object = malloc(sizeof(struct SocketObject));
    object->class = class;
    object->listenPort = nextPortNumber();
    object->retainCount = 0;
    
    msg_invoke(object, "init", voidArgValue);
    msg_listen(object);
    
    return object;
}

void deallocInstance(SocketObject *self){
    Pthread_cancel(self->listenThread);
    Close(self->sockfd);
    free(self);
}

/////////////////////////////////////////////////////////////////////////////
void *_msg_listen(void *arg) {
    SocketObject *obj = arg;
    
    int	sockfd;
    struct sockaddr_in servaddr;
    
    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(obj->listenPort);
    
    Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));
    
    obj->sockfd = sockfd;
    
    while (1) {
        msg_recieve(obj);
    }
    return NULL;
}

void msg_listen(SocketObject *obj) {
    pthread_t tid;
    Pthread_create(&tid, NULL, _msg_listen, obj);
    obj->listenThread = tid;
}

ArgValue msg_send(SocketObjectRef ref, Selector selector, ArgValue arg){
    char message[MAXLINE];
    sprintf(message, "%s %zu", selector, arg.size);
    
    size_t length = strlen(message);
    
    if (arg.size > 0) {
        message[length] = ' ';
        bcopy(arg.value, &message[length+1], arg.size);
        length += arg.size + 1;
    }
    
    sendto(ref->sockfd, message, length, 0, (SA *)ref->to, ref->tolen);
    
    char response[MAXLINE];

    recvfrom(ref->sockfd, response, MAXLINE, 0, (SA *)ref->to, &ref->tolen);
    int argsize;
    sscanf(response, "%d", &argsize);
    
    char *value = NULL;
    
    if (argsize > 0) {
        value = malloc(argsize);
        bcopy(strstr(response, " ")+sizeof(char), value, argsize);
    }
    
    ArgValue returnValue = {value, argsize};
    return returnValue;
}

void msg_recieve(SocketObject *obj) {
    char message[MAXLINE];
    char selector[MAXLINE];
    
    bzero(message, MAXLINE);
    
    struct sockaddr_in clientAddr;
    socklen_t clientlen = sizeof(clientAddr);
    
    recvfrom(obj->sockfd, message, MAXLINE, 0, (SA *)&clientAddr, &clientlen);

    int argsize;
    sscanf(message, "%s %d", selector, &argsize);
    
    char *value = NULL;

    if (argsize > 0) {
        value = malloc(argsize);
        bcopy(message, value, argsize);
    }
    
    ArgValue arg = {value, argsize};

    ArgValue result = msg_invoke(obj, selector, arg);
    
    if (value != NULL) free(value);

    char response[MAXLINE];
    sprintf(response, "%zu", result.size);
    size_t length = strlen(response);

    if (result.size > 0) {
        response[length] = ' ';
        bcopy(result.value, &response[length+1], result.size);
        length += result.size + 1;
    }
    
    sendto(obj->sockfd, response, length, 0, (SA *)&clientAddr, clientlen);
  
}

ArgValue msg_invoke(SocketObject *self, Selector selector, ArgValue arg){
    Method method = getMethodWithSelector(self->class, selector);
    if (method != NULL) return method(self,selector,arg);
    return voidArgValue;
}

ArgValue msg_invoke_super(SocketObject *self, Selector selector, ArgValue arg){
    Method method = getMethodWithSelector(self->class->superClass, selector);
    if (method != NULL) return method(self,selector,arg);
    return voidArgValue;
}

/////////////////////////////////////////////////////////////////////////////
ArgValue _dynamicPropertySet(SocketObject *self, Selector selector, ArgValue arg){
    InstanceProperty *property = getPropertyWithSelector(self->class, selector);
    if (property == NULL) return voidArgValue;
    setPropertyValue(self, property->getter, arg);
    return voidArgValue;
}

ArgValue _dynamicPropertyGet(SocketObject *self, Selector selector, ArgValue arg){
    InstanceProperty *property = getPropertyWithSelector(self->class, selector);
    if (property == NULL) return voidArgValue;
    return getPropertyValue(self, property->getter);
    return voidArgValue;
}

ObjectField *fieldWithName(SocketObject *self, const char *fieldName) {
    ListNode *node = self->fields;
    while (node != NULL) {
        ObjectField *field = node->value;
        if (!strcmp(field->name, fieldName)) break;
        node = node->next;
    }
    ObjectField *field = node != NULL ? node->value : NULL;
    return field;
}

void setPropertyValue(SocketObject *self, const char *fieldName, ArgValue value) {
    ObjectField *field = fieldWithName(self, fieldName);

    if (field == NULL) {
        field = malloc(sizeof(struct ObjectField));
        field->name = fieldName;
        field->value = value;
        self->fields = listNode(field, self->fields);
    } else {
        field->value = value;
    }
}

ArgValue getPropertyValue(SocketObject *self, const char *fieldName) {
    ObjectField *field = fieldWithName(self, fieldName);
    if (field == NULL) return voidArgValue;
    return field->value;
}

/////////////////////////////////////////////////////////////////////////////

Class getClassWithName(const char *name){
    ListNode *node = classList;
    while (node != NULL) {
        Class class = node->value;
        if (!strcmp(getClassName(class), name)) return class;
        node = node->next;
    }
    return NULL;
}

Class registerClass(const char *name, Class superClass){
    Class class = malloc(sizeof(struct Class));
    class->name = heapstrcpy(name);
    class->superClass = superClass;

    classList = listNode(class, classList);
    return class;
}

BOOL registerProperty(Class class, Selector selector){
    InstanceProperty *instanceProperty = malloc(sizeof(struct InstanceProperty));
    instanceProperty->getter = heapstrcpy(selector);
    instanceProperty->setter = setterForGetter(selector);
    
    registerMethod(class, instanceProperty->getter, _dynamicPropertyGet);
    registerMethod(class, instanceProperty->setter, _dynamicPropertySet);

    class->propertyList = listNode(instanceProperty, class->propertyList);
    return YES;
}

BOOL registerMethod(Class class, Selector selector, Method method){
    InstanceMethod *instanceMethod = malloc(sizeof(struct InstanceMethod));
    instanceMethod->selector = heapstrcpy(selector);
    instanceMethod->method = method;
    
    class->methodList = listNode(instanceMethod, class->methodList);
    return YES;
}

Method getMethodWithSelector(Class class, Selector selector){
    ListNode *node = class->methodList;
    while (node != NULL) {
        InstanceMethod *instanceMethod = node->value;
        if (!strcmp(instanceMethod->selector, selector)) return instanceMethod->method;
        node = node->next;
    }
    if (class->superClass == NULL) return NULL;
    return getMethodWithSelector(class->superClass, selector);
}

InstanceProperty *getPropertyWithSelector(Class class, Selector selector){
    ListNode *node = class->propertyList;
    while (node != NULL) {
        InstanceProperty *instanceProperty = node->value;
        if (!strcmp(instanceProperty->setter, selector) || !strcmp(instanceProperty->getter, selector)) return instanceProperty;
        node = node->next;
    }
    if (class->superClass == NULL) return NULL;
    return getPropertyWithSelector(class->superClass, selector);
}

