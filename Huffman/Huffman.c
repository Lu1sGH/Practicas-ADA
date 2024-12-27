#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

    /*
    cpy() --solo use esta parte para probar algo lkdkajsd
    char *ori = "ola.jpg";
    char *dest = "ola2.jpg";

    FILE *origen = abrir(ori, 1);
    FILE *destino = abrir(dest, 0);

    cpy(origen, destino);

    return 0;

    --EJEMPLO DE LECTURA DE dat
    arcO = abrir(nDest, 1);
    char *bits;
    extraerBits(arcO, &bits, &tam);
    for(int z = 0; z < tam; z++){
        printf("%d", bits[z]);
    }
    */

//============================ARBOL V.Mini============================

typedef struct nodo{
    int n;
    int tRec;
    char c;
    char *rec;
	struct nodo *enlaceDer;
	struct nodo *enlaceIzq;
}t_nodo, *Nodo;

int crearNodo(Nodo *);
int destruirNodo(Nodo *);
int escribirElemento(Nodo, int, char);

typedef struct arbol{
    Nodo raiz;
    int cantidad;
} t_arbol, *Arbol;

int crearArbol(Arbol *);
void destruirArbol(Arbol *);
void destruirNodosRecursivamente(Nodo);

void recBin(Nodo, Nodo *, char *, int *, int);

//====================================================================

FILE *abrir(char *, int);
void *cpy(FILE *, FILE *);
void destArrNodos(Nodo **, int);
void contC(Nodo **, int *, FILE *);
void quick(Nodo *, int, int);
void swap(Nodo *a, int i, int j);
void arbolHuffman(Arbol *, Nodo *, int);
void codigoHuffman(Arbol, Nodo *, int);
void mins(Nodo *, int *, int *, int);
void extraerBits(FILE *, char **, int *);
void codificar(char *, char *, char *);
void datFreq(Nodo *arr, int tam, FILE *, FILE *, FILE *);

int main(){
    char *origen = "ola.txt";
    char *destino = "Codificacion.dat";
    char *tablaFrecuencia = "Frecuencias.txt";

    codificar(origen, destino, tablaFrecuencia);
    return 0;
}

void codificar(char *nOrg, char *nDest, char *nFreq){
    //Opciones para acceder a un archivo con abrir(nomArch, tipo_de_lectura);
    //  1. rb
    //  2. wb
    //  3. w
    //  4. r
    struct timeval i_C, f_C, i_H, f_H;
    double t_C, t_H;
    gettimeofday(&i_C, NULL);

    int tam;
    Nodo *arr;
    Arbol aH;

    FILE *arcO = abrir(nOrg, 1);
    FILE *arcD = abrir(nDest, 2);
    FILE *arcTF = abrir(nFreq, 3);

    if(arcO == NULL && arcD == NULL && arcTF == NULL){
        printf("(!) No se pudo iniciar con la codificacion. \n");
        if(arcO != NULL) fclose(arcO);
        if(arcD != NULL) fclose(arcD);
        if(arcTF != NULL) fclose(arcTF);
        return;
    }

    contC(&arr, &tam, arcO);
    if(arr == NULL || tam == 1){
        printf("(!) No se pudo continuar con la codificacion. \nDir. Arreglo = %X\nTamanio del arreglo = %d", arr, tam);
        fclose(arcO);
        fclose(arcD);
        fclose(arcTF);
        return;
    }

    quick(arr, 0, tam-1);

    gettimeofday(&i_H, NULL);
    arbolHuffman(&aH, arr, tam);
    if(aH == NULL){
        printf("(!) El arbol de Huffman no se pudo crear. \n");
        destArrNodos(&arr, tam);
        return;
    }

    codigoHuffman(aH, arr, tam);
    gettimeofday(&f_H, NULL);

    datFreq(arr, tam, arcO, arcD, arcTF);

    destruirArbol(&aH);
    fclose(arcO);
    fclose(arcD);
    fclose(arcTF);
    free(arr);

    gettimeofday(&f_C, NULL);

    t_C = (f_C.tv_sec - i_C.tv_sec) + (f_C.tv_usec - i_C.tv_usec) / 1000000.0;
    t_H = (f_H.tv_sec - i_H.tv_sec) + (f_H.tv_usec - i_H.tv_usec) / 1000000.0;
    printf("Tiempo total de codificacion: %f\nTiempo total del algoritmo: %f\n", t_C, t_H);
}

void extraerBits(FILE *arc, char **bits, int *totalBits){
    unsigned char byte;
    int bitIndex = 0;
    int capacidad = 8;

    *bits = (char *) malloc(capacidad * sizeof(char));
    if(*bits == NULL){
        printf("(!) Ocurrio un error al asignar memoria para lectura de bits.\n");
        return;
    }

    while(fread(&byte, sizeof(unsigned char), 1, arc) == 1){
        for(int i = 0; i < 8; i++){
            if(bitIndex >= capacidad){
                capacidad *= 2;
                *bits = (char *) realloc(*bits, capacidad * sizeof(char));
                if (*bits == NULL) {
                    printf("(!) Error al leer los bits.\n");
                    return;
                }
            }
            (*bits)[bitIndex++] = (byte & (1 << i)) ? 1 : 0;
        }
    }

    fseek(arc, -1, SEEK_END);
    fread(&byte, sizeof(unsigned char), 1, arc);
    printf("Bits basura: %d\n", byte);

    *totalBits = bitIndex-8-byte;
}

void datFreq(Nodo *arr, int tam, FILE *org, FILE *dest, FILE *freq){
    int c, j = 0, k = 0, cont = 0;
    int ascii[256] = {0};

    for(j = 0; j < tam; j++){
        fprintf(freq, "%d\t%c\t%d\t", arr[j]->c, arr[j]->c, arr[j]->n);
        for(int l = 0; l < arr[j]->tRec; l++)
            fprintf(freq, "%d", arr[j]->rec[l]);
        fprintf(freq, "\n");

        ascii[(unsigned char)(arr[j]->c)] = j;
    }

    unsigned char b = 0;
    int bitPos = 0;

    while((c = fgetc(org)) != EOF){
        k = ascii[(unsigned char) c];
        for(int l = 0; l < arr[k]->tRec; l++){
            if(arr[k]->rec[l] == 1){
                b |= (1 << bitPos);
            }
            bitPos++;

            if(bitPos == 8){
                fwrite(&b, sizeof(unsigned char), 1, dest);
                b = 0;
                bitPos = 0;
            }
        }
    }

    if(bitPos > 0){
        fwrite(&b, sizeof(unsigned char), 1, dest);
    }
    b = bitPos;
    fwrite(&b, sizeof(unsigned char), 1, dest);
}

void codigoHuffman(Arbol aH, Nodo *arr, int tam){
    char *rec = (char *) malloc((aH->cantidad)*sizeof(char));
    int i = 0;

    recBin(aH->raiz, arr, rec, &i, 0);
}

void arbolHuffman(Arbol *a, Nodo *arr, int tam){
    Nodo aux;
    Arbol aH;
    int i, j, k, nDis = tam, cNodos = tam;

    crearArbol(&aH);

    while(nDis > 1){
        mins(arr, &i, &j, tam);
        crearNodo(&aux);
        escribirElemento(aux, arr[i]->n + arr[j]->n, 0);
        aux->enlaceIzq = arr[i];
        aux->enlaceDer = arr[j];

        arr[i] = NULL;
        arr[j] = NULL;

        for(k = 0; k<tam; k++){
            if(arr[k] == NULL){
                arr[k] = aux;
                k = tam;
            }
        }

        nDis--, cNodos++;
    }

    aH->raiz = arr[0];
    aH->cantidad = cNodos;
    *a = aH;

    arr[0] = NULL;
}

void mins(Nodo *arr, int *a, int *b, int tam) {
    int min1 = 0, min2 = 0;

    for(int i = 1; i < tam; i++){
        if(arr[i] != NULL){
            if(arr[i]->n < arr[min1]->n){
                min1 = i;
            }else if(arr[i]->n < arr[min2]->n || min1 == min2){
                min2 = i;
            }
        }
    }

    *a = min1;
    *b = min2;
}

void quick(Nodo *a, int m, int n){
    if((n >= 0) && (m <= n)){
        int piv, i, j;

        piv = n;
        j = m-1;

        for(i = m; i <= n; i++){
            if(a[i]->n <= a[piv]->n){
                j++;
                if(i>j){
                    swap(a, i, j);
                }
            }
        }

        quick(a, m, j-1);
        quick(a, j+1, n);

    }
}

void swap(Nodo *a, int i, int j){
    Nodo t;
    t = a[i];
    a[i] = a[j];
    a[j] = t;
}

void contC(Nodo **a, int *tam, FILE *arc){
    int c, i, j = 0, pos = 0, cCont = 0, sw = 0; *tam = 1;
    Nodo *arrA;
    Nodo aux;
    int ascii[256] = {0};

    while((c = fgetc(arc)) != EOF){
        if(((unsigned char)c) > 255) printf("(!) Se ha encontrado un char fuera del rango ASCII.\n%d %c %d \n", c, c, cCont);

        if(ascii[(unsigned char)c] == 0){
            cCont++;
        }
        ascii[(unsigned char)c]++;
    }

    Nodo *arr = (Nodo *) malloc(cCont*sizeof(t_nodo));
    if(arr == NULL){
        printf("(!) Algo salio mal al intentar crear el arreglo de nodos. \n");
        return;
    }

    for(i = 0; i < 256; i++){
        if(ascii[i] != 0){
            sw = crearNodo(&aux);
            if(!sw){
                printf("(!) Ha habido un problema al crear un nodo. \nChar: %d, %c\nID: %d\n", i, i, j);
                arr[j++] = NULL;
            }else{
                escribirElemento(aux, ascii[i], i);
                arr[j++] = aux;
            }
        }
    }

    *tam = cCont;
    *a = arr;
    rewind(arc);
}

void destArrNodos(Nodo **arr, int tam){
    Nodo *aux = *arr;
    for(int i=0; i<tam; i++){
        destruirNodo(aux+i);
    }
    free(aux);
    *arr = NULL;
}

FILE *abrir(char *nom, int t){
    char *tipo;

    if(t == 1){
        tipo = "rb";
    }else if(t == 2){
        tipo = "wb";
    }else if(t == 3){
        tipo = "w";
    }else{
        tipo = "r";
    }

    FILE *arch = fopen(nom, tipo);
    if(!arch){
        printf("(!) El archivo %s no se pudo abrir. \n", nom);
        return NULL;
    }
    return arch;
}

void *cpy(FILE *origen, FILE *destino){
    char b[4096];
    size_t tR;
    while (tR = fread(b, 1, sizeof(b), origen)) {
        fwrite(b, 1, tR, destino);
    }
}

//============================ARBOL V.Mini============================

int crearNodo(Nodo *n){
	Nodo m = NULL;

	m = (Nodo) malloc(sizeof(t_nodo));

	if(m == NULL)
		return 0;

	m->enlaceDer = NULL;
	m->n = 0;
	m->tRec = 0;
	m->c = 0;
	m->rec = NULL;
	m->enlaceIzq = NULL;

	*n = m;

	return 1;
}

int destruirNodo(Nodo *n){
	Nodo aux = NULL;

	if(*n == NULL)
		return 0;

	aux = *n;

	aux->n = 0;
	aux->tRec = 0;
    aux->c = 0;
    if(aux->rec != NULL) free(aux->rec);
    aux->rec = NULL;
	aux->enlaceDer = NULL;
	aux->enlaceIzq = NULL;

	free(aux);

	*n = NULL;

	return 1;
}

int escribirElemento(Nodo nodo, int n, char c){
	if(nodo == NULL)
	  return 0;

	nodo->n = n;
	nodo->c = c;

	return 1;
}

int crearArbol(Arbol *b){
	Arbol a = NULL;

	a = (Arbol) malloc(sizeof(t_arbol));

	if(a != NULL){
	  	a->cantidad =0;
	  	a->raiz = NULL;
	  	*b = a;
	  	return 1;
	}

	*b = NULL;

	return 0;
}

void destruirArbol(Arbol *b){
	Arbol a = NULL;
	int sw = 0;

	a = *b;

	if(a == NULL)
		return;

	if(a->raiz != NULL)
		destruirNodosRecursivamente(a->raiz);


	if(a->raiz != NULL)
		destruirNodo(&(a->raiz));

	a->raiz = NULL;
	a->cantidad = 0;

	free(a);
	*b = NULL;
}

void destruirNodosRecursivamente(Nodo central){
	Nodo izq = NULL, der = NULL;

	if(central == NULL)
		return;

	izq = central->enlaceIzq;
	der = central->enlaceDer;

	if(izq != NULL){
	    central->enlaceIzq = NULL;
	    destruirNodosRecursivamente(izq);
	}

	if(der != NULL){
	    central->enlaceDer = NULL;
	    destruirNodosRecursivamente(der);
	}

	if(izq == NULL && der == NULL)
		destruirNodo(&central);
}

void recBin(Nodo padre, Nodo *a, char *rec, int *i, int j){
    if((padre->enlaceIzq == NULL) && (padre->enlaceDer == NULL)){
        padre->rec = (char *) malloc((j)*sizeof(char));
        padre->tRec = j;
        for(int k = 0; k < j; k++)
            (padre->rec)[k] = rec[k];
        a[(*i)++] = padre;
        return;
    }

    rec[j] = 0;
    recBin(padre->enlaceIzq, a, rec, i, j+1);
    rec[j] = 1;
    recBin(padre->enlaceDer, a, rec, i, j+1);
}

//====================================================================
