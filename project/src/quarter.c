#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char *argv[])
{

    char *quarterId = argv[1];
    //fprintf(stderr, "quarter %s - ", quarterId);
    char *firstPlayerId = argv[2];
    char *secondPlayerId = argv[3];
    //fprintf(stderr, "first %s - ", firstPlayerId);
    //fprintf(stderr, "second %s - ", secondPlayerId);

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

        //fprintf(stderr, "out1: %d - ou2: %d\t\t", first, second);

        switch (first)
        {
        case rock:
            if (second == scissor)
            { // first winner
                winner = firstPlayerId;

                notParity = TRUE;
                //fprintf(stderr, "R S\n");
                printf("%s %s %s %s R S\n", quarterId, firstPlayerId, secondPlayerId, winner);
            }
            else if (second == paper)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                //fprintf(stderr, "R P\n");
                printf("%s %s %s %s R P\n", quarterId, firstPlayerId, secondPlayerId, winner);
            }
            break;
        case paper:
            if (second == rock)
            { // first winner
                winner = firstPlayerId;
                notParity = TRUE;
                //fprintf(stderr, "P R\n");
                printf("%s %s %s %s P R\n", quarterId, firstPlayerId, secondPlayerId, winner);
            }
            else if (second == scissor)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                //fprintf(stderr, "P S\n");
                printf("%s %s %s %s P S\n", quarterId, firstPlayerId, secondPlayerId, winner);
            }
            break;
        case scissor:
            if (second == paper)
            { // first winner
                winner = firstPlayerId;
                notParity = TRUE;
                //fprintf(stderr, "S P\n");
                printf("%s %s %s %s S P\n", quarterId, firstPlayerId, secondPlayerId, winner);
            }
            else if (second == rock)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                //fprintf(stderr, "S R\n");
                printf("%s %s %s %s S R\n", quarterId, firstPlayerId, secondPlayerId, winner);
            }
            break;
        default:
            fprintf(stderr, "Something weird happened during play.\n");
            exit(7);
            break;
        }
    } while (!notParity);

    //fprintf(stderr, "the winner is: %s\n", winner);

    //free(winner);

    return 0;
}