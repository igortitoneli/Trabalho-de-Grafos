#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "No.h"
#include "Aresta.h"

using namespace std;

class Grafo {
public:

    //--- Construtor e Destrutor ---
    Grafo(bool isDigrafo, bool weightedNo, bool weightedAresta);
    Grafo(int ordem, bool isDigrafo, bool weightedNo, bool weightedAresta);
    virtual ~Grafo();

    //--- Funcoes do No ---
    No *procurarNoPeloId(int idFindNo, bool anterior = false);
    No *insereNo(int idNostrings, int pesoNo);
    bool removeNo(int idNo, bool isDigrafo);
    No *getNoRaiz() { return this->noRaiz; };

    void fechoTransitivoDireto(ofstream &output_file, int idNo);
    void fechoTransitivoIndireto(ofstream &output_file, int idNo);
    bool getWeightedNode(){return this->weightedNo;};

    //--- Funcoes de Aresta ---
    bool insertAresta(int idNoOrigem, int idNoDestino, int pesoAresta);
    bool removeAresta(int idNoOrigem, int idNoDestino, bool isDirected);
    bool criaAresta(No *noFonte, No *Destino, int pesoAresta);
    void removeAresta(No *anterior, No *noAux);
    bool getWeightedEdge (){return this->weightedAresta;};

    //--- Caracteristica do Grafo ---
    void imprime(ofstream &output_file);
    float Dijkstra(int idNoinicio, int idNofim);
    Grafo* caminhoEmProfundidade(ofstream &output_file, int idNo);
    void escreveArquivoSaida(ofstream &output_file, string mensagem);
    
    void sdecOrdem();
    // Aresta* getArestaAux();
    string getGrau();
    bool isDigraph();  
    int* Floyd(ofstream &output_file, int idNoinicio, int idNofim);

    Grafo* prim(ofstream &output_file, int idNo);

private:
    //--- Seters ---
    void incOrdem();
    void decOrdem();


    int AtualizaNumAresta();
    int getNumAresta();
    int getOrdem();
    int getGrauEntrada();
    int getGrauSaida();
    bool decrementaNumAresta();
    void verificaGrau();
   

    void imprimeVetor(int vetor[], int tam);

   
    // int grau; // vertice com maior grau do grafo
    bool estarNoVetor(int vetor[], int idNo, int tam);
    bool arestaNoVetor(int vetor[], No* noAtual, int tam);
    void auxFechoTransitivoDireto(No* no, int* percorridos, int tam, int* n);

    int ordem, numAresta; // numero de vertices do grafo
    bool digrafo, weightedNo, weightedAresta;
    

    No *noRaiz;
};


#endif // GRAPH_H
