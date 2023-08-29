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
    No *procurarNoPeloId(int idFindNo);
    No *insereNo(int idNostrings, int pesoNo);
    bool removeNo(int idNo, bool isDigrafo);

    //--- Funcoes de Aresta ---
    bool insertAresta(int idNoOrigem, int idNoDestino, int pesoAresta, bool weigthArc, bool isDirected);
    bool removeAresta(int idNoOrigem, int idNoDestino, bool isDirected);
    

    //--- Caracteristica do Grafo ---
    int getNumAresta();
    No *getNoRaiz();
    int getOrdem();
    int getGrauEntrada();
    int getGrauSaida();
    
    void sdecOrdem();
    int removeArestas(int idNoOrigem, int idNoDestino, bool isDirected);
    Aresta* getArestaAux();
    string getGrau();
    bool isDigraph();

private:

    int ordem;
    int numAresta;
    bool digrafo;
    bool weigthNo;
    bool weightArc;

    No *noRaiz;

};


#endif // GRAPH_H
