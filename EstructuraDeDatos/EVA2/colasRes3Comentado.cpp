#include <iostream>
#include <ctime>
#include <iomanip>
#include <stdlib.h>
using namespace std;

// ===== ESTRUCTURA DEL NODO =====
// Representa a cada cliente en la cola
struct Nodo {
    string nombre;     // Nombre del cliente
    int personas;      // Cantidad de personas en el grupo
    time_t llegada;    // Hora de llegada (para calcular espera)
    Nodo* sig;         // Apuntador al siguiente nodo
};

// ===== VALIDACIONES =====

// Verifica que el texto solo tenga letras y espacios
bool esSoloTexto(const string& str) {
    if (str.empty()) return false; // No permite vacío
    for (char c : str) {
        if (!isalpha((unsigned char)c) && c != ' ') {
            return false; // Si encuentra algo distinto a letra o espacio
        }
    }
    return true;
}

// Verifica que el texto solo tenga números
bool esSoloNumeros(const string& str) {
    if (str.empty()) return false; // No permite vacío
    for (char c : str) {
        if (!isdigit((unsigned char)c)) {
            return false;
        }
    }
    return true;
}

// ===== COLA =====

// Agrega un cliente al final de la cola
void encolar(Nodo*& frente, Nodo*& fin, string nombre, int personas) {
    Nodo* nuevo = new Nodo(); // Crear nuevo nodo dinámico
    nuevo->nombre = nombre;
    nuevo->personas = personas;
    nuevo->llegada = time(0); // Guardar hora actual
    nuevo->sig = NULL;

    // Si la cola está vacía
    if (fin == NULL) {
        frente = fin = nuevo;
    } else {
        fin->sig = nuevo; // Enlazar al final
        fin = nuevo;      // Actualizar puntero final
    }
}

// Elimina un nodo específico de la cola
void eliminarNodo(Nodo*& frente, Nodo*& fin, Nodo* anterior, Nodo* actual) {
    // Si es el primer nodo
    if (anterior == NULL) {
        frente = actual->sig;
    } else {
        anterior->sig = actual->sig;
    }

    // Si es el último nodo
    if (actual == fin) {
        fin = anterior;
    }
}

// Busca un cliente adecuado según el tipo de mesa
Nodo* buscarCliente(Nodo* frente, Nodo*& anterior, int tipoMesa) {
    Nodo* actual = frente;
    Nodo* prev = NULL;

    while (actual != NULL) {

        // Mesa de 4 personas
        if (tipoMesa == 4) {
            if (actual->personas >= 1 && actual->personas <= 4)
                return actual;
        }

        // Mesa de 6 personas
        if (tipoMesa == 6) {
            if (actual->personas == 5 || actual->personas == 6)
                return actual;

            // Permite grupo de 4 si no hay grupos de 5 o 6
            if (actual->personas == 4) {
                Nodo* aux = frente;
                while (aux != NULL) {
                    if (aux->personas == 5 || aux->personas == 6)
                        return NULL;
                    aux = aux->sig;
                }
                return actual;
            }
        }

        // Mesa de 10 personas
        if (tipoMesa == 10) {
            if (actual->personas >= 7 && actual->personas <= 10)
                return actual;

            // Permite grupo de 6 si no hay grupos grandes
            if (actual->personas == 6) {
                Nodo* aux = frente;
                while (aux != NULL) {
                    if (aux->personas >= 7)
                        return NULL;
                    aux = aux->sig;
                }
                return actual;
            }
        }

        anterior = actual;       // Guardar nodo anterior
        actual = actual->sig;    // Avanzar en la lista
    }

    return NULL;
}

// Muestra la lista de espera completa
void mostrarCola(Nodo* frente) {
    if (frente==NULL)
    {
        cout << "Sin clientes en lista de espera\n";
        return;
    }
    cout << "\n--- LISTA DE ESPERA ---\n";
    while (frente != NULL) {
        // Convertir hora a formato legible
        tm* tiempoLocal = localtime(&frente->llegada);
        char hllegada[20];
        strftime(hllegada, sizeof(hllegada), "%H:%M:%S", tiempoLocal);

        // Mostrar datos del cliente
        cout << "Nombre: " << frente->nombre 
             << " | Personas: " << frente->personas 
             << " | Llegada: " << hllegada << endl;

        frente = frente->sig;
    }
}

// Intenta asignar mesa sin pasar por la cola
bool asignarDirecto(string nombre, int personas, int& m4, int& m6, int& m10, Nodo* cola) {

    Nodo* aux;
    
    // Para grupos pequeños
    if (personas >= 1 && personas <= 4) {

        if (m4 > 0) {
            m4--;
            cout << nombre << " pasa directo a mesa de 4\n";
            return true;
        }

        // Grupo de 4 puede usar mesa de 6 si no hay grupos mayores
        if (personas == 4 && m6 > 0) {

            bool hay5o6 = false;
            aux = cola;

            while (aux != NULL) {
                if (aux->personas == 5 || aux->personas == 6) {
                    hay5o6 = true;
                    break;
                }
                aux = aux->sig;
            }

            if (!hay5o6) {
                m6--;
                cout << nombre << " pasa a mesa de 6\n";
                return true;
            }
        }

    }

    // Para grupos medianos
    if (personas == 5 || personas == 6) {

        if (m6 > 0) {
            m6--;
            cout << nombre << " pasa directo a mesa de 6\n";
            return true;
        }

        // Grupo de 6 puede usar mesa de 10
        if (personas == 6 && m10 > 0) {

            bool hay7a10 = false;
            aux = cola;

            while (aux != NULL) {
                if (aux->personas >= 7 && aux->personas <= 10) {
                    hay7a10 = true;
                    break;
                }
                aux = aux->sig;
            }

            if (!hay7a10) {
                m10--;
                cout << nombre << " pasa a mesa de 10\n";
                return true;
            }
        }
    }

    // Para grupos grandes
    if (personas >= 7 && personas <= 10) {
        if (m10 > 0) {
            m10--;
            cout << nombre << " pasa directo a mesa de 10\n";
            return true;
        }
    }

    return false;
}

// Asigna mesas a clientes en espera (FIFO con reglas)
void asignarMesas(Nodo*& cola, Nodo*& fin, int& mesas4, int& mesas6, int& mesas10,double& totalTiempo, int& atendidos) {

    bool asignado;

    do {
        asignado = false;

        Nodo* actual;
        Nodo* anterior;
        Nodo* mejor;
        Nodo* mejorAnt;

        // ===== MESAS DE 4 =====
        if (mesas4 > 0) {
            actual = cola;
            anterior = NULL;
            mejor = NULL;
            mejorAnt = NULL;

            // Buscar primer cliente compatible (FIFO)
            while (actual != NULL) {
                if (actual->personas >= 1 && actual->personas <= 4) {
                    mejor = actual;
                    mejorAnt = anterior;
                    break;
                }
                anterior = actual;
                actual = actual->sig;
            }

            // Si se encontró cliente
            if (mejor != NULL) {
                eliminarNodo(cola, fin, mejorAnt, mejor);
                mesas4--;
                totalTiempo += difftime(time(0), mejor->llegada);
                atendidos++;
                cout << "\nMesa 4 asignada a " << mejor->nombre << endl;
                delete mejor;
                asignado = true;
            }
        }

        // ===== MESAS DE 6 =====
        if (mesas6 > 0) {

            actual = cola;
            anterior = NULL;
            mejor = NULL;
            mejorAnt = NULL;

            // Buscar 5 o 6 primero
            while (actual != NULL) {
                if (actual->personas == 5 || actual->personas == 6) {
                    mejor = actual;
                    mejorAnt = anterior;
                    break;
                }
                anterior = actual;
                actual = actual->sig;
            }

            // Si no hay, buscar grupo de 4
            if (mejor == NULL) {
                actual = cola;
                anterior = NULL;

                while (actual != NULL) {
                    if (actual->personas == 4) {
                        mejor = actual;
                        mejorAnt = anterior;
                        break;
                    }
                    anterior = actual;
                    actual = actual->sig;
                }
            }

            if (mejor != NULL) {
                eliminarNodo(cola, fin, mejorAnt, mejor);
                mesas6--;
                totalTiempo += difftime(time(0), mejor->llegada);
                atendidos++;
                cout << "\nMesa 6 asignada a " << mejor->nombre << endl;
                delete mejor;
                asignado = true;
            }
        }

        // ===== MESAS DE 10 =====
        if (mesas10 > 0) {

            actual = cola;
            anterior = NULL;
            mejor = NULL;
            mejorAnt = NULL;

            // Buscar grupos grandes
            while (actual != NULL) {
                if (actual->personas >= 7 && actual->personas <= 10) {
                    mejor = actual;
                    mejorAnt = anterior;
                    break;
                }
                anterior = actual;
                actual = actual->sig;
            }

            // Si no hay, buscar grupo de 6
            if (mejor == NULL) {
                actual = cola;
                anterior = NULL;

                while (actual != NULL) {
                    if (actual->personas == 6) {
                        mejor = actual;
                        mejorAnt = anterior;
                        break;
                    }
                    anterior = actual;
                    actual = actual->sig;
                }
            }

            if (mejor != NULL) {
                eliminarNodo(cola, fin, mejorAnt, mejor);
                mesas10--;
                totalTiempo += difftime(time(0), mejor->llegada);
                atendidos++;
                cout << "\nMesa 10 asignada a " << mejor->nombre << endl;
                delete mejor;
                asignado = true;
            }
        }

    } while (asignado); // Repite mientras siga asignando mesas

    // Mostrar estado final
    if (cola != NULL){
        cout << "\nNo hay mas mesas disponibles\n";
        cout << "Clientes pendientes:\n";
        mostrarCola(cola);
    }
    else {
        cout << "\nSin clientes en lista de espera\n";
    }
}

// Muestra visualmente las mesas disponibles
void mostrarMesas(int mesas4, int mesas6, int mesas10) {

    cout << "\n";
    cout << " __________\n";
    cout << "|\\         \\\n";
    cout << "| \\    4    \\\n";
    cout << "|  \\_________\\        Disponibles: " << mesas4 << "\n";
    cout << "   |         |\n";
    cout << "   |         |\n\n";

    cout << " ______________\n";
    cout << "|\\             \\\n";
    cout << "| \\      6      \\\n";
    cout << "|  \\_____________\\        Disponibles: " << mesas6 << "\n";
    cout << "   |             |\n";
    cout << "   |             |\n\n";

    cout << " ___________________\n";
    cout << "|\\                  \\\n";
    cout << "| \\        10        \\\n";
    cout << "|  \\__________________\\        Disponibles: " << mesas10 << "\n";
    cout << "   |                  |\n";
    cout << "   |                  |\n\n";
}

// ===== MAIN =====
int main() {
    Nodo *cola = NULL, *fin = NULL; // Cola de espera

    int mesas4 = 8, mesas6 = 5, mesas10 = 3; // Mesas disponibles

    double totalTiempo = 0; // Acumulador de tiempo de espera
    int atendidos = 0;      // Contador de clientes atendidos

    int op;

    do {
        cin.ignore();
        cout << "\n====================================\n";
        cout << "     RECEPCION RESTAURANTE\n";
        cout << "====================================\n";
        cout << "Mesas disponibles:\n";
        mostrarMesas(mesas4, mesas6, mesas10);

        cout << "\n1. Llegada de cliente\n";
        cout << "2. Salida de clientes\n";
        cout << "3. Ver lista de espera\n";
        cout << "4. Salir\n";
        cout << "Opcion: ";
        cin >> op;
        cin.clear();
        cin.ignore();

        switch (op) {

            // ===== LLEGADA =====
            case 1: {
                string nombre, personas;

                // Validar nombre
                do {
                    cout << "Nombre: ";
                    getline(cin, nombre);
                    if (!esSoloTexto(nombre)) {
                        cout << "Nombre invalido. Intente de nuevo\n";
                    }
                } while (!esSoloTexto(nombre));

                // Validar número de personas
                do {
                    cout << "Personas: ";
                    getline(cin, personas);

                    if (!esSoloNumeros(personas)) {
                        cout << "Numero invalido\nIntente de nuevo\n" << endl;
                    }
                } while (!esSoloNumeros(personas));

                int numPersonas = stoi(personas);

                // Si son más de 10 personas, se rechaza
                if (numPersonas > 10) {
                    cout << "Grupo rechazado\n";
                } else {
                    // Intentar asignar directo
                    if (!asignarDirecto(nombre, numPersonas, mesas4, mesas6, mesas10, cola)) {
                        encolar(cola, fin, nombre, numPersonas);
                        cout << "Cliente agregado a lista de espera\n";
                    } else {
                        atendidos++;
                    }
                }
                break;
            }

            // ===== SALIDA =====
            case 2: {
                int tipo;
                cout << "\n1. Mesa 4\n2. Mesa 6\n3. Mesa 10\nOpcion: ";
                cin >> tipo;

                switch (tipo) {
                case 1:
                    if (mesas4 < 8) {
                        mesas4++;
                        cout << "Mesa de 4 liberada\n";
                    } else {
                        cout << "Error: No hay mesas de 4 ocupadas\n";
                    }
                    asignarMesas(cola, fin, mesas4, mesas6, mesas10, totalTiempo, atendidos);
                    break;

                case 2:
                    if (mesas6 < 5) {
                        mesas6++;
                        cout << "Mesa de 6 liberada\n";
                    } else {
                        cout << "Error: No hay mesas de 6 ocupadas\n";
                    }
                    asignarMesas(cola, fin, mesas4, mesas6, mesas10, totalTiempo, atendidos);
                    break;

                case 3:
                    if (mesas10 < 3) {
                        mesas10++;
                        cout << "Mesa de 10 liberada\n";
                    } else {
                        cout << "Error: No hay mesas de 10 ocupadas\n";
                    }
                    asignarMesas(cola, fin, mesas4, mesas6, mesas10, totalTiempo, atendidos);
                    break;

                default:
                    cout << "Opcion invalida\n";
                }
                break;
            }

            // ===== MOSTRAR COLA =====
            case 3:
                mostrarCola(cola);
                break;

            // ===== SALIR =====
            case 4:
                if (cola != NULL){
                    cout << "Aun hay clientes en lista de espera\n";
                    op = 0;
                }
                break;

            default:
                cout << "Opcion invalida\n";
        }
    } while (op != 4);

    cout << "\n====================================\n";
    if (atendidos > 0) {
        cout << "Tiempo promedio de espera: " 
             << fixed << setprecision(2) 
             << totalTiempo / atendidos << " segundos\n";
    } else {
        cout << "Sin clientes atendidos\n";
    }

    system("pause"); // Pausa en Windows
    return 0;
}