#ifndef NO_H
#define NO_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include "../Headers/Aresta.h"

using namespace std;

class No {
public:

    No(int idNo, float x, float y);
    virtual ~No();

    // get()
    int getIdNo() { return this->idNo; };
    Aresta* getPrimeiraAresta() { return this->primeiraAresta; };
    Aresta* getUltimaAresta() { return this->ultimaAresta; };
    Aresta* getArestaAnterior(No *noDestino);
    // int getPeso(){ return this->pesoNo; } 
    int getGrau(){ return this->grauSaida; };
    int getGrauEntrada() { return this->grauEntrada; };
    int getGrauSaida() { return this->grauSaida; };
    No* getProxNo() { return this->proxNo; };
    int getX() { return this->x; };
    int getY() { return this->y; };
    int getDemanda() { return this->demanda; };

    // set()
    void setIdNo(int id) { this->idNo = id; };
    void setDemanda(int demanda) { this->demanda = demanda; };
    // void setPeso(int peso) { this->pesoNo = peso; };
    void setProxNo(No *novoNo) { this->proxNo = novoNo; };
    void setPrimeiraAresta(Aresta *novaAresta) { this->primeiraAresta = novaAresta; };
    void setUltimaAresta(Aresta *novaAresta) { this->ultimaAresta = novaAresta; };
    // void setOrdemMaisUm() { this->pesoNo++; };

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
    bool insereArestaNo(No *Destino, double pesoAresta);
    bool in_percorridos(No percorridos [], int tam);
    
private:

    Aresta* auxGetArestaAnterior(Aresta *aresta, No *noDestino);

    int idNo;
    int demanda;
    float x;
    float y;
    int grauEntrada;
    int grauSaida;
    No *proxNo;    
    Aresta *primeiraAresta;
    Aresta *ultimaAresta;
};

#endif // NO_H
