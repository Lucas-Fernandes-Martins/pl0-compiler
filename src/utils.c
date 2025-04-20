#include<stdio.h>
#include<string.h>
#include"headers.h"

int check_op(char c){
	return (c == ' ' || c == '+' || c == '-' || c == '*' || c == '/');
}

int is_letter(char c, Output *transition_line){
    // Check if it's in the lowercase range OR the uppercase range.
    return ((((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) 
	    & strcmp(transition_line['?'].output, "NOP") 
	    & strcmp(transition_line[c].output, "NOP") == 0);
}
