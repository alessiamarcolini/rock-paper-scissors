#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"

int bytesRead;
char message[MAXLEN];
char *messageToSendByLine[MAXLEN];
char *buffer;

int main(int argc, char *argv[])
{

    buffer = malloc(sizeof(char) * 1024);
    int playersNumber = atoi(argv[1]);
    char *season = argv[argc - 1];
    //printf("sono la giornata\n");
    //printf("%s\n", argv[1]);
    int i;

    for (i = 0; i < playersNumber / 2; i++)
    {
        //sleep(1);
        int fd[2];
        pipe(fd); /* Create unnamed pipe */
        pid_t pid = fork();

        if (pid > 0)
        { // day
            //printf("Sono il padre\n");

            close(fd[WRITE]); /* close other side */
            bytesRead = read(fd[READ], message, MAXLEN);

            sprintf(buffer, "%d %s\n", i, message); // dayId|message // TODO: mandare in blocco!!!!
            messageToSendByLine[i] = malloc(sizeof(char) * 1024);
            strcpy(messageToSendByLine[i], buffer);
            free(buffer);
            close(fd[READ]); /* close this side */
            free(buffer);
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

            char *const paramList[] = {"bin/match", str, firstPlayer, secondPlayer, NULL};
            int e = execv(paramList[0], paramList);
        }
    }

    char messageToSend[MAXLEN];
    strcat(messageToSend, "");
    for (i = 0; i < playersNumber / 2; i++)
    {
        //printf("%s", messageToSendByLine[i]);
        strcat(messageToSend, messageToSendByLine[i]);
    }
    printf("%s", messageToSend);

<<<<<<< HEAD
    for (i = 0; i < playersNumber / 2; i++) {
=======
    for (i = 0; i < playersNumber / 2; i++)
    {
>>>>>>> fix-free
        free(messageToSendByLine[i]);
    }

    return 0;
}