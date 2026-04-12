/*
Eduardo Rodriguez Ramos
Cesar Antonio Mata Robles 
Francisco Blanco Morales 
Rebeca Moreno Gonzalez
Perla Yuliana Gonzalez Campos 
*/
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

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

int operador(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

bool valido(string expr) {
    if (expr.length() == 0) return false;
    
    bool operando = false;
    for (char c : expr) {
        if (isalnum(c)) operando = true;
        if (!isalnum(c) && c != '+' && c != '-' && c != '*' && c != '/' && c != '^' && c != '(' && c != ')') {
            return false;
        }
    }
    return operando; 
}

string infijaAPosfija(string infija) {
    Pila s;
    string posfija = "";

    for (int i = 0; i < infija.length(); i++) {
    char c = infija[i];

    if (isdigit(c)) {
        string numero = "";

        while (i < infija.length() && isdigit(infija[i])) {
            numero += infija[i];
            i++;
        }

        posfija += numero + " "; 
        i--; 
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
    else { 
        while (!s.vacio() && ((operador(s.peek()) > operador(c)) || (operador(s.peek()) == operador(c) && c != '^'))) {
            posfija += s.pop();
        }
        s.push(c);
    }
}

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

    if (!valido(input)) {
        cout << "Error: Entrada no valida. Asegurese de no dejar espacios vacios y usar caracteres correctos." << endl;
        return 1;
    }

    string expresion = infijaAPosfija(input);
    cout << "Notacion Posfija: " << expresion << endl;
    system("pause");
    return 0;
}
