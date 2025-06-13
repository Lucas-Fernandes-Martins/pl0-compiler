#include <stdio.h>
#include <stdlib.h>

#include "dfa.h"
Transition **transition_matrix = NULL;

// Simple function to create a new state
State create_state(int number, int is_final, int is_error){
    State new_state;

    // Copy information to new state
    new_state.number = number;
    new_state.is_final = is_final;
    new_state.is_error = is_error;

    return new_state;
}

Transition **build_lexer_dfa(){
    Transition **dfa = create_transition_matrix(NUM_STATES);

    // From state q0
    // Add transition for EOF
    add_transition(dfa, 0, EOF_COLUMN, "EOF", 0, create_state(41, 1, 0));

    // Add transitions for whitespace caracters 
    add_transition(dfa, 0, ' ', "whitespace", 0, create_state(40, 1, 0));
    add_transition(dfa, 0, '\n', "whitespace", 0, create_state(40, 1, 0));
    add_transition(dfa, 0, '\t', "whitespace", 0, create_state(40, 1, 0));

    // Add transitions for digit
    add_digit_transitions(dfa, 0, 0, create_state(1, 0, 0));

    // Add transtitions for letter
    add_alpha_transitions(dfa, 0, 0, create_state(3, 0, 0));

    // Add transitions for relational operators
    add_transition(dfa, 0, '=', "=", 0, create_state(5, 0, 0));
    add_transition(dfa, 0, '<', "<", 0, create_state(6, 0, 0));
    add_transition(dfa, 0, '>', ">", 0, create_state(10, 0, 0));

    // Add transitions for arithmetic operators
    add_transition(dfa, 0, '/', "/", 0, create_state(16, 0, 0));
    add_transition(dfa, 0, '*', "*", 0, create_state(17, 0, 0));
    add_transition(dfa, 0, '-', "-", 0, create_state(18, 0, 0));
    add_transition(dfa, 0, '+', "+", 0, create_state(19, 0, 0));
    
    // Add transitions for ponctuation signs
    add_transition(dfa, 0, ':', ":", 0, create_state(13, 0, 0));
    add_transition(dfa, 0, '.', ".", 0, create_state(20, 0, 0));
    add_transition(dfa, 0, ';', ";", 0, create_state(21, 0, 0));
    add_transition(dfa, 0, ',', ",", 0, create_state(22, 0, 0));

    // Add transitions for comments
    add_transition(dfa, 0, '{', "{", 0, create_state(23, 0, 0));
    add_transition(dfa, 0, '}', "}", 0, create_state(25, 0, 0));

    // Add transitions for parentheses
    add_transition(dfa, 0, '(', "(", 0, create_state(42, 0, 0));
    add_transition(dfa, 0, ')', ")", 0, create_state(43, 0, 0));

    // From state q1
    add_digit_transitions(dfa, 1, 0, create_state(1, 0, 0));
    add_non_digit_transitions(dfa, 1, "numero", 1, create_state(2, 1, 0));

    // From state q3
    add_alpha_transitions(dfa, 3, 0, create_state(3, 0, 0));
    add_digit_transitions(dfa, 3, 0, create_state(3, 0, 0));
    add_non_alphanum_transitions(dfa, 3, "identificador", 1, create_state(4, 1, 0));

    // From state q5
    add_all_character_transitions(dfa, 5, "simb_igual", 1, create_state(29, 1, 0));

    // From state q6
    add_transition(dfa, 6, '=', "=", 0, create_state(7, 0, 0));
    add_transition(dfa, 6, '>', ">", 0, create_state(9, 0, 0));
    add_all_except_transitions(dfa, 6, ">=", "simb_menor_que", 1, create_state(8, 1, 0));

    // From state q7
    add_all_character_transitions(dfa, 7, "simb_menor_igual", 1, create_state(31, 1, 0));

    // From state q9
    add_all_character_transitions(dfa, 9, "simb_diferente", 1, create_state(32, 1, 0));
    
    // From state q10
    add_transition(dfa, 10, '=', "=", 0, create_state(12, 0, 0));
    add_all_except_transitions(dfa, 10, "=", "simb_maior_que", 1, create_state(11, 1, 0));

    // From state q12
    add_all_character_transitions(dfa, 12, "simb_maior_igual", 1, create_state(39, 1, 0));

    // From state q13
    add_transition(dfa, 13, '=', "=", 0, create_state(15, 0, 0));
    add_all_except_transitions(dfa, 13, "=", INCOMPLETE_SYMBOL_ERROR, 1, create_state(14, 1, 1));

    // From state q15
    add_all_character_transitions(dfa, 15, "simb_atribuicao", 1, create_state(30, 1, 0));

    // From state q16
    add_all_character_transitions(dfa, 16, "simb_div", 1, create_state(38, 1, 0));

    // From state q17
    add_all_character_transitions(dfa, 17, "simb_mult", 1, create_state(36, 1, 0));

    // From state q18
    add_all_character_transitions(dfa, 18, "simb_menos", 1, create_state(28, 1, 0));

    // From state q19
    add_all_character_transitions(dfa, 19, "simb_mais", 1, create_state(34, 1, 0));

    // From state q20
    add_all_character_transitions(dfa, 20, "simb_ponto", 1, create_state(35, 1, 0));

    // From state q21
    add_all_character_transitions(dfa, 21, "simb_ponto_virgula", 1, create_state(33, 1, 0));

    // From state q22
    add_all_character_transitions(dfa, 22, "simb_virgula", 1, create_state(37, 1, 0));

    // From state q23
    add_transition(dfa, 23, '}', "comment", 0, create_state(24, 1, 0));
    add_transition(dfa, 23, '\n', UNFINISHED_COMMENT_ERROR, 1, create_state(27, 1, 1));
    add_all_except_transitions(dfa, 23, "\n}", "", 0, create_state(23, 0, 0));

    // From state q25
    add_all_character_transitions(dfa, 25, INCOMPLETE_COMMENT_ERROR, 1, create_state(27, 1, 1));

    // From state q42
    add_all_character_transitions(dfa, 42, "simb_parent_esq", 1, create_state(44, 1, 0));

    // From state q43
    add_all_character_transitions(dfa, 43, "simb_parent_dir", 1, create_state(45, 1, 0));

    // Last state implemented is 45

    return dfa;
}