/*
Eduardo Rodriguez Ramos
Cesar Antonio Mata Robles 
Francisco Blanco Morales 
Rebeca Moreno Gonzalez
Perla Yuliana Gonzalez Campos 
*/

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct nodo{
            int noMov; 
            string nombre;
            int cuenta;
            string movimiento;
            int monto;
            struct nodo *sgte; 
};

typedef struct nodo *Pila;

// Variables globales
int noMov = 0;
int fondoCaja = 0;

void ingresarFondoCaja();
void mostrarMenu();
void ingresarMovimiento(Pila &caja);
void mostrarMovimientos(Pila caja);
void corteCaja(Pila &caja);
void limpiarPantalla();
bool validarNombre(string nombre);
bool validarNumeroCuenta(int cuenta);
bool validarMonto(int monto);

int main(){
    Pila caja = NULL;
    int opcion;
    cout << "  SISTEMA DE CONTROL DE CAJA BANCARIA   " << endl;
    cout << "\n-----------------------------------------------------------" << endl;
    ingresarFondoCaja()

    //Menu

}

void ingresarFondoCaja(){
    cout << "Ingrese el monto del fondo de caja (minimo $300,000): $";
    cin >> fondoCaja;
    
    while(fondoCaja < 300000){
        cout << "\nError: El fondo de caja debe ser mayor a $300,000" << endl;
        cout << "Ingrese un monto valido: $";
        cin >> fondoCaja;
    }
    
    cout << "\nFondo de caja: $"<< fixed << fondoCaja << endl;
    cout << "\n--------------------------------------" << endl;
}
void mostrarMovimientos(Pila caja){
    if(caja == NULL){
        cout << "\nNo hay movimientos registrados." << endl;
        return;
    }
    
    cout << "\n----------- MOVIMIENTOS BANCARIOS -----------" << endl;
    cout << "\nSaldo actual en caja: $" << fixed << fondoCaja << endl << endl;
    
    int contador = 0;
    while(caja != NULL){
        cout << "Movimiento #" << caja->noMov << endl;
        cout << "  Cliente: " << caja->nombre << endl;
        cout << "  Cuenta: " << caja->cuenta << endl;
        cout << "  Tipo: " << caja->movimiento << endl;
        cout << "  Monto: $" << fixed << caja->monto << endl;
        cout << "---" << endl;
        
        caja = caja->sgte;
        contador++;
    }
    
    cout << "\nNumero de movimientos: " << contador << endl;
}
bool validarNumeroCuenta(int cuenta){
    if(cuenta >= 10000000){  
        return true;
    }
    return false;
}