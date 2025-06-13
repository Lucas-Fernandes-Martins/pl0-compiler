#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

// Global variable to count the line of the token
int current_line = 1;

const char *type_to_lexeme(const char *type){
    for(int i = 0; i < (int) (sizeof(lexeme_type_pairs)/sizeof(lexeme_type_pairs[0])); ++i){
        if(strcmp(lexeme_type_pairs[i].type, type) == 0)
            return lexeme_type_pairs[i].lexeme;
    }

    return type; // For reserved word cases (e.g. lexeme VAR has type VAR) 
}

void print_lexical_error(Token current_token){
	if(strcmp(current_token.type, INVALID_CHARACTER_ERROR) == 0){
		printf("Lexical error on line %d: found invalid character {'%s'}\n", current_token.line, current_token.lexeme);
	}
	else if(strcmp(current_token.type, INCOMPLETE_SYMBOL_ERROR) == 0){
		printf("Lexical error on line %d: found incomplete symbol {'%s'}. Maybe meant ':='?\n", current_token.line, current_token.lexeme);
	}
	else if(strcmp(current_token.type, UNFINISHED_COMMENT_ERROR) == 0){
		printf("Lexical error on line %d: comment started without closing.\n", current_token.line);
	}
	else if(strcmp(current_token.type, INCOMPLETE_COMMENT_ERROR) == 0){
		printf("Lexical error on line %d: found incomplete symbol {'%s'}. Missing '{'.\n", current_token.line, current_token.lexeme);
	}
}

Token lexical_analyser(FILE *input_file){
    State current_state = create_state(0, 0, 0); // Initial state
    Transition current_transition;

    // Instantiates output variable
    Token new_token = {.lexeme[0] = '\0', .type[0] = '\0', .is_error = 0};

    // Loops while the char read is different from EOF
    int char_consumed;
    while(1){
        char_consumed = fgetc(input_file);
        int col = (char_consumed == EOF ? EOF_COLUMN : (unsigned char) char_consumed);

        current_transition = transition_matrix[current_state.number][col]; // Get the transition
        new_token.is_error = current_transition.next_state.is_error;

        if(char_consumed == '\n' && !current_transition.lookahead) current_line++; // If real line break

        // Check if next state is final (i.e., this is the last transition)
        if(current_transition.next_state.is_final){
            strcpy(new_token.type, current_transition.output);
            
            if(strcmp(new_token.type, "identificador") == 0){ // If type is identifier, check if lexeme is reserved word
                if(check_if_reserved_word(new_token.lexeme)) strcpy(new_token.type, new_token.lexeme);
            }

            if(strcmp(new_token.type, INVALID_CHARACTER_ERROR) == 0){ // If error of invalid character, save the character in token's lexeme
                new_token.lexeme[0] = (unsigned char) char_consumed;
                new_token.lexeme[1] = '\0';  
            }

            // Backtracks if the current transition demands
			if(current_transition.lookahead) fseek(input_file, -1, SEEK_CUR);
            
            // Save the line of the token
            new_token.line = current_line;

            return new_token;
        }

        // If the next state is not final, appends its output to the current output token lexeme
        else{
            strcat(new_token.lexeme, current_transition.output);
        }

        // Move to the next state
        current_state = current_transition.next_state;
    }
}

Token get_token(FILE *input_file){
    // Get token from lexical analyser
    Token token = lexical_analyser(input_file);

    while(1){
        // While token is whitespace or comment, keep getting other tokens
        if((strcmp(token.type, "whitespace") == 0) || (strcmp(token.type, "comment") == 0)) 
            token = lexical_analyser(input_file); 
        // While token is error, keep getting other tokens
        else if(token.is_error){
            print_lexical_error(token);
		    token = lexical_analyser(input_file);
        }
        // If token is not whitespace, comment or error, then token is valid and break
        else{
            break;
        }
    }

    if(strcmp(token.type, "EOF") == 0) // End of file
        exit(0);

    return token;
}
