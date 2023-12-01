#ifndef SOLUCAO_H
#define SOLUCAO_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "./No.h"
#include "./Grafo.h"

using namespace std;

class Solucao{
    public:
        Solucao(string txt);
        
        ~Solucao();
        Grafo* guloso(ofstream &output_file);
        Grafo* gulosoRandomizadoAdaptativo(ofstream &output_file, float param_1, int maxIter);
        Grafo* gulosoRandomizadoAdaptativoReativo(ofstream &output_file, float param_1, int maxIter);
        double custoMinimo(Grafo *grafo);

    private:
        int gerarNumeroAleatorio(int inicio, int fim);
        void lerArquivo(string txt);
        void construirArestas();
        void construirMatriz();
        void imprimeMatriz();
        No* findMinDistance(No* partida, vector<int> percorridos,  double capacidade);
        bool inPercorridos(No* procurado, vector<int> percorridos);
        unordered_map<int, bool> initHash();
        unordered_map<int, No*> initHashMenorCaminho(No* galpao);
        bool checadosHash(unordered_map<No*,bool> hash);
        unordered_map<int, double> setCapacidade();



        Grafo* grafo;
        unordered_map<int, unordered_map<int, float>> matrizDistancias;  
        int capacidade;
        int caminhoes;
        No* galpao;
};


#endif // SOLUCAO_H
