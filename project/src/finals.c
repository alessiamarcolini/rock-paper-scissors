#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "utils.h"

int bytesRead;
char message[MAXLEN];

int main(int argc, char *argv[])
{
    int i;

    pid_t pid;
    int status; // return status of child

    sleep(1);
    int fd[2];

    int e = pipe(fd);
    if (e < 0)
    {
        fprintf(stderr, "Error pipe: %s\n", strerror(errno));
        exit(5);
    }

    pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "Error while forking.\n");
        exit(3);
    }

    if (pid > 0)
    { //

        close(fd[WRITE]);
        bytesRead = read(fd[READ], message, MAXLEN);
        //printf("quarters - read %d bytes: %s\n", bytesRead, message);

        //sprintf(buffer, "%s\n", message); // dayId|message
        printf("%s\n", message);

        //strcpy(messageToSendByLine[i], buffer);

        close(fd[READ]);
    }
    else
    { // single "semi final"
        dup2(fd[WRITE], WRITE);
        close(fd[READ]);
        close(fd[WRITE]);

        //convert i to a string
        char str[12];
        sprintf(str, "%d", i);

        char *firstPlayer;
        char *secondPlayer;

        firstPlayer = argv[2];
        secondPlayer = argv[3];

        char *const paramList[] = {"bin/final", str, firstPlayer, secondPlayer, NULL};
        int e = execv(paramList[0], paramList);
        if (e < 0)
        {
            fprintf(stderr, "Error execv: %s\n", strerror(errno));
            exit(4);
        }
    }

    return 0;
}