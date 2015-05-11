//
//  SocketObject.h
//  SocketObjects
//
//  Created by Kyle Bailey on 4/10/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#ifndef __SocketObjects__SocketObject__
#define __SocketObjects__SocketObject__

#include <stdio.h>
#include "Util.h"

/////////////////////////////////////////////////////////////////////////////
//Struct representing an instance of an object
struct SocketObject;
typedef struct SocketObject SocketObject;

//Struct representing a reference to an object
struct SocketObjectRef;
typedef struct SocketObjectRef *SocketObjectRef;

/////////////////////////////////////////////////////////////////////////////
//Important type definitions!
struct Class;
typedef struct Class *Class;
typedef const char *Selector;
typedef ArgValue (*Method)(SocketObject *self, Selector selector, ArgValue arg);

/////////////////////////////////////////////////////////////////////////////
//Working with references
SocketObjectRef copyRef(SocketObjectRef ref);
void deleteRef(SocketObjectRef ref);
SocketObjectRef localReferenceToPort(int portnumber);

/////////////////////////////////////////////////////////////////////////////
//Allocate an object locally
SocketObjectRef alloc(Class class);

/////////////////////////////////////////////////////////////////////////////
//Perform a behavior
ArgValue performSelector(SocketObjectRef ref, Selector selector, ArgValue arg);

//Helpers
ArgValue valueForKey(SocketObjectRef ref, const char *key);
void setValueForKey(SocketObjectRef ref, const char *key, ArgValue value);

int intValueForKey(SocketObjectRef ref, const char *key);
void setIntValueForKey(SocketObjectRef ref, const char *key, int value);

#endif /* defined(__SocketObjects__SocketObject__) */
