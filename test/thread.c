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

struct apple {
    unsigned long long a;
    unsigned long long b;
};

struct orange {
    int a[ORANGE_MAX_VALUE];
    int b[ORANGE_MAX_VALUE];
};

int main(int argc, char* argv[])
{
    struct apple test;
    struct orange test1 = { {0}, {0} };

    unsigned long long sum=0,index=0;
    struct timeval tpstart,tpend;
    float timeuse;

    test.a = 0;
    test.b = 0;

    /* get start time */
    gettimeofday(&tpstart, NULL);


    for(sum=0;sum<APPLE_MAX_VALUE;sum++)
    {
        test.a += sum;
        test.b += sum;
    }

    for(index=0;index<ORANGE_MAX_VALUE;index++)
    {
        sum += test1.a[index]+test1.b[index];
    }

    gettimeofday(&tpend, NULL);
    timeuse=MSECOND*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    timeuse/=MSECOND;
    printf("main thread:%#lx,Used Time:%f\n",pthread_self(),timeuse);
    printf("a = %llu\nb = %llu\nsum = %llu\n",test.a,test.b,sum);

    return 0;
}
