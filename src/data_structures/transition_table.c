#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "transition_table.h"

Transition **create_transition_matrix(int number_states){
    // Create and allocate memory for the transition matrix
	Transition** transition_matrix = malloc(number_states * sizeof(Transition *));

    // For each state in the automata, allocates space and populates with standard transitions
	for(int i = 0; i < number_states; i++) {		
		transition_matrix[i] = calloc(ASCII_EXTENDED_SIZE, sizeof(Transition)); // Extra space for EOF column

		// Populate positions with a standard invalid transition
		for(int j = 0; j < ASCII_EXTENDED_SIZE; j++){
            transition_matrix[i][j].output = strdup(INVALID_CHARACTER_ERROR);
            transition_matrix[i][j].lookahead = 0;

            transition_matrix[i][j].next_state.number = 0;
            transition_matrix[i][j].next_state.is_final = 1;
            transition_matrix[i][j].next_state.is_error = 1;
		}
	}

    return transition_matrix;
}

void add_transition(Transition **matrix, int from_state, int input_char, const char *output, int lookahead, State next_state){
    if (from_state < 0 || input_char >= ASCII_EXTENDED_SIZE) return;

    Transition new_transition;

    // Copy information to a new transition
    new_transition.output = strdup(output);
    new_transition.lookahead = lookahead;
    new_transition.next_state = next_state;

    // If there's already one string in transition, free it
    if(matrix[from_state][input_char].output)
        free(matrix[from_state][input_char].output);

    matrix[from_state][input_char] = new_transition;
}


void add_range_transition_echo(Transition **matrix, int from_state, unsigned char start_char, unsigned char end_char,
                          int lookahead, State next_state){
    // For each value, insert the corresponding transition                        
    for(int c = start_char; c <= end_char; c++){
        char output[2] ={(char) c, '\0'};

        add_transition(matrix, from_state, c, output, lookahead, next_state);
    }
}

void add_digit_transitions(Transition **matrix, int from_state, int lookahead, State next_state){
    add_range_transition_echo(matrix, from_state, '0', '9', lookahead, next_state);
}

void add_alpha_transitions(Transition **matrix, int from_state, int lookahead, State next_state){
    add_range_transition_echo(matrix, from_state, 'a', 'z', lookahead, next_state);
    add_range_transition_echo(matrix, from_state, 'A', 'Z', lookahead, next_state);
}

void add_non_digit_transitions(Transition **matrix, int from_state, const char *output, int lookahead, State next_state){
    for(int c = 0; c < ASCII_EXTENDED_SIZE; c++){
        if(!isdigit((unsigned char) c))
            add_transition(matrix, from_state, c, output, lookahead, next_state);
    }
}

void add_non_alphanum_transitions(Transition **matrix, int from_state, const char *output, int lookahead, State next_state){
    for(int c = 0; c < ASCII_EXTENDED_SIZE; c++)
        if(!isalnum((unsigned char) c))
            add_transition(matrix, from_state, c, output, lookahead, next_state);
}

void add_all_character_transitions(Transition **matrix, int from_state, const char *output, int lookahead, State next_state){
    for(int c = 0; c < ASCII_EXTENDED_SIZE; c++)
        add_transition(matrix, from_state, c, output, lookahead, next_state);
}

void add_all_except_transitions(Transition **matrix, int from_state, const char *except_chars, const char *output, int lookahead, State next_state){
    int num_char = strlen(except_chars);

    for(int c = 0; c < ASCII_EXTENDED_SIZE; c++){
        int should_add = 1;
        for(int i = 0; i < num_char; i++)
            if(c == except_chars[i]){
                should_add = 0;
                break;
            }
        
        if(should_add) 
            add_transition(matrix, from_state, c, output, lookahead, next_state);
    }
}   

void destroy_transition_matrix(Transition **transition_matrix, int number_states){
    // For each state
    for(int i = 0; i < number_states; i++){
        // Fore each ASCII value
        for(int j = 0; j < ASCII_EXTENDED_SIZE; j++) 
            free(transition_matrix[i][j].output); // Free the output of the transition

        free(transition_matrix[i]); // Free the vector of transitions
    }

    free(transition_matrix); // Free the matrix itself
}