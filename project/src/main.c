#include <stdio.h>
#include <sys/types.h>
#include <string.h>

#define READ 0     /* read-side of pipes */
#define WRITE 1    /* write-side of pipes */
#define MAXLEN 100 /* max length of message */

//char *msg = "Content of pipe.";

int fd[2], bytesRead;
char message[MAXLEN];


int main(int argc, char *argv[])
{
    char * playersNumber_str = argv[1];
    int playersNumber = atoi(playersNumber_str);
    pipe(fd); /* Create unnamed pipe */
    printf("Sto forkando\n");
    pid_t pid = fork();

    if (pid > 0)
    { // parent
        printf("Sono il padre\n");

        close(fd[WRITE]); /* close other side */
        bytesRead = read(fd[READ], message, MAXLEN);
        printf("- main: Read %d bytes: %s\n", bytesRead, message);
        close(fd[READ]); /* close this side */
    }
    else
    { // child
        //printf("%d\n", fd);
        dup2(fd[WRITE], WRITE);
        close(fd[READ]);
        close(fd[WRITE]);
        char *const paramList[] = {"bin/day", playersNumber_str, NULL};
        int e = execv(paramList[0], paramList);
    }

    return 0;
}