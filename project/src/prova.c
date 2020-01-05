#include <stdio.h>
#include <string.h>
#define READ 0     /* read-side of pipes */
#define WRITE 1    /* write-side of pipes */
#define MAXLEN 100 /* max length of message */
char *msg = "Content of pipe.";
int main()
{

    int fd[2], bytesRead;
    char message[MAXLEN];
    pipe(fd); /* Create unnamed pipe */

    if (fork() > 0)
    {                                           /* Parent, writer */
        close(fd[READ]);                        /* close other side */
        write(fd[WRITE], msg, strlen(msg) + 1); /* include \0 */
        close(fd[WRITE]);                       /* close this side */
    }
    else
    {                     /* Child, reader*/
        close(fd[WRITE]); /* close other side */
        bytesRead = read(fd[READ], message, MAXLEN);
        printf("Read %d bytes: %s\n", bytesRead, message);
        close(fd[READ]); /* close this side */
    }
}