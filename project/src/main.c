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

char buffer[MAXLEN]; // used to convert int to str

int i, j;

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
        { // parent
            fprintf(stderr, "Sono il padre\n");

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
            //char ** paramList = malloc((playersNumber+3) * sizeof(char));

            //for (j = 0; j < playersNumber+3; j++){
            //   paramList[j] = malloc(256);
            //}

            paramList[0] = "bin/day";
            paramList[1] = playersNumber_str;
            paramList[playersNumber + 2] = NULL;

            //fork

            //char dayMatches[playersNumber*2];
            for (j = 0; j < playersNumber; j++)
            { // single day

                //printf("j=%d ", j);
                fprintf(stderr, "j=%d ", j);
                sprintf(buffer, "%d", championship[i][j]);

                //printf("%s ", buffer);
                fprintf(stderr, "%s ", buffer);
                //itoa(championship[i][j], buffer, 10);

                //strcpy(paramList[j], buffer);
                //strcpy(paramList[j + 2], buffer);
                paramList[j + 2] = buffer;
                buffer[0] = '\0';
            }
            fprintf(stderr, "\nparam list content: ");
            for (j = 0; j < playersNumber; j++)
            {
                //fprintf(stderr, "%s ", paramList[j]);
            }

            int e = execv(paramList[0], paramList);
            // wait
        }
    }

    return 0;
}