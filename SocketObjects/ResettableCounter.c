//
//  Counter.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/11/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "Counter.h"
#include "ResettableCounter.h"
#include "SocketObject_Private.h"

ArgValue reset(SocketObject *self, Selector selector, ArgValue arg){
    setLongPropertyValue(self, "count", 0);
    return getPropertyValue(self, "count");
}

void Resettable_Counter_init(){
    Class baseClass = getClassWithName("Counter");
  
    Class counter = registerClass("ResettableCounter", baseClass);
    registerMethod(counter, "reset", reset);
}
