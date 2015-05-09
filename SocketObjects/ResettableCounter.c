//
//  Counter.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/11/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "Counter.h"
#include "CiruclarCounter.h"
#include "SocketObject_Private.h"

ArgValue reset(SocketObject *self, Selector selector, ArgValue arg){
    setLongPropertyValue(self, "count", 0);
    return getPropertyValue(self, "count");
}

//<<<<<<< Updated upstream
ArgValue counter_init(SocketObject *self, Selector selector, ArgValue arg){
    msg_invoke_super(self, selector, arg);
    return voidArgValue;
}

//=======
//>>>>>>> Stashed changes
void Resettable_Counter_init(){
    Class baseClass = getClassWithName("Counter");
  
    Class counter = registerClass("ResettableCounter", baseClass);
    registerMethod(counter, "reset", reset);
}
