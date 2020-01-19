#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

#define READ 0  /* read-side of pipes */
#define WRITE 1 /* write-side of pipes */

int bytesRead;
char message[MAXLEN];

int main(int argc, char *argv[])
{
    int playersNumber = atoi(argv[1]);
    //printf("sono la giornata\n");
    //printf("%s\n", argv[1]);
    int i;

    //ciclo per la creazione del numero di match, testing

    for (i = 0; i < playersNumber / 2; i++)
    {
        sleep(1);
        int fd[2];
        pipe(fd); /* Create unnamed pipe */
        pid_t pid = fork();

        if (pid > 0)
        { // day
            //printf("Sono il padre\n");

            close(fd[WRITE]); /* close other side */
            bytesRead = read(fd[READ], message, MAXLEN);
            printf("%d %s\n", i, message); // dayId|message

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

            char *firstPlayer = argv[i * 2 + 2];
            char *secondPlayer = argv[i * 2 + 3];

            char *const paramList[] = {"bin/match", str, firstPlayer, secondPlayer, NULL};
            int e = execv(paramList[0], paramList);
        }
    }

    return 0;
}