/**
 * @brief 
 *
 * @author jervis
 * @time 12/06/10 13:00:35
 * @version 0.1
 *
 * Copyright (C) jervis <bsdcpp@gmail.com>
 *
 */ 
#include "jlib.h"

int main(int argc, char* argv[])
{
    int fd;
    mode_t mode = S_IRUSR;
    fd = Open("/home/jervis", O_RDONLY, mode);

    printf("%d\n", fd);

	return 0;
}
