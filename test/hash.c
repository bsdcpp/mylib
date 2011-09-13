/**
 * @brief 
 *
 * @author jervis
 * @time 01/07/10 13:24:38
 * @version 0.1
 *
 * Copyright (C) jervis <bsdcpp@gmail.com>
 *
 */ 

#include "jlib.h"
#include <search.h>

int main(int argc, char* argv[])
{
    int i = 0;
    ENTRY e, *ep, *ep2;
    FILE *fp;
    char buf[MAXBUF];
    char num[15];

    hcreate(1);

    fp = Fopen("/home/jervis/csapp/tinyweb/data/kj20100602.txt", "r");

    memset(buf, 0, MAXBUF);
    while (Fgets(buf, 50, fp))
    {
        i++;
        if (i >100)
            break;
        memset(num, 0, sizeof(num));
        strncpy(num, buf, 3);
        e.key = num;
        printf("[%s][%d]\n", e.key, strlen(e.key));
        e.data = (void *)i;
        if ((ep = hsearch(e, FIND)) != NULL) {
            printf("第%d个[%s]与%d个[%s]重复\n", i, e.key, (int)(ep->data), ep->key);
        } else {
            printf("------------------------\n");
            printf("[%s][%d]\n\n", e.key, strlen(e.key));
            if ((ep2 = hsearch(e, ENTER)) != NULL) {
                /* printf("插入%s=>%d\n", ep2->key, (int)(ep2->data)); */
            } else {
                perror("table full");
                exit(EXIT_FAILURE);
            }
        }
    }
    printf("===================\n");
    e.key = "677";
    printf("[%s][%d]\n", e.key, strlen(e.key));
    if ((ep = hsearch(e, FIND)) != NULL) {
        printf("find %d => %s\n", (int)(ep->data), ep->key);
    }
    hdestroy();
    Fclose(fp);
    return 0;
}
