#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
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
    int playersNumber = atoi(argv[1]);
    char *season = argv[argc - 1];
    int i;

    for (i = 0; i < playersNumber / 2; i++)
    {
        sleep(1);
        int fd[2];
        int e = pipe(fd);
        if (e < 0)
        {
            fprintf(stderr, "Error pipe: %s\n", strerror(errno));
            exit(5);
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            fprintf(stderr, "Error while forking.\n");
            exit(3);
        }

        if (pid > 0)
        {                     // day
            close(fd[WRITE]); /* close other side */
            bytesRead = read(fd[READ], message, MAXLEN);

            if (bytesRead <= 0)
            {
                fprintf(stderr, "Error reading from pipe.\n");
                exit(8);
            }

            sprintf(buffer, "%d %s\n", i, message); // dayId|message
            messageToSendByLine[i] = malloc(sizeof(char) * 1024);

            if (messageToSendByLine[i] == NULL)
            {
                fprintf(stderr, "Malloc failure: dinamic memory allocation not possible.\n");
                exit(6);
            }
            strcpy(messageToSendByLine[i], buffer);

            close(fd[READ]); /* close this side */
        }
        else
        { // match
            int e = dup2(fd[WRITE], WRITE);
            if (e < 0)
            {
                fprintf(stderr, "Error while duplicating file descriptor: %s\n", strerror(errno));
                exit(10);
            }
            close(fd[READ]);
            close(fd[WRITE]);

            //convert i to a string
            char str[12];
            sprintf(str, "%d", i);

            char *firstPlayer;
            char *secondPlayer;
            if (strcmp(season, "A") == 0)
            {
                firstPlayer = argv[i * 2 + 2];
                secondPlayer = argv[i * 2 + 3];
            }
            else
            {
                firstPlayer = argv[i * 2 + 3];
                secondPlayer = argv[i * 2 + 2];
            }

            char *const paramList[] = {"bin/matchTen", str, firstPlayer, secondPlayer, NULL};
            e = execv(paramList[0], paramList);
            if (e < 0)
            {
                fprintf(stderr, "Error execv: %s\n", strerror(errno));
                exit(4);
            }
        }
    }

    char messageToSend[MAXLEN];
    strcat(messageToSend, "");
    for (i = 0; i < playersNumber / 2; i++)
    {
        strcat(messageToSend, messageToSendByLine[i]);
    }
    printf("%s", messageToSend);

    for (i = 0; i < playersNumber / 2; i++)
    {
        free(messageToSendByLine[i]);
    }

    free(buffer);

    return 0;
}