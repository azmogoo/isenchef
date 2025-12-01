/**
 * @file args.c
 * @brief command line argument parsing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/isenchef.h"

/* convert format string to enum */
static int parse_format(const char *str) {
    if (strcmp(str, "bytes") == 0 || strcmp(str, "raw") == 0) {
        return FORMAT_BYTES;
    } else if (strcmp(str, "hex") == 0 || strcmp(str, "hexadecimal") == 0) {
        return FORMAT_HEX;
    } else if (strcmp(str, "base64") == 0 || strcmp(str, "b64") == 0) {
        return FORMAT_BASE64;
    }
    return -1;
}

/* convert action string to enum */
static int parse_action(const char *str) {
    if (strcmp(str, "CAESAR") == 0 || strcmp(str, "caesar") == 0) {
        return ACTION_CAESAR;
    } else if (strcmp(str, "RC4") == 0 || strcmp(str, "rc4") == 0) {
        return ACTION_RC4;
    } else if (strcmp(str, "UPPERCASE") == 0 || strcmp(str, "uppercase") == 0) {
        return ACTION_UPPERCASE;
    } else if (strcmp(str, "LOWERCASE") == 0 || strcmp(str, "lowercase") == 0) {
        return ACTION_LOWERCASE;
    } else if (strcmp(str, "XOR") == 0 || strcmp(str, "xor") == 0) {
        return ACTION_XOR;
    }
    return -1;
}

/* print usage help */
static void print_usage(const char *program_name) {
    fprintf(stderr, "usage: %s --in <file> --input-format <format> "
                    "[--action <action> [--<action>-param=<value>]] "
                    "--out <file> --output-format <format>\n\n", program_name);
    fprintf(stderr, "formats: bytes, hex, base64\n");
    fprintf(stderr, "actions: caesar, rc4, uppercase, lowercase, xor\n");
    fprintf(stderr, "action parameters:\n");
    fprintf(stderr, "  --caesar-shift=<n>     shift value for caesar cipher\n");
    fprintf(stderr, "  --rc4-key=<key>        key for rc4 cipher (or --rc4key=)\n");
    fprintf(stderr, "  --xor-key=<key>        key for xor operation (or --xorkey=)\n");
    fprintf(stderr, "\nexamples:\n");
    fprintf(stderr, "  %s --in input.bin --input-format hex --out output.bin --output-format base64\n",
            program_name);
    fprintf(stderr, "  %s --in input.bin --input-format hex --action XOR --xor-key=123 "
                    "--out output.bin --output-format base64\n", program_name);
}

int parse_arguments(int argc, char *argv[], config_t *config) {
    memset(config, 0, sizeof(config_t));
    config->input_format = FORMAT_BYTES;
    config->output_format = FORMAT_BYTES;
    config->action = ACTION_NONE;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--in") == 0 || strcmp(argv[i], "-i") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "--in requires a filename\n");
                return -1;
            }
            config->input_file = argv[++i];
        } else if (strcmp(argv[i], "--out") == 0 || strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "--out requires a filename\n");
                return -1;
            }
            config->output_file = argv[++i];
        } else if (strcmp(argv[i], "--input-format") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "--input-format requires a format\n");
                return -1;
            }
            int format = parse_format(argv[++i]);
            if (format < 0) {
                fprintf(stderr, "invalid input format: %s\n", argv[i]);
                return -1;
            }
            config->input_format = (format_t)format;
        } else if (strcmp(argv[i], "--output-format") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "--output-format requires a format\n");
                return -1;
            }
            int format = parse_format(argv[++i]);
            if (format < 0) {
                fprintf(stderr, "invalid output format: %s\n", argv[i]);
                return -1;
            }
            config->output_format = (format_t)format;
        } else if (strcmp(argv[i], "--action") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "--action requires an action name\n");
                return -1;
            }
            int action = parse_action(argv[++i]);
            if (action < 0) {
                fprintf(stderr, "invalid action: %s\n", argv[i]);
                return -1;
            }
            config->action = (action_t)action;
        } else if (strncmp(argv[i], "--caesar-shift=", 15) == 0) {
            config->action_param = argv[i] + 15;
        } else if (strncmp(argv[i], "--rc4-key=", 10) == 0 || strncmp(argv[i], "--rc4key=", 9) == 0) {
            config->action_param = (strncmp(argv[i], "--rc4-key=", 10) == 0) ? argv[i] + 10 : argv[i] + 9;
        } else if (strncmp(argv[i], "--xor-key=", 10) == 0 || strncmp(argv[i], "--xorkey=", 9) == 0) {
            config->action_param = (strncmp(argv[i], "--xor-key=", 10) == 0) ? argv[i] + 10 : argv[i] + 9;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return -2;  /* for help */
        } else {
            fprintf(stderr, "unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return -1;
        }
    }
    if (!config->input_file) {
        fprintf(stderr, "--in is required\n");
        print_usage(argv[0]);
        return -1;
    }
    if (!config->output_file) {
        fprintf(stderr, "--out is required\n");
        print_usage(argv[0]);
        return -1;
    }
    if (config->action != ACTION_NONE && 
        (config->action == ACTION_CAESAR || config->action == ACTION_RC4 || 
         config->action == ACTION_XOR) && !config->action_param) {
        fprintf(stderr, "action requires a parameter\n");
        return -1;
    }
    return 0;
}

void free_config(config_t *config) {
    (void)config;  /* rien a free ici ;) */
}



