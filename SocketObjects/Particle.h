//
//  Particle.h
//  SocketObjects
//
//  Created by Kyle Bailey on 4/11/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#ifndef __SocketObjects__Particle__
#define __SocketObjects__Particle__

#include <stdio.h>

void Particle_init();

typedef struct Vector Vector;
struct Vector {
    double x;
    double y;
};

#endif /* defined(__SocketObjects__Particle__) */
