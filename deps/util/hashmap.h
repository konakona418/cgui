//
// Created on: 2024/11/7, 09:57.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_HASHMAP_H
#define CGUI_HASHMAP_H

#include <stdlib.h>
#include <string.h>
#include "common.h"

#define DEFAULT_BUCKET_COUNT 16

/* Prototypes of hash node and hash table. */
typedef struct HashNode HashNode;
typedef struct HashTable HashTable;

/* Hash node and hash table. */
typedef struct HashNode {
    char* key;
    void* value;
    struct HashNode* next;
} HashNode;

typedef struct HashTable {
    HashNode** buckets;
    size_t bucket_count;
    size_t size;

    unsigned long (* hash_function)(const char* key, size_t bucket_count);

    int (* insert)(HashTable* table, const char* key, void* value);

    void* (* find)(HashTable* table, const char* key);

    void* (* find_if)(HashTable* table, void* target, IterPredicateResult (* predicate)(const char* key, void* value, void* target));

    int (* contains)(HashTable* table, const char* key);

    void* (* remove)(HashTable* table, const char* key);

    void (* iter)(HashTable* table, void (* callback)(const char* key, void* value));

    void (* iter_values)(HashTable* table, void (* callback)(void* value));

    void (* iter_keys)(HashTable* table, void (* callback)(const char* key));

    CGUI_Result (* iter_result)(HashTable* table, CGUI_Result (* callback)(const char* key, void* value));

    CGUI_Result (* iter_value_result)(HashTable* table, CGUI_Result (* callback)(void* value));

    CGUI_Result (* iter_key_result)(HashTable* table, CGUI_Result (* callback)(const char* key));
} HashTable;

/* Implementation of djb2 hash function. */
unsigned long hash_function(const char* key, size_t bucket_count);

/* Create a hash table with given bucket count. */
HashTable* create_hash_table(size_t bucket_count);

/* Insert a key-value pair into the hash table. */
int hash_table_insert(HashTable* table, const char* key, void* value);

/* Find the value associated with the given key. */
void* hash_table_find(HashTable* table, const char* key);

/* Find the value associated with the given key if the predicate is true. */
void* hash_table_find_if(HashTable* table, void* target, bool (* predicate)(const char* key, void* value, void* target));

/* Check whether the hash table contains the given key. */
int hash_table_contains(HashTable* table, const char* key);

/* Remove the key-value pair associated with the given key. */
void* hash_table_remove(HashTable* table, const char* key);

/* Destroy the hash table.
 * Note: this function does not free the memory of the values. */
void destroy_hash_table(HashTable* table);

/* Iterate over all key-value pairs in the hash table. */
void iter(HashTable* table, void (* callback)(const char* key, void* value));

/* Iterate over all values in the hash table. */
void iter_values(HashTable* table, void (* callback)(void* value));

/* Iterate over all keys in the hash table. */
void iter_keys(HashTable* table, void (* callback)(const char* key));

/* Iterate over all key-value pairs in the hash table and return the result.
 * If the callback function raises an Err, the iteration will stop and the Err will be returned.
 * Otherwise, the function will return an Ok(NULL). */
CGUI_Result iter_result(HashTable* table, CGUI_Result (* callback)(const char* key, void* value));

/* Iterate over all values in the hash table and return the result.
 * If the callback function raises an Err, the iteration will stop and the Err will be returned.
 * Otherwise, the function will return an Ok(NULL). */
CGUI_Result iter_value_result(HashTable* table, CGUI_Result (* callback)(void* value));

/* Iterate over all keys in the hash table and return the result.
 * If the callback function raises an Err, the iteration will stop and the Err will be returned.
 * Otherwise, the function will return an Ok(NULL). */
CGUI_Result iter_key_result(HashTable* table, CGUI_Result (* callback)(const char* key));

#endif //CGUI_HASHMAP_H
