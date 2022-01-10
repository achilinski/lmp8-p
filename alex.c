#include "alex.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

static int ln = 0;
static char ident[256];
static FILE* ci = NULL;

mag stack;

char** func_data;
int func_size = 0;

void alex_init4file(FILE* in) {
    ln = 0;
    ci = in;
}

static const char* keywords[] = { "default", "enum", "extern", "for", "if",
                                 "else", "int",
                                 "while",
                                 "break", "continue",
                                 "double", "float",
                                 "return", "char",
                                 "case", "const",
                                 "sizeof", "long",
                                 "short", "typedef",
                                 "switch",
                                 "void", "static",
                                 "struct" };

int isKeyword(char* str) {

    int i;
    for (i = 0; i < 24; i++)
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    return 0;

}


lexem_t alex_nextLexem(void) {
    int c;
    while ((c = fgetc(ci)) != EOF) {
        if (isspace(c) && c != '\n')
            continue;
        else if (c == '\n')
            ln++;
        else if (c == '(')
            return OPEPAR;
        else if (c == ')')
            return CLOPAR;
        else if (c == '{')
            return OPEBRA;
        else if (c == '}')
            return CLOBRA;
        else if (isalpha(c)) {
            int i = 1;
            ident[0] = c;
            while (isalnum(c = fgetc(ci)))
                ident[i++] = c;
            ident[i] = '\0';
            //dodano:
            if (ungetc(c, ci) == EOF)
                exit(-50);
            return isKeyword(ident) == 1 ? OTHER : IDENT;
        }
        else if (c == '"') {
            /* Uwaga: tu trzeba jeszcze poprawic obsluge nowej linii w trakcie napisu
               i \\ w napisie
            */
            char cp = 0;
            while ((c = fgetc(ci)) != EOF && (c != '"' || (cp == '\\' && c == '"')))
                cp = c;

            /*
             * Wykonuj dopóki:
             * != EOF i
             * (
             * c=" i cp=\ lub
             * c!="
             * )
             * */
            return c == EOF ? EOFILE : OTHER;
        }
        else if (c == '/') {
            if ((c = fgetc(ci)) != EOF) {
                if (c == '/') {
                    while ((c = fgetc(ci)) != EOF && c != '\n');
                }
                else if (c == '*') {
                    char cp = 0;
                    /*
                     * dopóki:
                     * !=EOF oraz
                     * !(cp==* oraz c==/)
                     * */

                    while ((c = fgetc(ci)) != EOF && !(cp == '*' && c == '/')) {
                        if (c == '\n')
                            ln++;
                        cp = c;
                    }
                }
            }
            //nie komentarz
            ungetc(c, ci);

            return OTHER;
        }
        if (isdigit(c) || c == '.') {
            /* liczba */
        }
        else {
            return OTHER;
        }
    }
    return EOFILE;
}

char* alex_ident(void) {
    return ident;
}

int alex_getLN() {
    return ln;
}

void new_func(char* funame) {
    int i;
    for (i = 0; i < func_size; i++)
        if (strcmp(func_data[i], funame) == 0)
            return;

    if (func_size++ == 0)
        func_data = malloc(sizeof * func_data);
    else
        func_data = realloc(func_data, func_size * sizeof * func_data);
    func_data[func_size - 1] = malloc((strlen(funame) + 1) * sizeof * func_data);
    strcpy(func_data[func_size - 1], funame);

}

void store_add_def(char* funame, int linia, char* inpname) {
    if (stack.size++ == 0)
        stack.data = malloc(sizeof * stack.data);
    else
        stack.data = realloc(stack.data, stack.size * sizeof * stack.data);

    stack.data[stack.size - 1].use = malloc(4 * sizeof(char));
    strcpy(stack.data[stack.size - 1].use, "def");

    stack.data[stack.size - 1].funame = malloc((strlen(funame) + 1) * sizeof(*funame));
    strcpy(stack.data[stack.size - 1].funame, funame);

    stack.data[stack.size - 1].linia = linia;

    stack.data[stack.size - 1].inpname = malloc((strlen(inpname) + 1) * sizeof(*inpname));
    strcpy(stack.data[stack.size - 1].inpname, inpname);
    new_func(funame);

}


void store_add_proto(char* funame, int linia, char* inpname) {

    if (stack.size++ == 0)
        stack.data = malloc(sizeof * stack.data);
    else
        stack.data = realloc(stack.data, stack.size * sizeof * stack.data);

    stack.data[stack.size - 1].use = malloc(6 * sizeof(char));
    strcpy(stack.data[stack.size - 1].use, "proto");

    stack.data[stack.size - 1].funame = malloc((strlen(funame) + 1) * sizeof(*funame));
    strcpy(stack.data[stack.size - 1].funame, funame);

    stack.data[stack.size - 1].linia = linia;

    stack.data[stack.size - 1].inpname = malloc((strlen(inpname) + 1) * sizeof(*inpname));
    strcpy(stack.data[stack.size - 1].inpname, inpname);
    new_func(funame);

}

void store_add_call(char* funame, int linia, char* inpname) {
    if (stack.size++ == 0)
        stack.data = malloc(sizeof * stack.data);
    else
        stack.data = realloc(stack.data, stack.size * sizeof * stack.data);

    stack.data[stack.size - 1].use = malloc(5 * sizeof(char));
    strcpy(stack.data[stack.size - 1].use, "call");

    stack.data[stack.size - 1].funame = malloc((strlen(funame) + 1) * sizeof(*funame));
    strcpy(stack.data[stack.size - 1].funame, funame);

    stack.data[stack.size - 1].linia = linia;

    stack.data[stack.size - 1].inpname = malloc((strlen(inpname) + 1) * sizeof(*inpname));
    strcpy(stack.data[stack.size - 1].inpname, inpname);
    new_func(funame);
}


void wypisz_funkcje() {
    /*
    int i;
        for(i=0;i<dane.size;i++)
            printf("%s(), typ: %s, linia: %d\n", dane.data[i].funame, dane.data[i].type, dane.data[i].line);
    */

    int i, j;
    for (i = 0; i < func_size; i++) {
        printf("Funkcja %s:\n", func_data[i]);
        printf("\tPrototyp:\n");
        for (j = 0; j < stack.size; j++)
            if (strcmp(stack.data[j].use, "proto") == 0 && strcmp(stack.data[j].funame, func_data[i]) == 0)
                printf("\t\tplik %s w linii %d\n", stack.data[j].inpname, stack.data[j].linia);

        printf("\tDefinicja:\n");
        for (j = 0; j < stack.size; j++)
            if (strcmp(stack.data[j].use, "def") == 0 && strcmp(stack.data[j].funame, func_data[i]) == 0)
                printf("\t\tplik %s w linii %d\n", stack.data[j].inpname, stack.data[j].linia);

        printf("\tUzycie:\n");
        for (j = 0; j < stack.size; j++)
            if (strcmp(stack.data[j].use, "call") == 0 && strcmp(stack.data[j].funame, func_data[i]) == 0)
                printf("\t\tplik %s w linii %d\n", stack.data[j].inpname, stack.data[j].linia);
    }
}