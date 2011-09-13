/**
 * @brief 
 *
 * @author jervis
 * @time 09/06/10 17:17:25
 * @version 0.1
 *
 * Copyright (C) jervis <bsdcpp@gmail.com>
 *
 */ 

#include "jlib.h"

static void echo(int fd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, fd);

    memset(buf, 0, sizeof(buf));
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
    {
        printf("Server received %d bytes\n", n);
        Rio_writen(fd, buf, n);
        memset(buf, 0, sizeof(buf));
    }

}

static void sigchld_handler(int sig)
{
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0)
    {
        pr_exit(status);
    }
    return;
}

void command(void)
{
    char buf[MAXLINE];
    if (Fgets(buf, MAXLINE, stdin) == NULL)
        exit(0);
    Fputs(buf, stdout);
}

int main(int argc, char* argv[])
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    fd_set read_set, ready_set;

    if (argc != 2) {
        more_error("usage: %s <port>", argv[0]);
    }

    Signal(SIGCHLD, sigchld_handler);
    listenfd = Open_listenfd(atoi(argv[1]));

    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);
    FD_SET(listenfd, &read_set);

    while (1)
    {
        ready_set = read_set;
        Select(listenfd + 1, &ready_set, NULL, NULL, NULL);
        if (FD_ISSET(STDIN_FILENO, &ready_set)) {
            command();
        } else if (FD_ISSET(listenfd, &ready_set)) {
            clientlen = sizeof(clientaddr);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            /* printf("%#x\n", ntohl(clientaddr.sin_addr.s_addr)); */
            hp = Gethostbyaddr((const void *)&clientaddr.sin_addr.s_addr, 
                    sizeof(clientaddr.sin_addr.s_addr), AF_INET);
            haddrp = inet_ntoa(clientaddr.sin_addr);
            printf("Server connected to %s [%s]\n", hp->h_name, haddrp);

            if (Fork() == 0) {
                Close(listenfd);
                echo(connfd);
                Close(connfd);
                exit(0);
            }
            Close(connfd);
        }
    }
    return 0;
}
