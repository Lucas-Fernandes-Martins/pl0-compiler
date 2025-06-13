#ifndef DFA_H
#define DFA_H

#include "../data_structures/transition_table.h"

#define NUM_STATES 46

#define INCOMPLETE_SYMBOL_ERROR "<ERRO_SIMBOLO_INCOMPLETO>"
#define INCOMPLETE_COMMENT_ERROR "<ERRO_COMENTARIO_MAL_FORMADO>"
#define UNFINISHED_COMMENT_ERROR "<ERRO_COMENTARIO_NAO_TERMINADO>"

extern Transition **transition_matrix;

State create_state(int number, int is_final, int is_error);
Transition **build_lexer_dfa();

#endif