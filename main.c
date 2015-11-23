#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "lexer.h"

int
main(int argc, char **argv)
{
    if (argc != 1) {
        printf("No arguments yet\n");
    }

    char *script_line;
    // Read from stdin for now
    while (script_line = readline(NULL)) {
        char *cur_pos = script_line;
        while (strlen(cur_pos) > 0) {
            nosh_token *t;
            cur_pos = next_token(cur_pos, &t);
            token_print(t);
            token_free(t);
        }
        free(script_line);
    }
}
