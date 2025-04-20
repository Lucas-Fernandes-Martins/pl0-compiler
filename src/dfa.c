#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"headers.h"

int hash_function(int state, char char_consumed){
	return state*1000 + char_consumed - 'A';
}

Output** csv_parser(char* file_name){
	FILE* file_p = fopen(file_name, "r");
	Output**transition_matrix = malloc(NBR_STATES*sizeof(Output*));
	
	//allocate memory for transition matrix
	for(int i = 0; i < NBR_STATES; i++){		
		transition_matrix[i] = calloc(NBR_CHARS,sizeof(Output));
		//populate positions
		for(int j = 0; j < NBR_CHARS; j++){
			Output not_implemented;
			not_implemented.output = "NOP";
			transition_matrix[i][j] = not_implemented;
		}
	}

	char line[100];

	if(file_p == NULL){
		printf("Error reading file!\n");
	}
	int lines_read = 0;
	while(fgets(line, sizeof(line), file_p)){
		if(lines_read++ == 0) continue; //discard csv header
		printf("Line: %s\n", line);

		int current_state = atoi(strtok(line, ","));
		char symbol = strtok(NULL, ",")[0];
		int next_state = atoi(strtok(NULL, ","));
		char *output = strtok(NULL, ",");
		//int hash_key = hash_function(current_state, symbol);
		printf("cs: %d sy: %c ns: %d o: %s \n",
		       current_state, symbol, next_state, output);
		Output new_output;
		new_output.next_state = next_state;
		new_output.output = strdup(output);
		transition_matrix[current_state][symbol] = new_output;
		printf("stored: %d | %s", transition_matrix[current_state][symbol].next_state, transition_matrix[current_state][symbol].output);
		lines_read += 1;
	}

	printf("hash elements: \n");
	Output output = transition_matrix[0]['V'];
	printf("state 0 read V -> next state: %d output: %si\n", output.next_state, output.output);
	Output output2 = transition_matrix[1]['A'];
	printf("state 1 read A -> next state: %d output: %si\n", output2.next_state, output2.output);


	return transition_matrix;
}


void test_hash(int argv, char** argc){
	char* file_name = argc[1];
	csv_parser(file_name);
}
