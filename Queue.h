//
// Created by johnl on 20/11/20.
//
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#ifndef HY486PROJ1_QUEUE_H
#define HY486PROJ1_QUEUE_H



struct queue {
    struct queueNode *Head;
    struct queueNode *Tail;
    pthread_mutex_t headLock;
    pthread_mutex_t tailLock;
};


struct queueNode {
    int postID;
    struct queueNode *next;
};

extern struct queue **categories;

void queue_init(int N,int M);

void enqueue(int postid,int j,int N);

int dequeue(int j,int N);

void count_total_queue_size(int check,int N);


void count_total_queue_keysum(int check,int N);

#endif //HY486PROJ1_QUEUE_H
