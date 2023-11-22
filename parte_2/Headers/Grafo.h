#ifndef GRAFO_H
#define GRAFO_H
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "../Headers/No.h"

using namespace std;

class Grafo {
public:

    //--- Construtor e Destrutor ---
    Grafo();
    virtual ~Grafo();

    //--- Seters ---
    void incOrdem();
    void decOrdem();

    //--- Funcoes do No ---
    No *procurarNoPeloId(int idFindNo, bool anterior);
    No *insereNo(int idNo, float x, float y, int demanda);
    No *insereNo(No* novoNo);
    bool removeNo(int idNo, bool isDigrafo);
    // Aresta* fechoTransitivo(int idNo);
    void fechoTransitivoDireto(int idNo);
    void fechoTransitivoIndireto(int idNo);
    void fechoTransitivoIndiretoFunc(int idNo);
    void imprimeVetor(int vetor[], int tam);
    No* getGalpao();

    //--- Funcoes de Aresta ---
    No* insertAresta(No* NoOrigem, No* NoDestino, double pesoAresta);
    bool insertAresta(int idNoOrigem, int idNoDestino, float pesoAresta);
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
    Grafo* caminhoEmProfundidade(int idNo);
    
    void sdecOrdem();
    // Aresta* getArestaAux();
    string getGrau();
    bool isDigraph();  
    int* Floyd(int idNoinicio, int idNofim);

    Grafo* prim(int idNo);

    double somaPesoArestas(); //implementar no cpp

private:
    // int grau; // vertice com maior grau do grafo
    bool estarNoVetor(int vetor[], int idNo, int tam);
    bool arestaNoVetor(int vetor[], No* noAtual, int tam);
    void auxFechoTransitivoDireto(No* no, int* percorridos, int tam, int* n);

    int ordem; // numero de vertices do grafo
    int numAresta;
    bool digrafo;

    No *noRaiz;
};



#endif // GRAPH_H
