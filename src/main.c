#include<stdio.h>
#include<stdlib.h>

#define N_RESERVED_WORDS 11
#define N_CONSUMABLE_CHARS 1
#define N_OPERATORS 7
#define N_STATES 1
#define N_CHARS 50

typedef struct {
	int number;
	char char_consumed;
} State;

//Moore's machine state transition output;
typedef struct {
	int next_state;
	char output;
} Output;


int char_index(char type) {
    return type - 'A';
}

int hash_function(int state, char char_consumed){
	return state*N_STATES + char_index(char_consumed);
}

void lexical_analyser(char *string){
	int number = 0;
	
	//Initialize hash table
	Output *hash_table = malloc(sizeof(Output)*N_STATES*N_CHARS);

	Output output1;
	output1.next_state = 1;
	output1.output = 'a';

	hash_table[0] = output1;	
	
	int is_final_state = 0;
	int i = 0;
	int current_state = 0;
	while(!is_final_state){
		char char_consumed = string[i];
		int hash_value = hash_function(current_state, char_consumed);
		Output output = hash_table[hash_value];
		printf("Current state: %d Next state: %d Output: %d\n", 
				current_state, output.next_state, output.output);
		current_state = output.next_state;
		i += 1;
	}	
}


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
		lexical_analyser(line);
	}
	
}

void main(int argc, char** argv){
	
	//list of reserved words
	char reserved_words_list[N_RESERVED_WORDS][10] = {"CONST""VAR", "PROCEDURE", "BEGIN", "END", "CALL", "WHILE", "DO", "ODD", "IF", "THEN"};	
	char consumable_characters[N_CONSUMABLE_CHARS][10] = {" "};
	char operators[N_OPERATORS][10] = {"+", "-", "*", "=", ">=", "<=", "=="};

	if(argc != 2){
		printf("ERROR! This analyser takes one and only one argument, which is the file name!\n");
		exit(1);
	}

	char *file_name = argv[1];

	printf("file name: %s\n", file_name);	
	
	printf("\n========== STARTING LEXICAL ANALYSIS! =======\n");
	syntatic_analyser(reserved_words_list, file_name);



}
