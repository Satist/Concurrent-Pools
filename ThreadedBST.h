//
// Created by johnl on 24/11/20.
//
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>


#ifndef HY486PROJ1_THREADEDBST_H
#define HY486PROJ1_THREADEDBST_H



struct treeNode {
    int postID;
    struct treeNode *lc;
    struct treeNode *rc;
    bool IsRightThreaded;
    bool IsLeftThreaded;
    pthread_mutex_t lock;
};

void tree_init(void );

void bst_insert(int postid);

extern struct treeNode *root;

void count_total_tree_size(int check );

bool bst_search(int postid);

void bst_delete(int postid);


void displayTree();
#endif //HY486PROJ1_THREADEDBST_H
