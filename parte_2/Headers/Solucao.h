#ifndef SOLUCAO_H
#define SOLUCAO_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include "./No.h"
#include "./Grafo.h"

using namespace std;

class Solucao{
    public:
        Solucao(string txt);
        ~Solucao();

        Grafo* guloso();
        Grafo* gulosoRandomizadoAdaptativo();
        Grafo* gulosoRandomizadoAdaptativoReativo();

    private:
        void lerArquivo(string txt);
        void construirArestas();
        void construirMatriz();
        void imprimeMatriz();

        Grafo* grafo;
        unordered_map<No*, unordered_map<No*, float>> matrizDistancias;  
        int capacidade;
        int caminhoes;
};


#endif // SOLUCAO_H
