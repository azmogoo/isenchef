/**
 * @file actions.h
 * @brief action processing functions
 */

#ifndef ACTIONS_H
#define ACTIONS_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief apply caesar cipher
 * @param data data buffer
 * @param size buffer size
 * @param shift shift value (positive or negative)
 * @return 0 on success, -1 on error
 */
int action_caesar(uint8_t *data, size_t size, int shift);

/**
 * @brief apply rc4 cipher
 * @param data data buffer
 * @param size buffer size
 * @param key encryption key (null-terminated string)
 * @return 0 on success, -1 on error
 */
int action_rc4(uint8_t *data, size_t size, const char *key);

/**
 * @brief convert text to uppercase
 * @param data data buffer
 * @param size buffer size
 * @return 0 on success, -1 on error
 */
int action_uppercase(uint8_t *data, size_t size);

/**
 * @brief convert text to lowercase
 * @param data data buffer
 * @param size buffer size
 * @return 0 on success, -1 on error
 */
int action_lowercase(uint8_t *data, size_t size);

/**
 * @brief apply xor operation
 * @param data data buffer
 * @param size buffer size
 * @param key xor key (null-terminated string)
 * @return 0 on success, -1 on error
 */
int action_xor(uint8_t *data, size_t size, const char *key);

#endif /* ACTIONS_H */

