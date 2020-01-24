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

#define RED_CODE "\x1b[31m"
#define GREEN_CODE "\x1b[32m"
#define YELLOW_CODE "\x1b[33m"
#define BLUE_CODE "\x1b[34m"
#define MAGENTA_CODE "\x1b[35m"
#define CYAN_CODE "\x1b[36m"
#define RESET_CODE "\x1b[0m"

#define RED(string) "\x1b[31m" string "\x1b[0m"
#define BLUE(string) "\x1b[34m" string "\x1b[0m"
#define YELLOW(string) "\x1b[33m" string "\x1b[0m"

#define printfRed(X) printf(RED(X), X);
#define printfBlue(X) printf(BLUE(X), X);
#define printfYellow(X) printf(YELLOW(X), X);

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