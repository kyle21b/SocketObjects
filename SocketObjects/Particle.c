//
//  Particle.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/11/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "Particle.h"
#include "SocketObject_Private.h"

ArgValue stepFrame(SocketObject *self, Selector selector, ArgValue arg){
    return voidArgValue;
}

void Particle_init(){
    Class baseClass = getClassWithName("Object");
    
    Class particle = registerClass("Particle", baseClass);
    registerProperty(particle, "position");
    registerProperty(particle, "velocity");
    registerMethod(particle, "stepFrame", stepFrame);
}