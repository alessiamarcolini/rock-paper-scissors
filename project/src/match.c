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
    printf("second player: %s", secondPlayerId);

    outcome first;
    outcome second;

    outcome firstPlayerOutcomes[nTrials];
    outcome secondPlayerOutcomes[nTrials];

    int firstPlayerPointsMatches[nTrials];
    int secondPlayerPointsMatches[nTrials];

    /* Intializes random number generator */
    srand(time(0));

    for (i = 0; i < nTrials; i++)
    {
        printf("lancio n: %d ", i);

        printf("%d -- ", (outcome)rand() % 3);

        first = (outcome)rand() % 3;
        second = (outcome)rand() % 3;
        firstPlayerOutcomes[i] = first;
        secondPlayerOutcomes[i] = second;

        //printf("ok fatto ");
    }
    return 0;
}