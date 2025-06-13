#ifndef GRAMMAR_SETS_H
#define GRAMMAR_SETS_H

#include "../data_structures/hash_table.h"

#define NUM_RESERVED_WORDS 11
#define MAX_RESERVED_WORD_LENGHT 10

// Sets the list of reserved words
static const char reserved_words[NUM_RESERVED_WORDS][MAX_RESERVED_WORD_LENGHT] = {
    "CONST", "VAR", "PROCEDURE", "BEGIN", "END", "CALL", "WHILE", "DO", "ODD", "IF", "THEN"
};

int check_if_reserved_word(char *word);

StringSet add_string_set_item(StringSet string_set, char **new_items, int num_items);
SymbolInfo create_symbol_info(const char **first, int first_count, const char **follow, int follow_count);

int is_in_string_set(StringSet *string_set, const char *symbol);
int insert_first_follow(HashTable *hash_table);

#endif