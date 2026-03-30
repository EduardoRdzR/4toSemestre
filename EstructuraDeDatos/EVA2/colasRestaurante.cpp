#include <iostream>
#include <ctime>
#include <iomanip>
#include <stdlib.h>
using namespace std;

// ===== ESTRUCTURA =====
struct Nodo {
    string nombre;
    int personas;
    time_t llegada;
    Nodo* sig;
};

// ===== COLA =====
void encolar(Nodo*& frente, Nodo*& fin, string nombre, int personas) {
    Nodo* nuevo = new Nodo();
    nuevo->nombre = nombre;
    nuevo->personas = personas;
    nuevo->llegada = time(0);
    nuevo->sig = NULL;

    if (fin == NULL) {
        frente = fin = nuevo;
    } else {
        fin->sig = nuevo;
        fin = nuevo;
    }
}

void eliminarNodo(Nodo*& frente, Nodo*& fin, Nodo* anterior, Nodo* actual) {
    if (anterior == NULL) {
        frente = actual->sig;
    } else {
        anterior->sig = actual->sig;
    }

    if (actual == fin) {
        fin = anterior;
    }
}

// ===== BUSCAR CLIENTE =====
Nodo* buscarCliente(Nodo* frente, Nodo*& anterior, int tipoMesa) {
    Nodo* actual = frente;
    Nodo* prev = NULL;

    while (actual != NULL) {

        if (tipoMesa == 4) {
            if (actual->personas >= 1 && actual->personas <= 4)
                return actual;
        }

        if (tipoMesa == 6) {
            if (actual->personas == 5 || actual->personas == 6)
                return actual;

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

        if (tipoMesa == 10) {
            if (actual->personas >= 7 && actual->personas <= 10)
                return actual;

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

        anterior = actual;
        actual = actual->sig;
    }

    return NULL;
}

// ===== MOSTRAR COLA =====
void mostrarCola(Nodo* frente) {
    cout << "\n--- LISTA DE ESPERA ---\n";
    while (frente != NULL) {
        cout << "Nombre: " << frente->nombre
             << " | Personas: " << frente->personas << endl;
        frente = frente->sig;
    }
}

// ===== ASIGNACION DIRECTA =====
bool asignarDirecto(string nombre, int personas,
                   int& m4, int& m6, int& m10) {

    // 1-4 personas
    if (personas >= 1 && personas <= 4) {
        if (m4 > 0) {
            m4--;
            cout << "✔ " << nombre << " pasa directo a mesa de 4\n";
            return true;
        }
        if (personas == 4 && m6 > 0) {
            m6--;
            cout << "✔ " << nombre << " pasa directo a mesa de 6\n";
            return true;
        }
    }

    // 5-6 personas
    if (personas == 5 || personas == 6) {
        if (m6 > 0) {
            m6--;
            cout << "✔ " << nombre << " pasa directo a mesa de 6\n";
            return true;
        }
        if (personas == 6 && m10 > 0) {
            m10--;
            cout << "✔ " << nombre << " pasa directo a mesa de 10\n";
            return true;
        }
    }

    // 7-10 personas
    if (personas >= 7 && personas <= 10) {
        if (m10 > 0) {
            m10--;
            cout << "✔ " << nombre << " pasa directo a mesa de 10\n";
            return true;
        }
    }

    return false;
}
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
    Nodo *cola = NULL, *fin = NULL;

    int mesas4 = 8, mesas6 = 5, mesas10 = 3;

    double totalTiempo = 0;
    int atendidos = 0;

    int op;

    do {
        cout << "\n====================================\n";
        cout << "     RECEPCION RESTAURANTE\n";
        cout << "====================================\n";
        cout << "Mesas disponibles:\n";
        mostrarMesas(mesas4, mesas6, mesas10);

        cout << "\n1. Llegada de cliente\n";
        cout << "2. Asignar desde lista de espera\n";
        cout << "3. Salida de clientes\n";
        cout << "4. Ver lista de espera\n";
        cout << "5. Salir\n";
        cout << "Opcion: ";
        cin >> op;

        if (op == 1) {
            string nombre;
            int personas;

            cout << "\nNombre: ";
            cin >> nombre;
            cout << "Personas: ";
            cin >> personas;

            if (personas > 10) {
                cout << "❌ Grupo rechazado\n";
            } else {
                if (!asignarDirecto(nombre, personas, mesas4, mesas6, mesas10)) {
                    encolar(cola, fin, nombre, personas);
                    cout << "⏳ Cliente agregado a lista de espera\n";
                } else {
                    atendidos++; // no hubo espera
                }
            }
        }

        else if (op == 2) {
            bool asignado;

            do {
                asignado = false;
                Nodo* anterior = NULL;

                if (mesas4 > 0) {
                    Nodo* c = buscarCliente(cola, anterior, 4);
                    if (c != NULL) {
                        eliminarNodo(cola, fin, anterior, c);
                        mesas4--;
                        double espera = difftime(time(0), c->llegada);
                        totalTiempo += espera;
                        atendidos++;
                        cout << "✔ Mesa 4 asignada a " << c->nombre << endl;
                        delete c;
                        asignado = true;
                    }
                }

                if (mesas6 > 0) {
                    Nodo* c = buscarCliente(cola, anterior, 6);
                    if (c != NULL) {
                        eliminarNodo(cola, fin, anterior, c);
                        mesas6--;
                        double espera = difftime(time(0), c->llegada);
                        totalTiempo += espera;
                        atendidos++;
                        cout << "✔ Mesa 6 asignada a " << c->nombre << endl;
                        delete c;
                        asignado = true;
                    }
                }

                if (mesas10 > 0) {
                    Nodo* c = buscarCliente(cola, anterior, 10);
                    if (c != NULL) {
                        eliminarNodo(cola, fin, anterior, c);
                        mesas10--;
                        double espera = difftime(time(0), c->llegada);
                        totalTiempo += espera;
                        atendidos++;
                        cout << "✔ Mesa 10 asignada a " << c->nombre << endl;
                        delete c;
                        asignado = true;
                    }
                }

            } while (asignado);

            cout << "✔ Asignacion completada\n";
        }

        else if (op == 3) {
            int tipo;
            cout << "\n1. Mesa 4\n2. Mesa 6\n3. Mesa 10\nOpcion: ";
            cin >> tipo;

            if (tipo == 1) {
                if (mesas4 < 8) {
                    mesas4++;
                    cout << "✔ Mesa de 4 liberada\n";
                } else {
                    cout << "❌ Error: No hay mesas de 4 ocupadas\n";
                }
            }

            if (tipo == 2) {
                if (mesas6 < 5) {
                    mesas6++;
                    cout << "✔ Mesa de 6 liberada\n";
                } else {
                    cout << "❌ Error: No hay mesas de 6 ocupadas\n";
                }
            }

            if (tipo == 3) {
                if (mesas10 < 3) {
                    mesas10++;
                    cout << "✔ Mesa de 10 liberada\n";
                } else {
                    cout << "❌ Error: No hay mesas de 10 ocupadas\n";
                }
            }

            else {
                cout << "❌ Opcion invalida\n";
            }
        }

        else if (op == 4) {
            mostrarCola(cola);
        }

    } while (op != 5);

    cout << "\n====================================\n";
    if (atendidos > 0) {
        cout << "Tiempo promedio de espera: "
             << fixed << setprecision(2)
             << totalTiempo / atendidos << " segundos\n";
    } else {
        cout << "Sin clientes atendidos\n";
    }
    system("pause");
    return 0;

}