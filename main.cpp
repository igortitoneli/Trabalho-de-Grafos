#include <iostream>
#include <stdlib.h>
#include <string>
#include "Grafo.cpp"
#include "Aresta.cpp"
#include "No.cpp"

using namespace std;

int main()
{
    int n, i, j, val=1;
    

    // do{
    //     cout << "Direcionado ou nao direcionado? 1/0   :   ";
    //     cin >> val;
    // }while(val!=0 && val!=1);

    bool direcionado = val;
    
    // cout << "Digite o numero de arestas : ";
    // cin >> n;
    
    Grafo grafo(direcionado, 0 , 0);
    
    // cout << "Nó (i j)" << endl;
    // for(int k=0; k<n; k++)
    // {
    //     cin >> i >> j;
    //     grafo.insertAresta(i,j,0,0,direcionado);    
    // }
    grafo.insertAresta(2,3,0,0,direcionado);
    grafo.insertAresta(2,4,0,0,direcionado);
    grafo.insertAresta(2,5,0,0,direcionado);
    grafo.insertAresta(3,4,0,0,direcionado);
    grafo.insertAresta(3,5,0,0,direcionado);
    grafo.insertAresta(4,5,0,0,direcionado);
    grafo.insertAresta(1,2,0,0,direcionado);
    grafo.insertAresta(1,3,0,0,direcionado);
    grafo.insertAresta(4,1,0,0,direcionado);
    grafo.insertAresta(5,1,0,0,direcionado);

    grafo.fechoTransitivoDireto(4);
    grafo.fechoTransitivoIndireto2(4);

    return 1;
}
