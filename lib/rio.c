#include "jlib.h"
/**********************************************************************
 * The Rio package - robust I/O functions
 **********************************************************************/
/*
 * rio_readn - robustly read n bytes (unbuffered)
 */    
ssize_t rio_readn(int fd, void *buf, size_t count)
{
    ssize_t nread = 0;
    size_t nleft = count;
    char *bufp = buf;

    while (nleft > 0)
    {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno == EINTR) { /* interrupted by sig handler return */
                nread = 0;        /* and call read() again */
            } else {
                return -1;
            }
        } else if (nread == 0) { /* EOF */
            break;
        } else {
            nleft -= nread;
            bufp  += nread;
        }
    }
    return (count - nleft);
}
/*
 * rio_writen - robustly write n bytes (unbuffered)
 */

ssize_t rio_writen(int fd, void *buf, size_t count)
{
    ssize_t nwritten = 0;
    size_t  nleft    = count;
    char *bufp       = buf;

    while (nleft > 0)
    {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR) {
                nwritten = 0;
            } else {
                return -1;
            }
        } else {
            nleft -= nwritten;
            bufp  += nwritten;
        }
    }
    return (count - nleft);
}

/* 
 * rio_read - This is a wrapper for the Unix read() function that
 *  transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *  buffer, where n is the number of bytes requested by the user and
 *  rio_cnt is the number of unread bytes in the internal buffer. On
 *  entry, rio_read() refills the internal buffer via a call to
 *  read() if the internal buffer is empty.
 */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t count)
{
    int cnt;
    while (rp->rio_cnt <= 0)
    {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));  /* full fill buffer */
        if (rp->rio_cnt < 0) {
            if (errno == EINTR) {
                rp->rio_cnt = 0;
            } else {
                return -1;
            }
        } else if (rp->rio_cnt == 0) {
            return 0;
        } else {
            rp->rio_bufptr = rp->rio_buf; /* reset buffer ptr: see the read call had full fill the rio_buf array up */
        }
    }
    cnt = min(rp->rio_cnt, count);
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt    -= cnt;

    return cnt;
}

/*
 * rio_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd     = fd;
    rp->rio_cnt    = 0;
    rp->rio_bufptr = rp->rio_buf;
}

/*
 * rio_readnb - Robustly read n bytes (buffered)
 */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    size_t nleft  = n;
    ssize_t nread = 0;
    char *bufp    = usrbuf;

    while (nleft > 0)
    {
        if ((nread = rio_read(rp, bufp, nleft)) < 0) {
            if (errno == EINTR) {
                nread = 0;
            } else {
                return -1;
            }
        } else if (nread == 0) {
            break;
        } else {
            nleft -= nread;
            bufp  += nread;
        }
    }
    return (n - nleft);
}


/* 
 * rio_readlineb - robustly read a text line (buffered)
 */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    int i, rc;
    char c, *bufp = usrbuf;

    for (i = 1; i < maxlen; i++)
    {
        if ((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            if (i == 1)
                return 0;
            else
                break;
        } else {
            return -1;
        }
    }
    *bufp = 0;
    return i;
}
