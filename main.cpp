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

    // cout << "Numero de arestas : " << grafo.getNumAresta() << endl;
    // cout << "Numero de arestas : " << grafo.getNumAresta2() << endl;

    cout << endl << "num arestas : " << grafo.getNumAresta2() << endl;
    grafo.fechoTransitivoIndireto(1);

    // No *vetnos = grafo.fechoTransitivo(1);
    // while(vetnos){
    //     cout << vetnos->getIdNo() << " - ";
    //     vetnos = vetnos->getProxNo();
    // }
    // cout << endl;

    return 1;
}
