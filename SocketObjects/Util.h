//
//  Util.h
//  SocketObjects
//
//  Created by Kyle Bailey on 4/10/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#ifndef __SocketObjects__Util__
#define __SocketObjects__Util__

#include <stdio.h>

typedef struct ListNode ListNode;
struct ListNode {
    void *value;
    ListNode *next;
};

ListNode *listNode(void *value, ListNode *next);

typedef char BOOL;

#define YES ((char)1);
#define NO ((char)0);

typedef struct ArgValue ArgValue;
struct ArgValue {
    void *value;
    size_t size;
};
static const ArgValue voidArgValue = {NULL, 0};

char *heapstrcpy(const char *str);

char *heapstrcat(const char *str, const char *appendingStr);

#endif /* defined(__SocketObjects__Util__) */
