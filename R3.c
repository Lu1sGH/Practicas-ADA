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

int main(){
    int n = 0, i = 0, mayor = 0, ini = 0, fin = 0, *a, **m1, **m2, s;
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


        free(a);
    }else{
        puts("========================================================");
        puts("===============Multiplicacion de matrices===============");
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
