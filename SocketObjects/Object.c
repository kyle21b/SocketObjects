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
    int retainCount = getIntPropertyValue(self, "retainCount") + 1;
    setIntPropertyValue(self, "retainCount", retainCount);
    printf("RetainCount: %d, Object Id# %d\n", retainCount, self->listenPort);
    return voidArgValue;
}

ArgValue release(SocketObject *self, Selector selector, ArgValue arg){
    int retainCount = getIntPropertyValue(self, "retainCount") - 1;
    setIntPropertyValue(self, "retainCount", retainCount);
    printf("RetainCount: %d, Object Id# %d\n", retainCount, self->listenPort);
    if (retainCount <= 0) {
        msg_invoke(self, "deinit", voidArgValue);
        deallocInstance(self);
    }
    return voidArgValue;
}

ArgValue init(SocketObject *self, Selector selector, ArgValue arg){
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
    registerProperty(baseClass, "retainCount");
    registerMethod(baseClass, "deinit", deinit);
    registerProperty(baseClass, "description");
}

