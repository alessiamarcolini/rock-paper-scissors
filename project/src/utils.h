#ifndef UTILS_H
#define UTILS_H

#endif

#define MAXN 1000
#define MAXLEN 7000      // max len of message
#define MAINSTREAMLEN 10 //length of number parameter passing by match->day->main

#define MAX_TEAMS 32U

#define READ 0  /* read-side of pipes */
#define WRITE 1 /* write-side of pipes */

#define TRUE 1
#define FALSE 0

#define nQuarters 4
#define nSemiFinals 2

enum rockPaperScissor
{
    rock,
    paper,
    scissor
};

typedef enum rockPaperScissor outcome;

int matching(int numSquadre, int championship[MAXN][MAXN], int even);
void tokenizer(char *buffer, int *tokens, char *delimiter, int length);
void swap(int *a, int *b);
void permute(int arr[], int n);
void printArray(int arr[], int n);
void tokenizerMultipleDelimiter(char *buffer, char *tokens[MAINSTREAMLEN * 4]);