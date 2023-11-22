#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <math.h>
#include <unordered_map>
#include "../Headers/Solucao.h"
#include "../Headers/Grafo.h"
#include "../Sources/Grafo.cpp"
#include "../Sources/Aresta.cpp"
#include "../Sources/No.cpp"
#include <cfloat>


using namespace std;

Solucao::Solucao(string txt)
{
    lerArquivo(txt);
    construirArestas();
    // this->grafo->imprime();
    construirMatriz();
    // imprimeMatriz();
    guloso();
}


Solucao::~Solucao()
{
    delete this->grafo;
}


void Solucao::lerArquivo(string txt)
{
    ifstream arquivo("./parte_2/arquivos/" + txt);

    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo " << txt << endl;
        exit(-1);
    }

    int dimension = -1;
    string linha;
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

// começar do no 1 (demanda = 0) (Galpão)
// iniciar o primeiro caminhao com o menor caminho do galpao ate um no
// fazer o mesmo para todos os caminhoes pegando nos nao acessados
// verificar se ir pra o proximo no de menor caminho nao acessado, irá passar o limite de demanda
// true ? ir para o proximo no de menor caminho
// false ? voltar para a base
Grafo* Solucao::guloso()
{
    auto imprimehash = [](unordered_map<int, No*> hash){
        for (auto i = hash.begin(); i != hash.end(); ++i) {
            cout << "Chave: " << i->first << ", Valor: " << i->second->getIdNo() << endl;
        }
        cout << endl;
    };

    auto equals = [this](unordered_map<int, No*> hash1, unordered_map<int, No*> hash2){
        for(int i=0; i<this->caminhoes; i++)
            if(hash1[i]->getIdNo() != hash2[i]->getIdNo())
                return false;
        return true;
    };

    No* galpao = this->grafo->getGalpao();

    if(!galpao){
        cout << "erro ao encontrar o galpao." << endl;
        return NULL;
    }

    unordered_map<No*,bool> percorridos = initHash();
    Grafo *guloso = new Grafo; 
    No* galpaoGuloso = guloso->insereNo(galpao->getIdNo(), galpao->getX(), galpao->getY(), galpao->getDemanda());
    
    unordered_map<int, No*> hashMenorCaminho = initHashMenorCaminho(galpaoGuloso);
    unordered_map<int, double> capacidade = setCapacidade();

    No *teste = new No(2, 2, 2, 2);
    unordered_map<int, No*> stop = initHashMenorCaminho(teste);

    while(!checadosHash(percorridos) && !equals(stop,hashMenorCaminho)){
        stop = hashMenorCaminho;
        for (int i=0; i<caminhoes; i++){
            // imprimehash(hashMenorCaminho);
            // imprimehash(stop);
            No* destino = findMinDistance(hashMenorCaminho[i], percorridos, capacidade[i]);
            if(destino != NULL){
                capacidade[i] += destino->getDemanda();
                double distancia = matrizDistancias[hashMenorCaminho[i]->getIdNo()][destino->getIdNo()];
                No* newDestino = guloso->insertAresta(hashMenorCaminho[i], destino, distancia);
                hashMenorCaminho[i] = newDestino;
                percorridos[destino] = true;
            }
            cout << "rota " << i << " - " << capacidade[i] << endl;
        }
    }

    for(int i=0; i<caminhoes; i++){
        float distancia = matrizDistancias[hashMenorCaminho[i]->getIdNo()][galpao->getIdNo()];
        guloso->insertAresta(hashMenorCaminho[i], galpaoGuloso, distancia);
    }

    guloso->imprime();
    if(stop == hashMenorCaminho){
        cout << "Nao foi possivel terminar a execucao do algoritmo." << endl;
    }
    this->custoMinimo(guloso);
    
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


Grafo* Solucao::gulosoRandomizadoAdaptativo()
{

}


Grafo* Solucao::gulosoRandomizadoAdaptativoReativo()
{

}

