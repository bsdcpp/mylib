/**
 * @brief 
 *
 * @author jervis
 * @time 11/06/10 08:45:10
 * @version 0.1
 *
 * Copyright (C) jervis <bsdcpp@gmail.com>
 *
 */ 

#include "jlib.h"

#define ORANGE_MAX_VALUE 1000000
#define APPLE_MAX_VALUE 100000000
#define MSECOND 1000000

typedef struct {
    unsigned long long a;
    unsigned long long b;
} apple;

typedef struct {
    int a[ORANGE_MAX_VALUE];
    int b[ORANGE_MAX_VALUE];
} orange;

pthread_mutex_t mutex;

void *addx(void *arg)
{
    unsigned long long sum=0;
    struct timeval tpstart,tpend;
    float timeuse;
    gettimeofday(&tpstart, NULL);

    /* Pthread_mutex_lock(&mutex); */
    for(sum=0;sum<APPLE_MAX_VALUE;sum++)
    {
        ((apple *)arg)->a += sum;
    }
    /* Pthread_mutex_unlock(&mutex); */

    gettimeofday(&tpend, NULL);
    timeuse=MSECOND*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    timeuse/=MSECOND;

    printf("addx thread:%#lx,Used Time:%f\n",pthread_self(),timeuse);
    return NULL;
}

void *addy(void *arg)
{
    unsigned long long sum=0;
    struct timeval tpstart,tpend;
    float timeuse;
    gettimeofday(&tpstart, NULL);

    /* Pthread_mutex_lock(&mutex); */
    for(sum=0;sum<APPLE_MAX_VALUE;sum++)
    {
        ((apple *)arg)->b += sum;
    }
    /* Pthread_mutex_unlock(&mutex); */

    gettimeofday(&tpend, NULL);
    timeuse=MSECOND*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    timeuse/=MSECOND;

    printf("addy thread:%#lx,Used Time:%f\n",pthread_self(),timeuse);
    return NULL;
}
int main(int argc, char* argv[])
{
    int i;
    apple test;
    orange test1 = { {0}, {0} };

    unsigned long long sum=APPLE_MAX_VALUE,index=0;
    struct timeval tpstart,tpend;
    float timeuse;

    pthread_attr_t attr;
    pthread_t    tid[2];

    test.a = 0;
    test.b = 0;

    /* get start time */
    gettimeofday(&tpstart, NULL);
    /* For portability, explicitly create threads in a joinable state */
    /* Pthread_attr_init(&attr);
    Pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    Pthread_mutex_init(&mutex, NULL); */

    Pthread_create(&tid[0], &attr, addx, &test);
    Pthread_create(&tid[1], &attr, addy, &test);


    for(index=0;index<ORANGE_MAX_VALUE;index++)
    {
        sum += +test1.a[index]+test1.b[index];
    }

    for (i = 0; i < 2; i++)
        Pthread_join(tid[i], NULL);
    /* Pthread_mutex_destroy(&mutex); */

    gettimeofday(&tpend, NULL);
    timeuse=MSECOND*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    timeuse/=MSECOND;
    printf("main thread:%#lx,Used Time:%f\n",pthread_self(),timeuse);
    printf("a = %llu\nb = %llu\nsum = %llu\n",test.a,test.b,sum);

    return 0;
}
