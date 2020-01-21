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
int even;

int main(int argc, char *argv[])
{
    buffer = malloc(sizeof(char) * 1024);
    char *playersNumber_str = malloc(sizeof(char) * 1024);
    int playersNumber = atoi(argv[1]);
    if (playersNumber % 2 != 0)
    {
        even = FALSE;
        playersNumber++;
    }
    else
    {
        even = TRUE;
    }
    snprintf(playersNumber_str, 1024, "%d", playersNumber);

    //arrays for calculate rate and parity
    int score[playersNumber];
    int differencePoints[playersNumber];
    int scoreBoardA[playersNumber][playersNumber]; //dove salveremo i punteggi d'andata
    int scoreBoardR[playersNumber][playersNumber]; //dove salveremo i punteggi di ritorno

    //initialization of score board matrix
    for (i = 0; i < playersNumber; i++)
    {
        for (j = 0; j < playersNumber; j++)
        {
            scoreBoardA[i][j] = 0;
            scoreBoardR[i][j] = 0;
        }
    }

    //initialization of score array and points difference array
    for (i = 0; i < playersNumber; i++)
    { //da deallocare
        //score[i] = malloc(sizeof(int) * 1024);
        score[i] = 0;
        //differencePoints[i] = malloc(sizeof(int) * 1024);
        differencePoints[i] = 0;
    }

    matching(playersNumber, championship, even); // matrix playersNumber x playersNumber-1

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

    for (k = 0; k < 2; k++)
    {                                           //forward and return (fix rewrite on same buffer and re-do of same matrix)
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
                int resultsTokenized[(MAINSTREAMLEN) * (playersNumber / 2)]; // *(playersNumber/2) dovuto al fatto che legge tutto lo stream della giornata, e non il singolo match
                tokenizer(message, resultsTokenized, " ", (MAINSTREAMLEN) * (playersNumber / 2));
                /*fprintf(stderr, "\n- tokenized array: ");
                for(j = 0; j < MAINSTREAMLEN*(playersNumber/2); j++) {
                    fprintf(stderr, "%d -", resultsTokenized[j]);
                }*/

                int homePlayer;
                int externalPlayer;
                int winFirstPlayer;
                int winSecondPlayer;
                for (j = 0; j < playersNumber / 2; j++)
                {
                    homePlayer = resultsTokenized[j * MAINSTREAMLEN + 1];
                    externalPlayer = resultsTokenized[j * MAINSTREAMLEN + 2];

                    if (homePlayer != 99 && externalPlayer != 99)
                    {
                        winFirstPlayer = resultsTokenized[j * MAINSTREAMLEN + 5];
                        winSecondPlayer = resultsTokenized[j * MAINSTREAMLEN + 6];

                        score[homePlayer] = score[homePlayer] + resultsTokenized[j * MAINSTREAMLEN + 3];                                                 //resultsTokenized[1] = firstPlayerId, resultsTokenized[3] = sumPointsFirstPlayer
                        differencePoints[homePlayer] = differencePoints[homePlayer] + winFirstPlayer - resultsTokenized[j * MAINSTREAMLEN + 7];          //resultsTokenized[5] = numberOfWinFirstPlayer, resultsTokenized[7] = numberOfLoseFirstPlayer
                        score[externalPlayer] = score[externalPlayer] + resultsTokenized[j * MAINSTREAMLEN + 4];                                         //resultsTokenized[2] = secondPlayerId, resultsTokenized[4] = sumPointsSecondPlayer
                        differencePoints[externalPlayer] = differencePoints[externalPlayer] + winSecondPlayer - resultsTokenized[j * MAINSTREAMLEN + 8]; //resultsTokenized[6] = numberOfWinSecondPlayer, resultsTokenized[8] = numberOfLoseSecondPlayer

                        if (k == 0)
                        {
                            // andata
                            scoreBoardA[homePlayer][externalPlayer] += winFirstPlayer;
                            scoreBoardA[externalPlayer][homePlayer] += winSecondPlayer;
                        }
                        else if (k == 1)
                        {
                            scoreBoardR[homePlayer][externalPlayer] += winFirstPlayer;
                            scoreBoardR[externalPlayer][homePlayer] += winSecondPlayer;
                        }
                    }
                }
                if (!even)
                {
                    score[playersNumber - 1] = -1;
                    differencePoints[playersNumber - 1] = -1000000;
                }

                fprintf(stderr, "\n");
                for (j = 0; j < playersNumber; j++)
                {
                    fprintf(stderr, "score:%d - ", score[j]);
                }
                fprintf(stderr, "\n");
                for (j = 0; j < playersNumber; j++)
                {
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
                if (k == 0)
                {
                    paramList[playersNumber + 2] = "A";
                }
                else
                {
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
    printf("\nandata\n");
    for (i = 0; i < playersNumber; i++)
    {
        for (j = 0; j < playersNumber; j++)
        {
            printf("i: %d, j: %d: , value: %d\n", i, j, scoreBoardA[i][j]);
        }
        printf("\n");
    }
    printf("\nritorno\n");
    for (i = 0; i < playersNumber; i++)
    {
        for (j = 0; j < playersNumber; j++)
        {
            printf("i: %d, j: %d: , value: %d\n", i, j, scoreBoardR[i][j]);
        }
        printf("\n");
    }

    //leaderboard calculation

    int leaderboard[8];
    int parityCheck[playersNumber];
    int playersInParity[playersNumber]; // points

    int max;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < playersNumber; j++)
        {
            parityCheck[j] = -1;
            playersInParity[j] = -1;
        }
        max = -1;
        int parityCount = 0;

        for (j = 0; j < playersNumber; j++)
        {
            if (score[j] > max)
            {
                max = score[j];
                //score[j] = -1;
            }
        }

        for (j = 0; j < playersNumber; j++)
        {
            if (score[j] == max)
            {
                parityCheck[j] = 1;
                parityCount++;
            }
        }
        if (parityCount == 1)

        { // only one max
            for (j = 0; j < playersNumber; j++)
            {
                if (parityCheck[j] == 1)
                {
                    leaderboard[i] = j;
                    score[j] = -1;
                    printf("sto inserendo fase 1 j=%d\n", j);
                    break;
                }
            }
        }
        else // more than one player with same points
        {
            printf("1. risolvo pari in score - parity count %d\n", parityCount);
            for (j = 0; j < playersNumber; j++)
            {
                if (parityCheck[j] == 1)
                {
                    for (k = j + 1; k < playersNumber; k++)
                    {
                        if (parityCheck[k] == 1)
                        {
                            // scontro j vs k
                            // calcolo sia andata che ritorno
                            if (scoreBoardA[j][k] > scoreBoardA[k][j])
                            {
                                if (playersInParity[j] == -1)
                                {
                                    playersInParity[j] = 2;
                                }
                                else
                                {
                                    playersInParity[j] += 2;
                                }
                            }
                            else if (scoreBoardA[j][k] == scoreBoardA[k][j])
                            {
                                if (playersInParity[j] == -1)
                                {
                                    playersInParity[j] = 1;
                                }
                                else
                                {
                                    playersInParity[j]++;
                                }
                                if (playersInParity[k] == -1)
                                {
                                    playersInParity[k] = 1;
                                }
                                else
                                {
                                    playersInParity[k]++;
                                }
                            }
                            else
                            {
                                if (playersInParity[k] == -1)
                                {
                                    playersInParity[k] = 2;
                                }
                                else
                                {
                                    playersInParity[k] += 2;
                                }
                            }
                            if (scoreBoardR[j][k] > scoreBoardR[k][j])
                            {
                                if (playersInParity[j] == -1)
                                {
                                    playersInParity[j] = 2;
                                }
                                else
                                {
                                    playersInParity[j] += 2;
                                }
                            }
                            else if (scoreBoardR[j][k] == scoreBoardR[k][j])
                            {
                                if (playersInParity[j] == -1)
                                {
                                    playersInParity[j] = 1;
                                }
                                else
                                {
                                    playersInParity[j]++;
                                }
                                if (playersInParity[k] == -1)
                                {
                                    playersInParity[k] = 1;
                                }
                                else
                                {
                                    playersInParity[k]++;
                                }
                            }
                            else if (playersInParity[k] == -1)
                            {
                                playersInParity[k] = 2;
                            }
                            else
                            {
                                playersInParity[k] += 2;
                            }
                            {
                            }
                        }
                    }
                }
            }
            // check players still in parity
            // find max

            max = -1;
            parityCount = 0;
            int playersInParity2[playersNumber];

            for (j = 0; j < playersNumber; j++)
            {
                playersInParity2[j] = -1;
            }

            for (j = 0; j < playersNumber; j++)
            {
                if (playersInParity[j] != -1)
                {
                    if (playersInParity[j] > max)
                    {
                        max = playersInParity[j];
                    }
                }
            }
            for (j = 0; j < playersNumber; j++)
            {
                if (playersInParity[j] == max)
                {
                    parityCount++;

                    playersInParity2[j] = 1; // store still in parity
                }
            }
            if (parityCount == 1) // found first one
            {
                for (j = 0; j < playersNumber; j++)

                {
                    printf("p1:%d\tp2:%d ", playersInParity[j], playersInParity2[j]);
                    if (playersInParity2[j] == 1)
                    {
                        leaderboard[i] = j;
                        score[j] = -1;
                        printf("sto inserendo fase 2 j=%d\n", j);
                        break;
                    }
                }
            }
            else
            {
                printf("2. risolvo pari in score board - parity count %d\n", parityCount);
                max = -10000;
                parityCount = 0;
                for (j = 0; j < playersNumber; j++)
                {
                    if (playersInParity2[j] == 1)
                    {
                        if (differencePoints[j] > max)
                        {
                            max = differencePoints[j];
                        }
                    }
                }
                int playersInParity3[playersNumber];
                for (j = 0; j < playersNumber; j++)
                {
                    playersInParity3[j] = -1;
                }
                for (j = 0; j < playersNumber; j++)
                {
                    if (differencePoints[j] == max && playersInParity2[j] == 1)
                    {
                        parityCount++;
                        playersInParity3[j] = 1;
                    }
                }
                if (parityCount == 1)
                {
                    for (j = 0; j < playersNumber; j++)
                    {
                        if (playersInParity3[j] == 1)
                        {
                            leaderboard[i] = j;
                            score[j] = -1;
                            printf("sto inserendo fase 3 j=%d\n", j);
                            break;
                        }
                    }
                }
                else
                {
                    printf("3. risolvo pari in diff - parity count %d, max:%d\n", parityCount, max);
                    //sorteggio
                    int sortedIndex;
                    // paritycount contiene quanti squadre pari ho
                    //for (k = 0; k < parityCount; k++)
                    //{
                    sortedIndex = rand() % parityCount;
                    printf("sorted index %d\n", sortedIndex);
                    for (j = 0; j < playersNumber; j++)
                    {
                        if (playersInParity3[j] == 1)
                        {
                            if (sortedIndex == 0)
                            {
                                leaderboard[i] = j;
                                score[j] = -1;
                                printf("sto inserendo fase 4 j=%d\n", j);
                                break;
                                //i++;
                            }
                            else
                            {
                                sortedIndex--;
                            }
                        }
                    }
                    //}
                    //i--;
                }
            }
        }
    }

    /////// we got the classifica
    printf("classifica!!\n");
    for (i = 0; i < 8; i++)
    {
        printf("%d ", leaderboard[i]);
    }

    permute(leaderboard, 8);

    printf("after permute!!\n");
    for (i = 0; i < 8; i++)
    {
        printf("%d ", leaderboard[i]);
    }
    return 0;
}