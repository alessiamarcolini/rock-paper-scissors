#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

int bytesRead;
char message[MAXLEN];

char *messageToSendByLine[MAXLEN];
char *buffer;

int main(int argc, char *argv[])
{
    buffer = malloc(sizeof(char) * 1024);
    int i;

    int nSemiFinals = 2;
    pid_t pid;
    int status; // return status of child

    for (i = 0; i < nSemiFinals; i++)
    {

        sleep(1);
        int fd[2];
        pipe(fd);
        pid = fork();

        if (pid > 0)
        { // quarters

            close(fd[WRITE]);
            bytesRead = read(fd[READ], message, MAXLEN);
            //printf("quarters - read %d bytes: %s\n", bytesRead, message);

            sprintf(buffer, "%s\n", message); // dayId|message
            messageToSendByLine[i] = malloc(sizeof(char) * 1024);
            strcpy(messageToSendByLine[i], buffer);

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

            firstPlayer = argv[i * 2 + 2];
            secondPlayer = argv[i * 2 + 3];

            char *const paramList[] = {"bin/semiFinal", str, firstPlayer, secondPlayer, NULL};
            int e = execv(paramList[0], paramList);
        }
    }
    char messageToSend[MAXLEN];
    strcat(messageToSend, "");
    for (i = 0; i < nSemiFinals; i++)
    {
        strcat(messageToSend, messageToSendByLine[i]);
    }

    printf("%s", messageToSend);

    return 0;
}