#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

struct Nodo {
    int dato;
    Nodo* sig;
};

void insertar(Nodo*& Lista, int valor) {
    Nodo* nuevo = new Nodo{valor, nullptr};

    if (Lista == nullptr) {
        Lista = nuevo;
        return;
    }

    Nodo* temp = Lista;
    while (temp->sig != nullptr) {
        temp = temp->sig;
    }
    temp->sig = nuevo;
}

void imprimir(Nodo* Lista) {
    while (Lista != nullptr) {
        cout << Lista->dato << " ";
        Lista = Lista->sig;
    }
    cout << endl;
}

Nodo* concatenar(Nodo* a, Nodo* b) {
    if (a == nullptr) return b;

    Nodo* temp = a;
    while (temp->sig != nullptr) {
        temp = temp->sig;
    }
    temp->sig = b;
    return a;
}

Nodo* quickSort(Nodo* Lista) {
    if (Lista == nullptr || Lista->sig == nullptr)
        return Lista;

    Nodo* pivote = Lista;
    Nodo* actual = Lista->sig;

    Nodo* menores = nullptr;
    Nodo* mayores = nullptr;

    Nodo* salida = nullptr;

    while (actual != nullptr) {
        if (actual->dato <= pivote->dato) {
            insertar(menores, actual->dato);
        } else {
            insertar(mayores, actual->dato);
        }
        actual = actual->sig;
    }

    menores = quickSort(menores);
    mayores = quickSort(mayores);

    pivote->sig = mayores;
    salida = concatenar(menores, pivote);

    return salida;
}

bool validacionNumero(string s) {
    if (s.empty()) 
        return false;
    for (char c : s) {
        if (!isdigit(c)) 
            return false;
    }
    return true;
}

int main() {
    Nodo* lista = nullptr;
    string numero;

    cout << "Ingresa los numeros de la lista (x para terminar):\n";

    while (true) {
        cout << "Numero: ";
        cin >> numero;
        if (numero == "x" || numero == "X")
            break;
        if (!validacionNumero(numero)) {
            cout << "Error. Ingresa un numero.\n";
            continue;
        }
        int valor = stoi(numero);
        insertar(lista, valor);
    }

    cout << endl << "Lista original: ";
    imprimir(lista);

    lista = quickSort(lista);

    cout << "Lista ordenada: ";
    imprimir(lista);

    system("pause");
    return 0;
}