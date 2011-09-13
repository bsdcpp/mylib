/**
 * @brief 
 *
 * @author jervis
 * @time 09/06/10 12:09:21
 * @version 0.1
 *
 * Copyright (C) jervis <bsdcpp@gmail.com>
 *
 */ 

#include "jlib.h"

int main(int argc, char* argv[])
{
    int clientfd;
    char buf[MAXLINE];
    rio_t rio;

    if (argc != 3) {
        more_error("usage: %s <host> <port>", argv[0]);
    }


    clientfd = Open_clientfd(argv[1], atoi(argv[2]));
    Rio_readinitb(&rio, clientfd);

    memset(buf, 0, sizeof(buf));
    while (Fgets(buf, MAXLINE, stdin) != NULL)
    {
        Rio_writen(clientfd, buf, strlen(buf));

        /* Read from server echo */
        memset(buf, 0, sizeof(buf));
        Rio_readlineb(&rio, buf, MAXLINE);
        Fputs(buf, stdout);
    }

    Close(clientfd);
	return 0;
}
