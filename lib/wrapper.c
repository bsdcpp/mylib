/**
 * @brief 
 * Wrappers for exist functions: deal with erro return ...
 *
 * @author jervis
 * @time 09/06/10 11:11:06
 * @version 0.1
 *
 * Copyright (C) jervis <bsdcpp@gmail.com>
 *
 */ 
#include "jlib.h"


/********************************
 * Wrappers for Unix I/O routines
 ********************************/
int Open(const char *pathname, int flags, mode_t mode)
{
    int rc;

    if ((rc = open(pathname, flags, mode))  < 0)
        more_error("Open error");
    return rc;
}

ssize_t Read(int fd, void *buf, size_t count)
{
    ssize_t rc;

    if ((rc = read(fd, buf, count)) < 0)
        more_error("Read error");
    return rc;
}

ssize_t Write(int fd, const void *buf, size_t count)
{
    ssize_t rc;

    if ((rc = write(fd, buf, count)) < 0)
        more_error("Write error");
    return rc;
}

off_t Lseek(int fildes, off_t offset, int whence)
{
    off_t rc;

    if ((rc = lseek(fildes, offset, whence)) < 0)
        more_error("Lseek error");
    return rc;
}

void Close(int fd)
{
    int rc;

    if ((rc = close(fd)) < 0)
        more_error("Close error");
}

int Select(int  n, fd_set *readfds, fd_set *writefds,
        fd_set *exceptfds, struct timeval *timeout)
{
    int rc;

    if ((rc = select(n, readfds, writefds, exceptfds, timeout)) < 0)
        more_error("Select error");
    return rc;
}

int Dup2(int fd1, int fd2)
{
    int rc;

    if ((rc = dup2(fd1, fd2)) < 0)
        more_error("Dup2 error");
    return rc;
}

void Stat(const char *filename, struct stat *buf)
{
    if (stat(filename, buf) < 0)
        more_error("Stat error");
}

void Fstat(int fd, struct stat *buf)
{
    if (fstat(fd, buf) < 0)
        more_error("Fstat error");
}

/*********************************************
 * Wrappers for Unix process control functions
 ********************************************/

/* $begin forkwrapper */
pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        more_error("Fork error");
    return pid;
}
/* $end forkwrapper */

void Execve(const char *filename, char *const argv[], char *const envp[])
{
    if (execve(filename, argv, envp) < 0)
        more_error("Execve error");
}
/* $begin wait */
pid_t Wait(int *status)
{
    pid_t pid;

    if ((pid  = wait(status)) < 0)
        more_error("Wait error");
    return pid;
}
/* $end wait */

pid_t Waitpid(pid_t pid, int *iptr, int options)
{
    pid_t retpid;

    if ((retpid  = waitpid(pid, iptr, options)) < 0)
        more_error("Waitpid error");
    return(retpid);
}

/* $begin kill */
void Kill(pid_t pid, int signum)
{
    int rc;

    if ((rc = kill(pid, signum)) < 0)
        more_error("Kill error");
}
/* $end kill */

void Pause(void)
{
    (void)pause();
    return;
}

unsigned int Sleep(unsigned int secs)
{
    unsigned int rc;

    if ((rc = sleep(secs)) < 0)
        more_error("Sleep error");
    return rc;
}

unsigned int Alarm(unsigned int seconds) {
    return alarm(seconds);
}

void Setpgid(pid_t pid, pid_t pgid) {
    int rc;

    if ((rc = setpgid(pid, pgid)) < 0)
        more_error("Setpgid error");
    return;
}

pid_t Getpgrp(void) {
    return getpgrp();
}
/************************************
 * Wrappers for Unix signal functions 
 ***********************************/
handler_t *Signal(int signum, handler_t *handler)
{
    struct sigaction action, old_action; 

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = 0;

    if (signum == SIGALRM) {
#ifdef SA_INTERRUPT
        action.sa_flags |= SA_INTERRUPT;
#endif
    } else {
#ifdef SA_RESTART
        action.sa_flags |= SA_RESTART;
#endif
    }
    if (sigaction(signum, &action, &old_action) < 0)
        more_error("Signal error");
    return (old_action.sa_handler);
}
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    if (sigprocmask(how, set, oldset) < 0)
        more_error("Sigprocmask error");
    return;
}

void Sigemptyset(sigset_t *set)
{
    if (sigemptyset(set) < 0)
        more_error("Sigemptyset error");
    return;
}

void Sigfillset(sigset_t *set)
{
    if (sigfillset(set) < 0)
        more_error("Sigfillset error");
    return;
}

void Sigaddset(sigset_t *set, int signum)
{
    if (sigaddset(set, signum) < 0)
        more_error("Sigaddset error");
    return;
}
void Sigdelset(sigset_t *set, int signum)
{
    if (sigdelset(set, signum) < 0)
        more_error("Sigdelset error");
    return;
}

int Sigismember(const sigset_t *set, int signum)
{
    int rc;
    if ((rc = sigismember(set, signum)) < 0)
        more_error("Sigismember error");
    return rc;
}

/*******************************
 * Wrappers for Posix semaphores
 *******************************/

void Sem_init(sem_t *sem, int pshared, unsigned int value)
{
    if (sem_init(sem, pshared, value) < 0)
        more_error("Sem_init error");
}

void P(sem_t *sem)
{
    if (sem_wait(sem) < 0)
        more_error("P error");
}

void V(sem_t *sem)
{
    if (sem_post(sem) < 0)
        more_error("V error");
}

/***************************************
 * Wrappers for memory mapping functions
 ***************************************/
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    void *ptr;

    if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *) -1))
        more_error("mmap error");
    return(ptr);
}

void Munmap(void *start, size_t length)
{
    if (munmap(start, length) < 0)
        more_error("munmap error");
}

/**************************** 
 * Sockets interface wrappers
 ****************************/

int Socket(int domain, int type, int protocol)
{
    int rc;

    if ((rc = socket(domain, type, protocol)) < 0)
        more_error("Socket error");
    return rc;
}

void Setsockopt(int s, int level, int optname, const void *optval, int optlen)
{
    int rc;

    if ((rc = setsockopt(s, level, optname, optval, optlen)) < 0)
        more_error("Setsockopt error");
}

void Bind(int sockfd, struct sockaddr *my_addr, int addrlen)
{
    int rc;

    if ((rc = bind(sockfd, my_addr, addrlen)) < 0)
        more_error("Bind error");
}

void Listen(int s, int backlog)
{
    int rc;

    if ((rc = listen(s,  backlog)) < 0)
        more_error("Listen error");
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    int rc;

    if ((rc = accept(s, addr, addrlen)) < 0)
        more_error("Accept error");
    return rc;
}

void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen)
{
    int rc;

    if ((rc = connect(sockfd, serv_addr, addrlen)) < 0)
        more_error("Connect error");
}

/************************
 * DNS interface wrappers 
 ***********************/

/* $begin gethostbyname */
struct hostent *Gethostbyname(const char *name)
{
    struct hostent *p;

    if ((p = gethostbyname(name)) == NULL)
        more_error("Gethostbyname error");
    return p;
}
/* $end gethostbyname */

struct hostent *Gethostbyaddr(const char *addr, int len, int type)
{
    struct hostent *p;

    if ((p = gethostbyaddr(addr, len, type)) == NULL)
        more_error("Gethostbyaddr error");
    return p;
}


/***************************************************
 a Wrappers for dynamic storage allocation functions
 ***************************************************/

void *Malloc(size_t size)
{
    void *p;

    if ((p  = malloc(size)) == NULL)
        more_error("Malloc error");
    return p;
}

void *Realloc(void *ptr, size_t size)
{
    void *p;

    if ((p  = realloc(ptr, size)) == NULL)
        more_error("Realloc error");
    return p;
}

void *Calloc(size_t nmemb, size_t size)
{
    void *p;

    if ((p = calloc(nmemb, size)) == NULL)
        more_error("Calloc error");
    return p;
}

void Free(void *ptr)
{
    free(ptr);
}

/******************************************
 * Wrappers for the Standard I/O functions.
 ******************************************/
void Fclose(FILE *fp)
{
    if (fclose(fp) != 0)
        more_error("Fclose error");
}

FILE *Fdopen(int fd, const char *type)
{
    FILE *fp;

    if ((fp = fdopen(fd, type)) == NULL)
        more_error("Fdopen error");

    return fp;
}

char *Fgets(char *ptr, int n, FILE *stream)
{
    char *rptr;

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
        more_error("Fgets error");

    return rptr;
}

FILE *Fopen(const char *filename, const char *mode)
{
    FILE *fp;

    if ((fp = fopen(filename, mode)) == NULL)
        more_error("Fopen error");

    return fp;
}

void Fputs(const char *ptr, FILE *stream)
{
    if (fputs(ptr, stream) == EOF)
        more_error("Fputs error");
}

size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t n;

    if (((n = fread(ptr, size, nmemb, stream)) < nmemb) && ferror(stream))
        more_error("Fread error");
    return n;
}

void Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    if (fwrite(ptr, size, nmemb, stream) < nmemb)
        more_error("Fwrite error");
}


/************************************************
 * Wrappers for Pthreads thread control functions
 ************************************************/
void Pthread_rwlock_destroy(pthread_rwlock_t *rwlock)
{
    int rc;
    if ((rc = pthread_rwlock_destroy(rwlock)) != 0)
        err_exit(rc, "Pthread_rwlock_destroy error");
}

void Pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
        const pthread_rwlockattr_t *restrict attr)
{
    int rc;
    if ((rc = pthread_rwlock_init(rwlock, attr)) != 0)
        err_exit(rc, "Pthread_rwlock_init error");
}

void Pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)
{
    int rc;
    if ((rc = pthread_rwlock_rdlock(rwlock)) != 0)
        err_exit(rc, "Pthread_rwlock_rdlock error");
}

void Pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)
{
    int rc;
    if ((rc = pthread_rwlock_tryrdlock(rwlock)) != 0)
        err_exit(rc, "Pthread_rwlock_tryrdlock error");
}

void Pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
{
    int rc;
    if ((rc = pthread_rwlock_wrlock(rwlock)) != 0)
        err_exit(rc, "Pthread_rwlock_rwlock error");
}

void Pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
    int rc;
    if ((rc = pthread_rwlock_trywrlock(rwlock)) != 0)
        err_exit(rc, "Pthread_rwlock_tryrwlock error");
}

void Pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
    int rc;
    if ((rc = pthread_rwlock_unlock(rwlock)) != 0)
        err_exit(rc, "Pthread_rwlock_unlock error");
}


void Pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    int rc;
    if ((rc = pthread_mutex_destroy(mutex)) != 0)
        err_exit(rc, "Pthread_mutex_destroy error");
}

void Pthread_mutex_init(pthread_mutex_t *restrict mutex,
        const pthread_mutexattr_t *restrict attr)
{
    int rc;
    if ((rc = pthread_mutex_init(mutex, attr)) != 0)
        err_exit(rc, "Pthread_mutex_init error");
}

void Pthread_mutex_lock(pthread_mutex_t *mutex)
{
    int rc;
    if ((rc = pthread_mutex_lock(mutex)) != 0)
        err_exit(rc, "Pthread_mutex_lock error");
}

void Pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    int rc;
    if ((rc = pthread_mutex_trylock(mutex)) != 0)
        err_exit(rc, "Pthread_mutex_trylock error");
}

void Pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    int rc;
    if ((rc = pthread_mutex_unlock(mutex)) != 0)
        err_exit(rc, "Pthread_mutex_unlock error");
}

void Pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
    int rc;
    if ((rc = pthread_attr_setdetachstate(attr, detachstate)) != 0)
        err_exit(rc, "Pthread_attr_etdetachstate error");
}
void Pthread_attr_destroy(pthread_attr_t *attr)
{
    int rc;
    if ((rc = pthread_attr_destroy(attr)) != 0)
        err_exit(rc, "Pthread_attr_destory error");
}

void Pthread_attr_init(pthread_attr_t *attr)
{
    int rc;
    if ((rc = pthread_attr_init(attr)) != 0)
        err_exit(rc, "Pthread_attr_init error");
}

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
        void * (*routine)(void *), void *argp) 
{
    int rc;

    if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0)
        err_exit(rc, "Pthread_create error");
}

void Pthread_cancel(pthread_t tid) {
    int rc;

    if ((rc = pthread_cancel(tid)) != 0)
        err_exit(rc, "Pthread_cancel error");
}      

void Pthread_join(pthread_t tid, void **thread_return) {
    int rc;

    if ((rc = pthread_join(tid, thread_return)) != 0)
        err_exit(rc, "Pthread_join error");
}
/* $begin detach */
void Pthread_detach(pthread_t tid) {
    int rc;

    if ((rc = pthread_detach(tid)) != 0)
        err_exit(rc, "Pthread_detach error");
}
/* $end detach */

void Pthread_exit(void *retval) {
    pthread_exit(retval);
}

pthread_t Pthread_self(void) {
    return pthread_self();
}

void Pthread_once(pthread_once_t *once_control, void (*init_function)()) {
    pthread_once(once_control, init_function);
}

/**********************************
 * Wrappers for robust I/O routines
 **********************************/
ssize_t Rio_readn(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;

    if ((n = rio_readn(fd, ptr, nbytes)) < 0)
        more_error("Rio_readn error");
    return n;
}

void Rio_writen(int fd, void *usrbuf, size_t n)
{
    if (rio_writen(fd, usrbuf, n) != n)
        more_error("Rio_writen error");
}

void Rio_readinitb(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
}

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
        more_error("Rio_readnb error");
    return rc;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
        more_error("Rio_readlineb error");
    return rc;
}

/******************************************
 * Wrappers for the client/server helper routines 
 ******************************************/
int Open_clientfd(char *hostname, int port)
{
    int rc;

    if ((rc = open_clientfd(hostname, port)) < 0) {
        if (rc == -1)
            more_error("Open_clientfd Unix error");
        else
            more_error("Open_clientfd DNS error");
    }
    return rc;
}

int Open_listenfd(int port)
{
    int rc;

    if ((rc = open_listenfd(port)) < 0)
        more_error("Open_listenfd error");
    return rc;
}
