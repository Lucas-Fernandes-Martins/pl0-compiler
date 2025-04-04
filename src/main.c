#include<stdio.h>
#include<stdlib.h>

#define N_RESERVED_WORDS 11


void syntatic_analyser(char reserved_words[N_RESERVED_WORDS][10], char* file_name){

	//Open file
	FILE *f_pointer;
	f_pointer = fopen(file_name, "r");
	//we're reading line by line, need to establish
	//max number of a line
	int batch_size = 100;
	char line[batch_size];

	if(f_pointer == NULL){
		printf("Error: Invalid file!\n");
		exit(1);
	}

	while(fgets(line, sizeof(line), f_pointer)){
		printf("Line: %s\n", line);
	}
	
}

void main(int argc, char** argv){
	
	//list of reserved words
	char reserved_words_list[N_RESERVED_WORDS][10] = {"CONST", "VAR", "PROCEDURE", "BEGIN", "END", "CALL", "WHILE", "DO", "ODD", "IF", "THEN"};	
	
	if(argc != 2){
		printf("ERROR! This analyser takes one and only one argument, which is the file name!\n");
		exit(1);
	}

	char *file_name = argv[1];

	printf("file name: %s\n", file_name);	
	
	printf("\n========== STARTING LEXICAL ANALYSIS! =======\n");
	syntatic_analyser(reserved_words_list, file_name);



}
