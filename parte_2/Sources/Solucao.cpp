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
#include <vector>
#include <algorithm>


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

using namespace std;


Solucao::Solucao(string txt)
{
    lerArquivo(txt);
    construirArestas();
    construirMatriz();
    srand(static_cast<unsigned>(time(nullptr)));
    fim = this->grafo->getOrdem();
    inicio = this->galpao->getIdNo();
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
                }
                else if(chave == "value:")
                    ss >> this->optimal_value;
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


bool Solucao::inPercorridos(No* procurado, vector<int> percorridos)
{   
    if (std::find(percorridos.begin(), percorridos.end(), procurado->getIdNo()) != percorridos.end()) {
        return true;
    }
    return false;
}


bool Solucao::noValido(No* destino , vector<int> percorridos, int i, unordered_map<int, rota> rotas) {  
    return (destino != NULL && !inPercorridos(destino,percorridos) && rotas[i].cargaAtual + destino->getDemanda() <= this->capacidade);
}


void Solucao::makeCandidatos(){    
    // para todas as rotas
    for(int i=0; i<this->caminhoes; i++)
    {
        // para todos os nós do grafo
        for(No *no = this->grafo->getNoRaiz(); no; no = no->getProxNo())
        {    
            if(!this->inPercorridos(no,percorridos)){
                // (peso / distancia) entre o ultimo nó da rota e o nó do for
                if(this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][no->getIdNo()] != 0){
                    
                    double euristica = no->getDemanda() / this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][no->getIdNo()];
                    
                    if(euristica > candidatos[i].euristica)
                    {
                        candidatos[i].no = no;
                        candidatos[i].euristica = euristica;
                    }
                }
            }
        }
    }
}


void Solucao::atualizaCandidatos(No* alterado){
    // para todas as rotas
    for(int i=0; i<this->caminhoes; i++)
    {
        if(rotas[i].ultimoNo->getIdNo() == alterado->getIdNo()){
            
            candidatos[i].euristica = 0;
            // para todos os nós do grafo
            for(No *no = this->grafo->getNoRaiz(); no; no = no->getProxNo())
            {
                // demanda / distancia entre o ultimo nó da rota e o nó do for
                double euristica = no->getDemanda() / this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][no->getIdNo()] ;
                
                if(euristica > candidatos[i].euristica &&
                    !this->inPercorridos(no,percorridos) && 
                    rotas[i].cargaAtual + no->getDemanda() <= this->capacidade)
                {
                    candidatos[i].no = no;
                    candidatos[i].euristica = euristica;
                }
            }
        }
    }
}


int Solucao::getCandidatos() {
    double max = DBL_MIN;
    int cont = -1;
    for(int i=0; i < this->caminhoes; i++){ 
        if(max < candidatos[i].euristica){
            max = candidatos[i].euristica;
            cont = i;
        }     
    }
    if(cont == -1){
        throw ExcecaoSemCandidatos(this);
    }
    return cont;
}


bool Solucao::parar() {
    for(int i=0; i<this->caminhoes; i++){
        if(stop[i].no->getIdNo() != candidatos[i].no->getIdNo())
            return false;
    }
    return true;
};


void Solucao::atualizaRota(int i, No* destino) {
    rotas[i].ultimoNo = destino;
    rotas[i].cargaAtual += destino->getDemanda();
    rotas[i].percurso.push_back(destino->getIdNo());
    rotas[i].distancia += this->matrizDistancias[galpao->getIdNo()][destino->getIdNo()];
};


void Solucao::imprimeRotas(bool completa) const{
    
    if(completa)
        cout << endl << GREEN << "Imprimindo as rotas" << endl << endl;
    else
        cout << endl << RED << "Imprimindo as rotas" << endl << endl;
    
    double total = 0;
    
    for(int i=0; i<this->caminhoes; i++){
        cout << "percurso rota " << i << " : < ";  
        for(int j=0; j<rotas.at(i).percurso.size(); j++){
            cout << rotas.at(i).percurso[j] << " ";
        }
        total += rotas.at(i).distancia;
        cout << ">  - distancia: " << rotas.at(i).distancia;
        cout << " - carga: " << rotas.at(i).cargaAtual << endl;
    }
    cout << "total - " << total << endl;
    if (this->optimal_value > 0){
        cout << "optimal_value: " << this->optimal_value << endl;
        cout << "% acima: " << ((total/this->optimal_value) - 1) * 100 << "%" << RESET << endl; 
    }
    else{
        cout << "Nao possui optimal_value" << RESET <<  endl;
    }
};


void Solucao::makeStop() {
    for(int i=0; i<this->caminhoes; i++){
        stop[i].no = this->grafo->getGalpao();
    }
};


void Solucao::backToGalpao(){
    for(int i=0; i< this->caminhoes; i++){
        atualizaRota(i, this->grafo->getGalpao());
    }
}


int Solucao::gerarNumeroAleatorio() {
    return (rand() % (fim - inicio + 1) + inicio);
}


unordered_map<int,rota> Solucao::guloso(ofstream &output_file)
{
    // inicializa randomicamente
    cout << this->caminhoes << endl;
    int i = 0;
    while(i < this->caminhoes){
        No* destino = this->grafo->procurarNoPeloId(gerarNumeroAleatorio());
        if(noValido(destino, percorridos, i, rotas)){
            cout << destino->getIdNo() << endl;
            percorridos.push_back(destino->getIdNo());
            atualizaRota(i, destino);
            i++;
        }
    }

    makeCandidatos();
    makeStop();
    
    // peso / distancia
    bool completa = true;
    while(percorridos.size() != this->grafo->getOrdem())
    {
        try{
            stop = candidatos;
            i = getCandidatos();
            percorridos.push_back(candidatos[i].no->getIdNo());
            atualizaRota(i, candidatos[i].no);
            atualizaCandidatos(candidatos[i].no);
        }
        catch(const ExcecaoSemCandidatos& e){
            backToGalpao();
            e.what();
            return rotas;
        }
    }
    backToGalpao();

    imprimeRotas(completa); 
    return this->rotas;
}


unordered_map<int,rota> Solucao::gulosoRandomizadoAdaptativo(ofstream &output_file, float param_1, int maxIter)
{
    return rotas;
}


unordered_map<int,rota> Solucao::gulosoRandomizadoAdaptativoReativo(ofstream &output_file, float param_1, int maxIter)
{
    return rotas;
}









No* Solucao::findMinDistance(No* partida, vector<int> percorridos, double capacidade)
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


unordered_map<int, double> Solucao::setCapacidade(){
    unordered_map<int, double> capacidade;
    for(int i=0; i<caminhoes; i++){
        capacidade[i] = 0;
    }
    return capacidade;
}

