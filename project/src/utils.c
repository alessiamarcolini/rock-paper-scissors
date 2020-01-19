#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "utils.h"

int squadre[MAX_TEAMS];
int calendario[MAX_TEAMS][MAX_TEAMS];

int matching(int numSquadre, int championship[MAXN][MAXN])
{
    int pivot, giornate;
    int Shift = 1;
    int i, first, temp;

    pivot = rand() % numSquadre;

    for (i = 0; i < numSquadre; i++)
    {
        squadre[i] = i;
    }

    temp = squadre[pivot];
    squadre[pivot] = squadre[numSquadre - 1];
    squadre[numSquadre - 1] = temp;

    --numSquadre;
    giornate = numSquadre;
    first = 0;

    //printf("Squadre........... %d\nPivot............. %d\n"
    //       "Shift............. %d\n",
    //       numSquadre + 1, squadre[numSquadre], Shift);

    for (i = 0; i < giornate; ++i)
    {
        int j = 0;

        size_t id1;
        int ofs;

        //printf(">> Giornata %2d *******************************************************\n"
        //       "%d-%d",
        //       i + 1, squadre[numSquadre], squadre[first]);

        calendario[squadre[numSquadre]][squadre[first]] = i + 1;
        calendario[squadre[first]][squadre[numSquadre]] = i + 1;

        championship[i][j] = squadre[numSquadre];
        championship[i][j + 1] = squadre[first];
        j += 2;

        for (id1 = (first + 1) % numSquadre, ofs = numSquadre - 2; ofs > 0; ofs -= 2)
        {
            size_t id2 = (id1 + ofs) % numSquadre;

            //printf(", %d-%d", squadre[id1], squadre[id2]);

            championship[i][j] = squadre[id1];
            championship[i][j + 1] = squadre[id2];

            j += 2;

            calendario[squadre[id1]][squadre[id2]] = i + 1;
            calendario[squadre[id2]][squadre[id1]] = i + 1;
            id1 = (id1 + 1) % numSquadre;
        }
        //puts("");
        first = (first + Shift) % numSquadre;
    }

    //puts("**********************************************************************\n");
    /*
    for (i = 0; i <= numSquadre; ++i)
    {
        size_t j;
        for (j = 0; j <= numSquadre; ++j)
        {
            printf("%2d ", calendario[i][j]);
        }
        puts("");
    }
    */

    return EXIT_SUCCESS;
}

//tokenizes a string into tokens delimited by a given delimiter char, and convert into int array
void tokenizer(char *buffer, int *tokens, char *delimiter)
{
    // split
    char *delim = delimiter;

    char *ptr;
    int tokenIndex;
    ptr = strtok(buffer, delim);
    //tokens[0] = (tokens[0] * 10) + ((*ptr) - '0'); //cast str to int
    //fprintf(stderr, "%d -", tokens[0]);
    /*int a;
    a = (a * 10) + ((*ptr) - '0');
    tokens[0] = a;*/

    tokenIndex = 1;

    while (ptr != NULL)
    {
        //printf("'%s'\n", ptr);
        ptr = strtok(NULL, delim);
        //tokens[tokenIndex] = (tokens[tokenIndex] * 10) + ((*ptr) - '0');
        //fprintf(stderr, "%d -", tokens[tokenIndex]);
        tokenIndex++;
    }
}