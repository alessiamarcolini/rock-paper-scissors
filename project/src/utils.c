#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "utils.h"

int squadre[MAXN];
int calendario[MAXN][MAXN];

int matching(int numSquadre, int championship[MAXN][MAXN], int even)
{

    int pivot, giornate;
    int Shift = 1;
    int i, first, temp;

    pivot = rand() % numSquadre;

    for (i = 0; i < numSquadre; i++)
    {
        squadre[i] = i;
    }
    if (!even)
    {
        squadre[numSquadre - 1] = 99;
    }

    temp = squadre[pivot];
    squadre[pivot] = squadre[numSquadre - 1];
    squadre[numSquadre - 1] = temp;

    --numSquadre;
    giornate = numSquadre;
    first = 0;

    for (i = 0; i < giornate; ++i)
    {
        int j = 0;

        size_t id1;
        int ofs;

        calendario[squadre[numSquadre]][squadre[first]] = i + 1;
        calendario[squadre[first]][squadre[numSquadre]] = i + 1;

        championship[i][j] = squadre[numSquadre];
        championship[i][j + 1] = squadre[first];
        j += 2;

        for (id1 = (first + 1) % numSquadre, ofs = numSquadre - 2; ofs > 0; ofs -= 2)
        {
            size_t id2 = (id1 + ofs) % numSquadre;

            championship[i][j] = squadre[id1];
            championship[i][j + 1] = squadre[id2];

            j += 2;

            calendario[squadre[id1]][squadre[id2]] = i + 1;
            calendario[squadre[id2]][squadre[id1]] = i + 1;
            id1 = (id1 + 1) % numSquadre;
        }
        first = (first + Shift) % numSquadre;
    }

    return EXIT_SUCCESS;
}

//tokenizes a string into tokens delimited by a given delimiter char, and convert into int array
void tokenizer(char *buffer, int *tokens, char *delimiter, int length)
{
    // split
    char *delim = delimiter;

    char *ptr;
    int tokenIndex;
    ptr = strtok(buffer, delim);
    char *b;
    long l = strtol(ptr, &b, 10);
    tokens[0] = l; //cast str to int (implicit by long to int)
    tokenIndex = 1;
    int i;
    for (i = 0; i < length; i++)
    {
        ptr = strtok(NULL, delim);
        l = strtol(ptr, &b, 10);
        tokens[tokenIndex] = l; //cast str to int (implicit by long to int)
        tokenIndex++;
    }
}

void tokenizerMultipleDelimiter(char *buffer, char *tokens[MAINSTREAMLEN * 6])
{
    char delimit[] = " \t\r\n\v\f"; // 2) POSIX whitespace characters
    char *token = strtok(buffer, delimit);
    tokens[0] = token;

    // Keep printing tokens while one of the
    // delimiters present in str[].
    int i = 0;
    while (tokens[i] != NULL)
    {
        i++;
        tokens[i] = strtok(NULL, delimit);
    }
}

void swap(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

void printArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
}

// randomly permute the array passed as parameter
void permute(int arr[], int n)
{
    int i, j, k;
    for (i = 0; i < 100; i++)
    {
        j = rand() % n;
        k = rand() % n;

        swap(&arr[j], &arr[k]);
    }
}
