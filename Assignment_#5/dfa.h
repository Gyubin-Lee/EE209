#ifndef DFA_INCLUDED
#define DFA_INCLUDED

struct Token
{
   char *pcValue;
};

void freeToken(void *pvItem, void *pvExtra);

int lexLine(const char *pcLine, DynArray_T oTokens);
#endif