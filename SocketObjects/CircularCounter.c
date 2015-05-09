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

ArgValue increment(SocketObject *self, Selector selector, ArgValue arg){
    long count = getLongPropertyValue(self, "count") + 1;
    long max = getLongPropertyValue(self, "max");
    setLongPropertyValue(self, "count", count % max);
    return getPropertyValue(self, "count");
}

//<<<<<<< Updated upstream
ArgValue counter_init(SocketObject *self, Selector selector, ArgValue arg){
    msg_invoke_super(self, selector, arg);
    setLongPropertyValue(max, "max", 100000);
    return voidArgValue;
}

//=======
//>>>>>>> Stashed changes
void Circular_Counter_init(){
    Class baseClass = getClassWithName("Counter");
  
    Class counter = registerClass("CircularCounter", baseClass);
    registerMethod(counter, "increment", increment);
    registerProperty(max, "max");
}
