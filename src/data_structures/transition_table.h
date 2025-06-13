#ifndef TRANSITION_TABLE_H
#define TRANSITION_TABLE_H

#define ASCII_EXTENDED_SIZE 257 // Include EOF as a character
#define EOF_COLUMN 256

#define INVALID_CHARACTER_ERROR "<ERRO_CARACTERE_INVALIDO>"

// Mealy's machine state
typedef struct {
        int number;
        int is_final;
        int is_error;
} State;

// Mealy's machine transition
typedef struct {
        char *output;
        int lookahead;
        State next_state;
} Transition;

Transition **create_transition_matrix(int number_states);
void destroy_transition_matrix(Transition **transition_matrix, int number_states);

void add_transition(Transition **matrix, int from_state, int input_char, const char *output, int lookahead, State next_state);

void add_range_transition_echo(Transition **matrix, int from_state, unsigned char start_char, unsigned char end_char, int lookahead, State next_state);

void add_digit_transitions(Transition **matrix, int from_state, int lookahead, State next_state);
void add_alpha_transitions(Transition **matrix, int from_state, int lookahead, State next_state);

void add_non_digit_transitions(Transition **matrix, int from_state, const char *output, int lookahead, State next_state);
void add_non_alphanum_transitions(Transition **matrix, int from_state, const char *output, int lookahead, State next_state);

void add_all_character_transitions(Transition **matrix, int from_state, const char *output, int lookahead, State next_state);
void add_all_except_transitions(Transition **matrix, int from_state, const char *except_chars, const char *output, int lookahead, State next_state);

#endif