//
//  Object.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/11/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "Object.h"
#include "SocketObject_Private.h"

ArgValue retain(SocketObject *self, Selector selector, ArgValue arg){
    self->retainCount++;
    return voidArgValue;
}

ArgValue release(SocketObject *self, Selector selector, ArgValue arg){
    self->retainCount--;
    if (self->retainCount <= 0) {
        msg_invoke(self, "deinit", voidArgValue);
        deallocInstance(self);
    }
    return voidArgValue;
}

ArgValue retainCount(SocketObject *self, Selector selector, ArgValue arg){
    ArgValue value = {&self->retainCount, sizeof(int)};
    return value;
}

ArgValue init(SocketObject *self, Selector selector, ArgValue arg){
    self->retainCount = 0;
    return voidArgValue;
}

ArgValue deinit(SocketObject *self, Selector selector, ArgValue arg){
    return voidArgValue;
}

void Object_init(){
    Class baseClass = registerClass("Object", NULL);
    registerMethod(baseClass, "init", init);
    registerMethod(baseClass, "retain", retain);
    registerMethod(baseClass, "release", release);
    registerMethod(baseClass, "retainCount", retainCount);
    registerMethod(baseClass, "deinit", deinit);
    registerProperty(baseClass, "description");
    
}

