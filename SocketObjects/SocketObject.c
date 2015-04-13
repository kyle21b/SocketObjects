//
//  SocketObject.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/10/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "SocketObject.h"
#include "SocketObjectRuntime.h"
#include "SocketObject_Private.h"
#include <stdlib.h>

typedef struct SocketObject SocketObject;

SocketObjectRef copyRef(SocketObjectRef ref){
    SocketObjectRef newRef = malloc(sizeof(struct SocketObjectRef));
 //   newRef->to;

    performSelector(ref, "retain", voidArgValue);

    return newRef;
}

void deleteRef(SocketObjectRef ref){
    performSelector(ref, "release", voidArgValue);
    free(ref);
}

struct sockaddr_in *localsockaddr(int port) {
    struct hostent *hp = gethostent();

    struct sockaddr_in *serveraddr = malloc(sizeof(struct sockaddr_in));
    bzero((char *)serveraddr, sizeof(serveraddr));
    serveraddr->sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0],
          (char *)&serveraddr->sin_addr.s_addr, hp->h_length);
    serveraddr->sin_port = htons(port);
    
    return serveraddr;
}

SocketObjectRef alloc(Class class){
    
    SocketObject* object = allocClass(class);
    
    SocketObjectRef ref = malloc(sizeof(struct SocketObjectRef));

    int clientfd = Socket(AF_INET, SOCK_DGRAM, 0);
    ref->sockfd = clientfd;
    ref->to = localsockaddr(object->listenPort);
    ref->tolen = sizeof(struct sockaddr_in);
    
    performSelector(ref, "retain", voidArgValue);
    
    return ref;
}

/*
Class getClass(SocketObjectRef ref){
    
}

BOOL respondsToSelector(SocketObjectRef ref, Selector selector){
    
}*/

ArgValue performSelector(SocketObjectRef ref, Selector selector, ArgValue arg){
    return msg_send(ref, selector, arg);
}

ArgValue valueForKey(SocketObjectRef ref, const char *key){
    return performSelector(ref, key, voidArgValue);
}

void setValueForKey(SocketObjectRef ref, const char *key, ArgValue arg){
    Selector setter = setterForGetter(key);
    performSelector(ref, setter, arg);
    free((void *)setter);
}

int intValueForKey(SocketObjectRef ref, const char *key){
    ArgValue returnValue = valueForKey(ref, key);
    if (returnValue.value == NULL) return 0;
    
    int value = *((int *)returnValue.value);
    free(returnValue.value);
    
    return value;
}

void setIntValueForKey(SocketObjectRef ref, const char *key, int value){
    ArgValue arg = {&value, sizeof(value)};
    setValueForKey(ref, key, arg);
}

