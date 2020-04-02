/*Gyubin Lee, Assignment 1, wc209.c*/

#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

/*global variables 'line', 'word', 'charac' indicates 
the number of line, word, character. 'temp'is use for indicating
the line where the comment start*/

int line = 1; int word = 0; int charac = 0; int temp = 1;

//define 7 state of DFA
enum DFAState {OUT, IN, A_1, A_2, B, C, D};

//if c is '\n', increase line and charac
void newline(void){
	line += 1;
	charac += 1;
}

//if isspace(c) is true, this function will be called
void space(int c){
	if (c == '\n') newline();
	else charac +=1;
}

int main(void){
	enum DFAState state = OUT; //Initial state of DFA is 'O'
	int c; //get character from input stream

	/*executing DFA by following while loop until there is
	no character in the input stream. This loop calculate the
	number of lines, words, and characters by increasing, or
	decreasing the global variable 'line', 'word' and 'charac'*/

	while((c = getchar()) != EOF){
		switch(state){
			case OUT:
				if(isspace(c)){
					space(c); break;
				} else if(c == '/'){ 
					word += 1; charac += 1;
					state = A_2; break;
				} else{
					word += 1; charac += 1;
					state = IN; break;
				}
			case IN:
				if(isspace(c)){
					space(c);
					state = OUT; break;
				} else if(c == '/'){
					charac += 1;
					state = A_1; break;
				} else{
					charac += 1; break;
				}
			case A_1:
				if(isspace(c)){
					space(c);
					state = OUT; break;
				} else if(c == '*'){
					charac -= 1;
					temp = line;
					state = B; break;
				} else if(c == '/'){
					charac += 1; break;
				} else{
					charac += 1;
					state = IN; break;
				}
			case B:
				if(c == '*'){
					state = C; break;
				} else{
					if(c == '\n') newline();
					break;
				}
			case C:
				if(c == '/'){
					charac += 1;
					state = D; break;
				} else if (c == '*'){
					break;
				}
				else{
					if(c == '\n') newline();
					state = B; break;
				}
			case D:
				if(isspace(c)){
					space(c);
					state = OUT; break;
				} else if(c == '/'){
					charac += 1;
					word += 1;
					state = A_2; break;
				} else{
					word += 1;
					charac += 1;
					state = IN; break;
				}
			case A_2:
				if(isspace(c)){
					space(c);
					state = OUT; break;
				} else if(c == '*'){
					charac -= 1;
					word -= 1;
					temp = line;
					state = B; break;
				} else if(c == '/'){
					charac += 1; break;
				} else{
					charac += 1;
					state = IN; break;
				}
			default:
				assert(0); break;
			}
	}
	
	/*if charac == 0, it means that there is nothing
	in the input stream. So, 'line' should be 0*/

	if(charac == 0) {
		line = 0;
	};

	/*state B and C means that the comment is unterminated. So we
	should throw the error in these two cases. Else, print the
	result in standard output stream*/

	if(state == B || state == C){
		fprintf(stderr, "Error: line %d: unterminated comment\n", temp);		
		return EXIT_FAILURE;	
	} else {
		printf("%d %d %d\n", line, word, charac);
		return EXIT_SUCCESS;
	}
}
