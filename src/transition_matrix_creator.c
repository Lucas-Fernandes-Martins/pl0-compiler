#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "headers.h"

void apply_specific_rules(char *symbol, Transition new_transition, Transition* transition_vector){
	if(!strcmp(symbol, "digito")){
		for(int i = 48; i <= 57; i++){
			new_transition.output = malloc(sizeof(char) * 2);
			sprintf(new_transition.output, "%c", i);
			free(transition_vector[i].output);
			transition_vector[i] = new_transition;
		}	
	}else if(!strcmp(symbol, "letra")){
		for(int i = 0; i <= 255; i++){
			if(isalpha(i)){
				new_transition.output = malloc(sizeof(char) * 2);
				sprintf(new_transition.output, "%c", i);
				free(transition_vector[i].output);
				transition_vector[i] = new_transition;
			}
		}	
	}else if(!strcmp(symbol, "nao_digito")){
		new_transition.output = "numero";
		new_transition.lookahead = 1;
		for(int i = 0; i <= 255; i++){
			if(isdigit(i)) continue;
			free(transition_vector[i].output);
			transition_vector[i] = new_transition;
		}
	}else if(!strcmp(symbol, "outro")){
		new_transition.output = "identificador";
		new_transition.lookahead = 1;
		for (int i = 0; i <= 255; i++){
			if (isalnum(i)) continue;
			free(transition_vector[i].output);
			transition_vector[i] = new_transition;
		}
	}else if(!strcmp(symbol, "outro_:=")){
		new_transition.output = "<ERRO_SIMBOLO_INCOMPLETO>";
		new_transition.lookahead = 1;
		for(int i = 0; i <= 255; i++){
			if(i == '=') continue;
			free(transition_vector[i].output);
			transition_vector[i] = new_transition;
		}
	}else if(!strcmp(symbol, "outro_=")){
		new_transition.output = "simb_maior_que";
		new_transition.lookahead = 1;
		for(int i = 0; i <= 255; i++){
			if(i == '=') continue;
			free(transition_vector[i].output);
			transition_vector[i] = new_transition;
		}
	}else if(!strcmp(symbol, "outro_>_=")){
		new_transition.output = "simb_menor_que";
		new_transition.lookahead = 1;
		for(int i = 0; i <= 255; i++){
			if(i == '=' || i == '>') continue;
			free(transition_vector[i].output);
			transition_vector[i] = new_transition;
		}
	}else if(!strcmp(symbol, "outro_}")){
		new_transition.output = "\0";
		for(int i = 0; i <= 255; i++){
			if(i == '}' || i == '\n') continue;
			free(transition_vector[i].output);
			transition_vector[i] = new_transition;
		}
	}
}

char* sanitize_token(char* symbol){
	if(!strcmp(symbol, "virgula"))
		return ",";
	else if(!strcmp(symbol, "tab"))
		return "\t";
	else if(!strcmp(symbol, "espaco"))
		return " ";
	else if(!strcmp(symbol, "quebra_linha"))
		return "\n";
	else if(!strcmp(symbol, "lambda"))
		return "\0";

	return symbol;
}

Transition** csv_parser(char* file_name){
	FILE* file_p = fopen(file_name, "r");

	if(file_p == NULL){
		printf("Error reading file!\n");
		exit(0);
	}

	char line[128];
	char buffer[128];

	// get number of states and transitions from the first line
	fgets(line, sizeof(line), file_p);
	int number_states = atoi(strtok(line, ","));
	int number_transitions = atoi(strtok(NULL, ","));

	// create and allocate memory for the transition matrix
	Transition** transition_matrix = malloc(number_states * sizeof(Transition*));
	
	for(int i = 0; i < number_states; i++){		
		transition_matrix[i] = calloc(ASCII_EXTENDED_SIZE, sizeof(Transition));

		// populate positions
		for(int j = 0; j < ASCII_EXTENDED_SIZE; j++){
			Transition not_implemented;
			not_implemented.next_state.number = 0;
			not_implemented.next_state.is_final = 1;
			not_implemented.lookahead = 0;
			not_implemented.num_outputs = 2;
			sprintf(buffer, "%c <ERRO_CARACTERE_INVALIDO>", (char) j);
			not_implemented.output = strdup(buffer);
			transition_matrix[i][j] = not_implemented;
		}
	}

	fgets(line, sizeof(line), file_p); // discard csv header

	while(fgets(line, sizeof(line), file_p)){
		int current_state = atoi(strtok(line, ","));
		//printf("current_state:%d\n", current_state);
		
		char *symbol = strtok(NULL, ",");
		symbol = sanitize_token(symbol);
		//printf("symbol:%s\n", symbol);

		int num_outputs = atoi(strtok(NULL, ","));
		//printf("num_outputs:%d\n", num_outputs);

		char *output = strtok(NULL, ",");
		output = sanitize_token(output);
		//printf("output:%s\n", output);

		int next_state = atoi(strtok(NULL, ","));
		//printf("next_state:%d\n", next_state);

		int next_state_is_final = atoi(strtok(NULL, ","));
		//printf("next_state_is_final:%d\n\n", next_state_is_final);

		
		Transition new_transition;	
		new_transition.num_outputs = num_outputs;
		new_transition.next_state.number = next_state;
		new_transition.next_state.is_final = next_state_is_final;

		if(strlen(symbol) == 1){
			new_transition.output = strdup(output);
			free(transition_matrix[current_state][symbol[0]].output);
			transition_matrix[current_state][symbol[0]] = new_transition;
		}
		else{
			apply_specific_rules(symbol, new_transition, transition_matrix[current_state]);	
		}
	}		

	return transition_matrix;
}
