#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sodium.h>

// TODO

typedef struct MerkleNode {
    unsigned char hash[crypto_generichash_BYTES];
    struct MerkleNode* left;
    struct MerkleNode* right;
} MerkleNode;

bool merkle_hash(MerkleNode* merkle, unsigned char* key) {

    // Leaf node?
    if (merkle->left == NULL || merkle->right == NULL) {
        // TODO: implement hash value ...
        return false;
    } 

    unsigned char* left_hash = merkle->left->hash;
    unsigned char* right_hash = merkle->right->hash;

    sodium_add(left_hash, right_hash, crypto_generichash_BYTES);
    unsigned char* sum_hash = left_hash; // result was stored in left hash

    if (crypto_generichash(merkle->hash, crypto_generichash_BYTES, sum_hash, crypto_generichash_BYTES, key, crypto_generichash_KEYBYTES) < 0) {
        return false;
    }
    
    return true;
}

MerkleNode* merkle_new(void) {
    MerkleNode* merkle = malloc(sizeof(MerkleNode));
    merkle->left = NULL;
    merkle->right = NULL;
    return merkle;
}
