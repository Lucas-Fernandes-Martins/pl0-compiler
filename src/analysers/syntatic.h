#ifndef SYNTATIC_H
#define SYNTATIC_H

#include "lexer.h"

void syntatic_analyser(FILE *input_file);

void programa(FILE *input_file);
void bloco(FILE *input_file, StringSet parent_follow_set);
void declaracao(FILE *input_file, StringSet parent_follow_set);
void constante(FILE *input_file, StringSet parent_follow_set);
void mais_const(FILE *input_file, StringSet parent_follow_set);
void variavel(FILE *input_file, StringSet parent_follow_set);
void mais_var(FILE *input_file, StringSet parent_follow_set);
void procedimento(FILE *input_file, StringSet parent_follow_set);
void comando(FILE *input_file, StringSet parent_follow_set);
void mais_cmd(FILE *input_file, StringSet parent_follow_set);
void expressao(FILE *input_file, StringSet parent_follow_set);
void operador_unario(FILE *input_file);
void termo(FILE *input_file, StringSet parent_follow_set);
void mais_termos(FILE *input_file, StringSet parent_follow_set);
void fator(FILE *input_file, StringSet parent_follow_set);
void mais_fatores(FILE *input_file, StringSet parent_follow_set);
void condicao(FILE *input_file, StringSet parent_follow_set);
void relacional(FILE *input_file, StringSet parent_follow_set);

#endif