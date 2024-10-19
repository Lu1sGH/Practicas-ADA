/*
Título: Algoritmos de ordenamiento
Descripción: Programa que recibe un archivo con números, a los que se les aplica
un algoritmo de ordenamiento.
Fecha: 30/09/2024
U. modificación: 14/10/2024
Versión: 2
Autor: Araujo, K. Martinez, A. & Zarate, L.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

//Declaracion de funciones que se utilizaran.
void impA(int *, int);
void rFile(int **, int *);

void bubble(int *, int);
void insertion(int *, int);
void selection(int *, int);
void shell(int *, int);
void tree(int *, int);
void quick(int *, int, int);

int pivot(int *, int, int);
void swap(int *, int, int);

//============================ARBOL V.Mini============================

typedef struct nodo{
    int elem;
	struct nodo *enlaceDer;
	struct nodo *enlaceIzq;
}t_nodo, *Nodo;

int crearNodo(Nodo *);
int destruirNodo(Nodo *);
int escribirElemento(Nodo, int);
int leerElemento(Nodo, int *);

typedef struct arbol{
    Nodo raiz;
    int cantidad;
} t_arbol, *Arbol;

int crearArbol(Arbol *);
void destruirArbol(Arbol *);
void destruirNodosRecursivamente(Nodo);

int insertarElemento(Arbol, int);
Nodo insertarElementoRecursivo(Nodo, int);

int recorrerArbolEnOrden(Arbol, int *);
void recorrerNodosEnOrden(Nodo, int *, int *);

//====================================================================

int main(){
	int *a, n = 0, opt = 6;
	clock_t i_cpu, f_cpu;
	struct timeval i_real, f_real;
	double t_cpu, t_real;

	/*Declaracion del largo del arreglo, se lee el archivo y se guarda en el *a*/
	n = 500000;
	rFile(&a, &n);
	if(a == NULL) return 1;
	printf("Numero de nums: %d \n", n);

	/*Imprime un menu para seleccionar el metodo de ordenamiento*/
	puts("Elija un metodo de ordenamiento:");
	puts("0. Burbuja.");
	puts("1. Insercion.");
	puts("2. Seleccion.");
	puts("3. Shell.");
	puts("4. Tree sort.");
	puts("5. Quick Sort");
	scanf("%d", &opt);

	switch (opt) {
        case 0:
            i_cpu = clock();
            gettimeofday(&i_real, NULL);

            bubble(a, n);

            f_cpu = clock();
            gettimeofday(&f_real, NULL);
            break;
        case 1:
            i_cpu = clock();
            gettimeofday(&i_real, NULL);

            insertion(a, n);

            f_cpu = clock();
            gettimeofday(&f_real, NULL);
            break;
        case 2:
            i_cpu = clock();
            gettimeofday(&i_real, NULL);

            selection(a, n);

            f_cpu = clock();
            gettimeofday(&f_real, NULL);
            break;
        case 3:
            i_cpu = clock();
            gettimeofday(&i_real, NULL);

            shell(a, n);

            f_cpu = clock();
            gettimeofday(&f_real, NULL);
            break;
        case 4:
            i_cpu = clock();
            gettimeofday(&i_real, NULL);

            tree(a, n);

            f_cpu = clock();
            gettimeofday(&f_real, NULL);
            break;
        case 5:
            i_cpu = clock();
            gettimeofday(&i_real, NULL);

            quick(a, 0, n-1);

            f_cpu = clock();
            gettimeofday(&f_real, NULL);
            break;
        default:
            puts("Opcion no valida");
    }
    /*Recoje el tiempo que tarda la cpu en ejecutarse y el tiempo real*/
    t_cpu = ((double) (f_cpu-i_cpu))/CLOCKS_PER_SEC;
    t_real = (f_real.tv_sec - i_real.tv_sec) + (f_real.tv_usec - i_real.tv_usec) / 1000000.0;

	//Despliega los tiempos de la CPU y Real.
    printf("CPU (Algoritmo): %f s.\n", t_cpu);
    printf("Real (Algoritmo): %f s.\n", t_real);

	free(a);
	return 0;
}

//Recibe un apuntador de enteros y la longitud de dicho arreglo para desplegarlo en pantalla.
void impA(int *a, int m){
    int i;
    for(i = 0; i < m; i++){
        printf("%d ", *(a+i));
    }
    puts("\n");
}

/* Recibe un apuntador sencillo de enteros y un doble apuntador, guarda los elementos que se 
encuentren en "numeros10millones.txt" y los guarda en  el doble apuntador. */
void rFile(int **a, int *n){
    FILE *file = fopen("numeros10millones.txt", "r");
    int j, t = 0;

    *a = (int *)malloc((*n)*sizeof(int));
    if(*a == NULL){puts("Memoria insuficiente"); return;}

    for(j=0; j < (*n); j++){
        fscanf(file, "%d", &t);
        (*a)[j] = t;
    }

    fclose(file);
}


/* Metodo de ordenamiento, recibe un arreglo dinamico y el tamaño de dicho arreglo.
metodo que va comparando dos de los elementos del arreglo, si el izquierdo es mayor que el
derecho los intercambia de lugar.*/
void bubble(int *a, int n){
    int i, j, t;
    for(i = 1; i < n; i++){
        for(j = 0; j < n-i; j++){
            if(a[j] > a[j+1]){
				t = a[j];
				a[j] = a[j+1];
				a[j+1] = t;
            }
        }
    }
}

/* Metodo de ordenamiento, recibe un arreglo dinamico y el tamaño de dicho arreglo.
metodo que recorre todo el arreglo y acomoda cada elemento, dependiendo de los ya ordenados
desplaza a la derecha de ser necesario*/
void insertion(int *a, int n) {
    int i, j, temp;
    for (i = 0; i < n; i++) {
        temp = a[i];
        j = i;
        while (j > 0 && temp < a[j - 1]) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = temp;
    }
}


/* Metodo de ordenamiento, recibe un arreglo dinamico y el tamaño de dicho arreglo.
metodo que recorre todo el arreglo hasta encontrar el mas pequeño y lo intercambia con la primera posicion
luego */
void selection(int *a, int n){
    int i, j, p, t;
    for(i = 0; i < n-1; i++){
        p = i;
        for(j = i+1; j < n; j++){
            if(a[p] > a[j]){
               p = j;
            }
        }
        t = a[i];
        a[i] = a[p];
        a[p] = t;
    }
}

/* Metodo de ordenamiento, recibe un arreglo dinamico y el tamaño de dicho arreglo.
metodo que funciona similar al selection, sin embargo, primero define una distancia en la que aplica
el metodo de ordenamiento para despues pasar con distancias mas cortos*/
void shell(int *a, int s){
    int gap, i;
    for (gap = s / 2; gap > 0; gap /= 2) {
        for ( i = gap; i < s; i += 1) {
            int temp = a[i];
            int j;
            for (j = i; j >= gap && a[j - gap] > temp; j -= gap) {
                a[j] = a[j - gap];
            }
            a[j] = temp;
        }
    }
}

/* Metodo de ordenamiento, recibe un arreglo dinamico y el tamaño de dicho arreglo.
metodo que inserta todos los elementos de la lista en un árbol binario de búsqueda, 
luego organiza los elementos de manera ordenada al seguir las reglas del árbol.*/
void tree(int *a, int n){
	int i;
	Arbol arbolito = NULL;
	crearArbol(&arbolito);

	if(arbolito == NULL) return;

	for(i = 0; i < n; i++){
		insertarElemento(arbolito, a[i]);
	}

	recorrerArbolEnOrden(arbolito, a);

	destruirArbol(&arbolito);
}

/* Metodo de ordenamiento, recibe un arreglo dinamico y el tamaño de dicho arreglo.
metodo que divide el arreglo de dos de manera recursiva hasta tener el intervalo mas pequeño
e ir organizando desde los intervalos mas pequeños hasta los mas grandes, todo esto tomando
desde un principio un pivote*/
void quick(int *a, int m, int n){
    if(m<n){
        int piv;

        piv = pivot(a, m, n);

        quick(a, m, piv-1);
        quick(a, piv+1, n);
    }
}


/*Define la posicion del pivote, al recibir un arreglo dinamico, la primera y ultima posicion
desde la que se busca encontrar el pivote */
int pivot(int *a, int m, int n){
	int p = a[n], i, j = m-1;

	for(i = m; i <= n; i++){
		if(a[i] <= p){
			j++;
			if(i>j){
                swap(a, i, j);
			}
		}
	}

	return j;
}


/*Recibe un arreglo dinamico y dos posiciones, de las cuales intercambia los elementos que
se encuentren en respectivas posiciones*/
void swap(int *a, int i, int j){
    int t;
    t = a[i];
    a[i] = a[j];
    a[j] = t;
}

//============================ARBOL V.Mini============================


/*Crea un nodo declarando sus variables como nulas.*/
int crearNodo(Nodo *n){
	Nodo m = NULL;

	m = (Nodo) malloc(sizeof(t_nodo));

	if(m == NULL)
		return 0;

	m->enlaceDer = NULL;
	m->elem = 0;
	m->enlaceIzq = NULL;

	*n = m;

	return 1;
}

/*Libera la memoria del nodo y posteriormente lo borra.*/
int destruirNodo(Nodo *n){
	Nodo aux = NULL;

	if(*n == NULL)
		return 0;

	aux = *n;

	aux->elem = 0;

	aux->enlaceDer = NULL;
	aux->enlaceIzq = NULL;

	free(aux);

	*n = NULL;

	return 1;
}

/*Recibe un nodo y un elemento, dentro del Nodo modifica
su elemento pr el recibido*/
int escribirElemento(Nodo n, int e){
	if(n == NULL)
	  return 0;

	n->elem = e;

	return 1;
}


/*Recibe un nodo y una direccin de memoria de un elemento
en el proceso la direccion de memoria recibe la del elemento del nodo*/
int leerElemento(Nodo n, int *e){
	if(n == NULL)
	  return 0;

	*e = n->elem;

	return 1;
}

/*Recibe la direccion de memoria de un arbol, a partir de la cual guarda espacio
donde se almacenara el arbol*/
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

/*Recibe la direccion de memoria de un arbol del cual libera el espacio de memoria,
primero liberando la de tdos los nodos del arbol*/
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

/*Recibe un nodo denominad cm central, a partir del cual comienza a liberar las conexiones
de ambos lados del nodo, para posteriormente destruir el nodo central*/
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


/*Recibe un elemento y un arbol, añade el elemento dentro de un nodo del arbol ademas de declararlo
como raiz si es el primer nodo en ser añadido y aumenta la cantidad registrada de nodos que tiene el arbol*/
int insertarElemento(Arbol a, int e){
	Nodo nuevo= NULL;

	if(a == NULL)
		return 0;

	nuevo = insertarElementoRecursivo(a->raiz, e);

	if(a->raiz == NULL)
		a->raiz = nuevo;

	if(nuevo != NULL)
		a->cantidad++;

	return 1;
}

/*Recibe un nodo central y un entero, si el entero es un numero menor a la cantidad de elementos entonces 
se llama revursivamente para hacer un enlace por la izquierda, de ser mayor lo hace por la derecha*/
Nodo insertarElementoRecursivo(Nodo central, int e){
	Nodo nuevo = NULL;
	Nodo izq = NULL, der = NULL;

	if(central == NULL){
	  	crearNodo(&nuevo);
	  	if(nuevo != NULL)
			nuevo->elem = e;
	  	return nuevo;
	}

	izq = central->enlaceIzq;
	der = central->enlaceDer;

	if(e < (central->elem)){
	  	nuevo = insertarElementoRecursivo(izq, e);
	  	if (izq == NULL)
			central->enlaceIzq = nuevo;
	}else{
	  	nuevo = insertarElementoRecursivo(der, e);
	  	if (der == NULL)
			central->enlaceDer = nuevo;
	}

	return nuevo;
}

/*Recibe un arbol y la direccion de memoria de un entero, llama a otra funcion para comenzar
a recorrer los nodos a partir desde la raiz, enviando la ultima y primera posicion de las que se
quiere recorrer el arbol*/
int recorrerArbolEnOrden(Arbol a, int *arr){
	if(a == NULL)
		return 0;

	if((a->raiz) == NULL)
		return 0;

	int i = 0;
	recorrerNodosEnOrden(a->raiz, arr, &i);

	return 1;
}

/*Recibe un nodo y dos direcciones de memoria, se llama a si misma para recorrer los nodos por la izquierda
y por la derecha del nodo que recibio.*/
void recorrerNodosEnOrden(Nodo central, int *a, int *i){
	int dato=0;

	if(central == NULL)
		return;

	recorrerNodosEnOrden(central->enlaceIzq, a, i);

	a[(*i)++] = central->elem;

	recorrerNodosEnOrden(central->enlaceDer, a, i);
}

//====================================================================
