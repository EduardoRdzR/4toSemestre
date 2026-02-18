//Programa que va insertando nuevos elementos al principio de la lista
#include<iostream>
using namespace std;

struct nodo{
            int nro; // campo de información en el nodo
            struct nodo *sgte; // campo liga
           };
int main()
{ 
  struct nodo Lista1; // creamos nuestro apuntador a nodo cabecera
  struct nodo *q;  // apuntador al nuevo nodo a insertar
  int i, n;
  
  Lista1.sgte=NULL; //aquí asignamos a nuestro nodo cabecera null -- lista vacia
  
  cout<<"Numero de elementos:";
  cin>>n;  
  
  for(i=0;i<n;i++)
  {
    cout<<"Elemento:"<<(i+1)<<endl;  
    q=new(struct nodo); // donde estamos creando el espacio en memoria para el nuevo nodo.
    cin>>q->nro; //asigno el valor que el usuario introduce al campo de información de nuestro nuevo nodo
    q->sgte=Lista1.sgte; //hacemos que el campo liga del nodo aux q apunte ahora hacia null ya que es el nuevo nodo insertado
    Lista1.sgte=q; //hacemos que nuestro nodo cabecera apunte a q que es el nuevo nodo cabecera
  }
 
  cout<<endl<<"Listado:"<<endl;
  
  q=Lista1.sgte; // hacemos que nuestro nodo aux apunte al nodo cabecera, en este caso es el último elemento insertado
  
  while(q!=NULL) //Mientras apuntador q sea diferente de null
  
  {
    cout<<q->nro<<endl;
    q=q->sgte; // modificACIÓN DEL PUNTERO
  }

  return 0;
}
