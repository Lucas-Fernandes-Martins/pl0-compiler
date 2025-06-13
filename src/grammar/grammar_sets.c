#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammar_sets.h"

/**
 * @brief Checks if the input string is a reserved word
 * @param word input string
 * @return 1 if reserved word, 0 otherwise
 */
int check_if_reserved_word(char *word){
	for(int i = 0; i < NUM_RESERVED_WORDS; i++)
		if(!strcmp(word, reserved_words[i])) return 1;
	
	return 0;
}

StringSet add_string_set_item(StringSet string_set, char **new_items, int num_items){
    // Check if the item is already in the set
    for(int j = 0; j < num_items; j++){
        for(int i = 0; i < string_set.count; i++){
            if(strcmp(string_set.items[i], new_items[j]) == 0)
                break; // Item already exists in the set 
        }

        // Adds item
        string_set.items[string_set.count++] = strdup(new_items[j]);
    }

    return string_set;
}

/**
 * @brief Checks if a given symbol (string) is in the StringSet.
 * @param string_set The StringSet structure containing the string set.
 * @param symbol The symbol to check for.
 * @return 1 if the symbol is in the StringSet, 0 otherwise.
 */
int is_in_string_set(StringSet *string_set, const char *symbol){
    for(int i = 0; i < string_set->count; i++){
        if(strcmp(string_set->items[i], symbol) == 0)
            return 1; // Symbol found in set
    }

    return 0; // Symbol not found in set
}


SymbolInfo create_symbol_info(const char **first, int first_count, const char **follow, int follow_count){
    SymbolInfo symbol_info;

    // Allocate memory for vector of firsts and followers
    symbol_info.first.items = malloc(first_count * sizeof(char *));
    symbol_info.follow.items = malloc(follow_count * sizeof(char *));

    // Copy each first
    for(int i = 0; i < first_count; i++)
        symbol_info.first.items[i] = strdup(first[i]);

    // Copy each follower
    for(int i = 0; i < follow_count; i++)
        symbol_info.follow.items[i] = strdup(follow[i]);

    // Copy number of firsts and followers
    symbol_info.first.count = first_count;
    symbol_info.follow.count = follow_count;

    return symbol_info;
}


int insert_first_follow(HashTable *hash_table){
    // Insert firsts and followers in hash table

    hash_insert(hash_table, "<programa>", 
        create_symbol_info((const char*[]){"CONST", "VAR", "PROCEDURE", "identificador", "CALL", "BEGIN", "IF", "WHILE"}, 8, 
        NULL, 0)
    );

    hash_insert(hash_table, "<bloco>", 
        create_symbol_info((const char*[]){"CONST", "VAR", "PROCEDURE", "identificador", "CALL", "BEGIN", "IF", "WHILE"}, 8, 
        (const char*[]){ "simb_ponto", "simb_ponto_virgula", "END", "THEN", "DO" }, 5)
    );

    hash_insert(hash_table, "<declaracao>", 
        create_symbol_info((const char*[]){"CONST", "VAR", "PROCEDURE"}, 3, 
        (const char*[]){"identificador", "CALL", "BEGIN", "IF", "WHILE"}, 5)
    );

    hash_insert(hash_table, "<constante>", 
        create_symbol_info((const char*[]){"CONST"}, 1, 
        (const char*[]){"VAR", "PROCEDURE", "identificador", "CALL", "BEGIN", "IF", "WHILE", "simb_ponto", "simb_ponto_virgula"}, 6)
    );

    hash_insert(hash_table, "<mais_const>", 
        create_symbol_info((const char*[]){"simb_virgula"}, 1, 
        (const char*[]){"simb_ponto_virgula"}, 1)
    );

    hash_insert(hash_table, "<variavel>", 
        create_symbol_info((const char*[]){"VAR"}, 1, 
        (const char*[]){"PROCEDURE", "identificador", "CALL", "BEGIN", "IF", "WHILE"}, 5)
    );

    hash_insert(hash_table, "<mais_var>", 
        create_symbol_info((const char*[]){"simb_virgula"}, 1, 
        (const char*[]){"simb_ponto_virgula"}, 1)
    );

    hash_insert(hash_table, "<procedimento>", 
        create_symbol_info((const char*[]){"PROCEDURE"}, 1, 
        (const char*[]){"identificador", "CALL", "BEGIN", "IF", "WHILE"}, 5)
    );

    hash_insert(hash_table, "<comando>", 
        create_symbol_info((const char*[]){"identificador", "CALL", "BEGIN", "IF", "WHILE"}, 5, 
        (const char*[]){"simb_ponto_virgula", "END", "THEN", "DO"}, 4)
    );

    hash_insert(hash_table, "<mais_cmd>", 
        create_symbol_info((const char*[]){"simb_ponto_virgula"}, 1, 
        (const char*[]){"END"}, 1)
    );

    hash_insert(hash_table, "<expressao>", 
        create_symbol_info((const char*[]){"simb_mais", "simb_menos", "identificador", "numero", "simb_parent_esq"}, 5, 
        (const char*[]){"THEN", "DO", "simb_igual", "simb_diferente", "simb_menor_que", "simb_menor_igual", "simb_maior_que", "simb_maior_igual", "simb_parent_dir", "simb_ponto_virgula", "END"}, 11)
    );

    hash_insert(hash_table, "<operador_unario>", 
        create_symbol_info((const char*[]){"simb_mais", "simb_menos"}, 2, 
        (const char*[]){"identificador", "numero", "simb_parent_esq"}, 3)
    );

    hash_insert(hash_table, "<termo>", 
        create_symbol_info((const char*[]){"identificador", "numero", "simb_parent_esq"}, 3, 
        (const char*[]){"simb_mais", "simb_menos", "THEN", "DO", "simb_igual", "simb_diferente", "simb_menor_que", "simb_menor_igual", "simb_maior_que", "simb_maior_igual", "simb_parent_dir", "simb_ponto_virgula", "END"}, 13)
    );

    hash_insert(hash_table, "<mais_termos>", 
        create_symbol_info((const char*[]){"simb_mais", "simb_menos"}, 2, 
        (const char*[]){ "THEN", "DO", "simb_igual", "simb_diferente", "simb_menor_que", "simb_menor_igual", "simb_maior_que", "simb_maior_igual", "simb_parent_dir", "simb_ponto_virgula", "END"}, 11)
    );

    hash_insert(hash_table, "<fator>", 
        create_symbol_info((const char*[]){"identificador", "numero", "simb_parent_esq"}, 3, 
        (const char*[]){ "simb_mult", "simb_div", "simb_mais", "simb_menos", "THEN", "DO", "simb_igual", "simb_diferente", "simb_menor_que", "simb_menor_igual", "simb_maior_que", "simb_maior_igual", "simb_parent_dir", "simb_ponto_virgula", "END"}, 15)
    );

    hash_insert(hash_table, "<mais_fatores>", 
        create_symbol_info((const char*[]){ "simb_mult", "simb_div"}, 2, 
        (const char*[]){ "simb_mais", "simb_menos", "THEN", "DO", "simb_igual", "simb_diferente", "simb_menor_que", "simb_menor_igual", "simb_maior_que", "simb_maior_igual", "simb_parent_dir", "simb_ponto_virgula", "END"}, 13)
    );

    hash_insert(hash_table, "<condicao>", 
        create_symbol_info((const char*[]){"ODD", "identificador", "numero", "simb_parent_esq", "simb_mais", "simb_menos"}, 4, 
        (const char*[]){ "THEN", "DO"}, 2)
    );

    hash_insert(hash_table, "<relacional>", 
        create_symbol_info((const char*[]){"simb_igual", "simb_diferente", "simb_menor_que", "simb_menor_igual", "simb_maior_que", "simb_maior_igual"}, 6, 
        (const char*[]){ "identificador", "numero", "simb_parent_esq" }, 3)
    );

    return 0;
}