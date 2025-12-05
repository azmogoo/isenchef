/**
 * @file bitkitchen.h
 * @brief main header file for bitkitchen tool
 */

#ifndef BITKITCHEN_H
#define BITKITCHEN_H

#include <stddef.h>
#include <stdint.h>

/* maximum buffer size for file operations */
#define MAX_BUFFER_SIZE (1024 * 1024)  /* 1MB */

/* input/output format types */
typedef enum {
    FORMAT_BYTES,
    FORMAT_HEX,
    FORMAT_BASE64
} format_t;

/* action types */
typedef enum {
    ACTION_NONE,
    ACTION_CAESAR,
    ACTION_RC4,
    ACTION_UPPERCASE,
    ACTION_LOWERCASE,
    ACTION_XOR
} action_t;

/* configuration structure */
typedef struct {
    char *input_file;
    char *output_file;
    format_t input_format;
    format_t output_format;
    action_t action;
    char *action_param;  /* for xor key, caesar shift, rc4 key, etc. */
} config_t;

/* data buffer structure */
typedef struct {
    uint8_t *data;
    size_t size;
} buffer_t;

/**
 * @brief parse command line arguments
 * @param argc argument count
 * @param argv argument vector
 * @param config configuration structure to fill
 * @return 0 on success, -1 on error
 */
int parse_arguments(int argc, char *argv[], config_t *config);

/**
 * @brief free configuration resources
 * @param config configuration structure
 */
void free_config(config_t *config);

/**
 * @brief read file and decode according to input format
 * @param filename input filename
 * @param format input format
 * @param buffer output buffer structure
 * @return 0 on success, -1 on error
 */
int read_file(const char *filename, format_t format, buffer_t *buffer);

/**
 * @brief encode buffer and write to file
 * @param filename output filename
 * @param format output format
 * @param buffer input buffer structure
 * @return 0 on success, -1 on error
 */
int write_file(const char *filename, format_t format, const buffer_t *buffer);

/**
 * @brief free buffer resources
 * @param buffer buffer structure
 */
void free_buffer(buffer_t *buffer);

#endif /* BITKITCHEN_H */

