/*
Título: Divide y Venceras
Descripción: Soluciones para los problemas de subarreglo de suma maxima, multiplicacion de matrices y conteo de inversiones
             por metodo directo y la metodologia DyV
Fecha: 26/11/2024
U. modificación: 26/11/2024
Versión: 1
Autor: Araujo, K. Martinez, A. & Zarate, L.
*/

#include <stdio.h>
#include <stdlib.h>

int sel(int **, int ***, int ***, int *);
int sumArr(int *, int, int);
void maxSum(int *, int, int, int *, int *, int *);
void maxSumMD(int *, int, int *, int *, int *);
void imprimeMatriz(int **m, int n);
void prodMatrizRec(int **, int **, int ***, int, int , int, int);
void prodMatriz(int ***, int ***, int n);

int main(){
    int n = 0, i = 0, mayor = 0, ini = 0, fin = 0, *a, **m1, **m2, **c, s;
    s = sel(&a, &m1, &m2, &n);

    if(s == 1){
        puts("========================================================");
        puts("================Subarreglo de suma max==================");
        //maxSumMD(a, n-1, &ini, &fin, &mayor);
        maxSum(a, 0, n-1, &mayor, &ini, &fin);
        printf(" Sum max = %d \n Pos inicial = %d \n Pos final = %d \n", mayor, ini, fin);
        printf("[");
        for(i = ini; i < fin; i++) printf("%d, ", a[i]);
        printf("%d] \n", a[fin]);
        puts("========================================================");
        puts("=================Conteo de inversiones==================");
        printf("\nEl conteo de inversiones es de %d \n", inverDiv(a, n,0,1));

        free(a);
    }else{
        puts("========================================================");
        puts("===============Multiplicacion de matrices===============");
	
	    
	//Creando c (la matriz suma) e inicianizandola en 0
	c = (int **) malloc(n*sizeof(int *));
	for(int x=0; x<n; x++){
		c[x]= (int *) malloc(n*sizeof(int));
		for(int y=0; y<n; y++)
			c[x][y] = 0;
	}
	putchar('\n');
        printf("Producto de matrices iterativo: \n");
        prodMatriz(&m1,&m2, n);
        putchar('\n');

        
	printf("Producto recursivo: \n");
	prodMatrizRec(m1, m2, &c, n, 0, 0, 0);
	imprimeMatriz(c, n);
    }

    return 0;
}

/*Esta funcion crea los arreglos o matrices que se van a utilizar. Recibe 4 apuntadores: el primero corresponde
a un arreglo unidimensional, los siguientes 2 a una matriz de nxn y el ultimo es el tamaño del arreglo o las matrices
*/
int sel(int **a, int ***m1, int ***m2, int *n){
    int s = 0;
    puts("Que programa quiere probar?");
    printf(" 1. Conteo de inversiones y subarreglo de suma maxima. \n 2. Multiplicacion de matrices. \n");
    scanf("%d", &s);
    if(s == 1){
        puts("Ingrese el tamanio del arreglo");
        scanf("%d", n);
        *a = (int *) malloc((*n)*sizeof(int));
        puts("Llene el arreglo con los valores deseados");
        for(int i = 0; i < *n; i++){
            scanf("%d", (*a)+i);
        }
    }else if(s == 2){
	    printf("Ingrese la dimension de las matrices cuadradas: \n");
	    scanf("%d", &n);

	    //Se crea la matriz 1
	    (*m1) = (int **) malloc(n*sizeof(int *)); 
	    for(int x=0; x<n; x++){
		    (*m1)[x]= (int *) malloc(n*sizeof(int));
	    }
	    
	    //Creando la matriz 2
	    (*m2) = (int **) malloc(n*sizeof(int *));
	    for(int x=0; x<n; x++){
		    (*m2)[x]= (int *) malloc(n*sizeof(int));
	    }
	    
	    //pidiendo la informacion de la matriz 1
	    printf("Ingrese los datos de la matriz 1: \n");
	    for(int i=0; i<n; i++){
		    for(int j=0; j<n; j++){
			    printf("m1[%d][%d]= ", i, j);
			    scanf("%d", m1[i][j]);
		    }
	    }
	    //Pidiendo al ususario los datos de la matriz 2
	    printf("Ingrese los datos de la matriz 2: \n");
	    for(int i=0; i<n; i++){
		    for(int j=0; j<n; j++){
			    printf("m2[%d][%d]= ", i, j);
			    scanf("%d", m2[i][j]);
		    }
	    }
    }else{
        puts("Opcion no valida");
        return 0;
    }

    return s;
}

/*Esta funcion busca el subarreglo de suma maxima por metodo directo. Recibe como primer parametro un arreglo como apuntador,
el tamaño del arreglo, un apuntador que corresponde a la suma max y dos apuntadores que corresponden
a los indices del subarreglo que contiene la suma max.
*/
void maxSumMD(int *a, int tam, int *sum, int *ini, int *fin) {
    int maxSum = 0;
    int suma;

    for (int i = 0; i <= tam; i++) {
        suma = 0;
        for (int j = i; j <= tam; j++) {
            suma += a[j];
            if (suma > maxSum) {
                *ini = i;
                *fin = j;
                maxSum = suma;
            }
        }
    }

    *sum = maxSum;
}

/*Esta funcion busca el subarreglo de suma maxima haciendo uso de DyV. Recibe como primer parametro un arreglo como apuntador,
el indice inicial y final del subarreglo, un apuntador que corresponde a la suma max global y dos apuntadores que corresponden
a los indices del subarreglo que contiene la suma max global.
*/
void maxSum(int *a, int m, int n, int *mayG, int *ini, int *fin){
    if(m == n) return;

    int mayL;
    mayL = sumArr(a, m, n);

    if(mayL > *mayG){
        *mayG = mayL;
        *ini = m;
        *fin = n;
    }

    maxSum(a, m, n-1, mayG, ini, fin);
    maxSum(a, m+1, n, mayG, ini, fin);
}

/*Esta funcion suma los elementos de un subarreglo*/
int sumArr(int *arr, int m, int n){
    int sum=0;
    for(int i=m; i<=n; i++){
        sum += arr[i];
    }
    return sum;
}
/*Una función que imprime una matriz almacenada en un doble apuntador*/
void imprimeMatriz(int **m, int n){
    for(int i=0; i<n;i++){
        for(int j=0; j<n; j++){
            printf("\t%d", m[i][j]);
        }
        printf("\n");
    }

}

/*la funcion que calcula el producto de dos matrices mediante divide y vencerás y almacena en el arreglo c proporcionado*/
void prodMatrizRec(int **m1, int **m2, int ***c, int n, int i, int j, int k){
    //caso base
     if(i>=n){
        return;
    }

    if(j>=n){
        prodMatrizRec(m1, m2, c, n, i+1, 0, 0);
        return;
    }

    if(k<n){
	//Se suma el producto de los dos terminos actuales
        ((*c)[i][j]) += ((m1[i][k])*(m2[k][j]));
        prodMatrizRec(m1, m2, c, n, i, j, k+1);
    }else{
        prodMatrizRec(m1, m2, c, n, i, j+1, 0);
    }
}

/*Una funcion que calcula la multiplicación de dos matrices mediante 3 fors anidados iterativamente*/
void prodMatriz(int ***m1, int ***m2, int n){
    int i,j,k;

    int **c = (int **) malloc(n*sizeof(int *));
    for(int x=0; x<n; x++){
        c[x]= (int *) malloc(n*sizeof(int));
        for(int y=0; y<n; y++)
            c[x][y] = 0;
    }

    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            for(k=0; k<n; k++){
                c[i][j] += (* m1)[i][k]*(* m2)[k][j];
            }
        }
    }

    imprimeMatriz(c, n);
}
/*Esta funcion de conteo de inversiones es utilizando el metodo directo, en esta se utilizan dos fors anidados
el primer for controlara de izquierda a derecha el primer elemento que se use para una comparacion para saber
si se aumenta las inversiones, el segundo for manejara el segundo elemento para la misma comparacion
El primer for se detiene cuando el primer elemento de la comparacion llegue a la misma posicion que el ultimo
pues ya no habria elementos mas adelante para comparar, mientras el segundo for tambien se detiene cuando alcanza al ultimo
elemento del arreglo*/
int inversionConteo(int *arr, int n)
{
	int conteo = 0;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (arr[i] > arr[j]) {
				conteo++;
			}
		}
	}
	return conteo;
}


/*Funcion generando utilizando el metodo DyV que cuenta las inversiones dentro de un arreglo*/
int inverDiv(int arr[], int n, int i, int j) {
    /*Caso baso en el que el parametro i es la penultima posicion para comparar todos los pares
    de numeros, la i señalara el primer numero del par de elementos que compara cada vez que quiere saber si ocurrira
    una inversion*/
    if (i == n - 1) {
        return 0;
    }

    /*El valor j controlara el segundo numero que compara, cuando este segundo numero alcance la ultima posicion
    entonces movera el primer numero y tambien declarara el segundo numero que comparara*/
    if (j == n) {
        return inverDiv(arr, n, i + 1, i + 2);
    }

    /*Comparacion entre dos posiciones, se le sumara conteo a la recursion, si el de la izquierda es mayor al de la derecha
    entonces si sumara una inversion*/
    int conteo;
    if(arr[i] > arr[j])
     conteo = 1;
    else
     conteo = 0;

    /*Llamada recursiva para seguir recorriendo todos los valores del segundo elemento que se usa para comparar*/
    return conteo + inverDiv(arr, n, i, j + 1);
}
