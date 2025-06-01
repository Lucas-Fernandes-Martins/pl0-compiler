#include <stdio.h>
#include <stdbool.h> // Added for bool type

#define NUM_RESERVED_WORDS 11
#define MAX_RESERVED_WORD_LENGHT 10
#define ASCII_EXTENDED_SIZE 256
#define MAX_TOKEN_LENGHT 256
#define MAX_SET_SIZE 20

#define MAX_SYMBOLS 20
#define MAX_NAME_LEN 50
#define HASH_SIZE 100

// Mealy's machine state
typedef struct {
        int number;
        int is_final;
} State;

// Mealy's machine transition;
typedef struct {
        int num_outputs;
        char* output;
        int lookahead;
        State next_state;
} Transition;

typedef struct {
	char token[MAX_TOKEN_LENGHT];
	char class[MAX_TOKEN_LENGHT];
	int end;
} LexicalOutput;

typedef struct {
    char first[MAX_SET_SIZE][20];
    int firstCount;

    char follow[MAX_SET_SIZE][20];
    int followCount;
} Data;

// Defines for First/Follow tables
// TODO: Adjust these values based on the actual number of non-terminals and terminals in the PL/0 grammar
#define MAX_NON_TERMINALS 50
#define MAX_TERMINALS 50

// Structure for O(1) access to First and Follow sets
// Assumes non-terminals and terminals are mapped to integer IDs (0 to MAX_NON_TERMINALS-1 or MAX_TERMINALS-1)
typedef struct {
    // first_set[non_terminal_id][terminal_id] is true if terminal_id is in FIRST(non_terminal_id)
    bool first_set[MAX_NON_TERMINALS][MAX_TERMINALS];

    // follow_set[non_terminal_id][terminal_id] is true if terminal_id is in FOLLOW(non_terminal_id)
    bool follow_set[MAX_NON_TERMINALS][MAX_TERMINALS];
} ParsingTables;

Transition** csv_parser(char* input_file_name);

//Hash table 

typedef struct {
    char first[MAX_SYMBOLS][MAX_NAME_LEN];
    int firstCount;
    char follow[MAX_SYMBOLS][MAX_NAME_LEN];
    int followCount;
} FirstFollowSet;

typedef struct HashNode {
    char key[MAX_NAME_LEN];
    FirstFollowSet set;
    struct HashNode* next;
} HashNode;

int insert_first_follow(HashNode* hashTable[]);

FirstFollowSet* hash_get(HashNode* hashTable[], const char *key);

// Lexical Analysis
LexicalOutput lexical_analyser(FILE *input_file, Transition** transition_matrix);
void return_token(FILE *input_file, LexicalOutput returned_token); // New function declaration
void free_memory(Transition** transition_matrix);

// Syntatic Analysis
void variavel(FILE *input_file, Transition** transition_matrix, FirstFollowSet data);

void mais_var(FILE *input_file, Transition** transition_matrix, FirstFollowSet data);
