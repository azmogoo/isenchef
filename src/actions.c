/**
 * @file actions.c
 * @brief action processing functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/actions.h"

int action_caesar(uint8_t *data, size_t size, int shift) {
    /* handle negative shifts */
    shift = shift % 26;
    if (shift < 0) {
        shift += 26;
    }
    for (size_t i = 0; i < size; i++) {
        if (data[i] >= 'a' && data[i] <= 'z') {
            data[i] = 'a' + ((data[i] - 'a' + shift) % 26);
        } else if (data[i] >= 'A' && data[i] <= 'Z') {
            data[i] = 'A' + ((data[i] - 'A' + shift) % 26);
        }
    }
    return 0;
}

/* rc4 key scheduling */
static void rc4_ksa(const char *key, size_t key_len, uint8_t *S) {
    for (int i = 0; i < 256; i++) {
        S[i] = (uint8_t)i;
    }

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + (uint8_t)key[i % key_len]) % 256;
        uint8_t temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
}

/* rc4 prga */
static uint8_t rc4_prga(uint8_t *S, int *i, int *j) {
    *i = (*i + 1) % 256;
    *j = (*j + S[*i]) % 256;
    uint8_t temp = S[*i];
    S[*i] = S[*j];
    S[*j] = temp;
    int t = (S[*i] + S[*j]) % 256;
    return S[t];
}

int action_rc4(uint8_t *data, size_t size, const char *key) {
    if (!key || strlen(key) == 0) {
        return -1;
    }
    size_t key_len = strlen(key);
    uint8_t S[256];
    int i = 0, j = 0;

    rc4_ksa(key, key_len, S);
    for (size_t k = 0; k < size; k++) {
        uint8_t keystream_byte = rc4_prga(S, &i, &j);
        data[k] ^= keystream_byte;
    }
    return 0;
}

int action_uppercase(uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (data[i] >= 'a' && data[i] <= 'z') {
            data[i] = (uint8_t)toupper((char)data[i]);
        }
    }
    return 0;
}

int action_lowercase(uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (data[i] >= 'A' && data[i] <= 'Z') {
            data[i] = (uint8_t)tolower((char)data[i]);
        }
    }
    return 0;
}

int action_xor(uint8_t *data, size_t size, const char *key) {
    if (!key || strlen(key) == 0) {
        return -1;
    }
    size_t key_len = strlen(key);
    for (size_t i = 0; i < size; i++) {
        data[i] ^= (uint8_t)key[i % key_len];
    }
    return 0;
}