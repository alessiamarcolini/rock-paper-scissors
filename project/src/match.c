#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "utils.h"

int main(int argc, char *argv[])
{

    int i, j;
    int nTrials = 10;

    char *matchId = argv[1];
    char *firstPlayerId = argv[2];
    char *secondPlayerId = argv[3];

    //printf("sono il match n %s", matchId);
    //printf("first player: %s - ", firstPlayerId);
    //printf("second player: %s\n", secondPlayerId);

    outcome first;
    outcome second;

    outcome firstPlayerOutcomes[nTrials];
    outcome secondPlayerOutcomes[nTrials];

    //int first//PlayerPointsMatches[nTrials];
    //int secondPlayerPointsMatches[nTrials];

    int firstPlayerNWins = 0;
    int secondPlayerNWins = 0;

    int firstPlayerNLosses = 0;
    int secondPlayerNLosses = 0;

    int nParity = 0;

    /* Intializes random number generator */
    srand(time(0));
    //srand(2);

    if (strcmp(firstPlayerId, "99") != 0 && strcmp(secondPlayerId, "99") != 0)
    {
        for (i = 0; i < nTrials; i++)
        {
            //printf("lancio n %d: ", i);

            //printf("%d -- ", (outcome)rand() % 3);

            first = (outcome)rand() % 3;
            second = (outcome)rand() % 3;
            firstPlayerOutcomes[i] = first;
            secondPlayerOutcomes[i] = second;

            //printf("%d %d ", first, second);

            switch (first)
            {
            case rock:
                if (second == scissor)
                { // first winner
                    //printf("first winner\n");
                    //firstPlayerPointsMatches[i] = 2;
                    //secondPlayerPointsMatches[i] = 0;

                    firstPlayerNWins++;
                    secondPlayerNLosses++;
                }
                else if (second == paper)
                { // second winner
                    //printf("second winner\n");
                    //firstPlayerPointsMatches[i] = 0;
                    //secondPlayerPointsMatches[i] = 2;

                    secondPlayerNWins++;
                    firstPlayerNLosses++;
                }
                else if (second == rock)
                { // parity
                    //printf("parity\n");
                    //firstPlayerPointsMatches[i] = 1;
                    //secondPlayerPointsMatches[i] = 1;

                    nParity++;
                }
                break;
            case paper:
                if (second == rock)
                { // first winner
                    //printf("first winner\n");
                    //firstPlayerPointsMatches[i] = 2;
                    //secondPlayerPointsMatches[i] = 0;

                    firstPlayerNWins++;
                    secondPlayerNLosses++;
                }
                else if (second == scissor)
                { // second winner
                    //printf("second winner\n");
                    //firstPlayerPointsMatches[i] = 0;
                    //secondPlayerPointsMatches[i] = 2;

                    secondPlayerNWins++;
                    firstPlayerNLosses++;
                }
                else if (second == paper)
                { // parity
                    //printf("parity\n");
                    //firstPlayerPointsMatches[i] = 1;
                    //secondPlayerPointsMatches[i] = 1;

                    nParity++;
                }
                break;
            case scissor:
                if (second == paper)
                { // first winner
                    //printf("first winner\n");
                    //firstPlayerPointsMatches[i] = 2;
                    //secondPlayerPointsMatches[i] = 0;

                    firstPlayerNWins++;
                    secondPlayerNLosses++;
                }
                else if (second == rock)
                { // second winner
                    //printf("second winner\n");
                    //firstPlayerPointsMatches[i] = 0;
                    //secondPlayerPointsMatches[i] = 2;

                    secondPlayerNWins++;
                    firstPlayerNLosses++;
                }
                else if (second == scissor)
                { // parity
                    //printf("parity\n");
                    //firstPlayerPointsMatches[i] = 1;
                    //secondPlayerPointsMatches[i] = 1;

                    nParity++;
                }
                break;
            default:
                fprintf(stderr, "Something weird happened during play.\n");
                exit(7);
                break;
            }
        }
    }

    int pointsFirst = 0;
    int pointsSecond = 0;

    if (strcmp(firstPlayerId, "99") != 0 && strcmp(secondPlayerId, "99") != 0)
    {
        //points calculation
        if (firstPlayerNWins > secondPlayerNWins)
        {
            pointsFirst = 2;
            pointsSecond = 0;
        }
        else if (firstPlayerNWins == secondPlayerNWins)
        {
            pointsFirst = 1;
            pointsSecond = 1;
        }
        else
        {
            pointsFirst = 0;
            pointsSecond = 2;
        }
    }

    /*for (j = 0; j < nTrials; j++)
    {
        sumPointsFirst += firstPlayerPointsMatches[j];
        sumPointsSecond += secondPlayerPointsMatches[j];
    }*/

    //printf("first %d, second %d\n", sumPointsFirst, sumPointsSecond);

    //printf("first wins %d - second wins %d - first losses %d - second losses %d\n", firstPlayerNWins, secondPlayerNWins, firstPlayerNLosses, secondPlayerNLosses);

    //printf("ok fatto ");

    printf("%s %s %d %d %d %d %d %d %d ", firstPlayerId, secondPlayerId, pointsFirst, pointsSecond, firstPlayerNWins, secondPlayerNWins, firstPlayerNLosses, secondPlayerNLosses, nParity);

    return 0;
}