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
    SocketObjectRef object = alloc(getClassWithName("ResettableCounter"));
    while (1) {
        //Tell the counter to increment its value
        ArgValue retval = performSelector(object, "increment", voidArgValue);

        //Get the actual value
        long count = *((long *)retval.value);
        //The return value of the message resides in malloced memory
        free(retval.value);

        //Count off every ten thousand loops
        if ((count % 10000) == 0) printf("%ld\n",count);
    }
    
    deleteRef(object);
    
    return 0;
}
