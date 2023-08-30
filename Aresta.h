#ifndef ARESTA_H
#define ARESTA_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include "No.h"

using namespace std;

class No;

class Aresta {
public:
    Aresta(int noDestino);
    Aresta(int noDestino, int pesoAresta);
    Aresta(No *noDestino, Aresta *proxAresta);
    Aresta(No *noDestino, Aresta *proxAresta, int peso);

    ~Aresta();

    int getPeso();
    void setPeso(int valor);
    void setNoDestino(No *valor);
    No* getNoDestino();
    void setProxAresta(Aresta *valor);
    Aresta* getProxAresta();

    
    //void setProxAresta(Aresta *valor);
    
private:
    int peso;
    No *noDestino;
    Aresta *proxAresta;
};

#endif // ARESTA_H
