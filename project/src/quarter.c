#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char *argv[])
{

    char *quarterId = argv[1];
    char *firstPlayerId = argv[2];
    char *secondPlayerId = argv[3];

    //printf("first: %s - second: %s\n", firstPlayerId, secondPlayerId);

    outcome first;
    outcome second;

    srand(time(0));

    char *winner = malloc(sizeof(char) * 10);

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
            }
            else if (second == paper)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                //printf("R P\n");
            }
            break;
        case paper:
            if (second == rock)
            { // first winner
                winner = firstPlayerId;
                notParity = TRUE;
                //printf("P R\n");
            }
            else if (second == scissor)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                //printf("P S\n");
            }
            break;
        case scissor:
            if (second == paper)
            { // first winner
                winner = firstPlayerId;
                notParity = TRUE;
                //printf("S P\n");
            }
            else if (second == rock)
            { // second winner
                winner = secondPlayerId;
                notParity = TRUE;
                //printf("S R\n");
            }
            break;
        }
    } while (!notParity);

    //printf("the winner is: %s\n", winner);
    printf("%s %s %s %s\n", quarterId, firstPlayerId, secondPlayerId, winner);

    free(winner);

    return 0;
}