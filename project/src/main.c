#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#define READ 0  /* read-side of pipes */
#define WRITE 1 /* write-side of pipes */

int championship[MAXN][MAXN];

int fd[2], bytesRead;
char message[MAXLEN];

char buffer[MAXLEN]; // used to convert int to str

int main(int argc, char *argv[])
{
    char *playersNumber_str = argv[1];
    int playersNumber = atoi(playersNumber_str);

    matching(playersNumber, championship); // matrix playersNumber x playersNumber-1

    //printf("championship\n");
    for (int i = 0; i < playersNumber - 1; i++)
    {
        for (int j = 0; j < playersNumber; j++)
        {
            printf("%d", championship[i][j]);
            printf(" ");
        }
        printf("\n");
    }

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
        //char *const paramList[] = {"bin/day", playersNumber_str, NULL};

        char *paramList[playersNumber + 3];
        paramList[0] = "bin/day";
        paramList[1] = playersNumber_str;
        paramList[playersNumber + 2] = NULL;

        int i, j;

        for (i = 0; i < playersNumber - 1; i++) // for each day
        {
            for (j = 0; j < playersNumber; j++)
            { // single day
                sprintf(buffer, "%d", championship[i][j]);

                printf("%s\n", buffer);
                //itoa(championship[i][j], buffer, 10);
                strcpy(paramList[j + 2], buffer);
            }

            int e = execv(paramList[0], paramList);
            // wait
        }
    }

    return 0;
}