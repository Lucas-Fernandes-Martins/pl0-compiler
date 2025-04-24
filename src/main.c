#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"headers.h"


LexicalOutput lexical_analyser(FILE *f_pointer, Output** transition_matrix){
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
	char capture_output[100];
	while((char_consumed = fgetc(f_pointer)) != EOF){
		//save input
		char original_char = char_consumed;
		//if(check_op(char_consumed)){
		//	char_consumed = '^';
			//capture_output[i++] = '\0';
		//}else{
		capture_output[i] = char_consumed;
		//}
		//if(is_letter(char_consumed, transition_matrix[current_state])) char_consumed = '?';
		Output output = transition_matrix[current_state][char_consumed];
		//printf("read: %c Current state: %d Next state: %d Output: %s\n", 
		//			char_consumed, current_state, output.next_state, output.output);
			
		current_state = output.next_state;
		if(output.is_final){

			if(!strcmp(output.output, "identifier") || !strcmp(output.output, "numero")){
				capture_output[i] = '\0';
				ungetc(original_char, f_pointer);
			}else{
				capture_output[++i] = '\0';
			}
			LexicalOutput lexical_output;
			strcpy(lexical_output.entity, output.output);
			strcpy(lexical_output.value, capture_output);
			lexical_output.end = 0;	
			return lexical_output;
		}

		i++;
	}
	
	LexicalOutput end_output;
	end_output.end = 1;
	return end_output;
}


int check_if_reserved_word(char *word, char reserved_words[N_RESERVED_WORDS][10]){
	
	for(int i = 0; i < N_RESERVED_WORDS; i++){
		if(!strcmp(word, reserved_words[i])) return 1;
	}

	return 0;

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
	
	int i = 0;
	while(1){
		
		LexicalOutput lexical_output = lexical_analyser(f_pointer, transition_matrix);
		
		if(lexical_output.end){
			printf("END OF FILE REACHED!\n");
			break;
		}


	
		//check if we want to print the output
		if(lexical_output.entity[0] == '!') continue;
		
		//check if identifier is reserved word		
		
		if(check_if_reserved_word(lexical_output.value, reserved_words)){
			printf("%s , %s \n", lexical_output.value, lexical_output.value);
		}else{
			printf("%s , %s \n", lexical_output.entity, lexical_output.value);
		}
		
	}

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
	char matrix_path[] = "./data/var.csv"; 

	printf("file name: %s\n", file_name);	
	
	printf("\n========== STARTING LEXICAL ANALYSIS! =======\n");
	syntatic_analyser(reserved_words_list, file_name, matrix_path);



}
