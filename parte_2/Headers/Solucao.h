#ifndef SOLUCAO_H
#define SOLUCAO_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "./No.h"
#include "./Grafo.h"
#include <exception>

using namespace std;


struct rota{
    No* ultimoNo;
    double cargaAtual = 0;
    vector<int> percurso = { 1 };
    double distancia = 0.0;
};


struct candidato{
    No* no;
    double euristica = 0;
};


class Solucao{
    public:
        Solucao(string txt);
        
        ~Solucao();
        unordered_map<int,rota> guloso(ofstream &output_file);
        unordered_map<int,rota> gulosoRandomizadoAdaptativo(ofstream &output_file, float alpha, int maxIter);
        unordered_map<int,rota> gulosoRandomizadoAdaptativoReativo(ofstream &output_file, float alpha, int maxIter);
        double custoMinimo(Grafo *grafo);
        void imprimeRotas(bool completa) const;

    private:
        int gerarNumeroAleatorio(int inicio, int fim);
        void lerArquivo(string txt);
        void construirArestas();
        void construirMatriz();
        void imprimeMatriz();
        void escreveRotas(ofstream &output_file, bool completa, int iter);
        
        
        No* findMinDistance(No* partida, vector<int> percorridos,  double capacidade);
        bool inPercorridos(No* procurado, vector<int> percorridos);
        unordered_map<int, bool> initHash();
        unordered_map<int, No*> initHashMenorCaminho(No* galpao);
        bool checadosHash(unordered_map<No*,bool> hash);
        unordered_map<int, double> setCapacidade();
        
        
        bool noValido(No* destino , vector<int> percorridos, int i, unordered_map<int, rota> rotas);
        void makeCandidatos();
        void makeCandidatosRandomizado(float alpha);
        void atualizaCandidatos(No* alterado);
        void atualizaCandidatosRandomizado(No* alterado, float alpha);
        int getCandidatos();
        bool parar();
        void atualizaRota(int i, No* destino);
        void makeStop();
        void backToGalpao();
        pair<No*, double> ordena(unordered_map<No*, double> listCandidatos, float alpha);
        void initVariables();



        Grafo* grafo;
        unordered_map<int, unordered_map<int, float>> matrizDistancias;  
        int capacidade;
        int caminhoes;
        No* galpao;
        double optimal_value = -1;
        unordered_map<int, rota> rotas;
        vector<int> percorridos = {1};
        unordered_map<int, candidato> candidatos;
        unordered_map<int, candidato> stop; 

        int inicio;
        int fim;
};


#include <stdexcept>

class ExcecaoSemCandidatos : public std::exception {
private:
    const Solucao* solucao;

public:
    ExcecaoSemCandidatos(const Solucao* solucao) : solucao(solucao) {}

    virtual const char* what() const noexcept override {
        cout << "ExcecaoSemCandidatos: ";
        cout << endl;
        if (solucao) {
            solucao->imprimeRotas(false);
        }
        return "Nao foi possivel encontrar novos Candidatos";
    }
};

#endif // SOLUCAO_H
