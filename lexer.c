#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"

#define TOKEN_BINDATA_SIZE 16

char *noshtokenclass_str[] = {"TOKEN_NONE", "TOKEN_WS", "TOKEN_NEWLINE", "TOKEN_OPTION", "TOKEN_FILE", "TOKEN_WORD", "TOKEN_VARIABLE"};

int
is_slash(char c)
{
    if (c == '/') {
        return 1;
    } else {
        return 0;
    }
}

int
is_whitespace(char c)
{
    switch (c) {
        case ' ':
        case '\t':
            return 1;
            break;
        default:
            return 0;
            break;
    }
}

int
is_newline(char c)
{
    if (c == '\n') {
        return 1;
    } else {
        return 0;
    }
}

int is_period(char c)
{
    if (c == '.') {
        return 1;
    } else {
        return 0;
    }
}

int is_hyphen(char c)
{
    if (c == '-') {
        return 1;
    } else {
        return 0;
    }
}

int is_word_start(char c) {
    switch (c) {
        case '_':
            return 1;
    }
    if ( c >= 'A' && c <= 'z' ) {
        return 1;
    } else {
        return 0;
    }
}

int
is_word_character(char c) {
    if (is_word_start(c) || c == '$' || c == '-') {
        return 1;
    } else {
        return 0;
    }
}

int is_file_character(char c)
{
    if (is_word_character(c) || is_period(c) || is_hyphen(c) || is_slash(c)) {
        return 1;
    } else {
        return 0;
    }
}

int
is_option_character(char c)
{
    return is_word_character(c) || c == '=';
}

int
is_var_character(char c)
{
    if (c >= 'A' && c <= 'z') {
        return 1;
    } else {
        return 0;
    }
}


int
is_dollar(char c)
{
    if (c == '$') {
        return 1;
    } else {
        return 0;
    }
}

void
token_print(nosh_token *t)
{
    if (t == NULL) {
        printf("NULL\n");
        return;
    }
    if (t->bindata == NULL) {
        printf("BAD TOKEN\n");
    }
    printf("%s %d %s\n", noshtokenclass_str[t->class], t->len, t->bindata);
}

void
token_add_character(nosh_token *t, char c)
{
    /* Expand string size if necessary */
    if ((t->len + 1) % 16 == 0) {
        t->bindata = realloc(t->bindata, t->len + TOKEN_BINDATA_SIZE + 1);
    }

    t->bindata[t->len] = c;
    t->len++;
}

char *
next_token(char *s, nosh_token **ret_t)
{
    nosh_token *t = calloc(1, sizeof(nosh_token));
    t->bindata = calloc(1, TOKEN_BINDATA_SIZE * sizeof (char));
    t->len = 0;

    /* First character gives us plenty of info */
    char c = *s;
    if (is_whitespace(c)) {
        /* Token is whitespace.  Gobble it up! */
        t->class = TOKEN_WS;
        while (is_whitespace(c)) {
            token_add_character(t, c);
            c = *(++s);
        }
    } else if (is_newline(c)) {
        t->class = TOKEN_NEWLINE;
        token_add_character(t, c);
        ++s;
    } else if (is_hyphen(c)) {
        t->class = TOKEN_OPTION;
        while (is_option_character(c)) {
            token_add_character(t, c);
            c = *(++s);
        }
    } else if (is_period(c) || is_slash(c)) {
        t->class = TOKEN_FILE;
        while (is_file_character(c)) {
            token_add_character(t, c);
            c = *(++s);
        }
    } else if (is_dollar(c)) {
        t->class = TOKEN_VARIABLE;
        do {
            token_add_character(t, c);
            c = *(++s);
        } while (is_var_character(c));
    } else if (is_word_start(c)) {
        t->class = TOKEN_WORD;
        do {
            token_add_character(t, c);
            c = *(++s);
        } while (is_word_character(c));
    } else if (c == '\0') {
        s = NULL;
    } else {
        t->class = TOKEN_NONE;
        while (!is_whitespace(c) && c != '\0') {
            token_add_character(t, c);
            c = *(++s);
        }
    }

    *ret_t = t;
    return s;
}

void
token_free(nosh_token *t)
{
    free(t->bindata);
    free(t);
}

/*
int
main(int argc, char **argv)
{
    char *s = argv[1];
    nosh_token *t;
    while (1) {
        s = next_token(s, &t);
        if (s == NULL) {
            printf("We're done\n");
            break;
        }
        token_print(t);
        token_free(t);
    }
    return 0;
}
*/
