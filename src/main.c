/**
 * @file main.c
 * @brief main entry point for isenchef tool
 */

#include "../include/isenchef.h"
#include "../include/processor.h"

int main(int argc, char *argv[]) {
    config_t config;

    int parse_result = parse_arguments(argc, argv, &config);
    if (parse_result == -2) {
        return 0;
    } else if (parse_result < 0) {
        return 1;
    }

    return execute_config(&config);
}

