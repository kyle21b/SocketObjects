//
//  SocketObjectRuntime.h
//  SocketObjects
//
//  Created by Kyle Bailey on 4/10/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#ifndef __SocketObjects__SocketObjectRuntime__
#define __SocketObjects__SocketObjectRuntime__

#include <stdio.h>
#include "SocketObject.h"
#include "Util.h"
#include <netinet/in.h>

/////////////////////////////////////////////////////////////////////////////
//The caller uses this
ArgValue msg_send(SocketObjectRef ref, Selector selector, ArgValue arg);

/////////////////////////////////////////////////////////////////////////////
//The reciever eventually calls through to this function
ArgValue msg_invoke(SocketObject *self, Selector selector, ArgValue arg);
ArgValue msg_invoke_super(SocketObject *self, Selector selector, ArgValue arg);

/////////////////////////////////////////////////////////////////////////////
//Access fields of an instance
void setPropertyValue(SocketObject *self, const char *fieldName, ArgValue value);
ArgValue getPropertyValue(SocketObject *self, const char *fieldName);

//Helpers
void setIntPropertyValue(SocketObject *self, const char *fieldName, int value);
int getIntPropertyValue(SocketObject *self, const char *fieldName);

void setLongPropertyValue(SocketObject *self, const char *fieldName, long value);
long getLongPropertyValue(SocketObject *self, const char *fieldName);

/////////////////////////////////////////////////////////////////////////////
//Main runtime functins
void initialize_runtime();

//Registering classes
Class registerClass(const char *name, Class superClass);
Class getClassWithName(const char *name);
const char *getClassName(Class);

//Registering properties
BOOL registerProperty(Class, Selector);
BOOL hasPropertyWithSelector(Class, Selector);

//Registering methods
BOOL registerMethod(Class, Selector, Method);
Method getMethodWithSelector(Class, Selector);

//Appends "set" to the front of the getter, and maintains proper camelCase
Selector setterForGetter(Selector getter);

//Allocate and deallocation
SocketObject *allocInstance(Class);
void deallocInstance(SocketObject *);

//System function to get port for a new object
in_port_t nextPortNumber();

#endif /* defined(__SocketObjects__SocketObjectRuntime__) */
