#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/processor.h"
#include "../include/bitkitchen.h"
#include "../include/actions.h"

static int process_action(buffer_t *buffer, const config_t *config) {
    if (config->action == ACTION_NONE) {
        return 0;
    }
    switch (config->action) {
        case ACTION_CAESAR: {
            int shift = atoi(config->action_param);
            return action_caesar(buffer->data, buffer->size, shift);
        }
        case ACTION_RC4:
            return action_rc4(buffer->data, buffer->size, config->action_param);
        case ACTION_UPPERCASE:
            return action_uppercase(buffer->data, buffer->size);
        case ACTION_LOWERCASE:
            return action_lowercase(buffer->data, buffer->size);
        case ACTION_XOR:
            return action_xor(buffer->data, buffer->size, config->action_param);
        default:
            fprintf(stderr, "unknown action\n");
            return -1;
    }
}

int execute_config(const config_t *config) {
    buffer_t buffer = {0};
    int result = 0;
    if (read_file(config->input_file, config->input_format, &buffer) < 0) {
        fprintf(stderr, "failed to read input file\n");
        result = 1;
        goto cleanup;
    }
    if (process_action(&buffer, config) < 0) {
        fprintf(stderr, "failed to apply action\n");
        result = 1;
        goto cleanup;
    }
    if (write_file(config->output_file, config->output_format, &buffer) < 0) {
        fprintf(stderr, "failed to write output file\n");
        result = 1;
        goto cleanup;
    }
    printf("processed file: %s -> %s\n",
           config->input_file, config->output_file);

cleanup:
    free_buffer(&buffer);
    return result;
}

