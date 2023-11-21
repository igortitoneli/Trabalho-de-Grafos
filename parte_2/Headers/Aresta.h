#ifndef ARESTA_H
#define ARESTA_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include "No.h"

using namespace std;

class No;

class Aresta
{
public:
    // Aresta(int noDestino);
    // Aresta(int noDestino, int pesoAresta);
    // Aresta(No *noDestino, Aresta *proxAresta);
    Aresta(No *noDestino, Aresta *proxAresta, float peso);

    ~Aresta();

    float getPeso();
    void setPeso(int peso);
    void setNoDestino(No *destino);
    No *getNoDestino();
    void setProxAresta(Aresta *prox);
    Aresta *getProxAresta();

    // void setProxAresta(Aresta *valor);

private:
    float peso;
    No *noDestino;
    Aresta *proxAresta;
};

#endif // ARESTA_H
