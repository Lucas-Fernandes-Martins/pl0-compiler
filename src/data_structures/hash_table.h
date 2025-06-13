#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define TABLE_SIZE 101 // Chosed to be a prime number

// Struct to store multiple strings and the number of strings
typedef struct{
    char **items;
    int count;
} StringSet;

// Struct to store first and follow for each symbol of PL0 grammar
typedef struct{
    StringSet first;
    StringSet follow;
} SymbolInfo;

// Struct to define the nodes of hash structure
typedef struct HashEntry {
    char *symbol;
    SymbolInfo info;
    struct HashEntry *next;
} HashEntry;

// Struct to define the hash tabel itself
typedef struct{
    HashEntry **buckets;
} HashTable;

HashTable *hash_create();
int hash_insert(HashTable *hash_table, const char *symbol, SymbolInfo symbol_info);
SymbolInfo *hash_get(HashTable *hash_table, const char *symbol);
void hash_destroy(HashTable *hash_table);

#endif


