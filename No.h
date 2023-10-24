#ifndef NO_H
#define NO_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include "Aresta.h"

using namespace std;
class Aresta;

class No {
public:

    No(int idNo, int weight);
    virtual ~No();

    // get()
    int getIdNo() { return this->idNo; };
    Aresta* getPrimeiraAresta() { return this->primeiraAresta; };
    Aresta* getUltimaAresta() { return this->ultimaAresta; };
    Aresta* getArestaAnterior(No *noDestino);
    int getPeso(){ return this->pesoNo; } 
    int getGrau(){ return this->grauSaida; };
    int getGrauEntrada() { return this->grauEntrada; };
    int getGrauSaida() { return this->grauSaida; };
    No* getProxNo() { return this->proxNo; };

    // set()
    void setIdNo(int id) { this->idNo = id; };
    void setPeso(int peso) { this->pesoNo = peso; };
    void setProxNo(No *novoNo) { this->proxNo = novoNo; };
    void setPrimeiraAresta(Aresta *novaAresta) { this->primeiraAresta = novaAresta; };
    void setUltimaAresta(Aresta *novaAresta) { this->ultimaAresta = novaAresta; };
    void setOrdemMaisUm() { this->pesoNo++; };

    // grau
    void incGrau() { this->grauEntrada++; this->grauSaida++; };
    void decGrau() { this->grauEntrada--; this->grauSaida--; };
    void incrementaGrauEntrada() { this->grauEntrada++; };
    void decrementaGrauEntrada() { this->grauEntrada--; };
    void incrementaGrauSaida() { this->grauSaida++; };
    void decrementaGrauSaida() { this->grauSaida--; };

    // procura()
    bool procuraAresta(Aresta *aresta, No *noDestino);
    Aresta* procuraAresta(No* noProcurado);

    // func()
    bool verificaRemoveAresta(No *destino);
    void removeAresta(No *noRemovido);
    Aresta *verificaNoAresta(No *procurado);
    void imprimeArestas();
    bool insereArestaNo(No *Destino, int pesoAresta);
    bool in_percorridos(No percorridos [], int tam);
    
private:

    Aresta* auxGetArestaAnterior(Aresta *aresta, No *noDestino);

    int idNo;
    int pesoNo;
    int grauEntrada;
    int grauSaida;
    No *proxNo;    
    Aresta *primeiraAresta;
    Aresta *ultimaAresta;
};

#endif // NO_H
