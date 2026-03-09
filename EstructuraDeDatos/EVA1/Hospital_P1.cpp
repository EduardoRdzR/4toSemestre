/*
Eduardo Rodriguez Ramos
Cesar Antonio Mata Robles
Francisco Blanco Morales
Rebeca Moreno Gonzalez
Perla Yuliana Gonzalez Campos
*/
#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <regex>
using namespace std;

struct PACIENTE{
    char nombre[40];
    char apellido [40];
    int codigo;
    char direccion[100];
    char telefono[10];
    float adeudo;
    int status; // 1 es ACTIVO, 0 es ALTA
};
struct nodo{
    PACIENTE paciente;
    struct nodo *sgte;
};
typedef struct nodo *Tlista;
void registrar_paciente(PACIENTE &p){
    bool validacion;
    do{
        validacion = true;
        cout << "Ingrese nombre del paciente: \n" << endl;
        cin.getline(p.nombre,40);
        if (strlen(p.nombre)== 0){
            validacion = false;
        }else {
            for (int i= 0; p.nombre[i] != '\0'; i++){
                if (isdigit(p.nombre[i])){
                    validacion = false;
                }
            }
        }
        if (!validacion){
            cout << "Nombre invalido";
        }
    } while (!validacion);
    do{
        validacion = true;
        cout << "Ingrese apellidos del paciente: \n" << endl;
        cin.getline(p.apellido, 40);
        if (strlen(p.apellido)== 0){
            validacion = false;
        }else {
            for (int i= 0; p.apellido[i] != '\0'; i++){
                if (isdigit(p.apellido[i])){
                    validacion = false;
                }
            }
        }
        if (!validacion){
            cout << "Apellido invalido\n";
        }
    } while (!validacion);
    srand(time(0));
    p.codigo = rand() %10000;
    cout << "Codigo: " << p.codigo << "\n";
    do{
        cout << "Ingrese direccion: \n" << endl;
        cin.getline(p.direccion, 100);
        if (strlen(p.direccion) == 0){
            cout << "Direccion Invalida, no puede estar vacia\n";
        }
    }while( strlen(p.direccion) == 0);
    do{
        validacion = true;
        cout << "Ingrese telefono: \n" << endl;
        cin >> p.telefono;
        if (strlen(p.telefono) != 10){
            validacion = false;
        } else {
            for (int i = 0; p.telefono[i] != '\0'; i++){
                if (!isdigit(p.telefono[i])){
                    validacion = false;
                    break;
                }
            }
        }
        if (!validacion){
            cout << "Telefono invalido. Debe tener 10 digitos.\n" << endl;
        }
    } while (!validacion);

    do{
    validacion = true;
    cout << "Ingrese el adeudo: \n" << endl;
    cin >> p.adeudo;
    if (cin.fail() ||  cin.peek() != '\n' || p.adeudo < 0){ 
            cout << "Adeudo invalido. Debe ser un numero positivo.\n";
            cin.clear();           
            cin.ignore(1000, '\n'); 
            validacion = false;
        }
    
    } while (!validacion);
    p.status = 1;
}
void agregarPacienteLista(Tlista &lista){
    Tlista nuevo = new struct nodo;
    cin.ignore(1000, '\n');
    registrar_paciente (nuevo -> paciente);
    nuevo ->sgte = NULL;
    if(lista == NULL){
        lista = nuevo;
    }else{
        Tlista aux = lista;
        while (aux ->sgte != NULL){
            aux = aux ->sgte;
        }
        aux -> sgte = nuevo;
    }
    cout << "Paciente agregado correctamente\n" << endl;
}
void modificar_paciente(Tlista &lista, int codigo){
    Tlista q = lista;
    int res, op;
    string input;

    regex nombre_regex("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]+$");
    regex telefono_regex("^[0-9]{10}$");
    regex numero_regex("^[0-9]+$");
    regex decimal_regex("^[0-9]+(\\.[0-9]+)?$");

    while(q != NULL){
        if(q->paciente.codigo == codigo){
            cout << "Paciente encontrado\n";
            cout << "Nombre: " << q->paciente.nombre << endl;
            cout << "Apellido: " << q->paciente.apellido << endl;
            cout << "Desea modificarlo? (1.Si, 2.No)\n";
            do{
                cin >> res;
                if(cin.fail()){
                    cout << "Entrada invalida.\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
            } while(res != 1 && res != 2);
            cin.ignore(1000, '\n');
            if(res == 1){
                cout << "1. Nombre\n";
                cout << "2. Apellido\n";
                cout << "3. Codigo\n";
                cout << "4. Direccion\n";
                cout << "5. Telefono\n";
                cout << "6. Adeudo\n";
                cout << "7. Status\n";
                cin >> op;
                cin.ignore(1000, '\n');
                switch(op){
                    case 1:
                        do{
                            cout << "Nuevo nombre: ";
                            getline(cin, input);
                        } while(!regex_match(input, nombre_regex));

                        strcpy(q->paciente.nombre, input.c_str());
                        break;
                    case 2:
                        do{
                            cout << "Nuevo apellido: ";
                            getline(cin, input);
                        } while(!regex_match(input, nombre_regex));

                        strcpy(q->paciente.apellido, input.c_str());
                        break;
                    case 3:
                        do{
                            cout << "Nuevo codigo: ";
                            getline(cin, input);
                        } while(!regex_match(input, numero_regex));

                        q->paciente.codigo = stoi(input);
                        break;
                    case 4:
                        cout << "Nueva direccion: ";
                        getline(cin, input);
                        strcpy(q->paciente.direccion, input.c_str());
                        break;
                    case 5:
                        do{
                            cout << "Nuevo telefono (10 digitos): ";
                            getline(cin, input);
                        } while(!regex_match(input, telefono_regex));

                        strcpy(q->paciente.telefono, input.c_str());
                        break;
                    case 6:
                        do{
                            cout << "Nuevo adeudo: ";
                            getline(cin, input);
                        } while(!regex_match(input, decimal_regex));

                        q->paciente.adeudo = stof(input);
                        break;
                    case 7:
                        do{
                            cout << "Nuevo status (0 o 1): ";
                            getline(cin, input);
                        } while(input != "0" && input != "1");
                        q->paciente.status = stoi(input);
                        break;
                    default:
                        cout << "Opcion invalida\n";
                }
                cout << "Paciente modificado correctamente\n";
            } else {
                cout << "Modificacion cancelada\n";
            }
            return;
        }
        q = q->sgte;
    }
    cout << "Paciente no encontrado\n";
}
void mostrar_pacientes(Tlista lista){
    if(lista == NULL){
        cout << "No hay pacientes registrados.\n";
        return;
    }
    Tlista aux = lista;
    while(aux != NULL){
        cout << "-----------------------------\n";
        cout << "Codigo: " << aux->paciente.codigo << endl;
        cout << "Nombre: " << aux->paciente.nombre << endl;
        cout << "Apellidos: " << aux->paciente.apellido << endl;
        cout << "Adeudo: $" << aux->paciente.adeudo << endl;
        if(aux->paciente.status == 1)
            cout << "Status: ACTIVO\n";
        else
            cout << "Status: ALTA\n";
		cout << "-----------------------------\n";
        aux = aux->sgte;
    }
}
void pagar_adeudo(Tlista lista, int codigo){
    if(lista == NULL){
        cout << "No hay pacientes registrados.\n";
        return;
    }
    Tlista aux = lista;
    while(aux != NULL){
        if(aux->paciente.codigo == codigo){
            if(aux->paciente.adeudo == 0){
                cout << "El paciente no tiene adeudo pendiente.\n";
                return;
            }
            aux->paciente.adeudo = 0;
            aux->paciente.status = 0;
            cout << "Adeudo pagado correctamente.\n";
            cout << "El paciente ahora tiene status ALTA.\n";
            return;
        }
        aux = aux->sgte;
    }
    cout << "Paciente no encontrado.\n";
}
void eliminar_paciente(Tlista &lista, int codigo){

    if(lista == NULL){
        cout << "No hay pacientes registrados.\n";
        return;
    }
    Tlista actual = lista;
    Tlista anterior = NULL;
    while(actual != NULL){
        if(actual->paciente.codigo == codigo){
            if(actual->paciente.status != 0){
                cout << "No se puede eliminar. El paciente debe estar en ALTA.\n";
                return;
            }
            if(anterior == NULL){
                lista = actual->sgte;
            }else{
                anterior->sgte = actual->sgte;
            }
            delete actual;
            cout << "Paciente eliminado correctamente.\n";
            return;
        }
        anterior = actual;
        actual = actual->sgte;
    }
    cout << "Paciente no encontrado.\n";
}
int main(){
    PACIENTE p;
    Tlista lista = NULL;
    int op;
    int codigo;
    do {
        cout << "MENU HOSPTAL" << endl;
        cout << "1. Regitrar pacientes" << endl;
        cout << "2. Modificar datos" << endl;
        cout << "3. Mostrar pacientes" << endl;
        cout << "4. Pagar adeudo" << endl;
        cout << "5. Eliminar paciente" << endl;
        cout << "6. Salir" << endl;
        cout << "Ingrese opcion: " << endl;
        cin >> op;
        switch(op){
            case 1: 
            agregarPacienteLista(lista); 
            break;
            case 2: 
            cout << "Ingrese codigo a buscar " << endl;
            cin >> codigo;
            modificar_paciente(lista, codigo);break;
            case 3:
    		mostrar_pacientes(lista);
    		break;
			case 4:
    		cout << "Ingrese codigo del paciente: ";
    		cin >> codigo;
    		pagar_adeudo(lista, codigo);
    		break;
			case 5:
    		cout << "Ingrese codigo del paciente a eliminar: ";
    		cin >> codigo;
    		eliminar_paciente(lista, codigo);
    		break;
            case 6:
            cout << "Saliendo del programa..." << endl; 
            break;
            default: 
            cout << "Opcion invalida" << endl;
        }
    }while(op != 6);
    return 0;
}
