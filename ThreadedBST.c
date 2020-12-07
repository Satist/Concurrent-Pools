//
// Created by johnl on 24/11/20.
//

#include "ThreadedBST.h"

struct treeNode *root;

void tree_init(void ) {
    root = (struct treeNode *) malloc(sizeof(struct treeNode));
    root->postID=999999999;
    root->IsLeftThreaded=true;
    root->rc=root->lc=root;
}


void bst_insert(int postid) {
    pthread_mutex_lock(&root->lock);
    struct treeNode *p = root;
    for (;;) {
        if (p->postID < postid) { //move to right thread
            if (p->IsRightThreaded)
                break;
            pthread_mutex_unlock(&p->lock);
            p = p->rc;
            pthread_mutex_lock(&p->lock);
        } else if (p->postID > postid) { // move to left thread
            if (p->IsLeftThreaded)
                break;
            pthread_mutex_unlock(&p->lock);
            p = p->lc;
            pthread_mutex_lock(&p->lock);
        } else {
            pthread_mutex_unlock(&p->lock);
            return;
        }
    }
    struct treeNode *temp = (struct treeNode *) malloc(sizeof(struct treeNode));
    temp->postID = postid;
    temp->IsRightThreaded= temp->IsLeftThreaded= true;
    if (p->postID < postid) {
        temp->rc = p->rc;
        temp->lc= p;
        p->rc = temp;
        p->IsRightThreaded= false;
    } else {
        temp->rc = p;
        temp->lc = p->lc;
        p->lc = temp;
        p->IsLeftThreaded = false;
    }
    pthread_mutex_unlock(&p->lock);
}

void bst_delete(int postid) {
    pthread_mutex_lock(& root->lock);
    struct treeNode *dest = root->lc, *p = root;
    pthread_mutex_lock(&dest->lock);
    for (;;) { //find Node and its parent.
        if (dest->postID < postid) {
            if (dest->IsRightThreaded) {
                pthread_mutex_unlock(&dest->lock);
                pthread_mutex_unlock(&p->lock);
                return;
            }
            pthread_mutex_unlock(&p->lock);
            p = dest;
            dest = dest->rc;
            pthread_mutex_lock(&dest->lock);
        } else if (dest->postID > postid) {
            if (dest->IsLeftThreaded) {
                pthread_mutex_unlock(&dest->lock);
                pthread_mutex_unlock(&p->lock);
                return;
            }
            pthread_mutex_unlock(&p->lock);
            p = dest;
            dest = dest->lc;
            pthread_mutex_lock(&dest->lock);
        } else {
            break;
        }
    }
    struct treeNode *target = dest;
    if (!dest->IsRightThreaded && !dest->IsLeftThreaded) {
        pthread_mutex_unlock(&p->lock);
        p = dest;  //has two children
        target = dest->lc;   //largest node at left child
        pthread_mutex_lock(&target->lock);
        while (!target->IsRightThreaded) {
            if (dest!=p)
                pthread_mutex_unlock(&p->lock);
            p = target;
            target = target->rc;
            pthread_mutex_lock(&target->lock);
        }
        dest->postID= target->postID; //replace mode
        if (dest!=p)
            pthread_mutex_unlock(&dest->lock);
    }
    if (p->postID >= target->postID) { //only left child
        if (target->IsRightThreaded && target->IsLeftThreaded) {
            p->lc = target->lc;
            p->IsLeftThreaded = true;
        } else if (target->IsRightThreaded) {
            struct treeNode* largest = target->lc;
            pthread_mutex_lock(&largest->lock);
            while (!largest->IsRightThreaded) {
                pthread_mutex_unlock(&largest->lock);
                largest = largest->rc;
                pthread_mutex_lock(&largest->lock);
            }
            largest->rc = p;
            p->lc= target->lc;
            pthread_mutex_unlock(&largest->lock);
        } else {
            struct treeNode *smallest = target->rc;
            pthread_mutex_lock(&smallest->lock);
            while (!smallest->IsLeftThreaded) {
                pthread_mutex_unlock(&smallest->lock);
                smallest = smallest->lc;
                pthread_mutex_lock(&smallest->lock);
            }
            smallest->lc = target->lc;
            p->lc = target->rc;
            pthread_mutex_unlock(&smallest->lock);
        }
    } else {//only right child
        if (target->IsRightThreaded && target->IsLeftThreaded) {
            p->rc= target->rc;
            p->IsRightThreaded = true;
        } else if (target->IsRightThreaded) {
            struct treeNode *largest = target->lc;
            pthread_mutex_lock(&largest->lock);
            while (!largest->IsRightThreaded) {
                pthread_mutex_unlock(&largest->lock);
                largest = largest->rc;
                pthread_mutex_lock(&largest->lock);
            }
            largest->rc= target->rc;
            p->rc = target->lc;
            pthread_mutex_unlock(&largest->lock);
        } else {
            struct treeNode *smallest = target->rc;
            pthread_mutex_lock(&smallest->lock);
            while (!smallest->IsLeftThreaded) {
                pthread_mutex_unlock(&smallest->lock);
                smallest = smallest->lc;
                pthread_mutex_lock(&smallest->lock);
            }
            smallest->lc= p;
            p->rc= target->rc;
            pthread_mutex_unlock(&smallest->lock);
        }
    }
    pthread_mutex_unlock(&dest->lock);
    pthread_mutex_unlock(&target->lock);
    pthread_mutex_unlock(&p->lock);
}

bool bst_search(int postid) {
    pthread_mutex_lock(&root->lock);
    struct treeNode *temp = root->lc;
    pthread_mutex_unlock(&root->lock);
    pthread_mutex_lock(&temp->lock);
    for (;;) {
        if (temp->postID < postid) { //search in left thread
            if (temp->IsRightThreaded) {
                pthread_mutex_unlock(&temp->lock);
                return false;
            }
            pthread_mutex_unlock(&temp->lock);
            temp = temp->rc;
            pthread_mutex_lock(&temp->lock);
        } else if (temp->postID > postid) { //search in right thread
            if (temp->IsLeftThreaded) {
                pthread_mutex_unlock(&temp->lock);
                return false;
            }
            pthread_mutex_unlock(&temp->lock);
            temp = temp->lc;
            pthread_mutex_lock(&temp->lock);
        } else {
            pthread_mutex_unlock(&temp->lock);
            return true;
        }
    }
}


void count_total_tree_size(int check)
{
    int count=0;
    struct treeNode *temp = root, *p;
    for (;;) {
        p = temp;
        temp = temp->rc;
        if (!p->IsRightThreaded) {
            while (!temp->IsLeftThreaded) {
                temp = temp->lc;
            }
        }
        if (temp == root)
            break;
       count++;
    }
    if (count != check) {
        printf("Unexpected Tree Size:%d",count);
        exit(0);
    } else printf("tree’s total size finished (expected: %d, found: %d)\n",check,count);
}

void displayTree() { //print the tree
    struct treeNode *temp = root, *p;
    for (;;) {
        p = temp;
        temp = temp->rc;
        if (!p->IsRightThreaded) {
            while (!temp->IsLeftThreaded) {
                temp = temp->lc;
            }
        }
        if (temp == root)
            break;
        printf("%d ",temp->postID);
    }
    printf("\n");
}
