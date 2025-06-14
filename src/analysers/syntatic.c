#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "syntatic.h"

Token current_token;
static HashTable *first_follow_table;


//Prints syntathical error message
void print_syntatical_error(char **expected_types, int num_expected){
	printf("Syntatic error on line %d: found '%s', expected {", current_token.line, current_token.lexeme);
	for(int i = 0; i < num_expected - 1; i++) printf("'%s', ", type_to_lexeme(expected_types[i]));
	printf("'%s'}\n", type_to_lexeme(expected_types[num_expected-1]));

	return;
}

//Panic mode implementation
void handle_error(FILE *input_file, StringSet follow_set, char **expected_types, int num_expected){
	if(strcmp(current_token.type, "EOF") == 0) exit(1);

	print_syntatical_error(expected_types, num_expected);

	while(!is_in_string_set(&follow_set, current_token.type)){
		current_token = get_token(input_file);
	}
	
	return;
}


/**
 * @brief Syntatic analyser function
 * @param 
 */
void syntatic_analyser(FILE *input_file){
	// For syntatic analysis
	first_follow_table = hash_create(); // Create first and follow table with hash structure
	insert_first_follow(first_follow_table); // Populate first and follow table 

	// Open output file
	FILE *output_file;
	output_file = fopen("output.txt", "w");
	if(output_file == NULL){
		printf("Error: Unable to open output file!\n");
		exit(1);
	}

	/////////////////////////////////////////////////////// 
	// while(1){
	// 	Token nekot = get_token(input_file);

	// 	if(strcmp(nekot.type, "EOF") == 0){
	// 		break;
	// 	}

	// 	printf("%s, %s : line %d\n", nekot.lexeme, nekot.type, nekot.line);
	// }
	//////////////////////////////////////////////////////

	// Starts analysis from programa
	programa(input_file);

	// Verify if the last type is EOF
		// If the last token is not the EOF, error

	// Closes output file
	fclose(output_file);

	// Free allocated memory
	destroy_transition_matrix(transition_matrix, NUM_STATES);
}


void programa(FILE *input_file){
	// Search the followers of "<programa>" in hash structure
	StringSet follow_set = hash_get(first_follow_table, "<programa>")->follow;

	// Reads the first token of input file
	current_token = get_token(input_file);

	// Recursion, calling bloco
	bloco(input_file, add_string_set_item(follow_set, (char *[]){"simb_ponto"}, 1));

	// Checks if token is different from "simb_ponto"
	if(strcmp(current_token.type, "simb_ponto") != 0){
		handle_error(input_file, follow_set, (char *[]){"simb_ponto"}, 1);
	}
}

//Procedimento block
void bloco(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	// Recursion, calling declaracao
	follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<comando>")->first.items, hash_get(first_follow_table, "<comando>")->first.count);
	declaracao(input_file, follow_set);

	// Recursion, calling comando
	follow_set = parent_follow_set;
	comando(input_file, follow_set);
}

//Procedimento declaracao
void declaracao(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	// Recursion, calling constante
    follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<variavel>")->first.items, hash_get(first_follow_table, "<variavel>")->first.count);
    constante(input_file, follow_set);

    // Recursion, calling variavel
    follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<procedimento>")->first.items, hash_get(first_follow_table, "<procedimento>")->first.count);
    variavel(input_file, follow_set);

    // Recursion, calling procedimento
    follow_set = parent_follow_set;
    procedimento(input_file, follow_set);
}


//Procedimento constante
void constante(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	// If token's type is "CONST"
	if(strcmp(current_token.type, "CONST") == 0){ 
		current_token = get_token(input_file);
	}
	else{
		return; // Consider lambda
	}

	// If next token's type is "identificador"
	if(strcmp(current_token.type, "identificador") == 0){ 
		current_token = get_token(input_file);
	}
	else{ // If token's type is not "identifier"
		follow_set = add_string_set_item(parent_follow_set, (char *[]){"simb_igual"}, 1);
		handle_error(input_file, follow_set, (char *[]){"identificador"}, 1);

		// If the token is NOT local's follow, return
		if(strcmp(current_token.type, "simb_igual") != 0) return;
	}

	// If next token's type is "simb_igual"
	if(strcmp(current_token.type, "simb_igual") == 0){ 
		current_token = get_token(input_file);
	}
	else{ // If token's type is not "simb_igual"
		follow_set = add_string_set_item(parent_follow_set, (char *[]){"numero"}, 1);
		handle_error(input_file, follow_set, (char *[]){"simb_igual"}, 1);

		// If the token is NOT local's follow, return
		if(strcmp(current_token.type, "numero") != 0) return;
	}

	// If next token's type is "numero"
	if(strcmp(current_token.type, "numero") == 0){ 
		current_token = get_token(input_file);
	}
	else{ // If token's type is not "numero"
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_const>")->first.items, hash_get(first_follow_table, "<mais_const>")->first.count);
		handle_error(input_file, follow_set, (char *[]){"numero"}, 1);

		// If the token is NOT local's follow, return
		if(is_in_string_set(&hash_get(first_follow_table, "<mais_const>")->first, current_token.type) == 0) return;
	}

	// Recursion, calling mais_const
	follow_set = add_string_set_item(parent_follow_set, (char *[]){"simb_ponto_virgula"}, 1);
	mais_const(input_file, follow_set);

	// If next token's type is "simb_ponto_virgula"
	if(strcmp(current_token.type, "simb_ponto_virgula") == 0){
		current_token = get_token(input_file);
		return;
	}
	else{ // If token's type is not "simb_ponto_virgula"
		follow_set = parent_follow_set;
		handle_error(input_file, follow_set, (char *[]){"simb_ponto_virgula"}, 1);

		// If the token is parent's follow, return
		if(is_in_string_set(&parent_follow_set, current_token.type)) return;
	}
}		


//Procedimento mais_const
void mais_const(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	// If token's type is "simb_virgula"
	if(strcmp(current_token.type, "simb_virgula") == 0){
		current_token = get_token(input_file);
	}
	else{ // If token's type is not "simb_virgula"
		return; // Consider lambda
	}

	// If token's type is "identificador"
	if(strcmp(current_token.type, "identificador") == 0){
		current_token = get_token(input_file);
	}
	else{ // If next token's type is not "identificador"
		follow_set = add_string_set_item(parent_follow_set, (char *[]){"simb_igual"}, 1);
		handle_error(input_file, follow_set, (char *[]){"identificador"}, 1);

		// If the token is NOT local's follow, return
		if(strcmp(current_token.type, "simb_igual") != 0) return;
	}

	if(strcmp(current_token.type, "simb_igual") == 0){
		current_token = get_token(input_file);
	}
	else{ // If next token's type is not "simb_igual"
		follow_set = add_string_set_item(parent_follow_set, (char *[]){"numero"}, 1);
		handle_error(input_file, follow_set, (char *[]){"simb_ponto_virgula"}, 1);
		
		// If the token is NOT local's follow, return
		if(strcmp(current_token.type, "numero") != 0) return;
	}

	if(strcmp(current_token.type, "numero") == 0){
		current_token = get_token(input_file);
	}
	else{ // If next token's type is not "numero"
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_const>")->first.items, hash_get(first_follow_table, "<mais_const>")->first.count);
		handle_error(input_file, follow_set, (char *[]){"numero"}, 1);

		// If the token is parent's follow, return
		if(is_in_string_set(&hash_get(first_follow_table, "<mais_const>")->first, current_token.type) == 0) return;
	}

	// Recursion, calling mais_const
	follow_set = parent_follow_set;
	mais_const(input_file, follow_set);
}


//Procedimento variavel
void variavel(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	if(strcmp(current_token.type, "VAR") == 0){
		current_token = get_token(input_file);
	}
	else{ // If token's type is not "VAR"
		return; // Consider lambda
	}

	if(strcmp(current_token.type, "identificador") == 0){
		current_token = get_token(input_file);
	}
	else{ // If next token's type is not "identificador"
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_var>")->first.items, hash_get(first_follow_table, "<mais_var>")->first.count);
		handle_error(input_file, follow_set, (char *[]){"identificador"}, 1);

		// If the token is NOT local's follow, return
		if(is_in_string_set(&hash_get(first_follow_table, "<mais_var>")->first, current_token.type) == 0) return;
	}

	// Recursion, calling mais_var
	follow_set = parent_follow_set;
	mais_var(input_file, parent_follow_set);

	if(strcmp(current_token.type, "simb_ponto_virgula") == 0){
		current_token = get_token(input_file);
	}
	else{ // If next token's type is not "simb_ponto_virgula"
		follow_set = parent_follow_set;
		handle_error(input_file, follow_set, (char *[]){"simb_ponto_virgula"}, 1);

		// If the token is parent's follow, return
		if(is_in_string_set(&parent_follow_set, current_token.type)) return;
	}

	return;
}

//Procedimento mais_var
void mais_var(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	if(strcmp(current_token.type, "simb_virgula") == 0){
		current_token = get_token(input_file);
	}
	else{ // If token's type is not "simb_virgula"
		return; // Consider lambda
	}

	if(strcmp(current_token.type, "identificador") == 0){
		current_token = get_token(input_file);
	}
	else{ // If next token's type is not "identificador"
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_var>")->first.items, hash_get(first_follow_table, "<mais_var>")->first.count);
		handle_error(input_file, follow_set, (char *[]){"identificador"}, 1);

		// If the token is NOT local's follow, return
		if(is_in_string_set(&hash_get(first_follow_table, "<mais_var>")->first, current_token.type) == 0) return;
	}

	// Recursion, calling mais_var
	follow_set = parent_follow_set;
	mais_var(input_file, follow_set);
}

//Procedimento procedimento
void procedimento(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	if(strcmp(current_token.type, "PROCEDURE") == 0){
		current_token = get_token(input_file);
	}
	else{ // If token's type is not "PROCEDURE"
		return; // Consider lambda
	}

	if(strcmp(current_token.type, "identificador") == 0){
		current_token = get_token(input_file);
	}
	else{ // If next token's type is not "identificador"
		follow_set = add_string_set_item(parent_follow_set, (char *[]){"simb_ponto_virgula"}, 1);
		handle_error(input_file, follow_set, (char *[]){"indentificador"}, 1);

		// If the token is NOT local's follow, return
		if(strcmp(current_token.type, "simb_ponto_virgula") != 0) return;
	}

	if(strcmp(current_token.type, "simb_ponto_virgula") == 0){
		current_token = get_token(input_file);
	}
	else{ // If next token's type is not "simb_ponto_virgula"
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<bloco>")->first.items, hash_get(first_follow_table, "<bloco>")->first.count);
		handle_error(input_file, follow_set, (char *[]){"simb_ponto_virgula"}, 1);

		// If the token is NOT local's follow, return
		if(is_in_string_set(&hash_get(first_follow_table, "<bloco>")->first, current_token.type) == 0) return;
	}

	// Recursion, calling bloco
	follow_set = add_string_set_item(parent_follow_set, (char *[]){"simb_ponto_virgula"}, 1);
	bloco(input_file, follow_set);

	if(strcmp(current_token.type, "simb_ponto_virgula") == 0){
		current_token = get_token(input_file);
	}
	else{ // If next token's type is not "simb_ponto_virgula"
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<procedimento>")->first.items, hash_get(first_follow_table, "<procedimento>")->first.count);
		handle_error(input_file, follow_set, (char *[]){"simb_ponto_virgula"}, 1);

		// If the token is NOT local's follow, return
		if(is_in_string_set(&hash_get(first_follow_table, "<procedimento>")->first, current_token.type) == 0) return;
	}

	// Recursion, calling procedimento
	follow_set = parent_follow_set;
	procedimento(input_file, follow_set);
}


//Procedimento comando
void comando(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	// Checks if token is "identificador"
	if(strcmp(current_token.type, "identificador") == 0){
		current_token = get_token(input_file);

		if(strcmp(current_token.type, "simb_atribuicao") == 0){
			current_token = get_token(input_file);
		}
		else{ // If next token's type is not "simb_atribuicao"
			follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<expressao>")->first.items, hash_get(first_follow_table, "<expressao>")->first.count);
			handle_error(input_file, follow_set, (char *[]){"simb_atribuicao"}, 1);

			// If the token is NOT local's follow, return
			if(is_in_string_set(&hash_get(first_follow_table, "<expressao>")->first, current_token.type)) return;
		}

		follow_set = parent_follow_set;
		expressao(input_file, follow_set);
	}
	// Checks if token is "CALL"
	else if(strcmp(current_token.type, "CALL") == 0){
		current_token = get_token(input_file);

		if(strcmp(current_token.type, "identificador") == 0){
			current_token = get_token(input_file);
			return;
		}
		else{
			follow_set = parent_follow_set;
			handle_error(input_file, follow_set, (char *[]){"indentificador"}, 1);

			// If the token is parent's follow, return
			if(is_in_string_set(&parent_follow_set, current_token.type)) return;
		}
	}
	// Checks if token is "BEGIN"
	else if(strcmp(current_token.type, "BEGIN") == 0){
		current_token = get_token(input_file);

		// Recursion, calling comando
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_cmd>")->first.items, hash_get(first_follow_table, "<mais_cmd>")->first.count);
		comando(input_file, follow_set);

		// Recursion, calling mais_cmd
		follow_set = add_string_set_item(parent_follow_set, (char *[]){"END"}, 1);
		mais_cmd(input_file, follow_set);

		if(strcmp(current_token.type, "END") == 0){
			current_token = get_token(input_file);
		}
		else{
			follow_set = parent_follow_set;
			handle_error(input_file, follow_set, (char *[]){"END"}, 1);
		}
	}
	// Checks if token is "IF"
	else if(strcmp(current_token.type, "IF") == 0){
		current_token = get_token(input_file);

		// Recursion, calling condicao
		follow_set = add_string_set_item(parent_follow_set, (char *[]){"THEN"}, 1);
		condicao(input_file, follow_set);

		if(strcmp(current_token.type, "THEN") == 0){
			current_token = get_token(input_file);
		}
		else{
			follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<comando>")->first.items, hash_get(first_follow_table, "<comando>")->first.count);
			handle_error(input_file, follow_set, (char *[]){"THEN"}, 1);

			// If the token is NOT local's follow, return
			if(is_in_string_set(&hash_get(first_follow_table, "<comando>")->first, current_token.type) == 0) return;
		}

		// Recursion, calling comando
		follow_set = parent_follow_set;
		comando(input_file, follow_set);
	}
	// Checks if token is "WHILE"
	else if(strcmp(current_token.type, "WHILE") == 0){
		current_token = get_token(input_file);	

		// Recursion, calling condicao
		follow_set = add_string_set_item(parent_follow_set, (char *[]){"DO"}, 1);
		condicao(input_file, follow_set);

		if(strcmp(current_token.type, "DO") == 0){
			current_token = get_token(input_file);
		}
		else{
			follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<comando>")->first.items, hash_get(first_follow_table, "<comando>")->first.count);
			handle_error(input_file, follow_set, (char *[]){"DO"}, 1);

			// If the token is NOT local's follow, return
			if(is_in_string_set(&hash_get(first_follow_table, "<comando>")->first, current_token.type) == 0) return;
		}

		// Recursion, calling comando
		follow_set = parent_follow_set;
		comando(input_file, follow_set);	
	}
	else{
		return; // Consider lambda
	}

	return;
}


//Procedimento mais_cmd
void mais_cmd(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	if(strcmp(current_token.type, "simb_ponto_virgula") == 0){
		current_token = get_token(input_file);
	}
	else{
		// If token's type is not local's follower, consider lambda
		if(is_in_string_set(&hash_get(first_follow_table, "<comando>")->first, current_token.type) == 0) 
			return;
		
		print_syntatical_error((char *[]){"simb_ponto_virgula"}, 1);
	}

	// Recursion, calling comando
	follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_cmd>")->first.items, hash_get(first_follow_table, "<mais_cmd>")->first.count);
	comando(input_file, follow_set);

	// Recursion, calling mais_cmd
	follow_set = parent_follow_set;
	mais_cmd(input_file, follow_set);
}


//Procedimento expressao
void expressao(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	// Recursion, calling operador_unario
	follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<termo>")->first.items, hash_get(first_follow_table, "<termo>")->first.count);
	operador_unario(input_file);

	// Recursion, calling termo
	follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_termos>")->first.items, hash_get(first_follow_table, "<mais_termos>")->first.count);
	termo(input_file, follow_set);

	// Recursion, calling mais_termos
	follow_set = parent_follow_set;
	mais_termos(input_file, follow_set);
}

//Procedimento operador_unario
void operador_unario(FILE *input_file){
	// Checks if token's type is "simb_menos"
	if(strcmp(current_token.type, "simb_menos") == 0){
		current_token = get_token(input_file);
	}
	// Checks if token's type is "simb_mais"
	else if(strcmp(current_token.type, "simb_mais") == 0){
		current_token = get_token(input_file);
	}
	else{
		return; // Consider lambda
	}

	return;
}


//Procedimento termo
void termo(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	// Recursion, calling fator
	follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_fatores>")->first.items, hash_get(first_follow_table, "<mais_fatores>")->first.count);
	fator(input_file, follow_set);

	// Recursion, calling mais_fatores
	follow_set = parent_follow_set;
	mais_fatores(input_file, follow_set);
}

void mais_termos(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;
	
	// Checks if token's type is "simb_mais"
	if(strcmp(current_token.type, "simb_mais") == 0){
		current_token = get_token(input_file);

		// Recursion, calling termo
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_termos>")->first.items, hash_get(first_follow_table, "<mais_termos>")->first.count);
		termo(input_file, follow_set);

		// Recursion, calling mais_termos
		follow_set = parent_follow_set;
		mais_termos(input_file, follow_set);
	}
	// Checks if token's type is "simb_menos"
	else if(strcmp(current_token.type, "simb_menos") == 0){
		current_token = get_token(input_file);

		// Recursion, calling termo
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_termos>")->first.items, hash_get(first_follow_table, "<mais_termos>")->first.count);
		termo(input_file, follow_set);

		// Recursion, calling mais_termos
		follow_set = parent_follow_set;
		mais_termos(input_file, follow_set);
	}
	else{
		return; // Consider lambda
	}
}


//Procedimento fator
void fator(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	// Checks if token's type is "identificador"
	if(strcmp(current_token.type, "identificador") == 0){
		current_token = get_token(input_file);
	}
	// Checks if token's type is "numero"
	else if(strcmp(current_token.type, "numero") == 0){
		current_token = get_token(input_file);
	}
	// Checks if token's type is "simb_parent_esq"
	else if(strcmp(current_token.type, "simb_parent_esq") == 0){
		current_token = get_token(input_file);

		// Recursion, calling expressao
		follow_set = add_string_set_item(parent_follow_set, (char *[]){")"}, 1);
		expressao(input_file, follow_set);

		if(strcmp(current_token.type, "simb_parent_dir") == 0){
			current_token = get_token(input_file);
		}
		else{ // If token's type is not "simb_parent_dir"
			follow_set = parent_follow_set;
			handle_error(input_file, follow_set, (char *[]){"simb_parent_dir"}, 1);
		}
	}
	else{ // If token's type is neither above
		follow_set = parent_follow_set;
		handle_error(input_file, follow_set, hash_get(first_follow_table, "<fator>")->first.items, hash_get(first_follow_table, "<fator>")->first.count);
	}

	return;
}


//Procedimento mais_fatores
void mais_fatores(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	if(strcmp(current_token.type, "simb_mult") == 0){
		current_token = get_token(input_file);

		// Recursion, calling fator
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_fatores>")->first.items, hash_get(first_follow_table, "<mais_fatores>")->first.count);
		fator(input_file, follow_set);

		// Recursion, calling mais_fatores		
		follow_set = parent_follow_set;
		mais_fatores(input_file, follow_set);

	}
	else if(strcmp(current_token.type, "simb_div") == 0){
		current_token = get_token(input_file);

		// Recursion, calling fator
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<mais_fatores>")->first.items, hash_get(first_follow_table, "<mais_fatores>")->first.count);
		fator(input_file, follow_set);

		// Recursion, calling mais_fatores		
		follow_set = parent_follow_set;
		mais_fatores(input_file, follow_set);
	}
	else{
		return; // Consider lambda
	}
}

//Procedimento condicao
void condicao(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;

	if(strcmp(current_token.type, "ODD") == 0){
		current_token = get_token(input_file);

		// Recursion, calling expressao
		follow_set = parent_follow_set;
		expressao(input_file, follow_set);

	}
	else{ // If token's type is not "ODD"
		// Recursion, calling expressao
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<relacional>")->first.items, hash_get(first_follow_table, "<relacional>")->first.count);
		expressao(input_file, follow_set);

		// Recursion, calling relacional
		follow_set = add_string_set_item(parent_follow_set, hash_get(first_follow_table, "<expressao>")->first.items, hash_get(first_follow_table, "<expressao>")->first.count);
		relacional(input_file, follow_set);

		// Recursion, calling expressao
		follow_set = parent_follow_set;
		expressao(input_file, follow_set);
	}

	return;
}


//Procedimento relacional
void relacional(FILE *input_file, StringSet parent_follow_set){
	StringSet follow_set;
	
	if(strcmp(current_token.type, "simb_igual") == 0){
		current_token = get_token(input_file);
	}
	else if(strcmp(current_token.type, "simb_diferente") == 0){
		current_token = get_token(input_file);
	}
	else if(strcmp(current_token.type, "simb_menor_que") == 0){
		current_token = get_token(input_file);
	}
	else if(strcmp(current_token.type, "simb_menor_igual") == 0){
		current_token = get_token(input_file);
	}
	else if(strcmp(current_token.type, "simb_maior_que") == 0){
		current_token = get_token(input_file);
	}
	else if(strcmp(current_token.type, "simb_maior_igual") == 0){
		current_token = get_token(input_file);
	}
	else{ // If token's type is neither above
		follow_set = parent_follow_set;
		handle_error(input_file, follow_set, hash_get(first_follow_table, "<relacional>")->first.items, hash_get(first_follow_table, "<relacional>")->first.count);
	}

	return;
}