#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char *argv[])
{

    char *semiFinalId = argv[1];
    char *firstPlayerId = argv[2];
    char *secondPlayerId = argv[3];

    //printf("first: %s - second: %s\n", firstPlayerId, secondPlayerId);

    outcome first;
    outcome second;

    srand(time(0));

    char *winner;

    int notParity = FALSE;

    do
    {
        first = (outcome)rand() % 3;
        second = (outcome)rand() % 3;

        switch (first)
        {
        case rock:
            if (second == scissor)
            { // first winner
                winner = firstPlayerId;

                notParity = TRUE;
                //printf("R S\n");
                printf("%s %s %s %s R S\n", semiFinalId, firstPlayerId, secondPlayerId, winner);
            }
            else if (second == paper)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                //printf("R P\n");
                printf("%s %s %s %s R P\n", semiFinalId, firstPlayerId, secondPlayerId, winner);
            }
            break;
        case paper:
            if (second == rock)
            { // first winner
                winner = firstPlayerId;
                notParity = TRUE;
                //printf("P R\n");
                printf("%s %s %s %s P R\n", semiFinalId, firstPlayerId, secondPlayerId, winner);
            }
            else if (second == scissor)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                //printf("P S\n");
                printf("%s %s %s %s P S\n", semiFinalId, firstPlayerId, secondPlayerId, winner);
            }
            break;
        case scissor:
            if (second == paper)
            { // first winner
                winner = firstPlayerId;
                notParity = TRUE;
                //printf("S P\n");
                printf("%s %s %s %s S P\n", semiFinalId, firstPlayerId, secondPlayerId, winner);
            }
            else if (second == rock)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                //printf("S R\n");
                printf("%s %s %s %s S R\n", semiFinalId, firstPlayerId, secondPlayerId, winner);
            }
            break;
        default:
            fprintf(stderr, "Something weird happened during play.\n");
            exit(7);
            break;
        }
    } while (!notParity);

    //printf("the winner is: %s\n", winner);
    //fprintf(stderr, "%s %s %s %s\n", semiFinalId, firstPlayerId, secondPlayerId, winner);
    //printf("%s %s %s %s\n", semiFinalId, firstPlayerId, secondPlayerId, winner);

    //free(winner);
    return 0;
}