/*
Título: Algoritmos de ordenamiento
Descripción: Programa que codifica y decodifica un archivo usando el algoritmo de Huffman.
Fecha: 12/01/2025
U. modificación: -
Versión: 1
Autor: Araujo, K. Martinez, A. & Zarate, L.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

//============================ARBOL V.Mini============================

typedef struct nodo{
    int n; //Frecuencia del caracter
    int tRec; //Tamaño del codigo de Huffman
    char c; //Caracter
    char *rec; //Codigo de Huffman
	struct nodo *enlaceDer;
	struct nodo *enlaceIzq;
}t_nodo, *Nodo;

int crearNodo(Nodo *); //Crea un nodo y lo inicializa
int destruirNodo(Nodo *); //Destruye un nodo
int escribirElemento(Nodo, int, char); //Escribe la frecuencia y el caracter en un nodo

typedef struct arbol{
    Nodo raiz; //Raiz del arbol
    int cantidad; //Numero de nodos
} t_arbol, *Arbol;

int crearArbol(Arbol *); //Crea un arbol y lo inicializa
void destruirArbol(Arbol *); //Destruye un arbol y sus nodos
void destruirNodosRecursivamente(Nodo); //Destruye nodos recursivamente

void recBin(Nodo, Nodo *, char *, int *, int); //Recorre todos los nodos del arbol

//====================================================================

FILE *abrir(char *, int); //Abre un archivo
void destArrNodos(Nodo **, int); //Destruye un arreglo de nodos
void contC(Nodo **, int *, FILE *); //Carga los caracteres que hay en un archivo y cuenta su frecuencia
void quick(Nodo *, int, int); //Quick Sort para ordenar los caracteres por su frecuencia
void swap(Nodo *a, int i, int j); //Funcion auxiliar de quick
void arbolHuffman(Arbol *, Nodo *, int); //Construye el arbol de Huffman a partir de la lista de nodos
void codigoHuffman(Arbol, Nodo *, int); //Crea el codigo de Huffman recorriendo el arbol
void mins(Nodo *, int *, int *, int); //Encuentra el nodo con la minima frecuencia en un arr de Nodos
void extraerBits(FILE *, char **, int *); //Extrae los bits de un .dat
void codificar(char *, char *, char *); //Funcion que agrupa todas las funciones para codificar
void datFreq(Nodo *arr, int tam, FILE *, FILE *, FILE *); //Escribe el archivo .dat y el .txt con la tabla de freq
void decodificar(char *, char *, char *); //Funcion que agrupa todas las funciones para decodificar
void obtenerFreq(Nodo **, int *,FILE *); //Obtiene la informacion del .dat y de la tabla de freq
void copiaCadena(char codigo[256],int,char *); //Copia una cadena
void capConsola(char [100]); //Captura un str de la consola

int main(){
    char origen[100] = {0};
    char destino[100] = {0};
    char *codigo = "Codificacion.dat";
    char *tablaFrecuencia = "Frecuencias.txt";
    int op = 0;

    printf("%cQu%c desea hacer?\n 1. Codificar un archivo.\n 2. Decodificar un archivo.\n 3. Las dos opciones anteriores.\n", 168, 130);
    scanf("%d", &op);
    while (getchar() != '\n');

    switch(op){
        case 1:
            printf("Escriba el nombre del archivo de entrada y su extension.\nEj: ola.txt\n");
            capConsola(origen);

            codificar(origen, codigo, tablaFrecuencia);
            break;
        case 2:
            printf("Escriba el nombre del archivo de salida y su extension.\nEj: ola.txt\n");
            capConsola(destino);

            decodificar(codigo, tablaFrecuencia, destino);
            break;
        case 3:
            printf("Escriba el nombre del archivo de entrada y su extension.\nEj: entrada.txt\n");
            capConsola(origen);
            printf("Escriba el nombre del archivo de salida y su extension.\nEj: salida.txt\n");
            capConsola(destino);

            if(strcmp(origen, destino)){
                codificar(origen, codigo, tablaFrecuencia);
                decodificar(codigo, tablaFrecuencia, destino);
            }else{
                puts("El nombre de la entrada no puede ser igual que el de la salida.");
            }
            break;
        default:
            puts("No existe la opcion.");
    }
    return 0;
}

void capConsola(char arr[100]){
    if (fgets(arr, 100, stdin) != NULL) {
        size_t len = strlen(arr);
        if (len > 0 && arr[len - 1] == '\n') {
            arr[len - 1] = '\0';
        }
    } else {
        printf("Error al leer la entrada.\n");
    }
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
    printf("Tiempo total de codificacion: %f\nTiempo total del algoritmo de Huffman: %f\n", t_C, t_H);
}

void decodificar(char *bits, char *frecuencias, char *salida){
    Nodo *arr; //arreglo de Nodos con el codigo y caracter ascii al que corresponde
    char *cadenaBits; //el arreglo en que se obtienen los bits
    int nBits, size, tamNodo, nActual=0,conde=0, binario, j=0; //nBits es la cantidad de bits en la cadena, size es el tamaño que deberá tener el arreglo de Nodos
    FILE *aBits = abrir(bits, 1); //se abre el archivo .dat con los bits
    FILE *aFrecuencias = abrir(frecuencias, 4); //se abre el .txt que contiene las frecuencias
    FILE *aSalida = abrir(salida, 2); //se abre el archivo en que se almacenara la decodificacion
    struct timeval i_C, f_C, i_H, f_H;
    double t_C, t_H;

    gettimeofday(&i_C, NULL);

    extraerBits(aBits, &cadenaBits, &nBits );

    //alocando un arreglo en que se guardan los nodos con los caracteres y codigos
    arr = (Nodo *)malloc(256*sizeof(t_nodo));
    obtenerFreq(&arr, &size,aFrecuencias);

    gettimeofday(&i_H, NULL);
    for(int i=0; i<nBits; i++){
        tamNodo= arr[nActual]->tRec;

        //printf("%c", arr[nActual]->rec[conde]);

        if(arr[nActual]->rec[conde]==48){
            binario = 0;
        } else{
            binario =1;
        }

        if(cadenaBits[i]==binario){ //comprueba que los bits coincidan
            if((conde+1) == tamNodo){
                fprintf(aSalida, "%c", arr[nActual]->c);

                if(i==nBits-1) i = nBits + 1; //otra condicion para evitar errores
                nActual=0;
                conde=0;
            }
            else{
                conde++;
            }
         } else{
            nActual++;
            i-=(conde+1);
            conde=0;
        }
    }

    gettimeofday(&f_H, NULL);

    destArrNodos(&arr, size);
    fclose(aBits);
    fclose(aFrecuencias);
    fclose(aSalida);

    gettimeofday(&f_C, NULL);
    t_C = (f_C.tv_sec - i_C.tv_sec) + (f_C.tv_usec - i_C.tv_usec) / 1000000.0;
    t_H = (f_H.tv_sec - i_H.tv_sec) + (f_H.tv_usec - i_H.tv_usec) / 1000000.0;
    printf("Tiempo total de decodificacion: %f\nTiempo total de Huffman't: %f\n", t_C, t_H);
}

//============================Codificacion============================

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
    *totalBits = bitIndex-8-byte;
}

void datFreq(Nodo *arr, int tam, FILE *org, FILE *dest, FILE *freq){
    int c, j = 0, k = 0, cont = 0;
    int ascii[256] = {0};

    for(j = 0; j < tam; j++){
        fprintf(freq, "%d\t", arr[j]->c);
        for(int l = 0; l < arr[j]->tRec; l++)
            fprintf(freq, "%d", arr[j]->rec[l]);
        fprintf(freq, "\t");
        if(((unsigned char) arr[j]->c) < 32 || 126 < ((unsigned char) arr[j]->c)){
            fprintf(freq, "%d\t%s\n", arr[j]->n, "[No_imprimible]");
        }else if(arr[j]->c == 32){
            fprintf(freq, "%d\t%s\n", arr[j]->n, "[Espacio]");
        }else{
            fprintf(freq, "%d\t%c\n", arr[j]->n, arr[j]->c);
        }

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

//===========================Decodificacion===========================

void obtenerFreq(Nodo **arr, int *size, FILE *fuente){
    if(fuente==NULL){
        printf("No se proporcionó un archivo o está vacío");
        return;
    }

    if(arr==NULL){
        printf("(!) Hubo un problema con el arreglo de nodos");
        return;
    }

    int i, ascii, frecuencia, control=1, count=0, tam;
    char caracter[20], codigo[256];
    char *auxCad;
    Nodo *arrAux;
    Nodo aux;

    while(control !=0){
        if(fscanf(fuente, "%d\t%s\t%d\t%s", &ascii, &codigo, &frecuencia, &caracter)>=3){
            tam = strlen(codigo);

            if(crearNodo(&aux)){
                aux->rec = (char *) malloc(tam*sizeof(char));

                copiaCadena(codigo, tam, aux->rec);

                aux->tRec = tam;
                aux->c = ascii;
                aux->n = frecuencia;

                (*arr)[count] = aux;
            }
            count++;

            for(i=0; i<256; i++){
                codigo[i]=0;
            }
        }
        else{
            control=0;
        }
    }

    arrAux = (Nodo *)realloc(*arr, count*sizeof(t_nodo));
    *arr = arrAux;
    *size = count;
}

void copiaCadena(char codigo[256], int n, char *aux){
    int i;

    for(i=0; i<n; i++){
        aux[i] = codigo[i];
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
