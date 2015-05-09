//
//  Counter.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/11/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "Counter.h"
#include "CircularCounter.h"
#include "SocketObject_Private.h"

ArgValue circular_increment(SocketObject *self, Selector selector, ArgValue arg){
    long count = getLongPropertyValue(self, "count") + 1;
    long max = getLongPropertyValue(self, "max");
    long realCount = count % max;
    setLongPropertyValue(self, "count", realCount);
    return getPropertyValue(self, "count");
}

ArgValue circular_counter_init(SocketObject *self, Selector selector, ArgValue arg){
    msg_invoke_super(self, selector, arg);
    setLongPropertyValue(self, "max", 100000);
    return voidArgValue;
}

void Circular_Counter_init(){
    Class baseClass = getClassWithName("Counter");
  
    Class counter = registerClass("CircularCounter", baseClass);
    registerMethod(counter, "init", circular_counter_init);
    registerMethod(counter, "increment", circular_increment);
    registerProperty(counter, "max");
}
