#ifndef COMMAND
#define COMMAND

struct Command{
   char ***com;
   char *input;
   char *output;
   int numItem;
};

struct Command *make_com(char** argv, int len);

#endif