//
// Created by johnl on 22/11/20.
//
#include "Queue.h"

struct queue **categories;

void queue_init(int N,int M){
    categories=malloc(N*sizeof(struct queue));
    for (int i = 0; i < M; ++i) {
        categories[i % (N / 4)]=(struct queue*)malloc(sizeof(struct queue));
        categories[i % (N / 4)]->Tail=(struct queueNode*)malloc(sizeof(struct queueNode));
        categories[i % (N / 4)]->Head=(struct queueNode*)malloc(sizeof(struct queueNode));
        categories[i % (N / 4)]->Tail=categories[i % (N / 4)]->Head;
        categories[i % (N / 4)]->Head=categories[i % (N / 4)]->Tail;
    }
}

void enqueue(int postid,int j,int N) {
    int pos;
    pos=j % (N / 4);
    struct queueNode* node=(struct queueNode*)malloc(sizeof(struct queueNode));
    node->postID=postid;
    node->next=NULL;
    pthread_mutex_lock(&categories[pos]->tailLock);
    categories[pos]->Tail->next=node;
    categories[pos]->Tail=node;
    pthread_mutex_unlock(&categories[pos]->tailLock);
}

int dequeue(int j,int N){
    int result;
    struct queue *curr=categories[j%(N/4)];
    pthread_mutex_lock(&curr->headLock);
    if (curr->Head->next==NULL) result=-1;
    else{
        result=curr->Head->next->postID;
        curr->Head=curr->Head->next;
    }
    pthread_mutex_unlock(&curr->headLock);
    return result;
}


void count_total_queue_size(int check,int N) {
    struct queueNode* curr;
    for (int i = 0; i < N/4; ++i) {
        int count=0;
        curr = categories[i]->Head->next;
        while (curr != NULL) {
            count++;
            curr = curr->next;
        }
        if (count != check) {
            printf("Unexpected Queue Size:%d",count);
            exit(0);
        } else printf("Categories[%d] queue'stotal size counted(expected: %d, found: %d)\n", i, check, count);
    }

}



void count_total_queue_keysum(int check,int N) {
    struct queueNode* curr;
    int sum=0;
    for (int i = 0; i < N/4; ++i) {
        curr=categories[i]->Head->next;
        while (curr!=NULL){
            sum=sum+curr->postID;
            curr=curr->next;
        }
    }
    if (sum != check) {
        printf("Unexpected Keysum:%d",sum);
        exit(0);
    } else printf("total keysum check counted(expected: %d, found: %d)\n",check,sum);
}




