#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "utils.h"

int bytesRead;
char message[MAXLEN];

char *messageToSendByLine[MAXLEN];
char *buffer;

int main(int argc, char *argv[])
{
    buffer = malloc(sizeof(char) * 1024);
    if (buffer == NULL)
    {
        fprintf(stderr, "Malloc failure: dinamic memory allocation not possible.\n");
        exit(6);
    }
    int i;
    pid_t pid;
    int status; // return status of child

    for (i = 0; i < nQuarters; i++)
    {

        //sleep(1);
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
        { // quarters

            close(fd[WRITE]);
            bytesRead = read(fd[READ], message, MAXLEN);
            if (bytesRead <= 0)
            {
                fprintf(stderr, "Error reading from pipe.\n");
                exit(8);
            }

            sprintf(buffer, "%s\n", message); // dayId|message // TODO: mandare in blocco!!!!
            messageToSendByLine[i] = malloc(sizeof(char) * 1024);
            if (messageToSendByLine[i] == NULL)
            {
                fprintf(stderr, "Malloc failure: dinamic memory allocation not possible.\n");
                exit(6);
            }
            strcpy(messageToSendByLine[i], buffer);
            free(buffer);
            close(fd[READ]);
        }
        else
        { // single "quarter"
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

            char *const paramList[] = {"bin/quarter", str, firstPlayer, secondPlayer, NULL};
            int e = execv(paramList[0], paramList);
            if (e < 0)
            {
                fprintf(stderr, "Error execv: %s\n", strerror(errno));
                exit(4);
            }
        }
    }

    char messageToSend[MAXLEN];
    strcat(messageToSend, "");
    for (i = 0; i < nQuarters; i++)
    {
        //printf("%s", messageToSendByLine[i]);
        strcat(messageToSend, messageToSendByLine[i]);
    }
    printf("%s", messageToSend);

    for (i = 0; i < nQuarters; i++)
    {
        free(messageToSendByLine[i]);
    }

    return 0;
}