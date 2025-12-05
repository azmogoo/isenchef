#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "bitkitchen.h"

/**
 * @brief Process the given configuration (read, transform, write)
 * @param config Configuration filled by the caller
 * @return 0 on success, non-zero otherwise
 */
int execute_config(const config_t *config);

#endif /* PROCESSOR_H */

