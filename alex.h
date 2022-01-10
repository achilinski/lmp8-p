#ifndef _ALEX_H_IS_INCLUDED_
#define _ALEX_H_IS_INCLUDED_

#include <stdio.h>


// interesujące leksemy: błąd, inny symbol, koniec pliku, otwierająca klamra {, 
//                       zamykająca klamra }, identyfikator, otwierający nawias, 
//                       zamykający nawias
typedef enum {
    ERROR, OTHER, EOFILE, OPEBRA, CLOBRA, IDENT, OPEPAR, CLOPAR
} lexem_t;

typedef struct {
    char* use;
    char* funame;
    int linia;
    char* inpname;
} mem;

typedef struct {
    mem* data;
    int size;
} mag;


void alex_init4file(FILE*);  // zacznij czytać nowy plik
lexem_t alex_nextLexem(void);    // daj kolejny leksem w czytanym pliku
char* alex_ident(void);        // daj ostatni identyfikator
int alex_getLN();              // daj aktualny nr linii

void store_add_def(char* funame, int line, char* ipname);

void store_add_proto(char* funame, int line, char* ipname);

void store_add_call(char* funame, int line, char* ipname);

void wypisz_funkcje();

#endif
