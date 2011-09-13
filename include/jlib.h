#ifndef __JLIB_H__
#define __JLIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef __EN_DBG
# define more_msg(msg...) \
    do { \
        fprintf(stderr, "\E[01;31m%s:%d: %s(): warning:\E[0m ", __FILE__, __LINE__, __func__); \
        err_msg(msg); \
    } while (0)

# define more_error(msg...) \
    do { \
        fprintf(stderr, "\E[01;31m%s():%d: %s(): error:\E[0m ", __FILE__, __LINE__, __func__); \
        err_quit(msg); \
    } while (0)
#else

# define more_msg(msg...)
# define more_error(msg...) 
#endif

#define LOG_PATH "/fsdata/USERS/cqduser/bin/query/"		 /* 日志路径 */

#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

/* Default file permissions are DEF_MODE & ~DEF_UMASK */
#define DEF_MODE   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define DEF_UMASK  S_IWGRP|S_IWOTH

/* Simplifies calls to bind(), connect(), and accept() */
typedef struct sockaddr SA;
/* External variables */
extern int h_errno;    /* defined by BIND for DNS errors */
extern char **environ; /* defined by libc */ 
         
/* Misc constants */
#define MAXLINE  8192  /* max text line length */
#define MAXBUF   8192  /* max I/O buffer size */
#define LISTENQ  1024  /* second argument to listen() */

/* Persistent state for the robust I/O (Rio) package */
#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* descriptor for this internal buf */
    int rio_cnt;               /* unread bytes in internal buf */
    char *rio_bufptr;          /* next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */
} rio_t;

/* Message */
void err_dump(const char *, ...);
void err_msg(const char *, ...);
void err_quit(const char *, ...);
void err_exit(int, const char *, ...);
void err_ret(const char *, ...);
void err_sys(const char *, ...);


/* Unix I/O wrappers */
int Open(const char *pathname, int flags, mode_t mode);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);
off_t Lseek(int fildes, off_t offset, int whence);
void Close(int fd);
int Select(int  n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
        struct timeval *timeout);
int Dup2(int fd1, int fd2);
void Stat(const char *filename, struct stat *buf);
void Fstat(int fd, struct stat *buf) ;

/* Process control wrappers */
pid_t Fork(void);
void Execve(const char *filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);
pid_t Waitpid(pid_t pid, int *iptr, int options);
void Kill(pid_t pid, int signum);
unsigned int Sleep(unsigned int secs); 
void Pause(void);
unsigned int Alarm(unsigned int seconds);
void Setpgid(pid_t pid, pid_t pgid);
pid_t Getpgrp();

/* Signal wrappers */
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
void Sigemptyset(sigset_t *set);
void Sigfillset(sigset_t *set);
void Sigaddset(sigset_t *set, int signum);
void Sigdelset(sigset_t *set, int signum);
int Sigismember(const sigset_t *set, int signum);

/* POSIX semaphore wrappers */
void Sem_init(sem_t *sem, int pshared, unsigned int value);
void P(sem_t *sem);
void V(sem_t *sem);

/* Memory mapping wrappers */
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
void Munmap(void *start, size_t length);

/* Sockets interface wrappers */
int Socket(int domain, int type, int protocol);
void Setsockopt(int s, int level, int optname, const void *optval, int optlen);
void Bind(int sockfd, struct sockaddr *my_addr, int addrlen);
void Listen(int s, int backlog);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

/* DNS wrappers */
struct hostent *Gethostbyname(const char *name);
struct hostent *Gethostbyaddr(const char *addr, int len, int type);

/* Dynamic storage allocation wrappers */
void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void *Calloc(size_t nmemb, size_t size);
void Free(void *ptr);

/* Standard I/O wrappers */
void Fclose(FILE *fp);
FILE *Fdopen(int fd, const char *type);
char *Fgets(char *ptr, int n, FILE *stream);
FILE *Fopen(const char *filename, const char *mode);
void Fputs(const char *ptr, FILE *stream);
size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
void Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

/* Pthreads thread control wrappers */
void Pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
void Pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
        const pthread_rwlockattr_t *restrict attr);
void Pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
void Pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
void Pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
void Pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
void Pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

void Pthread_mutex_destroy(pthread_mutex_t *mutex);
void Pthread_mutex_init(pthread_mutex_t *restrict mutex, 
        const pthread_mutexattr_t *restrict attr);
void Pthread_mutex_lock(pthread_mutex_t *mutex);
void Pthread_mutex_trylock(pthread_mutex_t *mutex);
void Pthread_mutex_unlock(pthread_mutex_t *mutex);

void Pthread_attr_destroy(pthread_attr_t *attr);
void Pthread_attr_init(pthread_attr_t *attr);
void Pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
        void * (*routine)(void *), void *argp);
void Pthread_join(pthread_t tid, void **thread_return);
void Pthread_cancel(pthread_t tid);
void Pthread_detach(pthread_t tid);
void Pthread_exit(void *retval);
pthread_t Pthread_self(void);
void Pthread_once(pthread_once_t *once_control, void (*init_function)());


/* Rio (Robust I/O) package */
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

/* Wrappers for Rio package */
ssize_t Rio_readn(int fd, void *usrbuf, size_t n);
void Rio_writen(int fd, void *usrbuf, size_t n);
void Rio_readinitb(rio_t *rp, int fd);
ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

/* Client/server helper functions */
int open_clientfd(char *hostname, int portno);
int open_listenfd(int portno);

/* Wrappers for client/server helper functions */
int Open_clientfd(char *hostname, int port);
int Open_listenfd(int port);

/* Other */
void pr_exit(int status);



/*********************
  获取月份(Month) MM
param: cSysMonth-月份
 **********************/
void GetSysMonth(char *cSysMonth);

/*********************
  获取上月份(Month) MM
param: cSysMonth-月份
 **********************/
void GetSysLastMonth(char *cSysMonth);

/****************************
  获取完整系统时间
  格式：yyyy-mm-dd  hh:mm:ss 
 *****************************/
void GetSysTime(char *ods_datetime);

/****************************
  获取当天日期 YYYYMMDD
param: cSysDate-当天日期
 *****************************/
void GetSysDate(char *cSysDate);

/**************************
  获取前一天日期 YYYYMMDD
param: cSysDate-前一天日期
 ***************************/
void GetLastDate(char *cSysDate);

/* 写日志 */
void writeLog(char *msg, char *type);




#endif /* _FUNC_H */
