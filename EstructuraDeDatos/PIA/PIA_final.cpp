// ============================================================
//  SISTEMA ESCOLAR IMAK  -  PIA Final
//  Opciones: Alta, Baja, Recuperar, Reportes, Inscripciones
// ============================================================

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <queue>

using namespace std;

// ============================================================
//  ESTRUCTURAS PRINCIPALES
// ============================================================

struct Alumno {
    int    matricula;
    string nombre;
    int    edad;
    double promedio;
    string direccion;
    string telefono;
};

// ----- Lista enlazada de alumnos activos -----
struct NodoLista {
    Alumno     dato;
    NodoLista* siguiente;
};

struct Lista {
    NodoLista* cabeza;
    int        cantidad;
};

// ----- Pila de bajas parciales -----
struct NodoPila {
    Alumno    dato;
    NodoPila* siguiente;
};

struct Pila {
    NodoPila* tope;
    int       cantidad;
};

// ============================================================
//  UTILIDADES
// ============================================================

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pausar() {
    cout << "\n  Presione ENTER para continuar...";
    limpiarBuffer();
    cin.get();
}

// Pide una cadena de texto hasta que no este vacia
string pedirCadena(const string& etiqueta) {
    string valor;
    do {
        cout << etiqueta;
        getline(cin, valor);
        if (valor.empty())
            cout << "  [!] El campo no puede estar vacio.\n";
    } while (valor.empty());
    return valor;
}

// Pide un telefono hasta que solo contenga digitos y no este vacio
string pedirTelefono() {
    string tel;
    bool valido;
    do {
        cout << "  Telefono   : ";
        getline(cin, tel);
        valido = !tel.empty();
        for (int i = 0; i < (int)tel.size() && valido; i++)
            if (!isdigit(tel[i])) valido = false;
        if (!valido)
            cout << "  [!] El telefono debe contener solo digitos y no estar vacio.\n";
    } while (!valido);
    return tel;
}

// ============================================================
//  OPERACIONES DE LISTA
// ============================================================

void inicializarLista(Lista& L) {
    L.cabeza   = nullptr;
    L.cantidad = 0;
}

// Busqueda binaria por matricula (lista siempre ordenada)
NodoLista* buscarMatricula(Lista& L, int matricula) {
    if (!L.cabeza) return nullptr;

    int n = L.cantidad;
    NodoLista** arr = new NodoLista*[n];

    NodoLista* actual = L.cabeza;
    for (int i = 0; i < n; i++) {
        arr[i] = actual;
        actual  = actual->siguiente;
    }

    int izq = 0, der = n - 1;
    NodoLista* resultado = nullptr;

    while (izq <= der) {
        int mid = izq + (der - izq) / 2;
        if (arr[mid]->dato.matricula == matricula) {
            resultado = arr[mid];
            break;
        } else if (arr[mid]->dato.matricula < matricula) {
            izq = mid + 1;
        } else {
            der = mid - 1;
        }
    }

    delete[] arr;
    return resultado;
}

// Busqueda secuencial por nombre (coincidencia parcial, sin importar mayusculas)
NodoLista** buscarNombre(Lista& L, const string& nombre, int& cantidad) {
    string buscar = nombre;
    for (int i = 0; i < (int)buscar.size(); i++)
        buscar[i] = tolower(buscar[i]);

    // Primera pasada: contar coincidencias
    cantidad = 0;
    NodoLista* actual = L.cabeza;
    while (actual) {
        string n = actual->dato.nombre;
        for (int i = 0; i < (int)n.size(); i++) n[i] = tolower(n[i]);
        if (n.find(buscar) != string::npos) cantidad++;
        actual = actual->siguiente;
    }

    if (cantidad == 0) return nullptr;

    // Segunda pasada: llenar arreglo de punteros
    NodoLista** arr = new NodoLista*[cantidad];
    int idx = 0;
    actual = L.cabeza;
    while (actual) {
        string n = actual->dato.nombre;
        for (int i = 0; i < (int)n.size(); i++) n[i] = tolower(n[i]);
        if (n.find(buscar) != string::npos) arr[idx++] = actual;
        actual = actual->siguiente;
    }
    return arr;
}

// Inserta en la posicion ordenada por matricula y verifica duplicados
bool insertarLista(Lista& L, const Alumno& a) {
    if (buscarMatricula(L, a.matricula)) {
        cout << "\n  [!] Ya existe un alumno con matricula " << a.matricula << ".\n";
        return false;
    }

    NodoLista* nuevo = new NodoLista;
    nuevo->dato      = a;
    nuevo->siguiente = nullptr;

    if (!L.cabeza || a.matricula < L.cabeza->dato.matricula) {
        nuevo->siguiente = L.cabeza;
        L.cabeza         = nuevo;
        L.cantidad++;
        return true;
    }

    NodoLista* actual = L.cabeza;
    while (actual->siguiente &&
           actual->siguiente->dato.matricula < a.matricula)
        actual = actual->siguiente;

    nuevo->siguiente  = actual->siguiente;
    actual->siguiente = nuevo;
    L.cantidad++;
    return true;
}

// Elimina el nodo con la matricula dada y guarda el alumno extraido
bool eliminarLista(Lista& L, int matricula, Alumno& eliminado) {
    if (!L.cabeza) return false;

    if (L.cabeza->dato.matricula == matricula) {
        eliminado        = L.cabeza->dato;
        NodoLista* tmp   = L.cabeza;
        L.cabeza         = L.cabeza->siguiente;
        delete tmp;
        L.cantidad--;
        return true;
    }

    NodoLista* actual = L.cabeza;
    while (actual->siguiente &&
           actual->siguiente->dato.matricula != matricula)
        actual = actual->siguiente;

    if (!actual->siguiente) return false;

    eliminado            = actual->siguiente->dato;
    NodoLista* tmp       = actual->siguiente;
    actual->siguiente    = tmp->siguiente;
    delete tmp;
    L.cantidad--;
    return true;
}

void liberarLista(Lista& L) {
    while (L.cabeza) {
        NodoLista* tmp = L.cabeza;
        L.cabeza       = L.cabeza->siguiente;
        delete tmp;
    }
    L.cantidad = 0;
}

// ============================================================
//  OPERACIONES DE PILA
// ============================================================

void inicializarPila(Pila& P) {
    P.tope     = nullptr;
    P.cantidad = 0;
}

void apilar(Pila& P, const Alumno& a) {
    NodoPila* nuevo  = new NodoPila;
    nuevo->dato      = a;
    nuevo->siguiente = P.tope;
    P.tope           = nuevo;
    P.cantidad++;
}

bool desapilar(Pila& P, Alumno& recuperado) {
    if (!P.tope) return false;
    recuperado       = P.tope->dato;
    NodoPila* tmp    = P.tope;
    P.tope           = P.tope->siguiente;
    delete tmp;
    P.cantidad--;
    return true;
}

void liberarPila(Pila& P) {
    while (P.tope) {
        NodoPila* tmp = P.tope;
        P.tope        = P.tope->siguiente;
        delete tmp;
    }
    P.cantidad = 0;
}

void imprimirAlumno(const Alumno& a) {
    cout << "  +------------------------------------------------+\n";
    cout << "  | Matricula : " << left << setw(35) << a.matricula        << " |\n";
    cout << "  | Nombre    : " << left << setw(35) << a.nombre           << " |\n";
    cout << "  | Edad      : " << left << setw(35) << a.edad             << " |\n";
    cout << "  | Promedio  : " << left << setw(35)
         << (to_string((int)a.promedio) + "." +
             (a.promedio - (int)a.promedio < 0.1 ? "0" : "") +
             to_string((int)((a.promedio - (int)a.promedio) * 100 + 0.5)))
         << " |\n";
    cout << "  | Direccion : " << left << setw(35) << a.direccion        << " |\n";
    cout << "  | Telefono  : " << left << setw(35) << a.telefono         << " |\n";
    cout << "  +------------------------------------------------+\n";
    cout << right; // restaurar alineacion por defecto
}

void mostrarLista(const Lista& L) {
    if (!L.cabeza) { cout << "  (Lista vacia)\n"; return; }

    cout << "  +----------+-----------------------------+------+----------+\n";
    cout << "  |Matricula | Nombre                      | Edad | Promedio |\n";
    cout << "  +----------+-----------------------------+------+----------+\n";

    NodoLista* actual = L.cabeza;
    while (actual) {
        const Alumno& a = actual->dato;
        cout << "  | " << right << setw(8) << a.matricula << " | "
             << left  << setw(27) << a.nombre.substr(0, 27) << right << " | "
             << setw(4) << a.edad << " | "
             << fixed << setprecision(2) << setw(8) << a.promedio << " |\n";
        actual = actual->siguiente;
    }

    cout << "  +----------+-----------------------------+------+----------+\n";
    cout << "  Total activos: " << L.cantidad << "\n";
}

void mostrarPila(const Pila& P) {
    if (!P.tope) { cout << "  (Pila vacia)\n"; return; }

    cout << "  +---+----------+-----------------------------+\n";
    cout << "  | # |Matricula | Nombre                      |\n";
    cout << "  +---+----------+-----------------------------+\n";

    NodoPila* actual = P.tope;
    int pos = 1;
    while (actual) {
        cout << "  | " << right << setw(1) << pos++ << " | "
             << setw(8) << actual->dato.matricula << " | "
             << left << setw(27) << actual->dato.nombre.substr(0, 27) << right << " |\n";
        actual = actual->siguiente;
    }

    cout << "  +---+----------+-----------------------------+\n";
    cout << "  Total en pila: " << P.cantidad << "\n";
}

// ============================================================
//  OPCION 1 - ALTA DE ALUMNOS
// ============================================================

void altaAlumno(Lista& L) {
    cout << "\n  === ALTA DE ALUMNO ===\n";
    Alumno a;

    // Matricula: entero positivo
    do {
        cout << "  Matricula  : ";
        cin >> a.matricula;
        limpiarBuffer();
        if (a.matricula <= 0)
            cout << "  [!] La matricula debe ser un numero positivo.\n";
    } while (a.matricula <= 0);

    // Nombre: cadena no vacia
    a.nombre = pedirCadena("  Nombre     : ");

    // Edad: entero positivo
    do {
        cout << "  Edad       : ";
        cin >> a.edad;
        limpiarBuffer();
        if (a.edad <= 0)
            cout << "  [!] La edad debe ser mayor a 0.\n";
    } while (a.edad <= 0);

    // Promedio: entre 0.0 y 100.0
    do {
        cout << "  Promedio   : ";
        cin >> a.promedio;
        limpiarBuffer();
        if (a.promedio < 0.0 || a.promedio > 100.0)
            cout << "  [!] El promedio debe estar entre 0.0 y 100.0.\n";
    } while (a.promedio < 0.0 || a.promedio > 100.0);

    // Direccion: cadena no vacia
    a.direccion = pedirCadena("  Direccion  : ");

    // Telefono: solo digitos, no vacio
    a.telefono = pedirTelefono();

    if (insertarLista(L, a)) {
        cout << "\n  Alumno \"" << a.nombre << "\" agregado correctamente.\n";
        cout << "\n  Lista actualizada:\n";
        mostrarLista(L);
    }

    pausar();
}

// ============================================================
//  OPCION 2 - BAJA DE ALUMNOS
// ============================================================

void bajaAlumno(Lista& L, Pila& P) {
    if (!L.cabeza) {
        cout << "  [!] No hay alumnos activos en el sistema.\n";
        pausar();
        return;
    }

    cout << "  Alumnos activos:\n";
    mostrarLista(L);

    cout << "\n  Como desea buscar al alumno a dar de baja?\n";
    cout << "  1. Por Matricula  (Busqueda Binaria)\n";
    cout << "  2. Por Nombre     (Busqueda Secuencial)\n";
    cout << "  0. Cancelar\n\n";
    cout << "  Opcion: ";
    int op;
    cin >> op;
    limpiarBuffer();

    NodoLista* encontrado = nullptr;

    if (op == 1) {
        cout << "\n  Ingrese la matricula: ";
        int mat;
        cin >> mat;
        limpiarBuffer();
        encontrado = buscarMatricula(L, mat);
        if (!encontrado)
            cout << "\n  No se encontro ningun alumno con matricula " << mat << ".\n";

    } else if (op == 2) {
        cout << "\n  Ingrese el nombre (o parte de el): ";
        string nom;
        getline(cin, nom);

        int cantidad = 0;
        NodoLista** resultados = buscarNombre(L, nom, cantidad);

        if (cantidad == 0) {
            cout << "\n  No se encontraron coincidencias.\n";
        } else if (cantidad == 1) {
            encontrado = resultados[0];
        } else {
            cout << "\n  Se encontraron " << cantidad << " coincidencias:\n";
            for (int i = 0; i < cantidad; i++)
                cout << "  [" << i + 1 << "] Mat: "
                     << resultados[i]->dato.matricula
                     << "  -  " << resultados[i]->dato.nombre << "\n";
            cout << "\n  Seleccione el numero del alumno: ";
            int sel;
            cin >> sel;
            limpiarBuffer();
            if (sel >= 1 && sel <= cantidad)
                encontrado = resultados[sel - 1];
            else
                cout << "\n  [!] Seleccion invalida.\n";
        }
        delete[] resultados;

    } else if (op != 0) {
        cout << "\n  Opcion invalida.\n";
    }

    if (encontrado) {
        cout << "\n  Alumno encontrado:\n";
        imprimirAlumno(encontrado->dato);

        cout << "\n  Confirma la baja de este alumno? (s/n): ";
        char conf;
        cin >> conf;
        limpiarBuffer();

        if (conf == 's' || conf == 'S') {
            Alumno eliminado;
            eliminarLista(L, encontrado->dato.matricula, eliminado);
            apilar(P, eliminado);

            cout << "\n  Alumno dado de baja y enviado a la pila.\n";
            cout << "\n  Lista actualizada:\n";
            mostrarLista(L);
            cout << "\n  Pila de bajas actualizada:\n";
            mostrarPila(P);
        } else {
            cout << "\n  [--] Baja cancelada.\n";
        }
    }

    pausar();
}

// ============================================================
//  OPCION 3 - RECUPERAR ALUMNO
// ============================================================

void recuperarAlumno(Lista& L, Pila& P) {
    if (!P.tope) {
        cout << "  La pila de bajas esta vacia. No hay alumnos que recuperar.\n";
        pausar();
        return;
    }

    cout << "  Alumnos en pila de bajas (el primero sera recuperado):\n";
    mostrarPila(P);

    cout << "\n  Alumno en el tope de la pila:\n";
    imprimirAlumno(P.tope->dato);

    cout << "\n  Desea recuperar a este alumno? (s/n): ";
    char conf;
    cin >> conf;
    limpiarBuffer();

    if (conf == 's' || conf == 'S') {
        Alumno recuperado;
        desapilar(P, recuperado);
        insertarLista(L, recuperado);

        cout << "\n  Alumno reintegrado a la lista activa.\n";
        cout << "\n  Lista actualizada:\n";
        mostrarLista(L);
        cout << "\n  Pila de bajas actualizada:\n";
        mostrarPila(P);
    } else {
        cout << "\n  [--] Recuperacion cancelada.\n";
    }

    pausar();
}

// ============================================================
//  OPCION 4 - REPORTES
// ============================================================

void reportes(const Lista& L, const Pila& P) {
    int op;
    do {
        cout << "\n  === REPORTES ===\n";
        cout << "  [1] Porcentajes de alumnos aprobados/reprobados\n";
        cout << "  [2] Datos generales de alumnos activos\n";
        cout << "  [3] Alumnos inactivos (pila de bajas)\n";
        cout << "  [0] Regresar al menu principal\n\n";
        cout << "  Opcion: ";
        cin >> op;
        limpiarBuffer();

        if (op == 1) {
            // ---- Porcentajes ----
            if (L.cantidad == 0) {
                cout << "  No hay alumnos activos.\n";
                pausar();
                continue;
            }
            int aprobados = 0;
            NodoLista* actual = L.cabeza;
            while (actual) {
                if (actual->dato.promedio >= 70.0) aprobados++;
                actual = actual->siguiente;
            }
            int reprobados = L.cantidad - aprobados;
            double pctApro = (double)aprobados  / L.cantidad * 100.0;
            double pctRepr = (double)reprobados / L.cantidad * 100.0;

            cout << "\n  +-------------------------------------------+\n";
            cout << "  |          PORCENTAJES DE DESEMPENO         |\n";
            cout << "  +-------------------------------------------+\n";
            cout << "  | Total de alumnos activos : " << left << setw(14) << L.cantidad  << " |\n";
            cout << "  | Aprobados  (promedio>=70): " << left << setw(5) << aprobados
                 << "  (" << fixed << setprecision(1) << setw(5) << pctApro << "%)  |\n";
            cout << "  | Reprobados (promedio<70) : " << left << setw(5) << reprobados
                 << "  (" << fixed << setprecision(1) << setw(5) << pctRepr << "%)  |\n";
            cout << "  +-------------------------------------------+\n";
            cout << right;
            pausar();

        } else if (op == 2) {
            // ---- Datos generales ----
            if (!L.cabeza) { cout << "  (Lista vacia)\n"; pausar(); continue; }
            cout << "\n  Datos generales de alumnos activos:\n";
            cout << "  +-----------------------------+------+----------------------+--------------+\n";
            cout << "  | Nombre                      | Edad | Direccion            | Telefono     |\n";
            cout << "  +-----------------------------+------+----------------------+--------------+\n";
            NodoLista* actual = L.cabeza;
            while (actual) {
                const Alumno& a = actual->dato;
                cout << "  | " << left  << setw(27) << a.nombre.substr(0, 27)    << right
                     << " | " << setw(4) << a.edad
                     << " | " << left  << setw(20) << a.direccion.substr(0, 20) << right
                     << " | " << left  << setw(12) << a.telefono.substr(0, 12)  << right
                     << " |\n";
                actual = actual->siguiente;
            }
            cout << "  +-----------------------------+------+----------------------+--------------+\n";
            cout << "  Total: " << L.cantidad << " alumno(s)\n";
            pausar();

        } else if (op == 3) {
            // ---- Alumnos inactivos ----
            if (!P.tope) { cout << "  (Pila vacia)\n"; pausar(); continue; }
            cout << "\n  Alumnos inactivos (pila de bajas):\n";
            mostrarPila(P);
            pausar();

        } else if (op != 0) {
            cout << "  Opcion invalida.\n";
        }

    } while (op != 0);
}

// ============================================================
//  OPCION 5 - CONTROL DE INSCRIPCIONES
// ============================================================

// Crea la cola ordenando por promedio desc, luego nombre asc
void crearColaInscripciones(const Lista& L, queue<Alumno>& cola) {
    // Vaciar cola anterior
    while (!cola.empty()) cola.pop();

    if (!L.cabeza) { cout << "  No hay alumnos activos.\n"; return; }

    // Copiar alumnos a arreglo dinamico para poder ordenarlos
    int n = L.cantidad;
    Alumno* arr = new Alumno[n];
    NodoLista* actual = L.cabeza;
    for (int i = 0; i < n; i++) {
        arr[i] = actual->dato;
        actual  = actual->siguiente;
    }

    // Ordenar: promedio DESC; si empatan, nombre ASC  (burbuja)
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bool intercambiar = false;
            if (arr[j].promedio < arr[j + 1].promedio)
                intercambiar = true;
            else if (arr[j].promedio == arr[j + 1].promedio &&
                     arr[j].nombre   >  arr[j + 1].nombre)
                intercambiar = true;

            if (intercambiar) {
                Alumno tmp = arr[j];
                arr[j]     = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }

    for (int i = 0; i < n; i++) cola.push(arr[i]);
    delete[] arr;

    cout << "\n  Cola de inscripciones creada con " << n << " alumno(s).\n";
}

void mostrarCola(queue<Alumno> colaAux) {
    if (colaAux.empty()) { cout << "\n  La cola esta vacia.\n"; return; }

    cout << "\n  === COLA DE INSCRIPCIONES (frente -> final) ===\n";
    cout << "  +----------+-----------------------------+----------+\n";
    cout << "  |Matricula | Nombre                      | Promedio |\n";
    cout << "  +----------+-----------------------------+----------+\n";

    while (!colaAux.empty()) {
        const Alumno& a = colaAux.front();
        cout << "  | " << right << setw(8) << a.matricula << " | "
             << left  << setw(27) << a.nombre.substr(0, 27) << right << " | "
             << fixed << setprecision(2) << setw(8) << a.promedio << " |\n";
        colaAux.pop();
    }

    cout << "  +----------+-----------------------------+----------+\n";
}

void inscribirAlumnos(queue<Alumno>& cola) {
    if (cola.empty()) {
        cout << "\n  [!] La cola esta vacia. Primero cree la cola (opcion 1).\n";
        pausar();
        return;
    }

    int grupos, capacidad;
    cout << "\n  Cuantos grupos desea crear  : ";
    cin >> grupos;
    limpiarBuffer();
    cout << "  Capacidad por grupo         : ";
    cin >> capacidad;
    limpiarBuffer();

    // Arreglo dinamico de arreglos para cada grupo
    Alumno** listaGrupos = new Alumno*[grupos];
    int*     tamGrupos   = new int[grupos];
    for (int i = 0; i < grupos; i++) {
        listaGrupos[i] = new Alumno[capacidad];
        tamGrupos[i]   = 0;
    }

    // Desencolar y llenar grupos en orden
    for (int i = 0; i < grupos && !cola.empty(); i++) {
        cout << "\n  === LLENANDO GRUPO " << i + 1 << " ===\n";
        while (!cola.empty() && tamGrupos[i] < capacidad) {
            Alumno a = cola.front();
            cola.pop();
            listaGrupos[i][tamGrupos[i]++] = a;
            cout << "  Inscrito: [" << a.matricula << "] "
                 << a.nombre << "  -  Promedio: "
                 << fixed << setprecision(2) << a.promedio << "\n";
        }
    }

    // Mostrar resumen final de grupos
    cout << "\n\n  === GRUPOS FINALES ===\n";
    for (int i = 0; i < grupos; i++) {
        cout << "\n  GRUPO " << i + 1
             << " (" << tamGrupos[i] << " alumno(s)):\n";
        if (tamGrupos[i] == 0) {
            cout << "  Sin alumnos.\n";
            continue;
        }
        cout << "  +----------+-----------------------------+----------+\n";
        cout << "  |Matricula | Nombre                      | Promedio |\n";
        cout << "  +----------+-----------------------------+----------+\n";
        for (int j = 0; j < tamGrupos[i]; j++) {
            const Alumno& a = listaGrupos[i][j];
            cout << "  | " << right << setw(8) << a.matricula << " | "
                 << left  << setw(27) << a.nombre.substr(0, 27) << right << " | "
                 << fixed << setprecision(2) << setw(8) << a.promedio << " |\n";
        }
        cout << "  +----------+-----------------------------+----------+\n";
    }

    if (!cola.empty())
        cout << "\n  [!] Quedaron " << cola.size()
             << " alumno(s) sin inscribir (grupos llenos).\n";

    // Liberar arreglos dinamicos
    for (int i = 0; i < grupos; i++) delete[] listaGrupos[i];
    delete[] listaGrupos;
    delete[] tamGrupos;

    pausar();
}

void controlInscripciones(const Lista& L) {
    queue<Alumno> cola;
    int op;
    do {
        cout << "\n  === CONTROL DE INSCRIPCIONES ===\n";
        cout << "  [1] Crear cola de inscripciones\n";
        cout << "  [2] Mostrar cola\n";
        cout << "  [3] Inscribir alumnos en grupos\n";
        cout << "  [0] Regresar al menu principal\n";
        cout << "  Alumnos en cola: " << cola.size() << "\n\n";
        cout << "  Opcion: ";
        cin >> op;
        limpiarBuffer();

        switch (op) {
            case 1: crearColaInscripciones(L, cola); pausar(); break;
            case 2: mostrarCola(cola);               pausar(); break;
            case 3: inscribirAlumnos(cola);                    break;
            case 0:                                            break;
            default: cout << "  Opcion invalida.\n";
        }
    } while (op != 0);
}

// ============================================================
//  MENU PRINCIPAL
// ============================================================

int main() {
    Lista lista;
    Pila  pila;
    inicializarLista(lista);
    inicializarPila(pila);

    int op;
    do {
        cout << "\n\n";
        cout << "  +================================================+\n";
        cout << "  |          SISTEMA ESCOLAR IMAK                  |\n";
        cout << "  +================================================+\n";
        cout << "  | [1]  Alta de Alumnos                           |\n";
        cout << "  | [2]  Baja de Alumnos                           |\n";
        cout << "  | [3]  Recuperar Alumno                          |\n";
        cout << "  | [4]  Reportes                                  |\n";
        cout << "  | [5]  Control de Inscripciones                  |\n";
        cout << "  | [0]  Salir                                     |\n";
        cout << "  +================================================+\n";
        cout << "  Activos: " << setw(4) << lista.cantidad
             << "   |   En pila: " << setw(4) << pila.cantidad << "\n";
        cout << "\n  Seleccione una opcion: ";
        cin >> op;
        limpiarBuffer();

        switch (op) {
            case 1: altaAlumno(lista);                    break;
            case 2: bajaAlumno(lista, pila);              break;
            case 3: recuperarAlumno(lista, pila);         break;
            case 4: reportes(lista, pila);                break;
            case 5: controlInscripciones(lista);          break;
            case 0: cout << "\n  Saliendo...!\n\n";       break;
            default: cout << "\n  Opcion invalida.\n"; pausar();
        }

    } while (op != 0);

    liberarLista(lista);
    liberarPila(pila);
    return 0;
}
