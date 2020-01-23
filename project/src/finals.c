#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
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
    pipe(fd);
    pid = fork();

    if (pid > 0)
    { //

        close(fd[WRITE]);
        bytesRead = read(fd[READ], message, MAXLEN);
        //printf("quarters - read %d bytes: %s\n", bytesRead, message);

        //sprintf(buffer, "%s\n", message); // dayId|message
        printf("%s\n", message);
        //messageToSendByLine[i] = malloc(sizeof(char) * 1024);
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
    }

    return 0;
}