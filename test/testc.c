/**
 * @brief 
 *
 * @author jervis
 * @time 18/06/10 15:32:51
 * @version 0.1
 *
 * Copyright (C) jervis <bsdcpp@gmail.com>
 *
 */ 
#include "jlib.h"

int main(int argc, char* argv[])
{
    char str[20];
    printf("%s\n", str);
    memset(str, 0, sizeof(str));
    int  length = strlen( str );
    strcpy(str, "Hello World");

    printf("%d\n", length);

    return 0;
}
