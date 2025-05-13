#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"

// Sets the list of reserved words
static const char reserved_words[NUM_RESERVED_WORDS][MAX_RESERVED_WORD_LENGHT] = {
    "CONST", "VAR", "PROCEDURE", "BEGIN", "END",
    "CALL", "WHILE", "DO", "ODD", "IF", "THEN"
};

/**
 * @brief Checks if the input string is a reserved word
 * @param word input string
 * @return 1 if reserved word, 0 otherwise
 */
int check_if_reserved_word(char *word){
	for(int i = 0; i < NUM_RESERVED_WORDS; i++)
		if(!strcmp(word, reserved_words[i])) return 1;
	
	return 0;
}

/**
 * @brief Lexical analyser function
 * @param input_file input file with the code to be compiled
 * @param transition_matrix transition matrix of the Mealy automata used
 * @return struct with the token read, its class and an int indicating if EOF was reached
 */
LexicalOutput lexical_analyser(FILE *input_file, Transition** transition_matrix){
	// Starts analysis in the 0 state
	State current_state;
	current_state.number = 0;
	current_state.is_final = 0;

	// Instantiates a standard output
	LexicalOutput lexical_output;
	lexical_output.end = 0;

	// Instantiates input and output variables
	char char_consumed;
	char capture_output[MAX_TOKEN_LENGHT];
	capture_output[0] = '\0';

	// Loops while the char read is different from EOF
	while((char_consumed = fgetc(input_file)) != EOF){
		// Checks the transition corresponding to the current state and char
		//if(char_consumed < 0){
		//	char_consumed = 0;
		//}
		Transition current_transition = transition_matrix[current_state.number][(unsigned char) char_consumed];

		// If the state is final, checks which case it belongs to
		if(current_transition.next_state.is_final){
			switch(current_transition.num_outputs){
				case 1: // If the transition to this state outputs only its class (Ex:"identificador"/"number")
					// If the identifier read is a reserved word, its class is updated
					if(check_if_reserved_word(capture_output)) strcpy(lexical_output.class, capture_output);
					// Otherwise, takes the class given by the output
					else strcpy(lexical_output.class, current_transition.output);
					break;
				case 2: // If the transition to this state outputs its token and class (Ex:"+ simb_mais")
					// Copies the current transition's output
					char *class = strdup(current_transition.output);
					// Stores the token part in "char* token" and class part in "char* class"
					char* token = strsep(&class, " ");
					// Appends the token to the current output token string
					strcat(capture_output, token);
					// Copies the class to the output class
					strcpy(lexical_output.class, class);
					free(token);
					break;
				default:
					printf("None or excessive number of outputs in Mealy machine!\n");
					exit(-1);
			}
			// Copies the token output string to the output struct
			strcpy(lexical_output.token, capture_output);

			// Backtracks if the current transition demands a backtrack
			if(current_transition.lookahead) fseek(input_file, -1, SEEK_CUR);

			// Output
			return lexical_output;
		} 
		else{
			// If the state is not final, appends its output to the current output token string
			strcat(capture_output, current_transition.output);
		}

		// Transitions to the next state
		current_state = current_transition.next_state;
	}

	lexical_output.end = 1;
	return lexical_output;
}

/**
 * @brief Frees the memory used by the transition matrix
 * @param transition_matrix transition matrix pointer
 */
void free_memory(Transition** transition_matrix) {
	for(int i = 0; i < 27; i++) {
		for(int j = 0; j < ASCII_EXTENDED_SIZE; j++) {
			// Checks if the output vriable can be freed
			if(strcmp(transition_matrix[i][j].output, "numero") != 0 
				&& strcmp(transition_matrix[i][j].output, "identificador") != 0 
				&& strcmp(transition_matrix[i][j].output, "<ERRO_SIMBOLO_INCOMPLETO>") != 0 
				&& strcmp(transition_matrix[i][j].output, "simb_maior_que") != 0 
				&& strcmp(transition_matrix[i][j].output, "simb_menor_que") != 0)
					// Frees the variable
					free(transition_matrix[i][j].output);
		}
		// Frees each line
		free(transition_matrix[i]);
	}
	// Frees the matrix
	free(transition_matrix);
}

/**
 * @brief Syntatic analyser function
 * @param input_file_name name of the input file with the code to be compiled
 * @param automata_path name of the file with the Mealy automata transitions
 */
void syntatic_analyser(char* input_file_name, char* automata_path){
	// Open input file
	FILE *input_file;
	input_file = fopen(input_file_name, "r");
	if(input_file == NULL){
		printf("Error: Unable to open input file!\n");
		exit(1);
	}

	// Open output file
	FILE *output_file;
	output_file = fopen("output.txt", "w");
	if(output_file == NULL){
		printf("Error: Unable to open output file!\n");
		exit(1);
	}

	// Load transition table representing the automata
	Transition **transition_matrix = csv_parser(automata_path);
	
	// Execution Loop
	while(1){
		// Execute lexical analyser
		LexicalOutput lexical_output = lexical_analyser(input_file, transition_matrix);
		
		// Reached EOF
		if(lexical_output.end){
			printf("\n========== END OF FILE REACHED! ========= \n");

			// Frees memory
			free_memory(transition_matrix);
			break;
		}
		// outputs lexical analysis to text file
		if(strlen(lexical_output.token) > 0) fprintf(output_file, "%s, %s \n", lexical_output.token, lexical_output.class);
	}

	// Closes input file
	fclose(input_file);
	// Closes output file
	fclose(output_file);
}

void main(int argc, char** argv){
	// Checks if the number of inputs is valid
	if(argc != 2){
		printf("ERROR! This analyser takes one and only one argument, which is the file name!\n");
		exit(1);
	}

	// Defines the input as the name of the input file
	char *input_file_name = argv[1];
	// Defines the automata transition table's file path
	char automata_path[] = "./data/transition_matrix.csv";
	
	printf("\n========== STARTING LEXICAL ANALYSIS! ========= \n\n");

	// Starts syntatic analysis
	syntatic_analyser(input_file_name, automata_path);
}
