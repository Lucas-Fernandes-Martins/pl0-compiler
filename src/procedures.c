#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"

LexicalOutput get_token(FILE *input_file, Transition** transition_matrix){
	LexicalOutput lexical_output;
	while(1){
		lexical_output = lexical_analyser(input_file, transition_matrix);
		if(strcmp(lexical_output.token, "")) return lexical_output;
	}
}

FirstFollowSet add_follow(FirstFollowSet data, const char *symbol) {
    // Check if the symbol is already in the follow set
    for (int i = 0; i < data.followCount; i++) {
        if (strcmp(data.follow[i], symbol) == 0) {
            printf("Simbolo '%s' ja existe no conjunto de seguidores.\n", symbol);
            return data; // Symbol already exists in follow set
        }
    }
    // Add the symbol to the follow set
    strcpy(data.follow[data.followCount++], symbol);

    return data;
}

/**
 * @brief Checks if a given symbol (string) is in the FOLLOW set.
 * @param data The FirstFollowSet structure containing the FOLLOW set.
 * @param symbol The symbol to check for.
 * @return 1 if the symbol is in the FOLLOW set, 0 otherwise.
 */
int is_in_follow_set(FirstFollowSet data, const char *symbol) {
    for (int i = 0; i < data.followCount; i++) {
        if (strcmp(data.follow[i], symbol) == 0) {
            return 1; // Symbol found in follow set
        }
    }
    return 0; // Symbol not found in follow set
}

int is_in_first_set(FirstFollowSet data, const char *symbol) {
    for (int i = 0; i < data.firstCount; i++) {
        if (strcmp(data.first[i], symbol) == 0) {
            return 1; // Symbol found in follow set
        }
    }
    return 0; // Symbol not found in follow set
}

void variavel(FILE *input_file, Transition** transition_matrix, FirstFollowSet data, HashNode* hashTable[]) {
    LexicalOutput lexical_output = lexical_analyser(input_file, transition_matrix);

    if(strcmp(lexical_output.token, "VAR")){
        printf("Error: Expected 'VAR' but found '%s'\n", lexical_output.token);
        //inserir funcao de modo panico
    }else{
        lexical_output = lexical_analyser(input_file, transition_matrix);
        if(strcmp(lexical_output.class, "identificador")){
            printf("Error: Expected identifier but found '%s' ' '  \n", lexical_output.token);
            //inserir funcao de modo panico
        }else{
            lexical_output = lexical_analyser(input_file, transition_matrix);
            if(!strcmp(lexical_output.token, ";")){
                printf("Declaracao de variaveis identificada! \n");                
            //}else if(!strcmp(lexical_output.token, ",")){
            }else if(is_in_first_set(*hash_get(hashTable, "<mais_var>"), lexical_output.token)){
                return_token(input_file, lexical_output);
                // adicionar ; a lista de seguidores
                FirstFollowSet new_data = add_follow(data, ";");
                printf("Chamando recursao! \n");
                mais_var(input_file, transition_matrix, new_data, hashTable);         
            }else{
                printf("Error: Expected ';' or ',' but found '%s'\n", lexical_output.token);
                //inserir funcao de modo panico
            }    
        }
        printf("Declaracao de variaveis identificada \n");

    }
}

void programa(FILE *input_file, Transition** transition_matrix, FirstFollowSet data, HashNode* hashTable[]) {
    LexicalOutput lexical_output = lexical_analyser(input_file, transition_matrix);

    if(strcmp(lexical_output.token, "PROGRAM")){
        printf("Error: Expected 'PROGRAM' but found '%s'\n", lexical_output.token);
        //inserir funcao de modo panico
    }else{
        lexical_output = lexical_analyser(input_file, transition_matrix);
        if(strcmp(lexical_output.class, "identificador")){
            printf("Error: Expected identifier but found '%s'\n", lexical_output.token);
            //inserir funcao de modo panico
        }else{
            lexical_output = lexical_analyser(input_file, transition_matrix);
            if(!strcmp(lexical_output.token, ";")){
                printf("Programa identificado! \n");
                variavel(input_file, transition_matrix, data, hashTable);
            }else{
                printf("Error: Expected ';' but found '%s'\n", lexical_output.token);
                //inserir funcao de modo panico
            }
        }
    }
    
}

void mais_var(FILE *input_file, Transition** transition_matrix, FirstFollowSet data, HashNode* hashTable[]) {
    LexicalOutput lexical_output = get_token(input_file, transition_matrix);

    if(strcmp(lexical_output.token, ",")){
        printf("Error: Expected ',' but found '%s'\n", lexical_output.token);
        //inserir funcao de modo panico
    }else{
        lexical_output = lexical_analyser(input_file, transition_matrix);
        if(strcmp(lexical_output.class, "identificador")){
            printf("Error: Expected identifier but found '%s'\n", lexical_output.token);
            //inserir funcao de modo panico
        }else{
            lexical_output = lexical_analyser(input_file, transition_matrix);

            if(!strcmp(lexical_output.token, ",")){
                return_token(input_file, lexical_output);
                printf("Chamando recursao! \n");
                mais_var(input_file, transition_matrix, data, hashTable);
            }

            if(is_in_follow_set(data, lexical_output.token)){
                return_token(input_file, lexical_output);
                return;
            }
        }
    }
}