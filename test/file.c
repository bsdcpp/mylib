/**
 * @brief 
 *
 * @author jervis
 * @time 24/06/10 17:38:18
 * @version 0.1
 *
 * Copyright (C) jervis <bsdcpp@gmail.com>
 *
 */ 

#include "jlib.h"

int main(int argc, char* argv[])
{
    FILE *fp;
    const char *file = "kj2010030144.txt";
    char buf[500];
    int len;

    memset(buf, 0, sizeof(buf));
    fp = Fopen(file, "r");
    Fgets(buf, 500, fp);
    len = strlen(buf) - 1;
    buf[len] = 0;

    printf("%s", buf);

    Fclose(fp);
	return 0;
}
