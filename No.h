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
    No();
    No(int idNo);
    No(int idNo, int weight);
    virtual ~No();

    int getIdNo();
    Aresta* getPrimeiraAresta();
    Aresta* getUltimaAresta();
    void setIdNo(int id);
    void setPeso(int peso);
    int getPeso();
    int getGrau();
    void incGrau();
    void decGrau();
    int getGrauEntrada();
    void incrementaGrauEntrada();
    void decrementaGrauEntrada();
    int getGrauSaida();
    void incrementaGrauSaida();
    void decrementaGrauSaida();
    void setProxNo(No *novoNo);
    No* getProxNo();
    void setPrimeiraAresta(Aresta *novaAresta);
    void setUltimaAresta(Aresta *novaAresta);
    void setOrdemMaisUm();
    bool procuraAresta(Aresta *aresta, No *noDestino);
    Aresta* procuraAresta(No* noProcurado);
    bool verificaRemoveAresta(No *destino);
    void removeAresta(No *noRemovido);
    Aresta *verificaNoAresta(No *procurado);

    Aresta* getArestaAnterior(No *noDestino);
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
