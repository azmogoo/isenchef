/**
 * @file encoders.c
 * @brief encoding/decoding functions for different formats
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/encoders.h"

static int hex_char_to_value(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;
}

static char value_to_hex_char(int value) {
    if (value >= 0 && value <= 9) {
        return '0' + value;
    } else if (value >= 10 && value <= 15) {
        return 'a' + (value - 10);
    }
    return '?';
}

int decode_hex(const char *hex, uint8_t *output, size_t output_size) {
    size_t hex_len = strlen(hex);
    size_t output_idx = 0;

    /* remove whitespace and handle pairs */
    for (size_t i = 0; i < hex_len && output_idx < output_size; i++) {
        if (isspace((unsigned char)hex[i])) {
            continue;
        }
        int high = hex_char_to_value(hex[i]);
        if (high < 0) {
            return -1;
        }
        i++;
        while (i < hex_len && isspace((unsigned char)hex[i])) {
            i++;
        }
        if (i >= hex_len) {
            /* odd number of hex digits, pad with 0 */
            output[output_idx++] = (uint8_t)(high << 4);
            break;
        }
        int low = hex_char_to_value(hex[i]);
        if (low < 0) {
            return -1;
        }
        output[output_idx++] = (uint8_t)((high << 4) | low);
    }
    return (int)output_idx;
}

int encode_hex(const uint8_t *input, size_t input_size, char *output) {
    for (size_t i = 0; i < input_size; i++) {
        output[i * 2] = value_to_hex_char((input[i] >> 4) & 0x0F);
        output[i * 2 + 1] = value_to_hex_char(input[i] & 0x0F);
    }
    output[input_size * 2] = '\0';
    return 0;
}

/* b64 alphabet */
static const char base64_table[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int base64_char_to_value(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    } else if (c >= 'a' && c <= 'z') {
        return c - 'a' + 26;
    } else if (c >= '0' && c <= '9') {
        return c - '0' + 52;
    } else if (c == '+') {
        return 62;
    } else if (c == '/') {
        return 63;
    } else if (c == '=') {
        return -2;  /* padding */
    }
    return -1;  /* invalid */
}

int decode_base64(const char *base64, uint8_t *output, size_t output_size) {
    size_t base64_len = strlen(base64);
    size_t output_idx = 0;
    int buffer = 0;
    int bits_collected = 0;
    for (size_t i = 0; i < base64_len; i++) {
        if (isspace((unsigned char)base64[i])) {
            continue;
        }
        int value = base64_char_to_value(base64[i]);
        if (value == -1) {
            return -1;  /* invalid character */
        }
        if (value == -2) {
            break;  /* padding, stop here */
        }
        buffer = (buffer << 6) | value;
        bits_collected += 6;
        if (bits_collected >= 8) {
            if (output_idx >= output_size) {
                return -1;  /* output buffer too small */
            }
            output[output_idx++] = (uint8_t)((buffer >> (bits_collected - 8)) & 0xFF);
            bits_collected -= 8;
        }
    }
    return (int)output_idx;
}

int encode_base64(const uint8_t *input, size_t input_size, char *output, size_t output_size) {
    size_t output_idx = 0;
    size_t i;
    for (i = 0; i < input_size; i += 3) {
        if (output_idx + 4 >= output_size) {
            return -1;  /* output buffer too small */
        }
        uint32_t buffer = 0;
        int bytes_to_process = (input_size - i < 3) ? (int)(input_size - i) : 3;

        /* build 24-bit buffer */
        for (int j = 0; j < bytes_to_process; j++) {
            buffer = (buffer << 8) | input[i + j];
        }

        /* encode to base64 */
        int bits = bytes_to_process * 8;
        int chars_to_write = (bits + 5) / 6;

        for (int j = 0; j < chars_to_write; j++) {
            int shift = (3 - j) * 6;
            int index = (buffer >> shift) & 0x3F;
            output[output_idx++] = base64_table[index];
        }

        /* add padding if needed */
        while (chars_to_write < 4) {
            output[output_idx++] = '=';
            chars_to_write++;
        }
    }
    output[output_idx] = '\0';
    return (int)output_idx;
}

