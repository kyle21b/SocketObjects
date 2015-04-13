//
//  SocketObject_Private.h
//  SocketObjects
//
//  Created by Kyle Bailey on 4/10/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#ifndef SocketObjects_SocketObject_Private_h
#define SocketObjects_SocketObject_Private_h

#include "csapp.h"
#include "SocketObjectRuntime.h"

typedef struct ObjectField ObjectField;
struct ObjectField {
    const char *name;
    ArgValue value;
};

struct SocketObject {
    Class class;
    
    ListNode *fields;

    int sockfd;
    
    in_port_t listenPort;
    pthread_t listenThread;
};

struct SocketObjectRef {
    struct sockaddr_in *to;
    socklen_t tolen;
    
    int sockfd;
};

#endif
