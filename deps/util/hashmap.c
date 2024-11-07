//
// Created on: 2024/11/7, 09:57.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "hashmap.h"


unsigned long hash_function(const char* key, size_t bucket_count) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char) *key++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash % bucket_count;
}

HashTable* create_hash_table(size_t bucket_count) {
    HashTable* table = malloc(sizeof(HashTable));
    if (!table) return NULL;

    table->bucket_count = bucket_count;
    table->size = 0;
    table->buckets = calloc(bucket_count, sizeof(HashNode*));
    if (!table->buckets) {
        free(table);
        return NULL;
    }
    table->hash_function = hash_function;
    table->insert = hash_table_insert;
    table->find = hash_table_find;
    table->contains = hash_table_contains;
    table->remove = hash_table_remove;

    table->iter_keys = iter_keys;
    table->iter_values = iter_values;
    table->iter = iter;

    return table;
}

int hash_table_insert(HashTable* table, const char* key, void* value) {
    unsigned long index = hash_function(key, table->bucket_count);
    HashNode* node = table->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) { // update existing value.
            node->value = value;
            return 1;
        }
        node = node->next;
    }

    // insert a node.
    node = malloc(sizeof(HashNode));
    if (!node) return 0;

    node->key = strdup(key);
    node->value = value;
    node->next = table->buckets[index];
    table->buckets[index] = node;
    table->size++;

    return 1;
}

void* hash_table_find(HashTable* table, const char* key) {
    unsigned long index = hash_function(key, table->bucket_count);
    HashNode* node = table->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

int hash_table_contains(HashTable* table, const char* key) {
    return hash_table_find(table, key) != NULL;
}

void* hash_table_remove(HashTable* table, const char* key) {
    unsigned long index = hash_function(key, table->bucket_count);
    HashNode* node = table->buckets[index];
    HashNode* prev = NULL;

    void* value;

    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                table->buckets[index] = node->next;
            }
            value = node->value;

            free(node->key);
            free(node);
            table->size--;
            return value;
        }
        prev = node;
        node = node->next;
    }
    return NULL;
}

void destroy_hash_table(HashTable* table) {
    for (size_t i = 0; i < table->bucket_count; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            HashNode* temp = node;
            node = node->next;
            free(temp->key);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

void iter_keys(HashTable* table, void (* callback)(const char*)) {
    for (size_t i = 0; i < table->bucket_count; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            callback(node->key);
            node = node->next;
        }
    }
}

void iter_values(HashTable* table, void (* callback)(void*)) {
    for (size_t i = 0; i < table->bucket_count; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            callback(node->value);
            node = node->next;
        }
    }
}

void iter(HashTable* table, void (* callback)(const char*, void*)) {
    for (size_t i = 0; i < table->bucket_count; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            callback(node->key, node->value);
            node = node->next;
        }
    }
}