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

// Estructura de nodo para la Pila
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
long fondoCaja = 0;

// Prototipos de funciones
void ingresarFondoCaja();
void mostrarMenu();
void ingresarMovimiento(Pila &caja);
void mostrarMovimientos(Pila caja);
void corteCaja(Pila &caja);
void limpiarPantalla();
bool validarNombre(string nombre);
bool validarNumeroCuenta(long cuenta);
bool validarMonto(long monto);

int main(){
    Pila caja = NULL;
    int opcion;
    
    cout << "\n=========================================" << endl;
    cout << "  SISTEMA DE CONTROL DE CAJA BANCARIA   " << endl;
    cout << "=========================================\n" << endl;
    
    // Solicitar fondo de caja inicial
    ingresarFondoCaja();
    
    // Menu principal
    do {
        mostrarMenu();
        cout << "\nIngrese su opcion: ";
        cin >> opcion;
        cin.ignore();  // Limpiar el buffer
        
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
                cout << "\n¡Gracias por usar el sistema! Hasta luego." << endl;
                break;
            default:
                cout << "\nOpcion invalida. Intente de nuevo." << endl;
        }
        
        if(opcion != 4){
            cout << "\nPresione ENTER para continuar...";
            cin.ignore();
        }
        
    } while(opcion != 4);
    
    return 0;
}

/*
FUNCION: ingresarFondoCaja
DESCRIPCION: Solicita y valida el monto inicial del fondo de caja (minimo $300,000)
PARAMETROS: Ninguno
RETORNA: void
*/
void ingresarFondoCaja(){
    cout << "Ingrese el monto inicial del fondo de caja (minimo $300,000): $";
    cin >> fondoCaja;
    
    while(fondoCaja < 300000){
        cout << "\nError: El fondo de caja debe ser superior a $300,000" << endl;
        cout << "Ingrese el monto inicial nuevamente: $";
        cin >> fondoCaja;
    }
    
    cout << "\nFondo de caja establecido: $" << fixed << setprecision(2) << fondoCaja << endl;
    cout << "\n=========================================" << endl;
}

/*
FUNCION: mostrarMenu
DESCRIPCION: Muestra el menu principal de opciones
PARAMETROS: Ninguno
RETORNA: void
*/
void mostrarMenu(){
    cout << "\n========== MENU PRINCIPAL ==========" << endl;
    cout << "1. INGRESAR MOVIMIENTO BANCARIO" << endl;
    cout << "2. MOSTRAR MOVIMIENTOS BANCARIOS" << endl;
    cout << "3. GENERAR CORTE DE CAJA" << endl;
    cout << "4. SALIR" << endl;
    cout << "===================================" << endl;
}

/*
FUNCION: ingresarMovimiento
DESCRIPCION: Ingresa un nuevo movimiento bancario (deposito o retiro) a la pila
PARAMETROS: Pila &caja (referencia a la pila)
RETORNA: void
*/
void ingresarMovimiento(Pila &caja){
    cout << "\n========== INGRESAR MOVIMIENTO ==========" << endl;
    cout << "\nSaldo actual en caja: $" << fixed << setprecision(2) << fondoCaja << endl;
    
    Pila nuevo = new nodo();
    noMov++;
    nuevo->noMov = noMov;
    
    cout << "\nNumero de movimiento (auto): " << nuevo->noMov << endl;
    
    cout << "Nombre del cliente: ";
    getline(cin, nuevo->nombre);
    
    cout << "Numero de cuenta (minimo 8 digitos): ";
    cin >> nuevo->cuenta;
    cin.ignore();
    
    while(!validarNumeroCuenta(nuevo->cuenta)){
        cout << "Error: La cuenta debe tener minimo 8 digitos." << endl;
        cout << "Ingrese el numero de cuenta nuevamente: ";
        cin >> nuevo->cuenta;
        cin.ignore();
    }
    
    cout << "Tipo de movimiento (DEPOSITO/RETIRO): ";
    getline(cin, nuevo->movimiento);
    
    cout << "Monto del movimiento: $";
    cin >> nuevo->monto;
    cin.ignore();
    
    // Validaciones
    if(!validarMonto(nuevo->monto)){
        cout << "\nError: Monto invalido." << endl;
        delete nuevo;
        return;
    }
    
    // Validar retiros
    if(nuevo->movimiento == "RETIRO" || nuevo->movimiento == "retiro"){
        if(nuevo->monto > 50000){
            cout << "\n*** AVISO IMPORTANTE ***" << endl;
            cout << "El cliente solicita un retiro superior a $50,000" << endl;
            cout << "Por favor, invite al cliente a pasar a GERENCIA." << endl;
            cout << "**********************\n" << endl;
            delete nuevo;
            return;
        }
        
        if(fondoCaja - nuevo->monto < 0){
            cout << "\nError: No hay suficiente saldo en caja para este retiro." << endl;
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
    
    // Apilar el movimiento
    nuevo->sgte = caja;
    caja = nuevo;
    
    cout << "\nMovimiento registrado exitosamente." << endl;
    cout << "Nuevo saldo en caja: $" << fixed << setprecision(2) << fondoCaja << endl;
}

/*
FUNCION: mostrarMovimientos
DESCRIPCION: Muestra todos los movimientos actuales en la pila
PARAMETROS: Pila caja (copia de la pila)
RETORNA: void
*/
void mostrarMovimientos(Pila caja){
    if(caja == NULL){
        cout << "\nNo hay movimientos registrados aun." << endl;
        return;
    }
    
    cout << "\n========== MOVIMIENTOS BANCARIOS ==========" << endl;
    cout << "\nSaldo actual en caja: $" << fixed << setprecision(2) << fondoCaja << endl;
    cout << "\n";
    
    int contador = 0;
    while(caja != NULL){
        cout << "Movimiento #" << caja->noMov << endl;
        cout << "  Cliente: " << caja->nombre << endl;
        cout << "  Cuenta: " << caja->cuenta << endl;
        cout << "  Tipo: " << caja->movimiento << endl;
        cout << "  Monto: $" << fixed << setprecision(2) << caja->monto << endl;
        cout << "---" << endl;
        
        caja = caja->sgte;
        contador++;
    }
    
    cout << "\nTotal de movimientos: " << contador << endl;
    cout << "==========================================\n" << endl;
}

/*
FUNCION: corteCaja
DESCRIPCION: Realiza el corte de caja, desapilando todos los movimientos
PARAMETROS: Pila &caja (referencia a la pila)
RETORNA: void
*/
void corteCaja(Pila &caja){
    cout << "\n========== CORTE DE CAJA ==========" << endl;
    cout << "\nADVERTENCIA:" << endl;
    cout << "Al efectuar el corte de caja, se BORRARAN todos los movimientos del dia." << endl;
    cout << "Cantidad que debe estar en efectivo: $" << fixed << setprecision(2) << fondoCaja << endl;
    
    char confirmacion;
    cout << "\n¿Desea continuar con el corte de caja? (S/N): ";
    cin >> confirmacion;
    cin.ignore();
    
    if(confirmacion == 'S' || confirmacion == 's'){
        // Desapilar todos los elementos
        while(caja != NULL){
            Pila temp = caja;
            caja = caja->sgte;
            delete temp;
        }
        
        noMov = 0;
        
        cout << "\nCorte de caja realizado exitosamente." << endl;
        cout << "Todos los movimientos han sido eliminados." << endl;
        cout << "El saldo actual es: $" << fixed << setprecision(2) << fondoCaja << endl;
        cout << "===================================\n" << endl;
    }
    else{
        cout << "\nCorte de caja cancelado." << endl;
    }
}

/*
FUNCION: validarNumeroCuenta
DESCRIPCION: Valida que el numero de cuenta tenga minimo 8 digitos
PARAMETROS: long cuenta (numero de cuenta)
RETORNA: bool (true si es valido, false si no)
*/
bool validarNumeroCuenta(long cuenta){
    if(cuenta >= 10000000){  // Minimo 8 digitos
        return true;
    }
    return false;
}

/*
FUNCION: validarMonto
DESCRIPCION: Valida que el monto sea mayor a cero
PARAMETROS: long monto (cantidad de dinero)
RETORNA: bool (true si es valido, false si no)
*/
bool validarMonto(long monto){
    if(monto > 0){
        return true;
    }
    return false;
}