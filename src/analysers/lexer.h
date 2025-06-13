#ifndef LEXER_H
#define LEXER_H

#include "dfa.h"
#include "../grammar/grammar_sets.h"

#define MAX_TOKEN_LENGTH 256
#define MAX_TYPE_LENGTH 50

typedef struct{
    const char *type;
    const char *lexeme;
} LexemeType;

static const LexemeType lexeme_type_pairs[] = {
    {"numero", "numero"},
    {"identificador", "identificador"},
    {"simb_igual", "="},
    {"simb_menor_que", "<"},
    {"simb_maior_que", ">"},
    {"simb_diferente", "<>"},
    {"simb_maior_igual", ">="},
    {"simb_menor_igual", "<="},
    {"simb_atribuicao", ":="},
    {"simb_parent_esq", "("},
    {"simb_parent_dir", ")"},
    {"simb_div", "/"},
    {"simb_mult", "*"},
    {"simb_menos", "-"},
    {"simb_mais", "+"},
    {"simb_virgula", ","},
    {"simb_ponto_virgula", ";"},
    {"simb_ponto", "."},
};

typedef struct{
    int line;
    int is_error;
    char type[MAX_TYPE_LENGTH];
    char lexeme[MAX_TOKEN_LENGTH];
} Token;

const char *type_to_lexeme(const char *type);
Token get_token(FILE *input_file);

#endif