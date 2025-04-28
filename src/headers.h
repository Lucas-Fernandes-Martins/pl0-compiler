#include <stdio.h>

#define NUM_RESERVED_WORDS 11
#define MAX_RESERVED_WORD_LENGHT 10
#define ASCII_EXTENDED_SIZE 256

// Mealy's machine state
typedef struct {
        int number;
        int is_final;
} State;

// Mealy's machine transition;
typedef struct {
        char char_consumed;
        char* output;
        State next_state;
} Transition;

typedef struct {
	char token[128];
	char class[128];
	int end;
} LexicalOutput;

Transition** csv_parser(char* file_name);
