#include <stdio.h>
#include <stdlib.h>

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

typedef struct nodo
{
	int elem;
	struct nodo *enlaceDer;
	struct nodo *enlaceIzq;
}t_nodo, *Nodo;

int crearNodo(Nodo *);
int destruirNodo(Nodo *);
int escribirElemento(Nodo, int);
int leerElemento(Nodo, int *);

typedef struct arbol
{
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

	rFile(&a, &n);
	if(a == NULL) return 1;
	printf("Numero de nums: %d \n", n);
	impA(a, n);

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
            bubble(a, n);
            break;
        case 1:
            insertion(a, n);
            break;
        case 2:
            selection(a, n);
            break;
        case 3:
            shell(a, n);
            break;
        case 4:
            tree(a, n);
            break;
        case 5:
            quick(a, 0, n-1);
            break;
        default:
            puts("Opcion no valida");
    }

    if(opt < 6) impA(a, n);

	free(a);
	return 0;
}

void impA(int *a, int m){
    int i;
    for(i = 0; i < m; i++){
        printf("%d ", *(a+i));
    }
    puts("\n");
}

void rFile(int **a, int *n){
    FILE *file = fopen("Nums.txt", "r");
    int j, t = 0;
    char c;

    while((c=fgetc(file)) != EOF){
	if(c == 10) (*n)++;
    }
    (*n)++;
    rewind(file);

    *a = (int *)malloc((*n)*sizeof(int));
    if(*a == NULL){puts("Memoria insuficiente"); return;}

    for(j=0; j < (*n); j++){
        fscanf(file, "%d", &t);
        (*a)[j] = t;
    }

    fclose(file);
}

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

void quick(int *a, int m, int n){
    if(m<n){
        int piv;

        piv = pivot(a, m, n);

        quick(a, m, piv-1);
        quick(a, piv+1, n);
    }
}

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

void swap(int *a, int i, int j){
    int t;
	t = a[i];
    a[i] = a[j];
    a[j] = t;
}

//============================ARBOL V.Mini============================

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

int escribirElemento(Nodo n, int e){
	if(n == NULL)
	  return 0;

	n->elem = e;

	return 1;
}

int leerElemento(Nodo n, int *e){
	if(n == NULL)
	  return 0;

	*e = n->elem;

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

int recorrerArbolEnOrden(Arbol a, int *arr){
	if(a == NULL)
		return 0;

	if((a->raiz) == NULL)
		return 0;

	int i = 0;
	recorrerNodosEnOrden(a->raiz, arr, &i);

	return 1;
}

void recorrerNodosEnOrden(Nodo central, int *a, int *i){
	int dato=0;

	if(central == NULL)
		return;

	recorrerNodosEnOrden(central->enlaceIzq, a, i);

	a[(*i)++] = central->elem;

	recorrerNodosEnOrden(central->enlaceDer, a, i);
}

//====================================================================
