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
struct SocketObject;
typedef struct SocketObject SocketObject;

struct SocketObjectRef;
typedef struct SocketObjectRef *SocketObjectRef;

/////////////////////////////////////////////////////////////////////////////
struct Class;
typedef struct Class *Class;
typedef const char *Selector;
typedef ArgValue (*Method)(SocketObject *self, Selector selector, ArgValue arg);

/////////////////////////////////////////////////////////////////////////////
SocketObjectRef copyRef(SocketObjectRef ref);
void deleteRef(SocketObjectRef ref);
SocketObjectRef localReferenceToPort(int portnumber);

/////////////////////////////////////////////////////////////////////////////
SocketObjectRef alloc(Class class);

/////////////////////////////////////////////////////////////////////////////
ArgValue performSelector(SocketObjectRef ref, Selector selector, ArgValue arg);

ArgValue valueForKey(SocketObjectRef ref, const char *key);
void setValueForKey(SocketObjectRef ref, const char *key, ArgValue value);

int intValueForKey(SocketObjectRef ref, const char *key);
void setIntValueForKey(SocketObjectRef ref, const char *key, int value);

#endif /* defined(__SocketObjects__SocketObject__) */
