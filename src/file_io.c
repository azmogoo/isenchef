/**
 * @file file_io.c
 * @brief file input/output operations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/bitkitchen.h"
#include "../include/encoders.h"

int read_file(const char *filename, format_t format, buffer_t *buffer) {
    FILE *file;
    size_t file_size;
    char *file_content = NULL;
    int result = -1;

    buffer->data = NULL;
    buffer->size = 0;

    file = fopen(filename, "rb");
    if (!file) {
        perror("opening input file");
        return -1;
    }


    /* get file size */
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (format == FORMAT_BYTES) {
        buffer->data = (uint8_t *)malloc(file_size);
        if (!buffer->data) {
            fprintf(stderr, "memory allocation failed\n");
            fclose(file);
            return -1;
        }
        buffer->size = fread(buffer->data, 1, file_size, file);
        if (buffer->size != file_size) {
            fprintf(stderr, "failed to read entire file\n");
            free(buffer->data);
            buffer->data = NULL;
            fclose(file);
            return -1;
        }
        result = 0;
    } else {
        /* need to decode hex or base64 */
        file_content = (char *)malloc(file_size + 1);
        if (!file_content) {
            fprintf(stderr, "memory allocation failed\n");
            fclose(file);
            return -1;
        }
        size_t read_size = fread(file_content, 1, file_size, file);
        file_content[read_size] = '\0';

        /* estimate output size, but idk if it's really optimal */
        size_t max_output_size = (format == FORMAT_HEX) ? (file_size / 2 + 1) : 
                                                          ((file_size * 3) / 4 + 1);
        buffer->data = (uint8_t *)malloc(max_output_size);
        if (!buffer->data) {
            fprintf(stderr, "memory allocation failed\n");
            free(file_content);
            fclose(file);
            return -1;
        }

        if (format == FORMAT_HEX) {
            int decoded_size = decode_hex(file_content, buffer->data, max_output_size);
            if (decoded_size < 0) {
                fprintf(stderr, "invalid hex format\n");
                free(buffer->data);
                buffer->data = NULL;
                result = -1;
            } else {
                buffer->size = (size_t)decoded_size;
                result = 0;
            }
        } else if (format == FORMAT_BASE64) {
            int decoded_size = decode_base64(file_content, buffer->data, max_output_size);
            if (decoded_size < 0) {
                fprintf(stderr, "invalid base64 format\n");
                free(buffer->data);
                buffer->data = NULL;
                result = -1;
            } else {
                buffer->size = (size_t)decoded_size;
                result = 0;
            }
        }
        free(file_content);
    }
    fclose(file);
    return result;
}

int write_file(const char *filename, format_t format, const buffer_t *buffer) {
    FILE *file;
    int result = -1;

    file = fopen(filename, "wb");
    if (!file) {
        perror("opening output file");
        return -1;
    }

    if (format == FORMAT_BYTES) {
        size_t written = fwrite(buffer->data, 1, buffer->size, file);
        if (written != buffer->size) {
            fprintf(stderr, "failed to write entire file\n");
            fclose(file);
            return -1;
        }
        result = 0;
    } else {
        /* encode to hex or base64 */
        size_t output_size;
        char *output = NULL;

        if (format == FORMAT_HEX) {
            output_size = buffer->size * 2 + 1;
            output = (char *)malloc(output_size);
            if (!output) {
                fprintf(stderr, "memory allocation failed\n");
                fclose(file);
                return -1;
            }
            if (encode_hex(buffer->data, buffer->size, output) < 0) {
                fprintf(stderr, "hex encoding failed\n");
                free(output);
                fclose(file);
                return -1;
            }
        } else if (format == FORMAT_BASE64) {
            output_size = ((buffer->size + 2) / 3) * 4 + 1;
            output = (char *)malloc(output_size);
            if (!output) {
                fprintf(stderr, "memory allocation failed\n");
                fclose(file);
                return -1;
            }
            int encoded_size = encode_base64(buffer->data, buffer->size, output, output_size);
            if (encoded_size < 0) {
                fprintf(stderr, "base64 encoding failed\n");
                free(output);
                fclose(file);
                return -1;
            }
            output_size = encoded_size;
        }
        size_t written = fwrite(output, 1, output_size - 1, file);
        if (written != output_size - 1) {
            fprintf(stderr, "failed to write entire file\n");
            free(output);
            fclose(file);
            return -1;
        }
        free(output);
        result = 0;
    }
    fclose(file);
    return result;
}

void free_buffer(buffer_t *buffer) {
    if (buffer->data) {
        free(buffer->data);
        buffer->data = NULL;
    }
    buffer->size = 0;
}

