//
// Created by johnl on 20/11/20.
//
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#ifndef HY486PROJ1_SINGLYLISTEDLIST_H
#define HY486PROJ1_SINGLYLISTEDLIST_H


struct LLNode {
        int postID;
        bool marked;
        pthread_mutex_t lock;
        struct LLNode *next;
};

struct SinglyLinkedList {
    struct LLNode *head;
    struct LLNode *tail;
};

extern struct SinglyLinkedList shared;

void list_init(void );

bool validate(struct LLNode* pred,struct LLNode* curr);

bool LL_insert(int postid);

bool LL_delete(int postid);

bool search(int postid);

void count_total_list_size(int check );

void count_total_keysum(int check );

#endif //HY486PROJ1_SINGLYLISTEDLIST_H
