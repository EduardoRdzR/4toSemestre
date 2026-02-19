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
#include <cstdlib>
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

    ingresarFondoCaja();
    do {
        mostrarMenu();
        cout << "\nIngrese opcion: ";
        cin >> opcion;
        cin.ignore();
        
        switch(opcion){
            case 1:
                ingresarMovimiento(caja);
                break;
            case 2:
                mostrarMovimientos(caja);
                break;
            case 3:
                corteCaja(caja);
                break;
            case 4:
                cout << "\nEsta saliendo del sistema..." << endl;
                break;
            default:
                cout << "\nOpcion invalida. Intente de nuevo." << endl;
        }
        
        if(opcion != 4){
            cout << "\nPresione ENTER para continuar...";
            cin.ignore();
	    limpiarPantalla();
        }
        
    } while(opcion != 4);
    
    return 0;
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
        cout << "Cliente: " << caja->nombre << endl;
        cout << "Cuenta: " << caja->cuenta << endl;
        cout << "Tipo: " << caja->movimiento << endl;
        cout << "Monto: $" << fixed << caja->monto << endl;
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

void mostrarMenu(){
    limpiarPantalla();
    cout << "\n----------- MENU PRINCIPAL -----------" << endl;
    cout << "1.Ingresar movimiento" << endl;
    cout << "2.Mostrar movimientos" << endl;
    cout << "3.Corte de caja" << endl;
    cout << "4.Salir" << endl << endl;
}

bool validarNombre(string nombre){
    if (nombre.empty()) {
        cout << "Error: El nombre no puede estar vacio." << endl;
        return false;
    }
    
    for (int i = 0; i < nombre.length(); i++) {
        if (!isalpha(nombre[i]) && nombre[i] != ' ') {
            cout << "Error: El nombre solo puede contener letras y espacios." << endl;
            return false;
        }
    }
    return true;
}

void corteCaja(Pila &caja){
    if(caja == NULL){
        cout << "\nNo hay movimientos para realizar el corte de caja." << endl;
        return;
    }
    cout << "\n----------- CORTE DE CAJA -----------" << endl;
    cout << "***Se Borraran todos los movimientos del dia.***" << endl;
    
    char confirmacion;
    cout << "\nDesea continuar con el corte de caja? (S/N): ";
    cin >> confirmacion;
    cin.ignore();
    
    if(confirmacion == 'S' || confirmacion == 's'){
        while(caja != NULL){
            Pila aux = caja;
            caja = caja->sgte;
            delete aux;
        }
        
        noMov = 0;
        
        cout << "\nSe ha realizado el corte de caja exitosamente." << endl;
        cout << "Efectivo actual en caja: $" << fixed << setprecision(2) << fondoCaja << endl;
    }
    else{
        cout << "\nNo se pudo realizar el corte de caja." << endl;
    }
}

void ingresarMovimiento(Pila &caja){
    cout << "\n----------- INGRESAR MOVIMIENTO -----------" << endl;
    cout << "\nSaldo actual en caja: $" << fixed << setprecision(2) << fondoCaja << endl;
    
    Pila nuevo = new nodo();
    noMov++;
    nuevo->noMov = noMov;
    
    cout << "\nNumero de movimiento (automatico): " << nuevo->noMov << endl;
    
    cout << "Nombre del cliente: ";
    getline(cin, nuevo->nombre);
    while(!validarNombre(nuevo->nombre)){
    cout << "Ingrese de nuevo el nombre del cliente: ";
    getline(cin, nuevo->nombre);
    }

    cout << "Numero de cuenta (minimo de 8 digitos): ";
    cin >> nuevo->cuenta;
    cin.ignore();
    
    while(!validarNumeroCuenta(nuevo->cuenta)){
        cout << "Error: La cuenta debe tener un minimo de 8 digitos." << endl;
        cout << "Ingrese de nuevo el numero de cuenta: ";
        cin >> nuevo->cuenta;
        cin.ignore();
    }
    
    cout << "Tipo de movimiento (DEPOSITO | RETIRO): ";
    getline(cin, nuevo->movimiento);
    cout << "Monto del movimiento: $";
    cin >> nuevo->monto;
    cin.ignore();

    if(!validarMonto(nuevo->monto)){
        cout << "\nError: Monto invalido." << endl;
        delete nuevo;
        return;
    }

    if(nuevo->movimiento == "RETIRO" || nuevo->movimiento == "retiro"){
        if(nuevo->monto > 50000){
            cout << "\n*** AVISO IMPORTANTE ***" << endl;
            cout << "El cliente solicita un retiro mayor a $50,000" << endl;
            cout << "Por favor haga pasar al cliente a GERENCIA." << endl;
            cout << "**********************\n" << endl;
            delete nuevo;
            return;
        }
        
        if(fondoCaja - nuevo->monto < 0){
            cout << "\nError: No hay saldo suficiente en caja para este retiro." << endl;
            cout << "Se recomienda efectuar un CORTE DE CAJA." << endl;
            delete nuevo;
            return;
        }
        
        fondoCaja -= nuevo->monto;
    }
    else if(nuevo->movimiento == "DEPOSITO" || nuevo->movimiento == "deposito"){
        fondoCaja += nuevo->monto;
    }
    else{
        cout << "\nError: Tipo de movimiento invalido." << endl;
        delete nuevo;
        return;
    }
    nuevo->sgte = caja;
    caja = nuevo;
    cout << "\nMovimiento registrado con exito." << endl;
    cout << "Nuevo saldo en caja: $" << fixed << setprecision(2) << fondoCaja << endl;
}

bool validarMonto(int monto) {
    if (monto > 0) {
        return true;
    }
    return false;
}

void limpiarPantalla() {
    system("cls");
}