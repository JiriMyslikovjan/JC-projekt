#include <stdio.h>
#include <stdlib.h>
#include "matice.h"

int getError()
{
    return chyba;
}

void writeError(int i)
{ 
    chyba = i;
}

int velikost(matice mat, int dimenze)
{
    if(mat.initialized)
    {
        switch (dimenze)
        {
            case 1:
                return mat.m;
            
            case 2:
                return mat.n;
            
            default:
                writeError(CHYBA_JINA);
                break;
        }
    }

    writeError(CHYBA_JINA);

    return -1;
}

float prvek(matice mat, int i, int j)
{
    if(i > 0 && j > 0 && i < mat.m && j < mat.n && mat.initialized)
        return mat.matrix[i][j];
    
    writeError(CHYBA_JINA);
    
    return -1;
}

void nastav_prvek(matice mat, int i, int j, float hodonota)
{
    if(i > 0 && j > 0 && i < mat.m && j < mat.n && mat.initialized)
        mat.matrix[i][j] = hodonota;

    else
        writeError(CHYBA_JINA);
}

matice inicializace(int m, int n)
{
    matice a;
    a.m = m;
    a.n = n;
    a.initialized = 0;

    writeError(BEZ_CHYBY);

    if((a.matrix = (float **)malloc(m * sizeof(float *))) != NULL)
    {
        for(int i = 0; i < m; i++)
            {
                if((a.matrix[i] = (float *) malloc(n * sizeof(float))) == NULL)
                    {
                        writeError(CHYBA_ALOKACE);
                        break;
                    }
            }
    }
    else
        writeError(CHYBA_ALOKACE);
    
    if(getError() == BEZ_CHYBY)
        a.initialized = 1;

    return a;
}

matice nulova(int m, int n)
{
    matice mat = inicializace(m, n);

    if(getError() != BEZ_CHYBY)
        return mat;

    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
            nastav_prvek(mat, i, j, 0);
    }

    return mat;
}

matice jednotkova(int m, int n)
{
    matice mat = nulova(m, n);
    int j = 0;

    /* Musí být čtvercová ?
    if(m != n)
        writeError(CHYBA_TYPU);
    */

    if(getError() != BEZ_CHYBY)
        return mat;

    for(int i = 0; i < m; i++)
    {
        nastav_prvek(mat, i, j, 1.0);
        j++;
    }

    return mat;
}

void odstran(matice mat)
{
    int m = mat.m;

    if(mat.initialized)
    {
        for (int i = 0; i < m; i++)
        {
            if(mat.matrix[i] != NULL)
                free(mat.matrix[i]);
        }

        free(mat.matrix);
    }

    else
        writeError(CHYBA_JINA);
}

void vypis(matice mat)
{
    if(mat.initialized)
    {
        for (int i = 0; i < mat.m; i++)
        {
            for (int j = 0; j < mat.n; j++)
                printf("%f ", prvek(mat, i, j));
            
            printf("\n");
        }
    }
    
    else
        writeError(CHYBA_JINA);
}

matice plus(matice mat1, matice mat2)
{
    matice result;

    if(mat1.m != mat2.m || mat1.n != mat2.n)
        writeError(CHYBA_TYPU);

    if(getError() == BEZ_CHYBY)
    {
        result = inicializace(mat1.m, mat1.n);

        for(int i = 0; i < mat1.m; i++)
        {
            for (int j = 0; j < mat1.n; j++)
                result.matrix[i][j] = mat1.matrix[i][j] + mat2.matrix[i][j];
                
        }
            
        return result;
    }
  
    return result;
}

matice minus(matice mat1, matice mat2)
{
    matice result = inicializace(mat1.m, mat1.n);

    if(mat1.m != mat2.m || mat1.n != mat2.n)
        writeError(CHYBA_TYPU);

    if(getError() == BEZ_CHYBY)
        {
            for(int i = 0; i < mat1.m; i++)
            {
                for (int j = 0; j < mat1.n; j++)
                    result.matrix[i][j] = mat1.matrix[i][j] - mat2.matrix[i][j];
                
            }
            
            return result;
        }
    
    return result;
}

matice nasobeni(matice mat1, float skalar)
{
    matice result = inicializace(mat1.m, mat1.n);

    if(getError() == BEZ_CHYBY)
        {
            for(int i = 0; i < mat1.m; i++)
            {
                for (int j = 0; j < mat1.n; j++)
                    result.matrix[i][j] = mat1.matrix[i][j] * skalar;
                
            }
            
            return result;
        }
  
    return result;
}

matice krat(matice mat1, matice mat2)
{
    matice result = inicializace(mat2.n, mat1.m);
    float sum = 0;
    
    if(mat1.n != mat2.m)
        writeError(CHYBA_TYPU);

    if(getError() == BEZ_CHYBY)
    {
        for(int i = 0; i < mat1.m; i++)
        {
            for(int j = 0; j < mat2.n; j++)
            {
                sum = 0;
                for(int k = 0; k < mat2.m; k++)
                {
                    sum += mat1.matrix[i][k] * mat2.matrix[k][j];
                }
                result.matrix[i][j] = sum;
            }
        }
    }

    return result;
}

matice transpozice(matice mat)
{
	matice result = inicializace(mat.n, mat.m);

    if(getError() == BEZ_CHYBY)
	{
        for (int i = 0; i < mat.m; i++)
	    {
		    for (int j = 0; j < mat.n; j++)
			    result.matrix[j][i] = mat.matrix[i][j];
	    }
    }
	
	return result;
}

void getFileRowsColumns(FILE * file, unsigned int * rows, unsigned int * cols)
{
    float val = 0.000;
    unsigned int floatsCnt = 0, rowsCnt = 0;
    char c = ' ';

    while(fscanf(file, "%f", &val) != EOF)
        floatsCnt++;

    rewind(file);

    while(! feof(file))
    {
        c = fgetc(file);

        if(c == '\n')
            rowsCnt++;
    }

    if(floatsCnt % rowsCnt != 0)
        writeError(CHYBA_TYPU);
    
    if(floatsCnt == 0 || rowsCnt == 0)
        writeError(CHYBA_TYPU);

    * cols = floatsCnt / rowsCnt;
    * rows = rowsCnt;

    rewind(file);
}

matice fileToMatrix(FILE * file)
{
    unsigned int rows = 0, cols = 0, i = 0, j = 0;
    float val = 0.000;
    matice mat;

    getFileRowsColumns(file, &rows, &cols);

    if(getError() != BEZ_CHYBY)
        return mat;

    mat = inicializace(rows, cols);

    while(fscanf(file, "%f", &val) != EOF)
    {
        mat.matrix[i][j] = val;
        
        j++;

        if(j >= rows)
        {
            j = 0;
            i++;
        }
    }

    return mat;
}

matice nacti_ze_souboru(const char * soubor)
{
    matice result;
    FILE * file;

    if((file = fopen(soubor, "r")) == NULL)
        writeError(CHYBA_OTEVRENI);
        
    if(getError() == BEZ_CHYBY)
        result = fileToMatrix(file);
  
    if(fclose(file) == EOF)
        writeError(CHYBA_ZAVRENI);

    return result;
}

void uloz_do_souboru(matice mat, const char * soubor)
{
    FILE * file = NULL;

    if((file = fopen(soubor, "w")) == NULL)
    {
        writeError(CHYBA_OTEVRENI);
        
        return;
    }

    for(int i = 0; i < mat.m; i++)
    {
        for (int j = 0; j < mat.n; j++)
            fprintf(file, "%f ", mat.matrix[i][j]);

        fputc('\n', file);
    }

    if(fclose(file) == EOF)
    {
        writeError(CHYBA_ZAVRENI);

        return;
    }
}

int main ()
{
    matice mat1, mat2, mat3, mat4, mat5, mat6 ;
    
    mat1 = jednotkova(3, 4);

    nastav_prvek(mat1, 0, 1, 2);
    nastav_prvek(mat1, 0, 2, 3);
    nastav_prvek(mat1, 0, 3, 4);
    nastav_prvek(mat1, 1, 0, 5);
    nastav_prvek(mat1, 2, 3, 1);
    
    printf("Matice mat1 :\n");
    vypis(mat1);
    printf("\n\n");

    mat2 = transpozice(mat1);

    printf("Transpozice matice mat1 :\n ");
    vypis(mat2);
    printf ("\n\n");

    mat3 = plus(mat1, mat1);

    printf ("Soucet matic mat1 + mat1 :\n");
    vypis (mat3);
    printf ("\n\n");

    mat4 = nasobeni(mat1, 3);

    printf ("Matice mat1 vynasobena 3:\n ");
    vypis (mat4);
    printf ("\n\n");

    mat5 = krat(mat1, mat2);

    printf ("Matice mat1 vynasobena transponovanou matici mat1 :\n");

    switch(chyba)
    {
        case CHYBA_TYPU:
            printf("S maticemi neni mozne provest nasobeni.");
            break;

        case CHYBA_ALOKACE:
            printf("Nastala chyba alokace pameti.");
            break;

        case BEZ_CHYBY:
            vypis(mat5);
            break;
    }

    printf("\n\n");

    mat6 = krat(mat1, mat1);

    printf("Matice mat1 vynasobena matici mat1 :\n");
    
    switch(chyba)
    {
        case CHYBA_TYPU:
            printf ("S maticemi neni mozne provest nasobeni.");
            break;

        case CHYBA_ALOKACE :
            printf("Nastala chyba alokace pameti.");
            break;

        case BEZ_CHYBY:
            vypis(mat6);
            break;
    }
    printf ( " \n \n " );

    odstran ( mat1 );
    odstran ( mat2 );
    odstran ( mat3 );
    odstran ( mat4 );
    odstran ( mat5 );
    odstran ( mat6 );

    return 0;
}
