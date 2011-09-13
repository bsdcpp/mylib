#include "jlib.h"
#define NITERS 200000000
void *count(void *arg);

/* shared counter variable */
unsigned int cnt = 0;
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;
sem_t semutex;

int main()
{
    pthread_t tid1, tid2;
    /* Pthread_mutex_init(&mutex, NULL); */
    /* Pthread_rwlock_init(&rwlock, NULL); */
    sem_init(&semutex, 0, 1);

    Pthread_create(&tid1, NULL, count, NULL);
    Pthread_create(&tid2, NULL, count, NULL);
    Pthread_join(tid1, NULL);
    Pthread_join(tid2, NULL);

    /* Pthread_mutex_destroy(&mutex); */
    /* Pthread_rwlock_destroy(&rwlock); */

    if (cnt != (unsigned)NITERS*2)
        printf("BOOM! cnt=%d\n", cnt);
    else
        printf("OK cnt=%d\n", cnt);
    exit(0);
}

/* thread routine */
void *count(void *arg)
{
    int i;
    /* Pthread_mutex_lock(&mutex); */
    /* Pthread_rwlock_wrlock(&rwlock); */
    P(&semutex);
    for (i = 0; i < NITERS; i++)
        cnt++;
    /* Pthread_mutex_unlock(&mutex); */
    /* Pthread_rwlock_unlock(&rwlock); */
    V(&semutex);
    return NULL;
}
