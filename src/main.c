#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "analysers/syntatic.h"

int main(int argc, char** argv){
	// Checks if the number of inputs is valid
	if(argc != 2){
		printf("ERROR! This analyser takes one and only one argument, which is the file name!\n");
		exit(1);
	}

	char *file_name = argv[1]; // Get the file name

	// For lexical analysis
	transition_matrix = build_lexer_dfa(); // Creates the lexer analyser automata 

	// Open the file
	FILE *input_file;
	input_file = fopen(file_name, "r");
	if(input_file == NULL){
		printf("Error: Unable to open input file!\n");
		exit(1);
	}

	syntatic_analyser(input_file);

	// Closes input file
	fclose(input_file);
}