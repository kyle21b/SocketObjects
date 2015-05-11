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
#include "Util.h"

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

in_port_t portNumber = 9000;
in_port_t nextPortNumber(){
    if (portNumber >= 10000) portNumber = 9000;
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
//Make the instance of the class, and initialize it
SocketObject *allocInstance(Class class) {
    SocketObject *object = malloc(sizeof(struct SocketObject));
    object->class = class;
    
    msg_invoke(object, "init", voidArgValue);
    msg_listen(object);
    
    return object;
}

void *_deallocInstance(void *arg){
    SocketObject *obj = arg;

    Pthread_cancel(obj->listenThread);
    Close(obj->sockfd);
    free(obj);
    
    return NULL;
}

//Deallocate the instance after a short delay to ensure the object is done communicating first
void deallocInstance(SocketObject *self){
    pthread_t tid;
    Pthread_create(&tid, NULL, _deallocInstance, self);
}

/////////////////////////////////////////////////////////////////////////////
//Listen loop while the object waits for a message
void *_msg_listen(void *arg) {
    SocketObject *obj = arg;
    
    printf("New %s with Object Id# %d\n",getClassName(obj->class),obj->listenPort);

    while (1) {
        msg_recieve(obj);
    }
    return NULL;
}

//Prepares the object to listen and then spawns the new thread that will wait for messages to arrive
void msg_listen(SocketObject *obj) {
    pthread_t tid;
    
    int	sockfd;
    struct sockaddr_in servaddr;
    
    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    
    int listenPort = nextPortNumber();
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(listenPort);
    
    //Search for an open port
    while (bind(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
        listenPort = nextPortNumber();
        servaddr.sin_port = htons(listenPort);
    }
    
    obj->listenPort = listenPort;
    obj->sockfd = sockfd;
    
    //Spawn the listening thread
    Pthread_create(&tid, NULL, _msg_listen, obj);
    obj->listenThread = tid;
}

//Send a message over the network
ArgValue msg_send(SocketObjectRef ref, Selector selector, ArgValue arg){
    //First construct the message in the buffer
    char message[MAXLINE];
    sprintf(message, "%s %zu", selector, arg.size);
    
    size_t length = strlen(message);
    
    if (arg.size > 0) {
        message[length] = ' ';
        bcopy(arg.value, &message[length+1], arg.size);
        length += arg.size + 1;
    }
    
    //Send it
    sendto(ref->sockfd, message, length, 0, (SA *)ref->to, ref->tolen);
    
    char response[MAXLINE];
    
    //Read the return value
    recvfrom(ref->sockfd, response, MAXLINE, 0, (SA *)ref->to, &ref->tolen);
    int argsize;
    sscanf(response, "%d", &argsize);
    
    char *value = NULL;
    
    //If there is an arg, move it to malloced memory else we will return a reference to something on the stack (classic c bug)
    if (argsize > 0) {
        value = malloc(argsize);
        bcopy(strstr(response, " ")+sizeof(char), value, argsize);
    }
    
    ArgValue returnValue = {value, argsize};
    return returnValue;
}

//Recieve a message
void msg_recieve(SocketObject *obj) {
    char message[MAXLINE];
    char selector[MAXLINE];
    
    bzero(message, MAXLINE);
    
    struct sockaddr_in clientAddr;
    socklen_t clientlen = sizeof(clientAddr);
    
    //Recieve it
    recvfrom(obj->sockfd, message, MAXLINE, 0, (SA *)&clientAddr, &clientlen);

    //Find out the selector and the arguments
    int argsize;
    sscanf(message, "%s %d", selector, &argsize);
    
    char *value = NULL;

    if (argsize > 0) {
        value = strstr(message+sizeof(char)*(strlen(selector)+1), " ")+sizeof(char);
    }
    
    ArgValue arg = {value, argsize};

    //Invoke the proper function
    ArgValue result = msg_invoke(obj, selector, arg);
    
    char response[MAXLINE];
    sprintf(response, "%zu", result.size);
    size_t length = strlen(response);

    if (result.size > 0) {
        response[length] = ' ';
        bcopy(result.value, &response[length+1], result.size);
        length += result.size + 1;
    }
    
    //Send back the return value
    sendto(obj->sockfd, response, length, 0, (SA *)&clientAddr, clientlen);
  
}

//Look up the proper c function pointer, and invoke the method
ArgValue msg_invoke(SocketObject *self, Selector selector, ArgValue arg){
    Method method = getMethodWithSelector(self->class, selector);
    if (method != NULL) return method(self,selector,arg);
    return voidArgValue;
}

//Invoke the method after swizzling the objects class
ArgValue msg_invoke_super(SocketObject *self, Selector selector, ArgValue arg){
    Class class = self->class;
    self->class = self->class->superClass;
    ArgValue returnValue = msg_invoke(self, selector, arg);
    self->class = class;
    return returnValue;
}

/////////////////////////////////////////////////////////////////////////////
//Sets the argument into the objects field specified by the selector
ArgValue _dynamicPropertySet(SocketObject *self, Selector selector, ArgValue arg){
    InstanceProperty *property = getPropertyWithSelector(self->class, selector);
    if (property == NULL) return voidArgValue;
    setPropertyValue(self, property->getter, arg);
    return voidArgValue;
}

//Gets the argument from the objects field specified by the selector
ArgValue _dynamicPropertyGet(SocketObject *self, Selector selector, ArgValue arg){
    InstanceProperty *property = getPropertyWithSelector(self->class, selector);
    if (property == NULL) return voidArgValue;
    return getPropertyValue(self, property->getter);
}

//Returns the field on an object with a specific name
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

//Sets a fields value
void setPropertyValue(SocketObject *self, const char *fieldName, ArgValue value) {
    ObjectField *field = fieldWithName(self, fieldName);
    ArgValue valueCopy = {heapcpy(value.value, value.size), value.size};

    if (field == NULL) {
        field = malloc(sizeof(struct ObjectField));
        field->name = fieldName;
        field->value = valueCopy;
        self->fields = listNode(field, self->fields);
    } else {
        free(field->value.value);
        field->value = valueCopy;
    }
}

//Gets a fields value
ArgValue getPropertyValue(SocketObject *self, const char *fieldName) {
    ObjectField *field = fieldWithName(self, fieldName);
    if (field == NULL) return voidArgValue;
    return field->value;
}

//Helper functions
void _setPropertyValue(SocketObject *self, const char *fieldName, void *value, size_t size){
    ArgValue argValue = {value, size};
    setPropertyValue(self, fieldName, argValue);
}

void * _getPropertyValue(SocketObject *self, const char *fieldName, size_t size){
    ArgValue argValue = getPropertyValue(self, fieldName);
    if (argValue.size == size) return argValue.value;
    return NULL;
}

void setIntPropertyValue(SocketObject *self, const char *fieldName, int value){
    _setPropertyValue(self, fieldName, &value, sizeof(int));
}
int getIntPropertyValue(SocketObject *self, const char *fieldName){
    int *valuePointer = _getPropertyValue(self, fieldName, sizeof(int));
    if (valuePointer == NULL) return 0;
    return *valuePointer;
}

void setLongPropertyValue(SocketObject *self, const char *fieldName, long value){
    _setPropertyValue(self, fieldName, &value, sizeof(long));
}
long getLongPropertyValue(SocketObject *self, const char *fieldName){
    long *valuePointer = _getPropertyValue(self, fieldName, sizeof(long));
    if (valuePointer == NULL) return 0;
    return *valuePointer;
}

/////////////////////////////////////////////////////////////////////////////
//Find a class in the list with the specified name
Class getClassWithName(const char *name){
    ListNode *node = classList;
    while (node != NULL) {
        Class class = node->value;
        if (!strcmp(getClassName(class), name)) return class;
        node = node->next;
    }
    return NULL;
}

//Register a class with a specified superclass
Class registerClass(const char *name, Class superClass){
    Class class = malloc(sizeof(struct Class));
    class->name = heapstrcpy(name);
    class->superClass = superClass;

    classList = listNode(class, classList);
    return class;
}

//Register a property to a class
BOOL registerProperty(Class class, Selector selector){
    InstanceProperty *instanceProperty = malloc(sizeof(struct InstanceProperty));
    instanceProperty->getter = heapstrcpy(selector);
    instanceProperty->setter = setterForGetter(selector);
    
    registerMethod(class, instanceProperty->getter, _dynamicPropertyGet);
    registerMethod(class, instanceProperty->setter, _dynamicPropertySet);

    class->propertyList = listNode(instanceProperty, class->propertyList);
    return YES;
}

//Register a method to a class
BOOL registerMethod(Class class, Selector selector, Method method){
    InstanceMethod *instanceMethod = malloc(sizeof(struct InstanceMethod));
    instanceMethod->selector = heapstrcpy(selector);
    instanceMethod->method = method;
    
    class->methodList = listNode(instanceMethod, class->methodList);
    return YES;
}

//Get the method on a specified class refered to by the selector
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

//Returns the property field for a given selector, for easily checking the names of the set and get functions
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

