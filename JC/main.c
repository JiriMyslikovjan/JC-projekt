#include <stdio.h>
#include "matice.h"

int main()
{
    matice a = nacti_ze_souboru("./soubor.txt");
    vypis(a);
}