#ifndef UTILS_H
#define UTILS_H

#endif

#define MAXN 100000
#define MAXLEN 7000 // max len of message

#define MAX_TEAMS 32U
int matching(int numSquadre, int championship[MAXN][MAXN]);

enum rockPaperScissor
{
    rock,
    paper,
    scissor
};

typedef enum rockPaperScissor outcome;