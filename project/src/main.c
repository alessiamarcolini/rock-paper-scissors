#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "utils.h"

int championship[MAXN][MAXN];

int fd[2], bytesRead;
char message[MAXLEN];
char messageQuarters[MAXLEN];
char messageSemiFinals[MAXLEN];
char messageFinals[MAXLEN];

char *winnersQuarters[nQuarters];
char *winnersSemiFinals[nSemiFinals];

char *buffer; // used to convert int to str

int i, j, k;
int even;

int main(int argc, char *argv[])
{
    char *param = argv[1];

    // check if number of players is an integer
    for (i = 0; param[i] != '\0'; i++)
    {
        if (!isdigit(param[i]))
        {
            fprintf(stderr, "You must specify an integer number of players!\n");
            exit(1);
        }
    }

    buffer = malloc(sizeof(char) * 1024);
    if (buffer == NULL)
    {
        fprintf(stderr, "Malloc failure: dinamic memory allocation not possible.\n");
        exit(6);
    }

    char *playersNumber_str = malloc(sizeof(char) * 1024);
    if (playersNumber_str == NULL)
    {
        fprintf(stderr, "Malloc failure: dinamic memory allocation not possible.\n");
        exit(6);
    }
    int playersNumber = atoi(argv[1]);

    // check if player number is >= 8
    if (playersNumber < 8)
    {
        fprintf(stderr, "You need at least 8 players!\n");
        exit(2);
    }

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

    printf("Number of days: %d\n", playersNumber - 1);

    int score[playersNumber];                      // cumulative (per day) number of points per player
    int differencePoints[playersNumber];           // difference between wins and losses per player
    int scoreBoardA[playersNumber][playersNumber]; // "andata" scoreboard
    int scoreBoardR[playersNumber][playersNumber]; // "ritorno" scoreboard

    //initialization of score board matrices
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
    {
        score[i] = 0;
        differencePoints[i] = 0;
    }

    matching(playersNumber, championship, even); // matrix playersNumber x playersNumber-1

    /*for (i = 0; i < playersNumber - 1; i++)
    {
        //printf("Day %d: ", i+1);
        for (j = 0; j < playersNumber; j++)
        {

            printf("%d ", championship[i][j]);
            fprintf(stderr, " ");
        }
        fprintf(stderr, "\n");
    }
    */

    pid_t pid;
    pid_t wait_pid;
    int status; // return status of child

    for (k = 0; k < 2; k++)
    {
        if (k == 0)
        {
            printf("\n----- FORWARD / ANDATA -----\n\n");
        }
        else
        {
            printf("\n----- RETURN / RITORNO -----\n\n");
        }
        //forward and return (fix rewrite on same buffer and re-do of same matrix)
        for (i = 0; i < playersNumber - 1; i++) // for each day
        {
            if (i != 0)
            {
                wait_pid = waitpid(pid, &status, 0);
                if (wait_pid == -1)
                {
                    fprintf(stderr, "Child process failed.\n");
                    exit(8);
                }
            }
            int e = pipe(fd);
            if (e < 0)
            {
                fprintf(stderr, "Error pipe: %s\n", strerror(errno));
                exit(5);
            }

            printf("DAY %d:\n", (i + 1));

            pid = fork();

            if (pid == -1)
            {
                fprintf(stderr, "Error while forking.\n");
                exit(3);
            }

            if (pid > 0)
            { // parent
                close(fd[WRITE]);
                bytesRead = read(fd[READ], message, MAXLEN);
                if (bytesRead <= 0)
                {
                    fprintf(stderr, "Error reading from pipe.\n");
                    exit(8);
                }
                //fprintf(stderr, "\n- main: Read %d bytes: \n%s", bytesRead, message);

                //parsing and upload on the points structures
                int resultsTokenized[(MAINSTREAMLEN) * (playersNumber / 2)]; // *(playersNumber/2) dovuto al fatto che legge tutto lo stream della giornata, e non il singolo match
                tokenizer(message, resultsTokenized, " ", (MAINSTREAMLEN) * (playersNumber / 2));

                int homePlayer;
                int externalPlayer;
                int winFirstPlayer;
                int winSecondPlayer;
                for (j = 0; j < playersNumber / 2; j++)
                {
                    homePlayer = resultsTokenized[j * MAINSTREAMLEN + 1];
                    externalPlayer = resultsTokenized[j * MAINSTREAMLEN + 2];

                    printf("\t%d vs %d\t", homePlayer, externalPlayer);

                    if (homePlayer != 99 && externalPlayer != 99)
                    {
                        winFirstPlayer = resultsTokenized[j * MAINSTREAMLEN + 5];
                        winSecondPlayer = resultsTokenized[j * MAINSTREAMLEN + 6];
                        printf("%d - %d\n", winFirstPlayer, winSecondPlayer);

                        score[homePlayer] = score[homePlayer] + resultsTokenized[j * MAINSTREAMLEN + 3];
                        //resultsTokenized[1] = firstPlayerId, resultsTokenized[3] = sumPointsFirstPlayer
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

                /*fprintf(stderr, "\n");
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
*/
            }
            else
            { // child

                int e = dup2(fd[WRITE], WRITE);
                if (e < 0)
                {
                    fprintf(stderr, "Error while duplicating file descriptor: %s\n", strerror(errno));
                    exit(10);
                }
                close(fd[READ]);
                close(fd[WRITE]);

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

                for (j = 0; j < playersNumber; j++)
                { // single day
                    snprintf(buffer, 1024, "%d", championship[i][j]);

                    paramList[j + 2] = malloc(sizeof(char) * 1024);
                    if (paramList[j + 2] == NULL)
                    {
                        fprintf(stderr, "Malloc failure: dinamic memory allocation not possible.\n");
                        exit(6);
                    }
                    strcpy(paramList[j + 2], buffer);
                }

                e = execv(paramList[0], paramList);
                if (e < 0)
                {
                    fprintf(stderr, "Error execv: %s\n", strerror(errno));
                    exit(4);
                }
            }
        }
    }
    /*
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
*/
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
                    //printf("sto inserendo fase 1 j=%d\n", j);
                    break;
                }
            }
        }
        else // more than one player with same points
        {
            //printf("1. risolvo pari in score - parity count %d\n", parityCount);
            for (j = 0; j < playersNumber; j++)
            {
                if (parityCheck[j] == 1)
                {
                    for (k = j + 1; k < playersNumber; k++)
                    {
                        if (parityCheck[k] == 1)
                        {
                            // j vs k
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
                    //printf("p1:%d\tp2:%d ", playersInParity[j], playersInParity2[j]);
                    if (playersInParity2[j] == 1)
                    {
                        leaderboard[i] = j;
                        score[j] = -1;
                        //printf("sto inserendo fase 2 j=%d\n", j);
                        break;
                    }
                }
            }
            else
            {
                //printf("2. risolvo pari in score board - parity count %d\n", parityCount);
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
                            //printf("sto inserendo fase 3 j=%d\n", j);
                            break;
                        }
                    }
                }
                else
                {
                    //printf("3. risolvo pari in diff - parity count %d, max:%d\n", parityCount, max);
                    //sorteggio
                    int sortedIndex;
                    // paritycount contiene quanti squadre pari ho

                    sortedIndex = rand() % parityCount;

                    for (j = 0; j < playersNumber; j++)
                    {
                        if (playersInParity3[j] == 1)
                        {
                            if (sortedIndex == 0)
                            {
                                leaderboard[i] = j;
                                score[j] = -1;
                                break;
                            }
                            else
                            {
                                sortedIndex--;
                            }
                        }
                    }
                }
            }
        }
    }

    printf("\n----- LEADERBOARD -----\n");
    for (i = 0; i < 8; i++)
    {
        printf("%d ", leaderboard[i]);
    }
    printf("\n");
    permute(leaderboard, 8);

    // quarterfinals

    printf("\n----- QUARTER FINALS -----\n");

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
    {
        close(fd[WRITE]);
        //open(fd[READ]);
        //message[0] = '\0';
        bytesRead = read(fd[READ], messageQuarters, MAXLEN);
        if (bytesRead <= 0)
        {
            fprintf(stderr, "Error reading from pipe.\n");
            exit(8);
        }
        //fprintf(stderr, "\n- main2: Read %d bytes: \n%s", bytesRead, messageQuarters);

        char *messageTokenized[MAINSTREAMLEN * nQuarters]; // *(playersNumber/2) dovuto al fatto che legge tutto lo stream della giornata, e non il singolo match
        //tokenizer(messageQuarters, messageTokenized, " ", (MAINSTREAMLEN)*4);
        tokenizerMultipleDelimiter(messageQuarters, messageTokenized);

        for (j = 0; j < nQuarters; j++)
        {
            char *firstPlayer = messageTokenized[j * 6 + 1];
            char *secondPlayer = messageTokenized[j * 6 + 2];
            char *winner = messageTokenized[j * 6 + 3];
            char *firstSign = messageTokenized[j * 6 + 4];
            char *secondSign = messageTokenized[j * 6 + 5];

            printf("\t%s vs %s\t %s - %s\n", firstPlayer, secondPlayer, firstSign, secondSign);
            winnersQuarters[j] = winner;
            //fprintf(stderr, "winner: %s\n", winner);
        }
    }
    else
    {
        int e = dup2(fd[WRITE], WRITE);
        if (e < 0)
        {
            fprintf(stderr, "Error while duplicating file descriptor: %s\n", strerror(errno));
            exit(10);
        }
        close(fd[READ]);
        close(fd[WRITE]);

        char *paramList[nQuarters * 2 + 3];

        paramList[0] = "bin/quarterFinals";
        paramList[1] = "8";
        paramList[playersNumber + 3] = NULL;

        for (i = 0; i < 8; i++)
        {
            snprintf(buffer, 1024, "%d", leaderboard[i]);

            paramList[i + 2] = malloc(sizeof(char) * 1024);
            if (paramList[i + 2] == NULL)
            {
                fprintf(stderr, "Malloc failure: dinamic memory allocation not possible.\n");
                exit(6);
            }

            strcpy(paramList[i + 2], buffer);
        }

        e = execv(paramList[0], paramList);
        if (e < 0)
        {
            fprintf(stderr, "Error execv: %s\n", strerror(errno));
            exit(4);
        }
    }
    wait_pid = waitpid(pid, &status, 0);
    if (wait_pid == -1)
    {
        fprintf(stderr, "Child process failed.\n");
        exit(8);
    }

    printf("\nRemaining players: ");
    for (i = 0; i < nQuarters; i++)
    {
        printf("%s ", winnersQuarters[i]);
    }
    printf("\n");
    // semi finals
    printf("\n----- SEMI FINALS -----\n");
    e = pipe(fd);
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
    {
        close(fd[WRITE]);
        //open(fd[READ]);
        //message[0] = '\0';
        bytesRead = read(fd[READ], messageSemiFinals, MAXLEN);
        if (bytesRead <= 0)
        {
            fprintf(stderr, "Error reading from pipe.\n");
            exit(8);
        }
        //fprintf(stderr, "\n- main3: Read %d bytes: \n%s", bytesRead, messageSemiFinals);

        char *messageTokenized[MAINSTREAMLEN * 4]; // *(playersNumber/2) dovuto al fatto che legge tutto lo stream della giornata, e non il singolo match
        //tokenizer(messageSemiFinals, messageTokenized, " ", (MAINSTREAMLEN)*4);
        tokenizerMultipleDelimiter(messageSemiFinals, messageTokenized);

        for (j = 0; j < nSemiFinals; j++)
        {
            char *firstPlayer = messageTokenized[j * 6 + 1];
            char *secondPlayer = messageTokenized[j * 6 + 2];
            char *winner = messageTokenized[j * 6 + 3];
            char *firstSign = messageTokenized[j * 6 + 4];
            char *secondSign = messageTokenized[j * 6 + 5];

            winnersSemiFinals[j] = winner;
            printf("\t%s vs %s\t %s - %s\n", firstPlayer, secondPlayer, firstSign, secondSign);
        }
    }
    else
    {
        int e = dup2(fd[WRITE], WRITE);
        if (e < 0)
        {
            fprintf(stderr, "Error while duplicating file descriptor: %s\n", strerror(errno));
            exit(10);
        }
        close(fd[READ]);
        close(fd[WRITE]);

        char *paramList[nSemiFinals * 2 + 3];

        paramList[0] = "bin/semiFinals";
        paramList[1] = "4";
        paramList[playersNumber + 3] = NULL;

        for (i = 0; i < nSemiFinals * 2; i++)
        {
            //snprintf(buffer, 1024, "%d", leaderboard[i]);

            paramList[i + 2] = malloc(sizeof(char) * 1024);
            if (paramList[i + 2] == NULL)
            {
                fprintf(stderr, "Malloc failure: dinamic memory allocation not possible.\n");
                exit(6);
            }
            strcpy(paramList[i + 2], winnersQuarters[i]);
        }

        e = execv(paramList[0], paramList);
        if (e < 0)
        {
            fprintf(stderr, "Error execv: %s\n", strerror(errno));
            exit(4);
        }
    }
    wait_pid = waitpid(pid, &status, 0);
    if (wait_pid == -1)
    {
        fprintf(stderr, "Child process failed.\n");
        exit(8);
    }

    printf("\nRemaining players: ");
    for (i = 0; i < nSemiFinals; i++)
    {
        printf("%s ", winnersSemiFinals[i]);
    }
    printf("\n");

    // finals

    printf("\n----- FINALS -----\n");

    e = pipe(fd);
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
    {
        close(fd[WRITE]);
        //open(fd[READ]);
        //message[0] = '\0';
        bytesRead = read(fd[READ], messageFinals, MAXLEN);
        if (bytesRead <= 0)
        {
            fprintf(stderr, "Error reading from pipe.\n");
            exit(8);
        }
        //fprintf(stderr, "\n- main4: Read %d bytes: \n%s", bytesRead, messageFinals);

        char *messageTokenized[MAINSTREAMLEN * 1]; // *(playersNumber/2) dovuto al fatto che legge tutto lo stream della giornata, e non il singolo match
        tokenizerMultipleDelimiter(messageFinals, messageTokenized);

        char *firstPlayer = messageTokenized[1];
        char *secondPlayer = messageTokenized[2];
        char *winner = messageTokenized[3];
        char *firstSign = messageTokenized[4];
        char *secondSign = messageTokenized[5];
        printf("\t%s vs %s\t %s - %s\n", firstPlayer, secondPlayer, firstSign, secondSign);

        printf("\n\nTHE WINNER IS: %s\n", winner);
    }
    else
    {
        int e = dup2(fd[WRITE], WRITE);
        if (e < 0)
        {
            fprintf(stderr, "Error while duplicating file descriptor: %s\n", strerror(errno));
            exit(10);
        }
        close(fd[READ]);
        close(fd[WRITE]);

        char *paramList[2 + 3];

        paramList[0] = "bin/finals";
        paramList[1] = "2";
        paramList[playersNumber + 3] = NULL;

        for (i = 0; i < 2; i++)
        {

            paramList[i + 2] = malloc(sizeof(char) * 1024);
            if (paramList[i + 2] == NULL)
            {
                fprintf(stderr, "Malloc failure: dinamic memory allocation not possible.\n");
                exit(6);
            }
            strcpy(paramList[i + 2], winnersSemiFinals[i]);
        }

        e = execv(paramList[0], paramList);
        if (e < 0)
        {
            fprintf(stderr, "Error execv: %s\n", strerror(errno));
            exit(4);
        }
    }
    wait_pid = waitpid(pid, &status, 0);
    if (wait_pid == -1)
    {
        fprintf(stderr, "Child process failed.\n");
        exit(8);
    }

    free(buffer);
    return 0;
}