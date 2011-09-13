/**
 * @brief 
 *
 * @author jervis
 * @time 18/06/10 12:39:05
 * @version 0.1
 *
 * Copyright (C) jervis <bsdcpp@gmail.com>
 *
 */ 

#include "jlib.h"

int main(int argc, char* argv[])
{
    int pid, ret;

    //忽略SIGCHLD信号，这常用于并发服务器的性能的一个技巧
    //因为并发服务器常常fork很多子进程，子进程终结之后需要
    //服务器进程去wait清理资源。如果将此信号的处理方式设为
    //忽略，可让内核把僵尸子进程转交给init进程去处理，省去了
    //大量僵尸进程占用系统资源。(Linux Only)
    signal(SIGCHLD, SIG_IGN); 


    pid = fork();             //生成一个子进程
    if (pid < 0)              // error check.
        perror("fork");

    if (pid == 0) {
        sleep(20);
        exit (0);      // child process.
    } else
        if (wait(&ret) < 0)
            perror("wait");        //parent process
    //在这里wait，都会得No Such process的错误，
    //因为子进程终止后，内核会向父进程发送SIGCHLD
    //信号，但是上面已将此信号设为忽略，实质上由
    //init来接收此子进程的处理。

    return 0;
}
