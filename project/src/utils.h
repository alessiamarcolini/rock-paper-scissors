#ifndef UTILS_H
#define UTILS_H

#endif

#define MAXN 100000
#define MAXLEN 7000 // max len of message
#define MAINSTREAMLEN 10 //length of number parameter passing by match->day->main

#define MAX_TEAMS 32U

enum rockPaperScissor
{
    rock,
    paper,
    scissor
};

typedef enum rockPaperScissor outcome;

int matching(int numSquadre, int championship[MAXN][MAXN]);
void tokenizer(char *buffer, int *tokens, char *delimiter);