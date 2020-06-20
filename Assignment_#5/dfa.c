/*--------------------------------------------------------------------*/
/* dfa.c                                                              */
/* Original Author: Bob Dondero                                       */
/* Illustrate lexical analysis using a deterministic finite state     */
/* automaton (DFA)                                                    */
/*--------------------------------------------------------------------*/

#include "dynarray.h"
#include "dfa.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pwd.h>
#include <wait.h>
#include <fcntl.h>

/*--------------------------------------------------------------------*/

enum {MAX_LINE_SIZE = 1024};

enum {FALSE, TRUE};

/*--------------------------------------------------------------------*/

void freeToken(void *pvItem, void *pvExtra)

/* Free token pvItem.  pvExtra is unused. */

{
   struct Token *psToken = (struct Token*)pvItem;
   free(psToken->pcValue);
   free(psToken);
}

/*--------------------------------------------------------------------*/

static struct Token *makeToken(char *pcValue, int type)

/* Create and return a Token whose type is eTokenType and whose
   value consists of string pcValue.  Return NULL if insufficient
   memory is available.  The caller owns the Token. */

{
   struct Token *psToken;

   psToken = (struct Token*)malloc(sizeof(struct Token));
   if (psToken == NULL)
      return NULL;


   psToken->pcValue = (char*)malloc(strlen(pcValue) + 1);
   if (psToken->pcValue == NULL)
   {
      free(psToken);
      return NULL;
   }

   strcpy(psToken->pcValue, pcValue);
   psToken->type = type;

   return psToken;
}

/*--------------------------------------------------------------------*/

int lexLine(const char *pcLine, DynArray_T oTokens)

/* Lexically analyze string pcLine.  Populate oTokens with the
   tokens that pcLine contains.  Return 1 (TRUE) if successful, or
   0 (FALSE) otherwise.  In the latter case, oTokens m}y contain
   tokens that were discovered before the error. The caller owns the
   tokens placed in oTokens. */

/* lexLine() uses a DFA approach.  It "reads" its characters from
   pcLine. */

{
   enum LexState {STATE_START, STATE_RD, STATE_CHAR, STATE_QUOTE, STATE_EXIT};

   enum LexState eState = STATE_START;

   int iLineIndex = 0;
   int iValueIndex = 0;
   char c, temp;
   char acValue[MAX_LINE_SIZE];
   struct Token *psToken;

   assert(pcLine != NULL);
   assert(oTokens != NULL);

   temp = pcLine[iLineIndex];
   if(temp == '\n') return FALSE;

   for(;;)
   {
      /* "Read" the next character from pcLine. */
      c = pcLine[iLineIndex++];

      switch (eState)
      {
         case STATE_START:
            if(isspace(c)){
               if ((c == '\n') || (c == '\0'))
                  return TRUE;
               else
                  eState = STATE_START;
            }
            else if ((c == '>') || (c == '|') || (c == '<'))
            {
               acValue[iValueIndex++] = c;
               eState = STATE_RD;
            }
            else if (c == '"')
            {
               eState = STATE_QUOTE;
            }
            else
            {
               acValue[iValueIndex++] = c;
               eState = STATE_CHAR;
            }
            break;

         case STATE_RD:
            if(isspace(c)){
               /* Create a RD token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(acValue, RD);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;

               if((c == '\n') || (c=='\0'))
                  return TRUE;
               else
                  eState = STATE_START;
            }
           else if ((c == '>') || (c == '|') || (c == '<'))
            {
               /*Create a RD token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(acValue, RD);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;
               acValue[iValueIndex++] = c;

               eState = STATE_RD;
            }
            else if (c == '"')
            {
               eState = STATE_QUOTE;
            }
            else
            {
               /*Create a RD token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(acValue, RD);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;
               acValue[iValueIndex++] = c;

               eState = STATE_CHAR;
            }
            break;

         case STATE_CHAR:
            if ((c == '\n') || (c == '\0'))
            {
               /* Create a CHAR token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(acValue, STR);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;

               if ((c == '\n') || (c == '\0'))
                  return TRUE;
               else
                  eState = STATE_START;
               
            }
           else if ((c == '>') || (c == '|') || (c == '<'))
            {
               /*Create a CHAR token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(acValue, STR);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;
               acValue[iValueIndex++] = c;

               eState = STATE_RD;
            }
            else if (c == '"')
            {
               eState = STATE_QUOTE;
            }
            else if ((c == ' ') || (c == '\t')){
               /*Create a CHAR token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(acValue, STR);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;

               eState = STATE_START;
            }
            else
            {
               acValue[iValueIndex++] = c;
               eState = STATE_CHAR;
            }
            break;
         case STATE_QUOTE:
            if((c == '\n') || (c == '\0')){
               fprintf(stderr, "./ish: Could not find quote pair\n");
               return FALSE;
            }
            else if(c == '"'){
               eState = STATE_EXIT;
            }
            else{
               acValue[iValueIndex++] = c;
               eState = STATE_QUOTE;
            }
            break;
         case STATE_EXIT:
            if ((c == '\n') || (c == '\0'))
            {
               /* Create a token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(acValue, STR);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;

               return TRUE;
            }
           else if ((c == '>') || (c == '|') || (c == '<'))
            {
               /*Create a token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(acValue, STR);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;
               acValue[iValueIndex++] = c;

               eState = STATE_RD;
            }
            else if (c == '"')
            {
               eState = STATE_QUOTE;
            }
            else if ((c == ' ') || (c == '\t')){
               /*Create a token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(acValue, STR);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;

               eState = STATE_START;
               }
            else
            {
               acValue[iValueIndex++] = c;
               eState = STATE_CHAR;
            }
            break;            

         default:
            assert(FALSE);
      }
   }
}

/*--------------------------------------------------------------------*/