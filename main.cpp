#include <iostream>
#include <stdlib.h>
#include <string>
#include "Grafo.cpp"
#include "Aresta.cpp"
#include "No.cpp"

using namespace std;

int main()
{
   // int n, i, j, val=1;
    

    // do{
    //     cout << "Direcionado ou nao direcionado? 1/0   :   ";
    //     cin >> val;
    // }while(val!=0 && val!=1);

    bool direcionado = 1;
    
    // cout << "Digite o numero de arestas : ";
    // cin >> n;
    
    Grafo grafo(direcionado, 0 , 0);
    
    // cout << "Nó (i j)" << endl;
    // for(int k=0; k<n; k++)
    // {
    //     cin >> i >> j;
    //     grafo.insertAresta(i,j,0,0,direcionado);    
    // }
    grafo.insertAresta(0,1,2,0);
    grafo.insertAresta(0,3,1,0);
    grafo.insertAresta(0,2,6,0);
    grafo.insertAresta(2,4,1,0);
    grafo.insertAresta(1,0,2,0);
    grafo.insertAresta(1,4,6,0);
    grafo.insertAresta(1,3,3,0);
    grafo.insertAresta(3,1,3,0);
    grafo.insertAresta(3,0,1,0);
    grafo.insertAresta(3,4,5,0);
    grafo.insertAresta(4,1,6,0);
    grafo.insertAresta(4,3,5,0);

    grafo.fechoTransitivoDireto(4);
    // grafo.imprime();
    grafo.fechoTransitivoIndireto(3);

    // cout << "menor caminho: " << grafo.Djkstra(0,4) << endl;

    return 1;
}
