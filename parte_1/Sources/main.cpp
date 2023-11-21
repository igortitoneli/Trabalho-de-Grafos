#include <iostream>
#include <stdlib.h>
#include <string>
#include "Grafo.cpp"
#include "Aresta.cpp"
#include "No.cpp"
// #include "Solucao.cpp"

using namespace std;

int main()
{
    bool direcionado = 0;
    
    Grafo grafo(direcionado);
    
    grafo.insertAresta(1, 2, 7);
    grafo.insertAresta(1, 4, 3);
    grafo.insertAresta(2, 3, 2);
    grafo.insertAresta(4, 3, 1);
    
    grafo.imprime();
    grafo.fechoTransitivoIndireto(3);

    return 1;
}


// Solução
// int main(){
//     Solucao("B-n50-k8.vrp.txt");
// }