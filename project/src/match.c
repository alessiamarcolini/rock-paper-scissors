#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

int main(int argc, char *argv[])
{
    //int i, j;
    //int nTrials = 10;
    //
    //char *matchId = argv[1];
    //char *firstPlayerId = argv[2];
    //char *secondPlayerId = argv[3];

    //outcome first;
    //outcome second;

    //outcome firstPlayerOutcomes[nTrials];
    //outcome secondPlayerOutcomes[nTrials];

    //int firstPlayerPointsMatches[nTrials];
    //int secondPlayerPointsMatches[nTrials];

    /* Intializes random number generator */
    //srand(time(0));

    printf("sono il match n %s\n", argv[1]);

    //for (i = 0; i < nTrials; i++)
    //{
    //    printf("lancio n: %d ", i);
    //
    //    printf("%d -- ", (outcome)rand() % 3);

    /*
        first = (outcome)rand() % 3;
        second = (outcome)rand() % 3;

        firstPlayerOutcomes[i] = first;
        secondPlayerOutcomes[i] = second;
*/
    /*switch (first)
        {
        case rock:
            if (second == scissor)
            { // first winner
                firstPlayerPointsMatches[i] = 2;
                secondPlayerPointsMatches[i] = 0;
            }
            else if (second == paper)
            { // second winner
                firstPlayerPointsMatches[i] = 0;
                secondPlayerPointsMatches[i] = 2;
            }
            else if (second == rock)
            { // parity
                firstPlayerPointsMatches[i] = 1;
                secondPlayerPointsMatches[i] = 1;
            }
            break;

        case paper:
            if (second == rock)
            { // first winner
                firstPlayerPointsMatches[i] = 2;
                secondPlayerPointsMatches[i] = 0;
            }
            else if (second == scissor)
            { // second winner
                firstPlayerPointsMatches[i] = 0;
                secondPlayerPointsMatches[i] = 2;
            }
            else if (second == paper)
            { // parity
                firstPlayerPointsMatches[i] = 1;
                secondPlayerPointsMatches[i] = 1;
            }
            break;

        case scissor:
            if (second == paper)
            { // first winner
                firstPlayerPointsMatches[i] = 2;
                secondPlayerPointsMatches[i] = 0;
            }
            else if (second == rock)
            { // second winner
                firstPlayerPointsMatches[i] = 0;
                secondPlayerPointsMatches[i] = 2;
            }
            else if (second == scissor)
            { // parity
                firstPlayerPointsMatches[i] = 1;
                secondPlayerPointsMatches[i] = 1;
            }
            break;
        }

        int sumPointsFirst = 0;
        int sumPointsSecond = 0;

        for (j = 0; j < nTrials; j++)
        {
            sumPointsFirst += firstPlayerPointsMatches[j];
            sumPointsSecond += secondPlayerPointsMatches[j];
        }

        printf("first %d, second %d\n", sumPointsFirst, sumPointsSecond);
        */
    //}
    return 0;
}