//
//  Counter.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/11/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "Counter.h"
#include "SocketObjectRuntime.h"
#include <stdlib.h>

ArgValue counter_init(SocketObject *self, Selector selector, ArgValue arg){
    msg_invoke_super(self, selector, arg);
    long *count = malloc(sizeof(int));
    *count = 0;
    
    ArgValue counterValue = {count, sizeof(long)};
    setPropertyValue(self, "count", counterValue);
    
    return voidArgValue;
}

ArgValue increment(SocketObject *self, Selector selector, ArgValue arg){
    ArgValue value = getPropertyValue(self, "count");
  
    long *count = value.value;
    *count = *count + 1;
    
    setPropertyValue(self, "count", value);
    
    return voidArgValue;
}

void Counter_init(){
    Class baseClass = getClassWithName("Object");
    
    Class counter = registerClass("Counter", baseClass);
    registerMethod(counter, "init", counter_init);
    registerProperty(counter, "count");
    registerMethod(counter, "increment", increment);

}
