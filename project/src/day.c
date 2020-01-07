#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "utils.h"

#define READ 0  /* read-side of pipes */
#define WRITE 1 /* write-side of pipes */

int bytesRead;
char message[MAXLEN];

int main(int argc, char *argv[])
{
    int playersNumber = atoi(argv[1]);
    printf("sono la giornata\n");
    //printf("%s\n", argv[1]);

    int i;
    for (i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }

    //ciclo per la creazione del numero di match, testing

    for (i = 0; i < playersNumber / 2; i++)
    {
        int fd[2];
        pipe(fd); /* Create unnamed pipe */
        pid_t pid = fork();

        if (pid > 0)
        { // day
            //printf("Sono il padre\n");

            close(fd[WRITE]); /* close other side */
            bytesRead = read(fd[READ], message, MAXLEN);
            printf("- day: Read %d bytes: %s\n", bytesRead, message);
            close(fd[READ]); /* close this side */
        }
        else
        { // match
            //printf("%d\n", fd);
            dup2(fd[WRITE], WRITE);
            close(fd[READ]);
            close(fd[WRITE]);
            //convert i to a string
            char str[12];
            sprintf(str, "%d", i);
            char *const paramList[] = {"bin/match", str, NULL};
            int e = execv(paramList[0], paramList);
        }
    }

    return 0;
}