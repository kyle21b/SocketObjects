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

/////////////////////////////////////////////////////////////////////////////
ArgValue msg_send(SocketObjectRef ref, Selector selector, ArgValue arg);

/////////////////////////////////////////////////////////////////////////////
ArgValue msg_invoke(SocketObject *self, Selector selector, ArgValue arg);
ArgValue msg_invoke_super(SocketObject *self, Selector selector, ArgValue arg);

/////////////////////////////////////////////////////////////////////////////
void setPropertyValue(SocketObject *self, const char *fieldName, ArgValue value);
ArgValue getPropertyValue(SocketObject *self, const char *fieldName);

/////////////////////////////////////////////////////////////////////////////
void initialize_runtime();

Class registerClass(const char *name, Class superClass);
Class getClassWithName(const char *name);
const char *getClassName(Class);

BOOL registerProperty(Class, Selector);
BOOL hasPropertyWithSelector(Class, Selector);

BOOL registerMethod(Class, Selector, Method);
Method getMethodWithSelector(Class, Selector);

Selector setterForGetter(Selector getter);

SocketObject *allocInstance(Class);
void deallocInstance(SocketObject *);

in_port_t nextPortNumber();

#endif /* defined(__SocketObjects__SocketObjectRuntime__) */
