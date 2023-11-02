#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "No.h"
#include "Aresta.h"

using namespace std;

class No;
class Aresta;

class Grafo {
public:

    //--- Construtor e Destrutor ---
    Grafo(bool isDigrafo, bool weightArc, bool weightNo);
    virtual ~Grafo();

    //--- Seters ---
    void incOrdem();
    void decOrdem();

    //--- Funcoes do No ---
    No *procurarNoPeloId(int idFindNo, bool anterior);
    No *insereNo(int idNostrings, int pesoNo);
    bool removeNo(int idNo, bool isDigrafo);
    // Aresta* fechoTransitivo(int idNo);
    void fechoTransitivoDireto(int idNo);
    void fechoTransitivoIndireto(int idNo);
    bool estarNoVetor(int vetor[], int idNo, int tam);
    bool arestaNoVetor(int vetor[], No* noAtual, int tam);
    void imprimeVetor(int vetor[], int tam);

    //--- Funcoes de Aresta ---
    bool insertAresta(int idNoOrigem, int idNoDestino, int pesoAresta);
    bool removeAresta(int idNoOrigem, int idNoDestino, bool isDirected);
    bool criaAresta(No *noFonte, No *Destino, int pesoAresta);
    void removeAresta(No *anterior, No *noAux);

    //--- Caracteristica do Grafo ---
    int AtualizaNumAresta();
    int getNumAresta();
    No *getNoRaiz() { return this->noRaiz; };
    int getOrdem();
    int getGrauEntrada();
    int getGrauSaida();
    bool decrementaNumAresta();
    void verificaGrau();
    void imprime();
    int Djkstra(int idNoinicio, int idNofim);
    
    void sdecOrdem();
    // Aresta* getArestaAux();
    string getGrau();
    bool isDigraph();  
    int Floyd(int idNoinicio, int idNofim);
private:
    // int grau; // vertice com maior grau do grafo

    int ordem; // numero de vertices do grafo
    int numAresta;
    bool digrafo;
    bool weigthNo;
    bool weightArc;

    No *noRaiz;
};


#endif // GRAPH_H
