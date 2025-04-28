#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"

static const char reserved_words[NUM_RESERVED_WORDS][MAX_RESERVED_WORD_LENGHT] = {
    "CONST", "VAR", "PROCEDURE", "BEGIN", "END",
    "CALL", "WHILE", "DO", "ODD", "IF", "THEN"
};

LexicalOutput lexical_analyser(FILE *f_pointer, Transition** transition_matrix){
	State current_state;
	current_state.number = 0;
	current_state.is_final = 0;

	LexicalOutput lexical_output;

	int i = 0;
	char char_consumed;
	char capture_output[128];
	capture_output[0] = '\0';

	while((char_consumed = fgetc(f_pointer)) != EOF){
		//save input
		char original_char = char_consumed;

		Transition current_transition = transition_matrix[current_state.number][char_consumed];

		printf("%s\n", current_transition.output);

		/*
		if(current_state.is_final == 1){
			break;
		}

		lexical_output.token[i] = current_transition.output[0];
		current_state = current_transition.next_state;

		
		
			//check if identifier is reserved word		
			if(check_if_reserved_word(lexical_output.value)){
				printf("%s , %s \n", lexical_output.value, lexical_output.value);
			}else{
				printf("%s , %s \n", lexical_output.value, lexical_output.entity);
			}

			if(!strcmp(output.output, "identificador") || !strcmp(output.output, "numero")){
				capture_output[i] = '\0';
				ungetc(original_char, f_pointer);
			}else{
				if(capture_output[i] == ' ' || capture_output[i] == '\n' || capture_output[i] == '\t') capture_output[i] = '\0';
				else capture_output[++i] = '\0';
			}
			LexicalOutput lexical_output;
			strcpy(lexical_output.entity, output.output);
			strcpy(lexical_output.value, capture_output);
			lexical_output.end = 0;	
			return lexical_output;
		} else{
			strcat(capture_output, output.output);
		}

		*/
		i++;
	}
	
	return lexical_output;
}


int check_if_reserved_word(char *word){
	for(int i = 0; i < NUM_RESERVED_WORDS; i++){
		if(!strcmp(word, reserved_words[i])) return 1;
	}

	return 0;

}


void syntatic_analyser(char* file_name, char* matrix_path){
	//Open file
	FILE *f_pointer;
	f_pointer = fopen(file_name, "r");

	if(f_pointer == NULL){
		printf("Error: Invalid file!\n");
		exit(1);
	}

	//Load transition table
	Transition **transition_matrix = csv_parser(matrix_path);
	
	while(1){
		LexicalOutput lexical_output = lexical_analyser(f_pointer, transition_matrix);
		
		if(lexical_output.end){
			printf("\n========== END OF FILE REACHED! ========= \n");
			break;
		}

		// printf("%s, %s \n", lexical_output.token, lexical_output.class);
	}
}

void main(int argc, char** argv){
	// set the list of reserved words

	if(argc != 2){
		printf("ERROR! This analyser takes one and only one argument, which is the file name!\n");
		exit(1);
	}

	char *file_name = argv[1];
	char matrix_path[] = "./data/transition_matrix.csv"; 

	//Transition **transition_matrix = csv_parser(matrix_path);
	
	printf("\n========== STARTING LEXICAL ANALYSIS! ========= \n\n");
	syntatic_analyser(file_name, matrix_path);
}
