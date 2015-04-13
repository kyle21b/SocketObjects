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

int main(int argc, const char * argv[]) {
    // Initialize the classes
    initialize_runtime();
    
    //Make an instance of a 'Counter'
    SocketObjectRef object = alloc(getClassWithName("Counter"));
    
    while (1) {
        //Tell the counter to increment its value
        performSelector(object, "increment", voidArgValue);
        //And ask it for the count
        ArgValue retval = performSelector(object, "count", voidArgValue);
       
        //Get the pointer to the value from the return structure
        long *valuePointer = retval.value;
        //Get the actual value
        long count = *valuePointer;
        //The return value of the message resides in malloced memory
        free(valuePointer);
        
        //Count off every thousand loops
        if ((count % 1000) == 0) printf("%ld\n",count);
    }
    
    return 0;
}
