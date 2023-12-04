#include <iostream>
#include <stdlib.h>
#include <string>
#include "../Headers/Aresta.h"

using namespace std;

/**
 * Construtor sobrescrito.
 * 
 * @param *noDestino (new valor)
 * @param *proxAresta (new valor)
 * @param peso (new valor)
 */
Aresta::Aresta(No *noDestino, Aresta *proxAresta, float peso=1) {
    this->peso = peso;
    this->noDestino = noDestino;
    this->proxAresta = proxAresta;
}

/** Destructor */
Aresta::~Aresta() {
    delete noDestino;
    delete proxAresta;
}

/**
 * A funcao define peso valor.
 * 
 * @param valor (new valor)
 */
void Aresta::setPeso(int peso) {
    this->peso = peso;
}

/**
 * A funcao retorna peso valor.
 */
float Aresta::getPeso() {
    return this->peso;
}

/**
 * A funcao define noDestino.
 * 
 * @param valor (new valor)
 */
void Aresta::setNoDestino(No *destino) {
    this->noDestino = destino;
}

/**
 * A funcao retorna noDestino.
 */
No* Aresta::getNoDestino() {
    return this->noDestino;
}

/**
 * A funcao define proxAresta.
 * 
 * @param valor (new valor)
 */
void Aresta::setProxAresta(Aresta *prox) {
    this->proxAresta = prox;
}

/**
 * A funcao retorna proxAresta;
 */
Aresta* Aresta::getProxAresta() {
    return this->proxAresta;
}
