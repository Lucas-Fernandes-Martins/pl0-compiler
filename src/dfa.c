#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"headers.h"

int hash_function(int state, char char_consumed){
	return state*1000 + char_consumed - 'A';
}

void apply_specific_rules(char *symbol, Output output, Output* transition_vector){
	if(!strcmp(symbol, "dígito")){
		for(int i = 48; i <= 57; i++){
			transition_vector[i] = output;
		}	
	}else if(!strcmp(symbol, "letra")){
		for(int i = 0; i <= 255; i++){
			if(isalpha(i)){
				Output output_new_final;
				output_new_final.is_final = output.is_final;
				output_new_final.next_state = output.next_state;
				
				output_new_final.output = malloc(sizeof(char) * 70);
				sprintf(output_new_final.output, "%c", i);
				transition_vector[i] = output_new_final;
			}
		}	
	}else if(!strcmp(symbol, "outro_=")){
		for(int i = 0; i <= 255; i++){
			if(i == 61) continue;
			transition_vector[i] = output;
		}
	}else if(!strcmp(symbol, "outro_}")){
		for(int i = 0; i <= 255; i++){
			if(i == 125) continue;
			if(i == '\n') continue;
			transition_vector[i] = output;
		}
	}else if(!strcmp(symbol, "nao_digito")){
		for(int i = 0; i <= 255; i++){
			if(isdigit(i)) continue;
			transition_vector[i] = output;
		}
	}else if(!strcmp(symbol, "outro")){
		for (int i = 0; i <= 255; i++){
			if (isalnum(i)) continue;
			transition_vector[i] = output;
		}
	}
}

char* sanitize_symbol(char* symbol){
	if(!strcmp(symbol, "virgula")){
		return ",";
	}else if(!strcmp(symbol, "quebra_linha")){
		return "\n";
	}else if(!strcmp(symbol, "tab")){
		return  "	";
	}else{
		return symbol;
	}

}

Output** csv_parser(char* file_name){
	FILE* file_p = fopen(file_name, "r");
	Output** transition_matrix = malloc(NBR_STATES * sizeof(Output*));
	
	// allocate memory for transition matrix
	for(int i = 0; i < NBR_STATES; i++){		
		transition_matrix[i] = calloc(NBR_CHARS, sizeof(Output));
		// populate positions
		for(int j = 0; j < NBR_CHARS; j++){
			Output not_implemented;
			not_implemented.output = "<ERRO_CARAC_INVALIDO>";
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

		int current_state = atoi(strtok(line, ","));
		char *symbol = strtok(NULL, ",");
		symbol = sanitize_symbol(symbol);

		int next_state = atoi(strtok(NULL, ","));
		char *output = strtok(NULL, ",");

		int is_final = atoi(strtok(NULL, ","));

		Output new_output;
		new_output.next_state = next_state;
		new_output.is_final = is_final;
		new_output.output = strdup(output);

		if(strlen(symbol) == 1)
			transition_matrix[current_state][symbol[0]] = new_output;
		else
			apply_specific_rules(symbol, new_output, transition_matrix[current_state]);			

		lines_read += 1;
	}

	return transition_matrix;
}
