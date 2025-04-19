#include<stdio.h>
#include<stdlib.h>
#include"headers.h"


void lexical_analyser(FILE *f_pointer, Output** transition_matrix){
	int number = 0;
	
	//Initialize hash table
	
	//Output output1;
	//output1.next_state = 1;
	//output1.output = 'a';

	//hash_table[0] = output1;	
	
	int is_final_state = 0;
	int i = 0;
	int current_state = 0;
	char char_consumed;
	while((char_consumed = fgetc(f_pointer)) != EOF){
		Output output = transition_matrix[current_state][char_consumed];
		printf("read: %c Current state: %d Next state: %d Output: %s\n", 
				char_consumed, current_state, output.next_state, output.output);
		current_state = output.next_state;
		i += 1;
	}	
}


void syntatic_analyser(char reserved_words[N_RESERVED_WORDS][10], char* file_name, char* matrix_path){

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
	//Load transition table
	Output **transition_matrix = csv_parser(matrix_path);
	lexical_analyser(f_pointer, transition_matrix);

	//while(fgets(line, sizeof(line), f_pointer)){
	//	printf("Line: %s\n", line);
	//	lexical_analyser(line, transition_matrix);
	//}
	
}

void main(int argc, char** argv){
	
	//list of reserved words
	char reserved_words_list[N_RESERVED_WORDS][10] = {"CONST", "VAR", "PROCEDURE", "BEGIN", "END", "CALL", "WHILE", "DO", "ODD", "IF", "THEN"};	
	char consumable_characters[N_CONSUMABLE_CHARS][10] = {" "};
	char operators[N_OPERATORS][10] = {"+", "-", "*", "=", ">=", "<=", "=="};

	if(argc != 2){
		printf("ERROR! This analyser takes one and only one argument, which is the file name!\n");
		exit(1);
	}

	char *file_name = argv[1];
	char matrix_path[] = "../data/test.csv"; 

	printf("file name: %s\n", file_name);	
	
	printf("\n========== STARTING LEXICAL ANALYSIS! =======\n");
	syntatic_analyser(reserved_words_list, file_name, matrix_path);



}
