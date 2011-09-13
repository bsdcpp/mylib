#include "jlib.h"

void reap_zombie(int signo)
{
    int errno_old = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = errno_old;
}

int main(int argc, char *argv[])
{
    pid_t pid;

    if (argc == 2) {
        if (strcmp(argv[1], "signal") == 0)
            signal(SIGCHLD, SIG_IGN);
        else if (strcmp(argv[1], "handle") == 0)
            signal(SIGCHLD, reap_zombie);
        else {
            struct sigaction act;

            memset(&act, 0, sizeof(act));
            act.sa_flags |= SA_NOCLDWAIT;
            sigaction(SIGCHLD, &act, NULL);
        }
    }
    pid = Fork();
    if (pid == 0) {
        fprintf(stderr, "I am child\n");
        sleep(1);
        fprintf(stderr, "Child exits\n");

        return EXIT_SUCCESS;
    }
    sleep(2);
    system("ps -el | grep sigaction");
    wait(NULL);
    perror("wait");
    system("ps -el | grep sigaction");

    return EXIT_SUCCESS;
}
