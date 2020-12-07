
#include "SinglyListedList.h"
#include "Queue.h"
#include "ThreadedBST.h"
#include <unistd.h>

pthread_barrier_t   barrier_1st_phase_end;
pthread_barrier_t   barrier_2nd_phase_start;
pthread_barrier_t   barrier_2nd_phase_end;
pthread_barrier_t   barrier_3rd_phase_start;
pthread_barrier_t   barrier_3rd_phase_end;
pthread_barrier_t   barrier_4th_phase_start;
pthread_barrier_t   barrier_4th_phase_end;


void* create_thread(void* ptr) {
    int *j = (int *) ptr;
    for (int i = 0; i < N; ++i) {
        LL_insert((*j) + (i * M));
    }
    pthread_barrier_wait(&barrier_1st_phase_end);
    if (*j == 0) {
        count_total_list_size( 2 * pow(N, 2));
        count_total_keysum((2 * pow(N, 4) - pow(N, 2)));
    }
    pthread_barrier_wait(&barrier_2nd_phase_start);
    if (*j < N) {
        for (int i = 0; i < N; ++i) {
            if (search((*j) + (i * M))) {
                LL_delete((*j) + (i * M));
                enqueue((*j) + (i * M), *j,N);
            } else printf("list postid not found\n");

            if (search((*j) + (i * M) + N)) {
                LL_delete((*j) + (i * M) + N);
                enqueue((*j) + (i * M) + N, *j,N);
            }else printf("list postid not found\n");
        }
    }
    pthread_barrier_wait(&barrier_2nd_phase_end);
    if (*j == 0) {
        count_total_queue_size(8*N,N);
        count_total_queue_keysum((2 * pow(N, 4) - pow(N, 2)),N);
        count_total_list_size(1);
    }
    pthread_barrier_wait(&barrier_3rd_phase_start);
    if (*j >= N && *j <=2 * N - 1) {
        for (int i = 0; i < N; ++i) {
            int t = dequeue(*j,N);
            if (t==-1)printf("queue not found\n");
            bst_insert(t);
        }
    }
    pthread_barrier_wait(&barrier_3rd_phase_end);
    if (*j == N) {
        count_total_tree_size(N*N);
        count_total_queue_size(4*N,N);
    }
    pthread_barrier_wait(&barrier_4th_phase_start);
    if (*j<N){
        for (int i = 0; i < N; ++i) {
            if (bst_search((*j) + (i * M))) {
                enqueue((*j) + (i * M),*j,N);
                bst_delete((*j) + (i * M));
            }

            if (bst_search((*j) + (i * M) + N)) {
                enqueue((*j) + (i * M) + N,*j,N);
                bst_delete((*j) + (i * M) + N);
            }
        }
    }
    pthread_barrier_wait(&barrier_4th_phase_end);
    if (*j==0){
        count_total_tree_size(0);
        count_total_queue_size(8*N,N);
        count_total_queue_keysum((2 * pow(N, 4) - pow(N, 2)),N);
    }
}

int main(int argc, char *argv[]){
    N=atoi(argv[1]);
    M=atoi(argv[2]);
    pthread_t publisher[M];
    int c_num[M];
    pthread_barrier_init(&barrier_1st_phase_end, NULL, M);
    pthread_barrier_init(&barrier_2nd_phase_start,NULL,M);
    pthread_barrier_init(&barrier_2nd_phase_end,NULL,M);
    pthread_barrier_init(&barrier_3rd_phase_start,NULL,M);
    pthread_barrier_init(&barrier_3rd_phase_end,NULL,M);
    pthread_barrier_init(&barrier_4th_phase_start,NULL,M);
    pthread_barrier_init(&barrier_4th_phase_end,NULL,M);
    //List Init
    list_init();
    //Queue Init
    queue_init(N,M);
    //Tree Init
    tree_init();
    for (int i = 0; i < M; ++i) {
        c_num[i]=i;
        pthread_create(&publisher[i], NULL,(void *) create_thread, (void *) &c_num[i]);
    }
    for (int i=0;i<M;i++){
        pthread_join(publisher[i],NULL);
    }
    pthread_barrier_destroy(&barrier_1st_phase_end);
    pthread_barrier_destroy(&barrier_2nd_phase_start);
    pthread_barrier_destroy(&barrier_2nd_phase_end);
    pthread_barrier_destroy(&barrier_3rd_phase_start);
    pthread_barrier_destroy(&barrier_3rd_phase_end);
    pthread_barrier_destroy(&barrier_4th_phase_start);
    pthread_barrier_destroy(&barrier_4th_phase_end);
    return 99;
}