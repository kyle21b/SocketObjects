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

//This is the linked list node used in various places
typedef struct ListNode ListNode;
struct ListNode {
    void *value;
    ListNode *next;
};

//A convenience function to malloc a node and fill out the fields
ListNode *listNode(void *value, ListNode *next);

//Implementation of boolean type
typedef char BOOL;
#define YES ((char)1);
#define NO ((char)0);

//Generic argument struct
typedef struct ArgValue ArgValue;
struct ArgValue {
    void *value;
    size_t size;
};
//Void argument
static const ArgValue voidArgValue = {NULL, 0};

//Helper functions that return malloced pointers
void *heapcpy(void *, size_t);
char *heapstrcpy(const char *str);

char *heapstrcat(const char *str, const char *appendingStr);

#endif /* defined(__SocketObjects__Util__) */
