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
#include <cfloat>
#include <climits>

using namespace std;


struct rota{
    No* ultimoNo;
    double cargaAtual = 0;
    vector<int> percurso = { 1 };
    double distancia = 0.0;
};


struct candidato{
    No* no;
    double heuristica = 0;
};


class Solucao{
    public:
        Solucao(string txt);
        
        ~Solucao();
        unordered_map<int,rota> guloso(ofstream &output_file);
        pair<double,double> gulosoRandomizadoAdaptativo(ofstream &output_file, float alpha, int maxIter);
        unordered_map<int,rota> gulosoRandomizadoAdaptativoReativo(ofstream &output_file, vector<float> alpha, int maxIter, int bloco);
        double custoMinimo(Grafo *grafo);
        void imprimeRotas(bool completa);
        void initBest();

    private:
        int gerarNumeroAleatorio(int inicio, int fim);
        void lerArquivo(string txt);
        void construirArestas();
        void construirMatriz();
        void imprimeMatriz();
        void escreveRotas(ofstream &output_file, bool completa, int iter);
        
        
        No* findMinDistance(No* partida, vector<int> percorridos,  double capacidade);
        unordered_map<int, bool> initHash();
        unordered_map<int, No*> initHashMenorCaminho(No* galpao);
        bool checadosHash(unordered_map<No*,bool> hash);
        unordered_map<int, double> setCapacidade();
        
        
        bool inPercorridos(No* procurado, vector<int> percorridos);
        bool noValido(No* destino , vector<int> percorridos, int i, unordered_map<int, rota> rotas);
        bool noValidoReativo(No* destino , vector<int> percorridos, int i, unordered_map<int, rota> rotas);
        void makeCandidatos();
        void makeCandidatosRandomizado(float alpha);
        void atualizaCandidatos(No* alterado);
        void atualizaCandidatosRandomizado(float alpha);
        int getCandidatos();
        bool parar();
        void atualizaRota(int i, No* destino);
        void makeStop();
        void backToGalpao();
        pair<No*, double> ordena(unordered_map<No*, double> listCandidatos, float alpha);
        void initVariables();
        void calculaBest();
        void Estatisticas();
        double calcMedia();
        double desvioPadrao(double media);
        void mape();
        double calculaFormulaRaio();
        double getRaio(No* no);
        bool verificaRaio(No* destino, vector<int> percorridos);
        bool VerificaVeracidade();
        void initRotas();
        unordered_map<int,rota> verificaBest();
        void imprimeBest();
        void atualizaProb(vector<float> alpha);
        void initVetores(int tam);
        int getAlpha();
        void atualizaMedia(double newSolucao, int indexAlpha);



        Grafo* grafo;
        unordered_map<int, unordered_map<int, float>> matrizDistancias;  
        int capacidade, caminhoes, fim, inicio;
        No* galpao;
        double optimal_value = -1;
        vector<int> percorridos = {1};
        vector<double> total_rotas;
        unordered_map<int, rota> rotas, best;
        unordered_map<int, candidato> candidatos, stop;
        int maiorX = INT_MIN;
        int menorX = INT_MAX;
        int maiorY = INT_MIN;
        int menorY = INT_MAX;
        int maiorDemanda = INT_MIN;
        int menorDemanda = INT_MAX;
        int area;
        double raio;
        double best_distancia = DBL_MAX;
        double soma_iter = 0;
        double n_iter = 0;
        vector<vector<double>> media;
        vector<double> prob;
};


#include <stdexcept>

class ExcecaoSemCandidatos : public std::exception {
private:
    const Solucao* solucao;

public:
    ExcecaoSemCandidatos(const Solucao* solucao) : solucao(solucao) {}

    virtual const char* what() const noexcept override {
        cout << "ExcecaoSemCandidatos: "<< endl;
        // cout << endl;
        // if (solucao) {
        //     solucao->imprimeRotas(false);
        // }
        return "Nao foi possivel encontrar novos Candidatos";
    }
};

#endif // SOLUCAO_H
