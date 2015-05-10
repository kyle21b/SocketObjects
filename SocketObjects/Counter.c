//
//  Counter.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/11/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "Counter.h"
#include "SocketObject_Private.h"

ArgValue increment(SocketObject *self, Selector selector, ArgValue arg){
    long count = getLongPropertyValue(self, "count") + 1;
    setLongPropertyValue(self, "count", count);
    return getPropertyValue(self, "count");
}

ArgValue counter_init(SocketObject *self, Selector selector, ArgValue arg){
    msg_invoke_super(self, selector, arg);
    setLongPropertyValue(self, "count", 0);
    return voidArgValue;
}

void Counter_init(){
    Class baseClass = getClassWithName("Object");
    
    Class counter = registerClass("Counter", baseClass);
    registerMethod(counter, "init", counter_init);
    registerProperty(counter, "count");
    registerMethod(counter, "increment", increment);

}
