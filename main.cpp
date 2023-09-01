#include <iostream>
#include <stdlib.h>
#include <string>
#include "Grafo.cpp"
#include "Aresta.cpp"
#include "No.cpp"

using namespace std;

int main()
{
    int n, i, j, val;
    

    do{
        cout << "Direcionado ou nao direcionado? 1/0   :   ";
        cin >> val;
    }while(val!=0 && val!=1);

    bool direcionado = val;
    
    cout << "Digite o numero de nos : ";
    cin >> n;
    
    Grafo grafo(direcionado, 0 , 0);
    
    cout << "Nó (i j)" << endl;
    for(int k=0; k<n; k++)
    {
        cin >> i >> j;

        grafo.insertAresta(i,j,0,0,direcionado);
    }
    
    // grafo.fechoTransitivoIndireto(1);

    // grafo.removeAresta(i, j, direcionado);
    cout << "ordem do grafo antes : " << grafo.getOrdem() << endl;

    cout << endl << "num arestas : " << grafo.getNumAresta() << endl;

    grafo.removeNo(i, direcionado);

    cout << "ordem do grafo depois : " << grafo.getOrdem() << endl;

    // cout << endl << "num arestas  : " << grafo.AtualizaNumAresta() << endl;
    
    cout << endl << "num arestas  : " << grafo.getNumAresta() << endl;
    
    return 1;
}
