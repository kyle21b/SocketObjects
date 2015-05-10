//
//  main.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/10/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include <stdio.h>
#include "SocketObject.h"
#include "SocketObjectRuntime.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, const char * argv[]) {
    // Initialize the classes
    initialize_runtime();
    
    //Make an instance of a 'Counter'
    SocketObjectRef object = localReferenceToPort(argv[1]);

    while (1) {
        getchar();
        ArgValue retval = performSelector(object, "reset", voidArgValue);
    }
    
    deleteRef(object);
    
    return 0;
}
