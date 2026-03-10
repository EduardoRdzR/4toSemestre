#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

struct Nodo {
    int dato;
    struct Nodo *sig;
};
typedef struct Nodo *Tlista;

void insertar(Tlista& Lista, int valor) {
    Tlista nuevo = new(struct Nodo);
    nuevo->dato = valor;
    nuevo->sig = nullptr;

    if (Lista == nullptr) {
        Lista = nuevo;
        return;
    }

    Tlista temp = Lista;
    while (temp->sig != nullptr) {
        temp = temp->sig;
    }
    temp->sig = nuevo;
}

void imprimir(Tlista Lista) {
    while (Lista != nullptr) {
        cout << Lista->dato << " ";
        Lista = Lista->sig;
    }
    cout << endl;
}

Tlista concatenar(Tlista a, Tlista b) {
    if (a == nullptr) return b;

    Tlista temp = a;
    while (temp->sig != nullptr) {
        temp = temp->sig;
    }
    temp->sig = b;
    return a;
}

Tlista quickSort(Tlista Lista) {
    if (Lista == nullptr || Lista->sig == nullptr)
        return Lista;

    Tlista pivote = Lista;
    Tlista actual = Lista->sig;

    Tlista menores = nullptr;
    Tlista mayores = nullptr;

    Tlista salida = nullptr;

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
    Tlista lista = nullptr;
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