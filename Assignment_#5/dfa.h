#ifndef DFA_INCLUDED
#define DFA_INCLUDED

enum {STR, RD};

struct Token
{
   char *pcValue;
   int type;
};

void freeToken(void *pvItem, void *pvExtra);

int lexLine(const char *pcLine, DynArray_T oTokens);
#endif