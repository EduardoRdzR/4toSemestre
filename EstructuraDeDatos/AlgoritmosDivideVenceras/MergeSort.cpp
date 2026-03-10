#include <iostream>
#include <stdlib.h>
using namespace std;

// Definicion del nodo
struct Nodo {
    int dato;
    Nodo* sig;
};

// Crear nodo
Nodo* crearNodo(int valor) {
    Nodo* nuevo = new Nodo();
    nuevo->dato = valor;
    nuevo->sig = NULL;
    return nuevo;
}

// Insertar al inicio
void insertar(Nodo*& cabeza, int valor) {
    Nodo* nuevo = crearNodo(valor);
    nuevo->sig = cabeza;
    cabeza = nuevo;
}

// Imprimir lista
void imprimir(Nodo* cabeza) {
    while (cabeza != NULL) {
        cout << cabeza->dato << " ";
        cabeza = cabeza->sig;
    }
    cout << endl;
}

// Unir dos listas ordenadas
Nodo* merge(Nodo* a, Nodo* b) {

    if (a == NULL) return b;
    if (b == NULL) return a;

    Nodo* resultado;

    if (a->dato <= b->dato) {
        resultado = a;
        resultado->sig = merge(a->sig, b);
    }
    else {
        resultado = b;
        resultado->sig = merge(a, b->sig);
    }

    return resultado;
}

// Encontrar mitad de la lista
void dividirLista(Nodo* fuente, Nodo** frente, Nodo** atras) {

    Nodo* rapido;
    Nodo* lento;

    if (fuente == NULL || fuente->sig == NULL) {
        *frente = fuente;
        *atras = NULL;
    }
    else {
        lento = fuente;
        rapido = fuente->sig;

        while (rapido != NULL) {
            rapido = rapido->sig;
            if (rapido != NULL) {
                lento = lento->sig;
                rapido = rapido->sig;
            }
        }

        *frente = fuente;
        *atras = lento->sig;
        lento->sig = NULL;
    }
}

// Merge Sort para lista enlazada
void mergeSort(Nodo** cabeza) {

    Nodo* head = *cabeza;
    Nodo* a;
    Nodo* b;

    if (head == NULL || head->sig == NULL)
        return;

    dividirLista(head, &a, &b);

    mergeSort(&a);
    mergeSort(&b);

    *cabeza = merge(a, b);
}

int main() {

    Nodo* lista = NULL;

    insertar(lista, 5);
    insertar(lista, 2);
    insertar(lista, 9);
    insertar(lista, 1);
    insertar(lista, 7);

    cout << "Lista original:\n";
    imprimir(lista);

    mergeSort(&lista);

    cout << "Lista ordenada:\n";
    imprimir(lista);
    system("pause");
    return 0;
}
