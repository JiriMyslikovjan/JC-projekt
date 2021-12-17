#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BEZ_CHYBY 0
#define CHYBA_ALOKACE -1
#define CHYBA_OTEVRENI -2
#define CHYBA_ZAVRENI -3
#define CHYBA_TYPU -4
#define CHYBA_JINA -5

int chyba;

typedef struct MATICE
{
    int m;
    int n;
    float ** matrix;
    int initialized;
} matice;

int getError();

void writeError(int i);

int velikost(matice mat, int dimenze);

float prvek(matice mat, int i, int j);

void nastav_prvek(matice mat, int i, int j, float hodonota);

matice inicializace(int m, int n);

matice nulova(int m, int n);

matice jednotkova(int m, int n);

void odstran(matice mat);

void vypis(matice mat);

matice plus(matice mat1, matice mat2);

matice minus(matice mat1, matice mat2);

matice nasobeni(matice mat1, float skalar);

matice krat(matice mat1, matice mat2);

matice transpozice(matice mat);

void getFileRowsColumns(FILE * file, unsigned int * rows, unsigned int * cols);
    // Gets number of lines and columns of a matrix in a file, checks matrix formatting

matice fileToMatrix(FILE * file);

matice nacti_ze_souboru(const char * soubor);

void uloz_do_souboru(matice mat, const char * soubor);
