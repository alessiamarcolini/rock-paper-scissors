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

    outcome first;
    outcome second;

    outcome firstPlayerOutcomes[nTrials];
    outcome secondPlayerOutcomes[nTrials];

    int firstPlayerNWins = 0;
    int secondPlayerNWins = 0;

    int firstPlayerNLosses = 0;
    int secondPlayerNLosses = 0;

    int nParity = 0;

    /* Intializes random number generator */
    srand(time(0));

    if (strcmp(firstPlayerId, "99") != 0 && strcmp(secondPlayerId, "99") != 0)
    {
        for (i = 0; i < nTrials; i++)
        {
            first = (outcome)rand() % 3;
            second = (outcome)rand() % 3;
            firstPlayerOutcomes[i] = first;
            secondPlayerOutcomes[i] = second;

            switch (first)
            {
            case rock:
                if (second == scissor)
                { // first winner
                    firstPlayerNWins++;
                    secondPlayerNLosses++;
                }
                else if (second == paper)
                { // second winner
                    secondPlayerNWins++;
                    firstPlayerNLosses++;
                }
                else if (second == rock)
                { // parity
                    nParity++;
                }
                break;
            case paper:
                if (second == rock)
                { // first winner
                    firstPlayerNWins++;
                    secondPlayerNLosses++;
                }
                else if (second == scissor)
                { // second winner
                    secondPlayerNWins++;
                    firstPlayerNLosses++;
                }
                else if (second == paper)
                { // parity
                    nParity++;
                }
                break;
            case scissor:
                if (second == paper)
                { // first winner
                    firstPlayerNWins++;
                    secondPlayerNLosses++;
                }
                else if (second == rock)
                { // second winner
                    secondPlayerNWins++;
                    firstPlayerNLosses++;
                }
                else if (second == scissor)
                { // parity
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

    printf("%s %s %d %d %d %d %d %d %d ", firstPlayerId, secondPlayerId, pointsFirst, pointsSecond, firstPlayerNWins, secondPlayerNWins, firstPlayerNLosses, secondPlayerNLosses, nParity);

    return 0;
}