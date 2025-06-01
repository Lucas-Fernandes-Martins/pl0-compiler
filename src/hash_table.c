#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"

unsigned int hash(const char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % HASH_SIZE;
}

void hash_insert(HashNode* hashTable[], const char *key, FirstFollowSet set) {
    unsigned int idx = hash(key);
    HashNode *node = (HashNode*)malloc(sizeof(HashNode));
    strcpy(node->key, key);
    node->set = set;
    node->next = hashTable[idx];
    hashTable[idx] = node;
}

FirstFollowSet* hash_get(HashNode* hashTable[], const char *key) {
    unsigned int idx = hash(key);
    HashNode *node = hashTable[idx];
    while (node) {
        if (strcmp(node->key, key) == 0)
            return &node->set;
        node = node->next;
    }
    return NULL;
}

FirstFollowSet createSet(const char *first[], int firstCount, const char *follow[], int followCount) {
    FirstFollowSet set;
    set.firstCount = firstCount;
    set.followCount = followCount;
    for (int i = 0; i < firstCount; i++)
        strcpy(set.first[i], first[i]);
    for (int i = 0; i < followCount; i++)
        strcpy(set.follow[i], follow[i]);
    return set;
}

int insert_first_follow(HashNode* hashTable[]) {
    // FIRST and FOLLOW sets for all non-terminals

    hash_insert(hashTable, "<programa>", createSet(
        (const char*[]){"CONST", "VAR", "PROCEDURE", "ident", "CALL", "BEGIN", "IF", "WHILE", "λ"}, 9,
        (const char*[]){ "." }, 1
    ));

    hash_insert(hashTable, "<bloco>", createSet(
        (const char*[]){"CONST", "VAR", "PROCEDURE", "ident", "CALL", "BEGIN", "IF", "WHILE", "λ"}, 9,
        (const char*[]){ ".", ";", "END", "THEN", "DO" }, 5
    ));

    hash_insert(hashTable, "<declaracao>", createSet(
        (const char*[]){"CONST", "VAR", "PROCEDURE", "λ"}, 4,
        (const char*[]){"ident", "CALL", "BEGIN", "IF", "WHILE", "λ"}, 6
    ));

    hash_insert(hashTable, "<constante>", createSet(
        (const char*[]){"CONST", "λ"}, 2,
        (const char*[]){"VAR", "PROCEDURE", "ident", "CALL", "BEGIN", "IF", "WHILE", "λ"}, 7
    ));

    hash_insert(hashTable, "<mais_const>", createSet(
        (const char*[]){",", "λ"}, 2,
        (const char*[]){";"}, 1
    ));

    hash_insert(hashTable, "<variavel>", createSet(
        (const char*[]){"VAR", "λ"}, 2,
        (const char*[]){"PROCEDURE", "ident", "CALL", "BEGIN", "IF", "WHILE", "λ"}, 6
    ));

    hash_insert(hashTable, "<mais_var>", createSet(
        (const char*[]){",", "λ"}, 2,
        (const char*[]){";"}, 1
    ));

    hash_insert(hashTable, "<procedimento>", createSet(
        (const char*[]){"PROCEDURE", "λ"}, 2,
        (const char*[]){"ident", "CALL", "BEGIN", "IF", "WHILE", "λ"}, 6
    ));

    hash_insert(hashTable, "<comando>", createSet(
        (const char*[]){"ident", "CALL", "BEGIN", "IF", "WHILE", "λ"}, 6,
        (const char*[]){";", "END", "THEN", "DO"}, 4
    ));

    hash_insert(hashTable, "<mais_cmd>", createSet(
        (const char*[]){";", "λ"}, 2,
        (const char*[]){"END"}, 1
    ));

    hash_insert(hashTable, "<expressao>", createSet(
        (const char*[]){"+", "-", "ident", "numero", "("}, 5,
        (const char*[]){"THEN", "DO", "=", "<>", "<", "<=", ">", ">=", ")", ";", "END"}, 11
    ));

    hash_insert(hashTable, "<operador_unario>", createSet(
        (const char*[]){"+", "-", "λ"}, 3,
        (const char*[]){"ident", "numero", "("}, 3
    ));

    hash_insert(hashTable, "<termo>", createSet(
        (const char*[]){"ident", "numero", "("}, 3,
        (const char*[]){"+", "-", "THEN", "DO", "=", "<>", "<", "<=", ">", ">=", ")", ";", "END"}, 13
    ));

    hash_insert(hashTable, "<mais_termos>", createSet(
        (const char*[]){"+", "-", "λ"}, 3,
        (const char*[]){ "THEN", "DO", "=", "<>", "<", "<=", ">", ">=", ")", ";", "END"}, 11
    ));

    hash_insert(hashTable, "<fator>", createSet(
        (const char*[]){"ident", "numero", "("}, 3,
        (const char*[]){ "*", "/", "+", "-", "THEN", "DO", "=", "<>", "<", "<=", ">", ">=", ")", ";", "END"}, 15
    ));

    hash_insert(hashTable, "<mais_fatores>", createSet(
        (const char*[]){ "*", "/", "λ"}, 3,
        (const char*[]){ "+", "-", "THEN", "DO", "=", "<>", "<", "<=", ">", ">=", ")", ";", "END"}, 13
    ));

    hash_insert(hashTable, "<condicao>", createSet(
        (const char*[]){"ODD", "ident", "numero", "("}, 4,
        (const char*[]){ "THEN", "DO"}, 2
    ));

    hash_insert(hashTable, "<relacional>", createSet(
        (const char*[]){"=", "<>", "<", "<=", ">", ">="}, 6,
        (const char*[]){ "ident", "numero", "(" }, 3
    ));

    const char* lookup = "<expressao>";
    FirstFollowSet *retrieved = hash_get(hashTable, lookup);
    if (retrieved) {
        printf("FIRST of %s: ", lookup);
        for (int i = 0; i < retrieved->firstCount; i++)
            printf("%s ", retrieved->first[i]);
        printf("\nFOLLOW of %s: ", lookup);
        for (int i = 0; i < retrieved->followCount; i++)
            printf("%s ", retrieved->follow[i]);
        printf("\n");
    } else {
        printf("%s not found.\n", lookup);
    }

    return 0;
}

// void main() {
//     // Initialize the hash table
//     memset(hashTable, 0, sizeof(hashTable));

//     // Insert FIRST and FOLLOW sets into the hash table
//     insert_first_follow();

//     // Example usage of hash_get
//     const char *key = "<programa>";
//     FirstFollowSet *set = hash_get(key);
//     if (set) {
//         printf("Retrieved FIRST and FOLLOW sets for %s:\n", key);
//         printf("FIRST: ");
//         for (int i = 0; i < set->firstCount; i++)
//             printf("%s ", set->first[i]);
//         printf("\nFOLLOW: ");
//         for (int i = 0; i < set->followCount; i++)
//             printf("%s ", set->follow[i]);
//         printf("\n");
//     } else {
//         printf("No entry found for %s.\n", key);
//     }
// }