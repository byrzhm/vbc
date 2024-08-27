#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "defs.h"

int main() {
    char *input;
    bool success;
    uint32_t result;
    init_regex();
    
    for (;;) {
        input = readline("> ");

        if (input == NULL) {
            printf("Exited.\n");
            break;
        }

        result = parse(input, &success);
        if (!success) {
            printf("Invalid expression\n");
        } else {
            printf("%u\n", result);
        }

        add_history(input);
        free(input);
    }

    return 0;
}
