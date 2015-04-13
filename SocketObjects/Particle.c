//
//  Particle.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/11/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "Particle.h"
#include "SocketObjectRuntime.h"
#include <stdlib.h>

ArgValue stepFrame(SocketObject *self, Selector selector, ArgValue arg){
    ArgValue value = getPropertyValue(self, "count");
    
    if (value.value == NULL) {
        int *count = malloc(sizeof(int));
        *count = 0;
        value.value = count;
        value.size = sizeof(int);
    }
    
    int *count = value.value;
    *count = *count + 1;
    
    setPropertyValue(self, "count", value);
    
    return voidArgValue;
}

void Particle_init(){
    Class baseClass = getClassWithName("Object");
    
    Class particle = registerClass("Particle", baseClass);
    registerProperty(particle, "position");
    registerProperty(particle, "velocity");
    registerMethod(particle, "stepFrame",stepFrame);
}