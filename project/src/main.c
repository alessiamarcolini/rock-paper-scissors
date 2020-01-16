#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#define READ 0  /* read-side of pipes */
#define WRITE 1 /* write-side of pipes */

//setbuf(stdout, NULL);

int championship[MAXN][MAXN];

int fd[2], bytesRead;
char message[MAXLEN];

char *buffer; // used to convert int to str

int i, j;

int main(int argc, char *argv[])
{
    buffer = malloc(sizeof(char) * 1024);
    char *playersNumber_str = argv[1];
    int playersNumber = atoi(playersNumber_str);

    matching(playersNumber, championship); // matrix playersNumber x playersNumber-1

    //printf("championship\n");
    for (int i = 0; i < playersNumber - 1; i++)
    {
        for (int j = 0; j < playersNumber; j++)
        {
            fprintf(stderr, "%d", championship[i][j]);
            fprintf(stderr, " ");
        }
        fprintf(stderr, "\n");
    }

    pid_t pid;
    int status; // return status of child

    for (i = 0; i < playersNumber - 1; i++) // for each day
    {
        if (i != 0)
        {
            waitpid(pid, &status, 0);
        }
        pipe(fd); /* Create unnamed pipe */
        //printf("\n\nSto forkando\n");

        pid = fork();

        if (pid > 0)
        {                     // parent
            close(fd[WRITE]); /* close other side */
            bytesRead = read(fd[READ], message, MAXLEN);
            fprintf(stderr, "- main: Read %d bytes: %s\n", bytesRead, message);
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

            //fork

            //char dayMatches[playersNumber*2];
            for (j = 0; j < playersNumber; j++)
            { // single day
                snprintf(buffer, 1024, "%d", championship[i][j]);

                paramList[j + 2] = malloc(sizeof(char) * 1024);
                // TODO: fix memory leak
                strcpy(paramList[j + 2], buffer);
            }

            int e = execv(paramList[0], paramList);
            // wait
        }
    }

    return 0;
}