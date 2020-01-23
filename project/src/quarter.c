#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char *argv[])
{

    char *quarterId = argv[1];
    fprintf(stderr, "quarter %s - ", quarterId);
    char *firstPlayerId = argv[2];
    char *secondPlayerId = argv[3];
    fprintf(stderr, "first %s - ", firstPlayerId);
    fprintf(stderr, "second %s - ", secondPlayerId);

    //printf("first: %s - second: %s\n", firstPlayerId, secondPlayerId);

    outcome first;
    outcome second;

    srand(time(0));

    char *winner; //= malloc(sizeof(char) * 10);

    int notParity = FALSE;

    do
    {
        first = (outcome)rand() % 3;
        second = (outcome)rand() % 3;

        fprintf(stderr, "out1: %d - ou2: %d\t\t", first, second);

        switch (first)
        {
        case rock:
            if (second == scissor)
            { // first winner
                winner = firstPlayerId;

                notParity = TRUE;
                fprintf(stderr, "R S\n");
            }
            else if (second == paper)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                fprintf(stderr, "R P\n");
            }
            break;
        case paper:
            if (second == rock)
            { // first winner
                winner = firstPlayerId;
                notParity = TRUE;
                fprintf(stderr, "P R\n");
            }
            else if (second == scissor)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                fprintf(stderr, "P S\n");
            }
            break;
        case scissor:
            if (second == paper)
            { // first winner
                winner = firstPlayerId;
                notParity = TRUE;
                fprintf(stderr, "S P\n");
            }
            else if (second == rock)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                fprintf(stderr, "S R\n");
            }
            break;
        }
    } while (!notParity);

    fprintf(stderr, "the winner is: %s\n", winner);
    printf("%s %s %s %s\n", quarterId, firstPlayerId, secondPlayerId, winner);

<<<<<<< HEAD
    free(winner);
=======
    //free(winner);
>>>>>>> fix-free

    return 0;
}