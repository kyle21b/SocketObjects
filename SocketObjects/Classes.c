//
//  Classes.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/11/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "Classes.h"

#include "Object.h"
#include "Counter.h"
#include "ResettableCounter.h"
#include "CircularCounter.h"

void Classes_init(){
    Object_init();
    Counter_init();
    Resettable_Counter_init();
    Circular_Counter_init();
}