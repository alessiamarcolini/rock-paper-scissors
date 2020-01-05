#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Limite totalmente arbitrario */
#define MAX_TEAMS 32U

typedef unsigned int uint;
typedef enum
{
    FALSE,
    TRUE
} Boole_t;

uint squadre[MAX_TEAMS];
uint calendario[MAX_TEAMS][MAX_TEAMS];

int main()
{
    size_t numSquadre = 7;
    size_t pivot, giornate;
    size_t Shift = 1;
    size_t i, first, temp;

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

    printf("Squadre........... %d\nPivot............. %d\n"
           "Shift............. %d\n",
           numSquadre + 1, squadre[numSquadre], Shift);

    for (i = 0; i < giornate; ++i)
    {
        size_t id1;
        int ofs;

        printf(">> Giornata %2d *******************************************************\n"
               "%d-%d",
               i + 1, squadre[numSquadre], squadre[first]);

        calendario[squadre[numSquadre]][squadre[first]] = i + 1;
        calendario[squadre[first]][squadre[numSquadre]] = i + 1;

        for (id1 = (first + 1) % numSquadre, ofs = numSquadre - 2; ofs > 0; ofs -= 2)
        {
            size_t id2 = (id1 + ofs) % numSquadre;

            printf(", %d-%d", squadre[id1], squadre[id2]);

            calendario[squadre[id1]][squadre[id2]] = i + 1;
            calendario[squadre[id2]][squadre[id1]] = i + 1;
            id1 = (++id1) % numSquadre;
        }
        puts("");
        first = (first + Shift) % numSquadre;
    }

    puts("**********************************************************************\n");

    for (i = 0; i <= numSquadre; ++i)
    {
        size_t j;
        for (j = 0; j <= numSquadre; ++j)
        {
            printf("%2d ", calendario[i][j]);
        }
        puts("");
    }

    return EXIT_SUCCESS;
}