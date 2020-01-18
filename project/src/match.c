#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

int main(int argc, char *argv[])
{

    int i, j;
    int nTrials = 10;

    char *matchId = argv[1];
    char *firstPlayerId = argv[2];
    char *secondPlayerId = argv[3];

    printf("sono il match n %s", matchId);
    printf("first player: %s - ", firstPlayerId);
    printf("second player: %s\n", secondPlayerId);

    outcome first;
    outcome second;

    outcome firstPlayerOutcomes[nTrials];
    outcome secondPlayerOutcomes[nTrials];

    int firstPlayerPointsMatches[nTrials];
    int secondPlayerPointsMatches[nTrials];

    int firstPlayerNWins = 0;
    int secondPlayerNWins = 0;

    int firstPlayerNLosses = 0;
    int secondPlayerNLosses = 0;

    int nParity = 0;

    /* Intializes random number generator */
    srand(time(0));

    for (i = 0; i < nTrials; i++)
    {
        printf("lancio n %d: ", i);

        //printf("%d -- ", (outcome)rand() % 3);

        first = (outcome)rand() % 3;
        second = (outcome)rand() % 3;
        firstPlayerOutcomes[i] = first;
        secondPlayerOutcomes[i] = second;

        printf("%d %d ", first, second);

        switch (first)
        {
        case rock:
            if (second == scissor)
            { // first winner
                //printf("first winner\n");
                firstPlayerPointsMatches[i] = 2;
                secondPlayerPointsMatches[i] = 0;

                firstPlayerNWins++;
                secondPlayerNLosses++;
            }
            else if (second == paper)
            { // second winner
                //printf("second winner\n");
                firstPlayerPointsMatches[i] = 0;
                secondPlayerPointsMatches[i] = 2;

                secondPlayerNWins++;
                firstPlayerNLosses++;
            }
            else if (second == rock)
            { // parity
                //printf("parity\n");
                firstPlayerPointsMatches[i] = 1;
                secondPlayerPointsMatches[i] = 1;

                nParity++;
            }
            break;
        case paper:
            if (second == rock)
            { // first winner
                //printf("first winner\n");
                firstPlayerPointsMatches[i] = 2;
                secondPlayerPointsMatches[i] = 0;

                firstPlayerNWins++;
                secondPlayerNLosses++;
            }
            else if (second == scissor)
            { // second winner
                //printf("second winner\n");
                firstPlayerPointsMatches[i] = 0;
                secondPlayerPointsMatches[i] = 2;

                secondPlayerNWins++;
                firstPlayerNLosses++;
            }
            else if (second == paper)
            { // parity
                //printf("parity\n");
                firstPlayerPointsMatches[i] = 1;
                secondPlayerPointsMatches[i] = 1;

                nParity++;
            }
            break;
        case scissor:
            if (second == paper)
            { // first winner
                //printf("first winner\n");
                firstPlayerPointsMatches[i] = 2;
                secondPlayerPointsMatches[i] = 0;

                firstPlayerNWins++;
                secondPlayerNLosses++;
            }
            else if (second == rock)
            { // second winner
                //printf("second winner\n");
                firstPlayerPointsMatches[i] = 0;
                secondPlayerPointsMatches[i] = 2;

                secondPlayerNWins++;
                firstPlayerNLosses++;
            }
            else if (second == scissor)
            { // parity
                //printf("parity\n");
                firstPlayerPointsMatches[i] = 1;
                secondPlayerPointsMatches[i] = 1;

                nParity++;
            }
            break;
        }
    }
    int sumPointsFirst = 0;
    int sumPointsSecond = 0;
    for (j = 0; j < nTrials; j++)
    {
        sumPointsFirst += firstPlayerPointsMatches[j];
        sumPointsSecond += secondPlayerPointsMatches[j];
    }

    printf("first %d, second %d\n", sumPointsFirst, sumPointsSecond);

    printf("first wins %d - second wins %d - first losses %d - second losses %d\n", firstPlayerNWins, secondPlayerNWins, firstPlayerNLosses, secondPlayerNLosses);

    printf("ok fatto ");

    return 0;
}