#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

// Estructura para el manejo manual de la pila (Stack)
struct Nodo {
    char dato;
    Nodo* siguiente;
};

struct Pila {
    Nodo* top = nullptr;

    void push(char valor) {
        Nodo* nuevoNodo = new Nodo();
        nuevoNodo->dato = valor;
        nuevoNodo->siguiente = top;
        top = nuevoNodo;
    }

    char pop() {
        if (vacio()) return '\0';
        Nodo* temp = top;
        char valor = temp->dato;
        top = top->siguiente;
        delete temp;
        return valor;
    }

    char peek() {
        return (vacio()) ? '\0' : top->dato;
    }

    bool vacio() {
        return top == nullptr;
    }
};

// Determina la jerarquía de los operadores
int operador(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// Valida si la entrada es correcta (no vacía y con caracteres válidos)
bool valido(string expr) {
    if (expr.length() == 0) return false;
    
    bool operando = false;
    for (char c : expr) {
        if (isalnum(c)) operando = true;
        // Permite números, letras, operadores básicos y paréntesis
        if (!isalnum(c) && c != '+' && c != '-' && c != '*' && c != '/' && c != '^' && c != '(' && c != ')') {
            return false;
        }
    }
    return operando; // Evita entradas que sean solo operadores
}

string infijaAPosfija(string infija) {
    Pila s;
    string posfija = "";

    for (int i = 0; i < infija.length(); i++) {
    char c = infija[i];

    // ?? Si es d�gito ? construir n�mero completo
    if (isdigit(c)) {
        string numero = "";

        while (i < infija.length() && isdigit(infija[i])) {
            numero += infija[i];
            i++;
        }

        posfija += numero + " "; // espacio despu�s del n�mero
        i--; // retrocede porque el for lo incrementa
    } 
    else if (isalpha(c)) {
        posfija += c;
    }
    else if (c == '(') {
        s.push(c);
    } 
    else if (c == ')') {
        while (!s.vacio() && s.peek() != '(') {
            posfija += s.pop();
        }
        s.pop();
    } 
    else { // operador
        while (!s.vacio() && 
              ((operador(s.peek()) > operador(c)) || 
              (operador(s.peek()) == operador(c) && c != '^'))) {
            posfija += s.pop();
        }
        s.push(c);
    }
}

    // Vaciar la pila restante
    while (!s.vacio()) {
        posfija += s.pop();
    }

    return posfija;
}

int main() {
    string input;

    cout << "--- CONVERSOR INFIJA A POSFIJA ---" << endl;
    cout << "Ingrese la expresion: ";
    getline(cin, input);

    // Validación de entrada
    if (!valido(input)) {
        cout << "Error: Entrada no valida. Asegurese de no dejar espacios vacios y usar caracteres correctos." << endl;
        return 1;
    }

    string expresion = infijaAPosfija(input);
    cout << "Notacion Posfija: " << expresion << endl;
    system("pause");
    return 0;
}
