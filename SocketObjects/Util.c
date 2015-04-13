//
//  Util.c
//  SocketObjects
//
//  Created by Kyle Bailey on 4/10/15.
//  Copyright (c) 2015 Kyle Bailey. All rights reserved.
//

#include "Util.h"
#include <string.h>
#include <stdlib.h>

ListNode *listNode(void *value, ListNode *next){
    ListNode *node = malloc(sizeof(struct ListNode));
    node->value = value;
    node->next = next;
    return node;
}

void *heapcpy(void *value, size_t size){
    void *copy = malloc(size);
    bcopy(value, copy, size);
    return copy;
}

char *heapstrcpy(const char *str){
    char *copy = calloc(strlen(str)+1, sizeof(char));
  
    strcpy(copy, str);
  
    return copy;
}

char *heapstrcat(const char *str, const char *appendingStr){
    char *catstr = calloc(strlen(str)+strlen(appendingStr)+1, sizeof(char));
    
    strcpy(catstr, str);
    strcat(catstr, appendingStr);
    
    return catstr;
}