#include "dynarray.h"
#include "dfa.h"
#include "pipe.h"
#include "makecommand.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>
#include <wait.h>
#include <fcntl.h>
#define MAX_STR_LEN 1023

void execute_line(char* buf){
   DynArray_T oTokens;
   oTokens = DynArray_new(0);
   int iSuccessful;
   int status;

   /*Lexical Analysis and tokenize*/
   iSuccessful = lexLine(buf, oTokens);
     
   if (iSuccessful) //check the element in the array
      {   
         /*Synthetic analysis*/
         size_t len = DynArray_getLength(oTokens);
         struct Token **temp = calloc(sizeof(struct Token *), len);
         char **argv = calloc(sizeof(char *), len+1);

         DynArray_toArray(oTokens, (void **)temp);
   
         size_t i;        
         for (i=0; i<len;i++){
            argv[i] = temp[i]->pcValue;
         }

         free(temp);

         struct Command *command = make_com(argv, len);

         if(!command) return;

         argv = command->com[0];

         if(command->numItem < 2){
            
            if(!strcmp(argv[0], "setenv")){          
               char *temp1, *temp2;
               
               if(!argv[2]){
                  if(!argv[1]){
                     fprintf(stderr, "./ish: need more arguments\n");
                  } else{
                     if(setenv(argv[1], "", 1) == -1){
                        fprintf(stderr, "./ish: fail to set env");
                     }
                  }
               } else{
                  if(setenv(argv[1], argv[2], 1) == -1){
                     fprintf(stderr, "./ish: fail to set env");
                  }
               }
            } 
            
            else if(!strcmp(argv[0], "unsetenv")){
               if(!argv[1]){
                  fprintf(stderr, "./ish: need more arguments\n");
               } else{
                  if(unsetenv(argv[1]) == -1){
                     fprintf(stderr, "./ish: fail to unset env");
                  }
               }
            } 
            
            
            else if(!strcmp(argv[0], "cd")){
               if(!argv[1]){
                  char *dir ;

                  struct passwd *p;
                  p = getpwuid(getuid());
                  dir = p ->pw_dir;

                  if(chdir(dir) == -1){
                     fprintf(stderr, "./ish: fail to change directory into HOME\n");
                  }
               } else{
                  if(chdir(argv[1]) == -1){
                     fprintf(stderr, "./ish: No such file or directory\n");
                  }
               }
            }
            
            
            else if(!strcmp(argv[0], "exit")){
               exit(0);
            } 
            
            else{
               int pid, fd1, status;
               pid = fork();
               if(pid == 0){
                  if(command->input){
                     fd1 = open(command->input, O_RDWR);
                     close(0);
                     dup(fd1);
                     close(fd1);
                  }
                  if(command->output){
                     int fd2 = open(command->output, O_RDWR);
                     close(1);
                     dup(fd2);
                     close(fd2);
                  }
                  execvp(argv[0], argv);
                  fprintf(stderr, "./%s: No such file or directory\n", argv[0]);
                  exit(0);
               }
               pid = wait(&status);
            
            }
        } else{                
         pipeline(command->com, command->input, command->output);

        }
      }
   
   /*freeing DynArray*/
   DynArray_map(oTokens, freeToken, NULL);
   DynArray_free(oTokens); 
}

void execute_ishrc(){
   char *ishrc_path;
   char *filename = "/.ishrc";
   char line[MAX_STR_LEN];
   char* CMD_STR = "%";
   int CMD_STR_LEN = 2;
   int i;
   FILE *fp;

   ishrc_path = getenv("HOME");

   strcat(ishrc_path, filename);

   if((fp = fopen(ishrc_path, "r")) == NULL)
      return;

   while(fgets(line, sizeof(line), fp)){

      printf("%% %s", line);

      execute_line(line);
   }

   fclose(fp);
}

void QUIT(int iSig){
   exit(0);
}

void handleQUIT(int iSig){
   printf("\nType Ctrl-\\ again within 5 seconds to exit\n");
   write(1, "% ", 2);
   signal(SIGQUIT, QUIT);
   alarm(5);
}

void handleALRM(int iSig){
   signal(SIGQUIT, handleQUIT);
}

int main(){
   int i;
   int CMD_STR_LEN = 2;
   char *CMD_STR = "% ";
   char buf[MAX_STR_LEN + 1];

   sigset_t sSet;
   sigemptyset(&sSet);
   sigaddset(&sSet, SIGINT);
   sigaddset(&sSet, SIGALRM);
   sigaddset(&sSet, SIGQUIT);
   sigprocmask(SIG_UNBLOCK, &sSet, NULL);

   signal(SIGINT, SIG_IGN);
   signal(SIGALRM, handleALRM);
   signal(SIGQUIT, handleQUIT);

   execute_ishrc();

   while(1){      
      if(i = write(1, CMD_STR, CMD_STR_LEN) != CMD_STR_LEN){
         fprintf(stderr, "write error\n");
         return -1;
      }

      fgets(buf, sizeof(buf), stdin);
      execute_line(buf);
   }

   return 0;
}