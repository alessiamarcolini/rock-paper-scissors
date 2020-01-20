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

char *buffer; // used to convert int to str

int i, j, k;

int main(int argc, char *argv[])
{
    buffer = malloc(sizeof(char) * 1024);
    char *playersNumber_str = argv[1];
    int playersNumber = atoi(playersNumber_str);

    //arrays for calculate rate and parity
    int score[playersNumber];
    int differencePoints[playersNumber];
    int scoreBoard[playersNumber][playersNumber]; //dove salveremo i punteggi d'andata per poi sommarli al ritorno, utili per il calcolo degli scontri diretti

    //initialization of score board matrix
    for(i = 0; i < playersNumber; i++) {
        for(j = 0; j < playersNumber; j++) {
            scoreBoard[i][j] = 0;
        }
    }

    //initialization of score array and points difference array
    for(i = 0; i < playersNumber; i++) { //da deallocare
        //score[i] = malloc(sizeof(int) * 1024);
        score[i] = 0;
        //differencePoints[i] = malloc(sizeof(int) * 1024);
        differencePoints[i] = 0; 
    }

    matching(playersNumber, championship); // matrix playersNumber x playersNumber-1

    //printf("championship\n");
    for (i = 0; i < playersNumber - 1; i++)
    {
        for (j = 0; j < playersNumber; j++)
        {
            fprintf(stderr, "%d", championship[i][j]);
            fprintf(stderr, " ");
        }
        fprintf(stderr, "\n");
    }

    pid_t pid;
    int status; // return status of child

    for (k = 0; k < 2; k++) { //forward and return (fix rewrite on same buffer and re-do of same matrix)
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
                fprintf(stderr, "\n- main: Read %d bytes: \n%s", bytesRead, message);
                
                //parsing and upload on the points structures
                int resultsTokenized[(MAINSTREAMLEN)*(playersNumber/2)]; // *(playersNumber/2) dovuto al fatto che legge tutto lo stream della giornata, e non il singolo match
                tokenizer(message, resultsTokenized, " ", (MAINSTREAMLEN)*(playersNumber/2));
                /*fprintf(stderr, "\n- tokenized array: ");
                for(j = 0; j < MAINSTREAMLEN*(playersNumber/2); j++) {
                    fprintf(stderr, "%d -", resultsTokenized[j]);
                }*/
                
                int homePlayer;
                int externalPlayer;
                int winFirstPlayer;
                int winSecondPlayer;
                for(j = 0; j < playersNumber/2; j++) {
                    homePlayer = resultsTokenized[j*MAINSTREAMLEN+1];
                    externalPlayer = resultsTokenized[j*MAINSTREAMLEN+2];
                    winFirstPlayer = resultsTokenized[j*MAINSTREAMLEN+5];
                    winSecondPlayer = resultsTokenized[j*MAINSTREAMLEN+6];

                    score[homePlayer] = score[homePlayer] + resultsTokenized[j*MAINSTREAMLEN+3]; //resultsTokenized[1] = firstPlayerId, resultsTokenized[3] = sumPointsFirstPlayer
                    differencePoints[homePlayer] = differencePoints[homePlayer] + winFirstPlayer - resultsTokenized[j*MAINSTREAMLEN+7]; //resultsTokenized[5] = numberOfWinFirstPlayer, resultsTokenized[7] = numberOfLoseFirstPlayer
                    score[externalPlayer] = score[externalPlayer] + resultsTokenized[j*MAINSTREAMLEN+4]; //resultsTokenized[2] = secondPlayerId, resultsTokenized[4] = sumPointsSecondPlayer
                    differencePoints[externalPlayer] = differencePoints[externalPlayer] + winSecondPlayer - resultsTokenized[j*MAINSTREAMLEN+8]; //resultsTokenized[6] = numberOfWinSecondPlayer, resultsTokenized[8] = numberOfLoseSecondPlayer    
                    
                    scoreBoard[homePlayer][externalPlayer] += winFirstPlayer;
                    scoreBoard[externalPlayer][homePlayer] += winSecondPlayer;
                }
                
                fprintf(stderr, "\n");
                for(j = 0; j < playersNumber; j++) {
                    fprintf(stderr, "score:%d - ", score[j]);
                }
                fprintf(stderr, "\n");
                for(j = 0; j < playersNumber; j++) {
                    fprintf(stderr, "diff:%d - ", differencePoints[j]);
                }
                fprintf(stderr, "\n");

                close(fd[READ]); /* close this side */
            }
            else
            { // child
                //printf("%d\n", fd);
                dup2(fd[WRITE], WRITE);
                close(fd[READ]);
                close(fd[WRITE]);
                //char *const paramList[] = {"bin/day", playersNumber_str, NULL};

                char *paramList[playersNumber + 4];

                paramList[0] = "bin/day";
                paramList[1] = playersNumber_str;
                if(k==0) {
                    paramList[playersNumber + 2] = "A";
                } else {
                    paramList[playersNumber + 2] = "R";
                }
                paramList[playersNumber + 3] = NULL;

                //fork

                //char dayMatches[playersNumber*2];
                for (j = 0; j < playersNumber; j++)
                { // single day
                    snprintf(buffer, 1024, "%d", championship[i][j]);

                    paramList[j + 2] = malloc(sizeof(char) * 1024);
                    // TODO: fix memory leak
                    strcpy(paramList[j + 2], buffer);
                    //fprintf(stderr, "-----%s-----", buffer);
                    //paramList[j + 2] = buffer;
                    //buffer[0] = '\0';
                }

                int e = execv(paramList[0], paramList);
                // wait
            }
        }
    }
    printf("\n");
    for(i = 0; i < playersNumber; i++) {
        for(j=0; j < playersNumber; j++) {
            printf("i: %d, j: %d: , value: %d\n", i, j, scoreBoard[i][j]);
        }
        printf("\n");
    }
    return 0;
}