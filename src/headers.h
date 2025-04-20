#include<stdio.h>

#define N_RESERVED_WORDS 11
#define N_CONSUMABLE_CHARS 1
#define N_OPERATORS 7
#define NBR_STATES 10
#define NBR_CHARS 256

typedef struct {
        int number;
        char char_consumed;
} State;

//Mealy's machine state transition output;
typedef struct {
        int next_state;
        char* output;
} Output;

int hash_function(int current_state, char symbol);

Output** csv_parser(char* file_name);

int check_op(char c);

int is_letter(char c, Output* transition_line); 
