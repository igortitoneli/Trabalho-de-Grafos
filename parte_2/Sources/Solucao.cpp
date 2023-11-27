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


bool Solucao::inPercorridos(No* procurado, unordered_map<int,bool> percorridos)
{
    return percorridos[procurado->getIdNo()];
}


No* Solucao::findMinDistance(No* partida, unordered_map<int,bool> percorridos, double capacidade)
{
    // ALTERAR PARA MAIOR VALOR DA LINGUAGEM 
    //double minDistance = 999999;
    double minDistance = DBL_MAX;
    No* destino = NULL;
    
    for (No *i = this->grafo->getNoRaiz(); i; i = i->getProxNo())
    {
        if(matrizDistancias[partida->getIdNo()][i->getIdNo()] < minDistance && 
            !inPercorridos(i,percorridos) && 
            i!= partida && 
            capacidade + i->getDemanda() < this->capacidade
            ){
            minDistance = matrizDistancias[partida->getIdNo()][i->getIdNo()];
            destino = i;
        }
    }

    return destino;
}


unordered_map<int, bool> Solucao::initHash(){
    unordered_map<int, bool> hash;

    for(No* i = grafo->getNoRaiz(); i; i=i->getProxNo()){
        hash[i->getIdNo()] = false;
    }
    hash[this->galpao->getIdNo()] = true;
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
    // funçoes
        // atualizar candidatos
        // getMinDist
        // 

    // estruturas
        // ultimo no da rota // a carga atual da rota
        // os candidatos
        // percorridos

    struct rota{
        No* ultimoNo;
        double cargaAtual = 0;
    };

    struct candidato{
        No* no;
        double distancia;
    };

    unordered_map<int, rota> rotas;
    unordered_map<int, bool> percorridos;

    auto noValido = [this](No* destino , unordered_map<int, bool> percorridos, int i, unordered_map<int, rota> rotas) -> bool {
        return (destino != NULL && !inPercorridos(destino,percorridos) && rotas[i].cargaAtual + destino->getDemanda() <= this->capacidade);
    };

    auto makeCandidatos = [this](unordered_map<int, bool> percorridos, unordered_map<int, rota> rotas) -> unordered_map<int, candidato> {
        unordered_map<int, candidato> candidatos;
        
        // para todas as rotas
        for(int i=0; i<this->caminhoes; i++)
        {
            candidatos[i].distancia = DBL_MAX;
            // para todos os nós do grafo
            for(No *no = this->grafo->getNoRaiz(); no; no = no->getProxNo())
            {    
                // distancia entre o ultimo nó da rota e o nó do for
                double dist = this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][no->getIdNo()] ;

                if(dist < candidatos[i].distancia && !this->inPercorridos(no,percorridos))
                {
                    candidatos[i].no = no;
                    candidatos[i].distancia = dist;
                }
            }
            // cout << "canditado - "<< i << " - " << candidatos[i].no->getIdNo() << " - " << candidatos[i].distancia << endl; 
        }
        return candidatos;
    };

    auto atualizaCandidatos = [this](unordered_map<int, bool> percorridos, unordered_map<int, rota> rotas, No* alterado, unordered_map<int, candidato> candidatos) -> unordered_map<int, candidato> {
        // para todas as rotas
        for(int i=0; i<this->caminhoes; i++)
        {
            cout << "rotas[" << i << "].ultimoNo->getIdNo()  -  " << rotas[i].ultimoNo->getIdNo() << endl;
            cout << "alterado->getIdNo()  -  " << alterado->getIdNo() << endl;
            if(rotas[i].ultimoNo->getIdNo() == alterado->getIdNo()){
                cout << "entou" << endl;
                cout << "rotas[" << i << "] - " << rotas[i].ultimoNo->getIdNo() << endl;
                candidatos[i].distancia = DBL_MAX;
                // para todos os nós do grafo
                for(No *no = this->grafo->getNoRaiz(); no; no = no->getProxNo())
                {
                    // distancia entre o ultimo nó da rota e o nó do for
                    double dist = this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][no->getIdNo()] ;

                    if(dist < candidatos[i].distancia && !this->inPercorridos(no,percorridos))
                    {
                        candidatos[i].no = no;
                        candidatos[i].distancia = dist;
                    }
                }
            }
            // cout << "canditado - "<< i << " - " << candidatos[i].no->getIdNo() << " - " << candidatos[i].distancia << endl; 
        }
        return candidatos;
    };

    auto getMinDist = [this](unordered_map<int, candidato> candidatos) -> int {
        double min = DBL_MAX;
        int cont = -1;
        for(int i=0; i < this->caminhoes; i++){
            if(min > candidatos[i].distancia){
                min = candidatos[i].distancia;
                cont = i;
            }     
        }
        return cont;
    };

    auto parar = [this](unordered_map<int, candidato> candidatos, unordered_map<int, candidato> stop) -> bool {
        for(int i=0; i<this->caminhoes; i++){
            if(stop[i].no->getIdNo() != candidatos[i].no->getIdNo())
                return false;
        }
        return true;
    };

    Grafo *guloso = new Grafo; 

    No* galpaoGuloso = guloso->insereNo(galpao->getIdNo(), galpao->getX(), galpao->getY(), galpao->getDemanda());

    int fim = this->grafo->getOrdem();
    int inicio = this->galpao->getIdNo();

    srand(static_cast<unsigned>(time(nullptr)));

    // inicializa randomicamente
    for(int i=0; i<this->caminhoes; i){
        int numeroAleatorio = rand() % (fim - inicio + 1) + inicio;
        No* destino = this->grafo->procurarNoPeloId(numeroAleatorio);
        
        cout << destino->getIdNo() << endl;
        // cout << (noValido(destino, percorridos, i, rotas) ? "true" : "false") << endl;
        if(noValido(destino, percorridos, i, rotas)){
            rotas[i].ultimoNo = destino;
            rotas[i].cargaAtual += destino->getDemanda();
            percorridos[destino->getIdNo()] = true;
            guloso->insertAresta(galpaoGuloso, destino, this->matrizDistancias[galpao->getIdNo()][destino->getIdNo()]);
            i++;
        }
    } 

    unordered_map<int, candidato> candidatos = makeCandidatos(percorridos,rotas);
    unordered_map<int, candidato> stop;
    
    while(guloso->getOrdem() < grafo->getOrdem() || parar(candidatos, stop)){
        stop = candidatos;
        int i = getMinDist(candidatos);
        if(i == -1) break;
        guloso->insertAresta(rotas[i].ultimoNo,candidatos[i].no, candidatos[i].distancia);
        percorridos[candidatos[i].no->getIdNo()] = true;
        rotas[i].ultimoNo = candidatos[i].no; 
        candidatos = atualizaCandidatos(percorridos, rotas, candidatos[i].no, candidatos);
    }


    guloso->imprime();
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
    
}


Grafo* Solucao::gulosoRandomizadoAdaptativo(ofstream &output_file, float param_1, int maxIter)
{
return NULL;
}


Grafo* Solucao::gulosoRandomizadoAdaptativoReativo(ofstream &output_file, float param_1, int maxIter)
{
return NULL;
}

