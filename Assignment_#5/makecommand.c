#include "makecommand.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Command *make_com(char** argv, int* rdIndex, int len){
   
   int I_count = 0, O_count = 0, P_count = 0, num = 1;
   int is_I = 0, is_O = 0;
   size_t i, count;

   struct Command *com = malloc(sizeof(struct Command));

   com->numItem = 0;
   com->input = NULL;
   com->output = NULL;
   com->com = NULL;

   for(i = 0; i<len; i++){
     if(!strcmp(argv[i], "<") && rdIndex[i] == 1) {
         if(is_I == 1){
            fprintf(stderr, "./ish: Multiple redirection of standard input\n");
            return NULL;
         }
         is_I = 1;
         I_count = i;
      }
      if(!strcmp(argv[i], ">")&& rdIndex[i] == 1) {
         if(is_O == 1){
            fprintf(stderr, "./ish: Multiple redirection of standard output\n");
            return NULL;
         }
         is_O = 1;
         O_count = i;
      }
      if(!strcmp(argv[i], "|")&& rdIndex[i] == 1) {
         P_count = i;
         num++;
      }
   }

   if((is_I == 1 && I_count == 0) || (is_O==1 && O_count == 0)){
      fprintf(stderr, "./ish: Missing command line\n");
      return NULL;
   }

   if(is_I){
      for(i = I_count; i>0; i--){
         if(!strcmp(argv[i], "|")&& rdIndex[i] == 1) {
            if(I_count == i + 1){
               fprintf(stderr, "./ish: Pipe or redirection destination not sepcified\n");
            }             
            fprintf(stderr, "./ish: Multiple redirection of standard input\n");
            return NULL;
         }
      }
   }

   if(is_O){
      for(i = O_count; i<len; i++){
         if(!strcmp(argv[i], "|")&& rdIndex[i] == 1) {
            if(O_count + 1 == i){
               fprintf(stderr, "./ish: Pipe or redirection destination not sepcified\n");
            }            
            fprintf(stderr, "./ish: Multiple redirection of standard output\n");
            return NULL;
         }
      }
   }

   if(P_count){
      if(!strcmp(argv[P_count-1], "|")&& rdIndex[P_count-1] == 1) {
         fprintf(stderr, "./ish: Pipe or redirection destination not sepcified\n");
         return NULL;
      }if(P_count == len -1){
         fprintf(stderr, "./ish: Pipe or redirection destination not sepcified\n");
         return NULL;
      }
   }

   char ***arg = calloc(sizeof(char**), num+1);
   char **ar;

   if(num == 1){
      if(is_I>0){
         com->input = argv[I_count+1];
         argv[I_count+1] = "<";
      }
      if(is_O>0){
         com->output = argv[O_count+1];
         argv[O_count+1] = ">";  
      }

      count = 0;
      ar = calloc(sizeof(char*), len-is_I-is_O+1);

      for(i=0;i<len;i++){
         if(strcmp(argv[i], "<") && (strcmp(argv[i], ">"))){          
            ar[count] = argv[i];
            count++;
         }
      }
      arg[0] = ar;
      com->numItem++;
   }

   if(num > 1){
      if(is_I>0){
         com->input = argv[I_count+1];
         argv[I_count+1] = "<";   
      }
      if(is_O>0){
         com->output = argv[O_count+1];
         argv[O_count+1] = ">";  
      }

      int index = 0;
      for(i=0;i<num;i++){
         count = 0;
         ar = calloc(sizeof(char*), len-is_I-is_O+1);
         while(index<len && strcmp(argv[index], "|")){
            if(strcmp(argv[index], "<") && (strcmp(argv[index], ">"))){          
               ar[count] = argv[index];
               count++;          
            }
            index++;
         }
         arg[i] = ar;
         com->numItem++;
         index++;
      }
   }
   com->com = arg;

   return com;
}

void freeCommand(struct Command *temp){
   free(temp->com);
   free(temp);
}