#include <stdio.h>
 
int buscarBinario(int arr[], int n, int objetivo) {
    int izquierda = 0;
    int derecha = n - 1;
 
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
 
        if (arr[medio] == objetivo) {
            return medio;
        }
 
        if (arr[medio] < objetivo) {
            izquierda = medio + 1;
        } else {
            derecha = medio - 1; 
        }
    }
 
    return -1;
}
 
int main() {
    int arreglo[] = {10, 20, 30, 40, 50};
    int n = sizeof(arreglo) / sizeof(arreglo[0]);
    int objetivo = 30;
 
    int resultado = buscarBinario(arreglo, n, objetivo);
 
    if (resultado != -1) {
        printf("Elemento %d encontrado en la posición %d.\n", objetivo, resultado);
    } else {
        printf("Elemento %d no encontrado.\n", objetivo);
    }
 
    return 0;
}
 