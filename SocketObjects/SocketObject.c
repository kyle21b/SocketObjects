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

//Quickly make a local sockaddr_in struct with a given port number
struct sockaddr_in *localsockaddr(int port) {
    struct sockaddr_in *serveraddr = malloc(sizeof(struct sockaddr_in));
    bzero(serveraddr, sizeof(struct sockaddr_in));
    
    serveraddr->sin_family = AF_INET;
    serveraddr->sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/
    serveraddr->sin_port = htons(port);
    
    return serveraddr;
}

//Copy a reference, sending the retain message after completion
SocketObjectRef copyRef(SocketObjectRef ref){
    SocketObjectRef newRef = malloc(sizeof(struct SocketObjectRef));
    
    newRef->to = malloc(sizeof(struct sockaddr_in));
    bcopy(ref->to, newRef->to, ref->tolen);
    newRef->tolen = ref->tolen;
    
    int clientfd = Socket(AF_INET, SOCK_DGRAM, 0);
    newRef->sockfd = clientfd;
    
    performSelector(newRef, "retain", voidArgValue);
    
    return newRef;
}

//Delete a reference, sending the release message
void deleteRef(SocketObjectRef ref){
    performSelector(ref, "release", voidArgValue);
    
    Close(ref->sockfd);
    free(ref->to);
    free(ref);
}

//Returns a refrence to a local object on the specified port
SocketObjectRef localReferenceToPort(int portnumber){
    SocketObjectRef ref = malloc(sizeof(struct SocketObjectRef));
    
    int clientfd = Socket(AF_INET, SOCK_DGRAM, 0);
    ref->sockfd = clientfd;
    ref->to = localsockaddr(portnumber);
    ref->tolen = sizeof(struct sockaddr_in);
    
    performSelector(ref, "retain", voidArgValue);
    return ref;
}

//Allocates a new object locally, returning a reference
SocketObjectRef alloc(Class class){
    
    SocketObject* object = allocInstance(class);
    
    SocketObjectRef ref = malloc(sizeof(struct SocketObjectRef));

    int clientfd = Socket(AF_INET, SOCK_DGRAM, 0);
    ref->sockfd = clientfd;
    ref->to = localsockaddr(object->listenPort);
    ref->tolen = sizeof(struct sockaddr_in);
    
    performSelector(ref, "retain", voidArgValue);
    
    return ref;
}

//Perform a specified selector by sending a message over the network
ArgValue performSelector(SocketObjectRef ref, Selector selector, ArgValue arg){
    return msg_send(ref, selector, arg);
}

//Helper functions
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

