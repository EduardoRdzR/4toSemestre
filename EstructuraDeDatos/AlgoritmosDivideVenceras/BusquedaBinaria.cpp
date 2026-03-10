#include <stdio.h>
#include <stdlib.h>

struct Nodo {
    int valor;
    struct Nodo* siguiente;
};

struct Nodo* nuevoNodo(int valor) {
    struct Nodo* nodo = (struct Nodo*)malloc(sizeof(struct Nodo));
    nodo->valor = valor;
    nodo->siguiente = NULL;
    return nodo;
}

void insertar(struct Nodo** cabeza, int valor) {
    struct Nodo* nodo = nuevoNodo(valor);
    if (*cabeza == NULL) {
        *cabeza = nodo;
        return;
    }
    struct Nodo* temp = *cabeza;
    while (temp->siguiente != NULL) {
        temp = temp->siguiente;
    }
    temp->siguiente = nodo;
}

void mostrar(struct Nodo* cabeza) {
    struct Nodo* actual = cabeza;
    printf("Lista: ");
    while (actual != NULL) {
        printf("%d -> ", actual->valor);
        actual = actual->siguiente;
    }
    printf("NULL\n");
}

int buscar(struct Nodo* cabeza, int objetivo) {
    int pos = 0;
    while (cabeza != NULL) {
        if (cabeza->valor == objetivo) {
            return pos; 
        }
        cabeza = cabeza->siguiente;
        pos++;
    }
    return -1; 
}

int main() {
    struct Nodo* lista = NULL;

    insertar(&lista, 10);
    insertar(&lista, 20);
    insertar(&lista, 30);
    insertar(&lista, 40);
    insertar(&lista, 50);

    mostrar(lista);

    int objetivo = 30;
    int resultado = buscar(lista, objetivo);

    if (resultado != -1) {
        printf("Elemento %d encontrado en la posicion %d.\n", objetivo, resultado);
    } else {
        printf("Elemento %d no encontrado.\n", objetivo);
    }
    system("pause");
    return 0;
}
