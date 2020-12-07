//
// Created by johnl on 22/11/20.
//
#include "SinglyListedList.h"

struct SinglyLinkedList shared;

void list_init(void ){
    shared.head = (struct LLNode *) malloc(sizeof(struct LLNode));
    shared.tail = (struct LLNode *) malloc(sizeof(struct LLNode));
    shared.head->next=shared.tail;

}

bool validate(struct LLNode* pred,struct LLNode* curr){
    if (pred->marked==0 && curr->marked==0 && pred->next==curr) return 1;
    else return 0;
}

bool LL_insert(int postid) {
    struct LLNode *pred, *curr;
    bool result;
    bool return_flag = 0;
    while (1) {
        pred = shared.head;
        curr = pred->next;
        while (curr->postID < postid && curr!=shared.tail) {
            pred = curr;
            curr = curr->next;
        }
        pthread_mutex_lock(&pred->lock);
        pthread_mutex_lock(&curr->lock);
        if (validate(pred, curr)) {
            if (postid == curr->postID) {
                result = 0;
                return_flag = 1;
            } else {
                struct LLNode *node = (struct LLNode *) malloc(sizeof(struct LLNode));
                node->next = curr;
                node->postID = postid;
                pred->next = node;
                result = 1;
                return_flag = 1;
            }
        }
        pthread_mutex_unlock(&pred->lock);
        pthread_mutex_unlock(&curr->lock);
        if (return_flag) return result;
    }
}

bool LL_delete(int postid){
    struct LLNode *pred, *curr;
    bool result;
    bool return_flag=0;
    while (1){
        pred=shared.head;
        curr=pred->next;
        while (curr->postID<postid){
            if (curr==shared.tail) break;
            pred=curr;
            curr=curr->next;
        }
        pthread_mutex_lock(&pred->lock);
        pthread_mutex_lock(&curr->lock);
        if (validate(pred,curr)){
            if (postid==curr->postID){
                curr->marked=1;
                pred->next=curr->next;
                result=1;
            } else result=0;
            return_flag=1;
        }
        pthread_mutex_unlock(&pred->lock);
        pthread_mutex_unlock(&curr->lock);
        if (return_flag==1)
            return result;
    }
}
bool search(int postid){
    struct LLNode* curr=shared.head;
    while (curr!=shared.tail){
        if (curr->postID==postid && curr->marked!=1){
            return true;
        }
        curr=curr->next;
    }
    return false;
}
void count_total_list_size(int check ){
    struct LLNode *curr;
    int count=0;
    curr=shared.head;
    while (curr!=shared.tail){
        if (curr->marked==0) {
            count++;
            curr=curr->next;
        }
    }
    if (count != check) {
        printf("Unexpected List Size:%d",count);
        exit(0);
    }
    else printf("total list size counted(expected: %d, found: %d)\n",check, count);
}

void count_total_keysum(int check){
    struct LLNode *curr;
    int sum=0;
    curr=shared.head;
    while (curr!=shared.tail){
        if (curr->marked==0) {
            sum=sum+curr->postID;
            curr=curr->next;
        }
    }
    if (sum != check) {
        printf("Unexpected List Keysum:%d",sum);
        exit(0);
    } else printf("total keysum counted (expected:%d, found: %d)\n", check, sum);
}



