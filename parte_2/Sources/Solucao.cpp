#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <math.h>
#include <chrono>
#include <cstdlib>
#include <unordered_map>
#include "../Headers/Solucao.h"
#include "../Headers/Grafo.h"
#include "../Headers/Aresta.h"
#include "../Headers/No.h"
#include <cfloat>
#include <ctime>
#include <random>


using namespace std;

Solucao::Solucao(string txt)
{
    lerArquivo(txt);
    construirArestas();
    construirMatriz();
}


Solucao::~Solucao()
{
    delete this->grafo;
}


void Solucao::lerArquivo(string txt)
{
    ifstream arquivo("./parte_2/Instances/" + txt);

    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo " << txt << endl;
        exit(-1);
    }

    int dimension = -1;
    string linha;
    auto start = chrono::steady_clock::now();
    while (getline(arquivo, linha))
    {
        istringstream ss(linha);

        string chave;
        ss >> chave;
        if (chave == "COMMENT")
        {
            while (ss >> chave) {
                if (chave == "trucks:") {
                    ss >> this->caminhoes;
                    break;   
                }
            }
        }
        else if (chave == "DIMENSION")
        {
            ss >> chave;
            ss >> dimension;
        }
        else if (chave == "CAPACITY")
        {
            ss >> chave;
            ss >> this->capacidade;
            break;
        }
    } 
    
    getline(arquivo, linha);
    this->grafo = new Grafo();
    
    for (int i = 0; i < dimension; i++)
    {
        getline(arquivo, linha);
        istringstream ss(linha);
        int idno;
        float x, y;
        ss >> idno >> x >> y;
        grafo->insereNo(idno, x, y);
    }

    getline(arquivo, linha);

    for(int i = 0; i < dimension; i++)
    {
        getline(arquivo, linha);
        istringstream ss(linha);
        int no, demanda;
        ss >> no >> demanda;
        grafo->procurarNoPeloId(no)->setDemanda(demanda);
    }

    this->galpao = this->grafo->getGalpao();
    auto end = chrono::steady_clock::now();

    cout << "Demorou  "
            << chrono::duration_cast<chrono::milliseconds>(end - start).count()
            << " ms para ler o arquivo de entrada." << endl;

    arquivo.close();
}


void Solucao::construirArestas()
{
    for(No* i = this->grafo->getNoRaiz(); i; i=i->getProxNo()){
        for(No* j = this->grafo->getNoRaiz(); j; j=j->getProxNo()){
            if(i!=j){
                float difX = abs(i->getX() - j->getX()) * abs(i->getX() - j->getX());
                float difY = abs(i->getY() - j->getY()) * abs(i->getY() - j->getY());

                float pesoAresta = sqrt(difX + difY);
                i->insereArestaNo(j,pesoAresta);
            }
        }
    }
}


void Solucao::construirMatriz()
{
    for(No *i = this->grafo->getNoRaiz(); i; i=i->getProxNo()){
        for(Aresta *a = i->getPrimeiraAresta(); a; a=a->getProxAresta()){
            this->matrizDistancias[i->getIdNo()][a->getNoDestino()->getIdNo()] = a->getPeso(); 
        }
    }
}


void Solucao::imprimeMatriz()
{
    cout << "   ";
        for (No *no = this->grafo->getNoRaiz(); no != NULL; no = no->getProxNo())
        {
            cout << no->getIdNo() << "\t";
        }
        cout << endl
                << "   ";
        for (No *no = this->grafo->getNoRaiz(); no != NULL; no = no->getProxNo())
        {
            cout << "-" << "\t";
        }
        cout << endl;

        for (No *coluna = this->grafo->getNoRaiz(); coluna != NULL; coluna = coluna->getProxNo())
        {
            cout << coluna->getIdNo() << "|\t";
            for (No *linha = this->grafo->getNoRaiz(); linha != NULL; linha = linha->getProxNo())
            {

                cout << this->matrizDistancias[linha->getIdNo()][coluna->getIdNo()] << "\t";
            }
            cout << endl;
        }
}


bool Solucao::inPercorridos(No* procurado, unordered_map<No*,bool> percorridos)
{
    return percorridos[procurado];
}


No* Solucao::findMinDistance(No* partida, unordered_map<No*,bool> percorridos, double capacidade)
{
    // ALTERAR PARA MAIOR VALOR DA LINGUAGEM 
    //double minDistance = 999999;
    double minDistance = DBL_MAX;
    No* destino = NULL;

    // cout << "\n\n findMinDistance() " << endl; 
    for (No *i = this->grafo->getNoRaiz(); i; i = i->getProxNo())
    {
        // cout << "demanda noh " << i->getIdNo() << " - " << i->getDemanda() << endl;
        // if(matrizDistancias[partida->getIdNo()][i->getIdNo()] >= minDistance) 
        //     cout << "matrizDistancias " << i->getIdNo() << " >= " << minDistance << endl;
        // if(inPercorridos(i,percorridos))
        //     cout << i->getIdNo() << " ja percorrido" << endl;
        // if(i == partida)
        //     cout << i->getIdNo() << " == " << partida->getIdNo() << endl;
        // if(capacidade + i->getDemanda() >= this->capacidade)
        //     cout << i->getIdNo() << " passa da demanda" << endl;

        if(matrizDistancias[partida->getIdNo()][i->getIdNo()] < minDistance && 
            !inPercorridos(i,percorridos) && 
            i!= partida && 
            capacidade + i->getDemanda() < this->capacidade
            ){
                // cout << "noh " << i->getIdNo() << " SUPRIU" << endl;
            minDistance = matrizDistancias[partida->getIdNo()][i->getIdNo()];
            destino = i;
        }
    }
    // cout << "noh " << destino->getIdNo() << " FOI ACEITO!" << endl;

    return destino;
}


unordered_map<No*, bool> Solucao::initHash(){
    unordered_map<No*, bool> hash;

    for(No* i = grafo->getNoRaiz(); i; i=i->getProxNo()){
        hash[i] = false;
    }
    hash[this->galpao] = true;
    return hash;
}

// primeira iteraçao 100% randomizada
// criar vetor de candidatos
// perguntar pra todos os nos, qual o no mais perto de algum ultimo no de uma rota
// tira da lista de candidatos o no que foi inserido
// procurar outros candidatos 
// verificar se inseriu todos
        // true ? volta pro galpao
        // false ? continua

Grafo* Solucao::guloso(ofstream &output_file)
{
    No* galpao = this->grafo->getGalpao();

    if(!galpao){
        cout << "erro ao encontrar o galpao." << endl;
        return NULL;
    }

    struct info_node {
        No* no;
        double distancia;
        int demanda;
    };

    // <rota_id>, <candidato>, <distancia>, <carga>
    unordered_map<int, info_node> candidatos;
    
    unordered_map<No*, bool> percorridos = initHash();
    
    Grafo *guloso = new Grafo; 
    No* galpaoGuloso = guloso->insereNo(galpao->getIdNo(), galpao->getX(), galpao->getY(), galpao->getDemanda());
    
    // Capacidade de cada rota (caminhao)
    unordered_map<int, double> capacidade = setCapacidade();

    srand(static_cast<unsigned>(time(nullptr)));

    // Inicializa """randomicamente"""
    for(int i=0; i<this->caminhoes; i++){

        int fim = this->grafo->getOrdem();
        int inicio = this->galpao->getIdNo();
        int numeroAleatorio = gerarNumeroAleatorio(inicio, fim);
        
        No* destino = this->grafo->procurarNoPeloId(numeroAleatorio);
        
        if(destino != NULL && !inPercorridos(destino,percorridos)){
            capacidade[i] += destino->getDemanda();
            percorridos[destino] = true;
            cout << "rota " << i << " - " << "No - " << destino->getIdNo() << " - capacidade - " << capacidade[i] << endl;
        }
    } 


return NULL;
}

unordered_map<int, double> Solucao::setCapacidade(){
    unordered_map<int, double> capacidade;
    for(int i=0; i<caminhoes; i++){
        capacidade[i] = 0;
    }
    return capacidade;
}


bool Solucao::checadosHash(unordered_map<No*,bool> hash){
    // int cont = 0;
    for(No *i = this->grafo->getNoRaiz(); i; i = i->getProxNo())
        if(hash[i] == false){
            // cont ++;
            // cout << "FALTA O Noh " << i->getIdNo() << endl; 
            return false;
        }
    // return cont == 0;
    return true;
}


double Solucao::custoMinimo(Grafo *grafo)
{   
    cout << "O custo minimo foi de " << grafo->somaPesoArestas() / 2 << endl;
    return grafo->somaPesoArestas() / 2;
}


unordered_map<int, No*> Solucao::initHashMenorCaminho(No* galpao){
    unordered_map<int, No*> hashMenorCaminho;
    for(int i=0; i<caminhoes; i++){
        hashMenorCaminho[i] = galpao;
    }
    return hashMenorCaminho;
}


int Solucao::gerarNumeroAleatorio(int inicio, int fim) {
    // Cria um gerador de números aleatórios
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define a faixa desejada
    std::uniform_int_distribution<int> distribuicao(inicio, fim);

    // Gera e retorna o número aleatório
    return distribuicao(gen);
}


Grafo* Solucao::gulosoRandomizadoAdaptativo(ofstream &output_file, float param_1, int maxIter)
{
return NULL;
}


Grafo* Solucao::gulosoRandomizadoAdaptativoReativo(ofstream &output_file, float param_1, int maxIter)
{
return NULL;
}

