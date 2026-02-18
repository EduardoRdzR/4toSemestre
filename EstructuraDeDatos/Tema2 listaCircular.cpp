#include <iostream>

using namespace::std;

struct nodo{
	int dato;  //campo de información
	nodo* siguiente;  //campo liga, apuntador a nodo siguiente
} *primero, *ultimo; // apuntadores al primer y ultimo elemento, control de nodos y recorridos ---- son apuntadores que pertenecen a la estructura de nodo y se encuentran vacios


void insertarNodo(); // declaracion de procedimientos
void desplegarLista();


int main(){
	insertarNodo(); // se esta definiendo por default que se soliciten 5 datos númericos
	insertarNodo();
	insertarNodo();
	insertarNodo();
	insertarNodo();
	cout << " \n\n LISTA CIRCULAR SIMPLE INGRESADA\n";
	desplegarLista(); //se muestra la lista en pantalla
	return 0;
}

 
void insertarNodo(){
	nodo* nuevo = new nodo(); // se esta declarando un apuntador a un nuevo nodo, solicito espacio en memoria
	cout << " Ingrese el dato que contendra el nuevo Nodo: "; //solicito campo de información
	cin >> nuevo->dato; //lo almaceno
	
	if(primero==NULL){   // si el apontador a primero es null quiere decir que estamos frete a una lista vacia
		primero = nuevo; //hacemos entonces que primero apunte hacia el nuevo nodo a insertar
		primero->siguiente = primero; // y hacemos que el apuntador a nodo siguiente de primero que antes apuntaba a null apunte a el mismo
		ultimo = primero; // posicionamos el apuntador ultimo en primero
	}else{ //en caso de que no sea el primer nodo que se inserta
		ultimo->siguiente = nuevo; // vamos a hacer que el apuntador a nodo siguiente de ultimo apunte hacia nuevo
		nuevo->siguiente = primero; // primero ya no se va a mover, vamos a hacer que el apuntador a nodo siguiente del nuevo nodo apunte a primero
		ultimo = nuevo; //el apuntador a ultimo se mueve a nuevo
	}
	cout << "\n Nodo Ingresado\n\n";
}

void desplegarLista(){  //procedimiento para recorrer la lista circular
	nodo* actual = new nodo();  //vamos a reservar un espacio en memoria para el nodo actual
	actual = primero; // hacemos que actual apunte hacia el primero nodo
		if(primero!=NULL){  // preguntamos el primer nodo es diferente de null
		do{ // si es si entramos a un do while que me va a permitir ir recorriendo la lista
			cout << "\n " << actual->dato; // mando imprimir en pantalla el campo de información donde se encuentra actual
			actual = actual->siguiente; // vpy a mover actual hacia el siguiente nodo
		}while(actual!=primero); // esto se va a repetir mientra no lleguemos nuevamente a apuntar al primer nodo
	}else{ // si primero no es diferente de null quiere decir que la lista se encuentra vacia
		cout << "\n La lista se encuentra vacia\n\n"; // imprimimos en pantalla lista vacia
	} 
}



