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


using namespace std;

Solucao::Solucao(string txt)
{
    lerArquivo(txt);
    construirArestas();
    // this->grafo->imprime();
    construirMatriz();
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
            this->matrizDistancias[i][a->getNoDestino()] = a->getPeso(); 
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

                cout << this->matrizDistancias[linha][coluna] << "\t";
            }
            cout << endl;
        }
}


bool Solucao::inPercorridos(No* procurado, unordered_map<No*,bool> percorridos)
{
    return percorridos[procurado];
}


No* Solucao::findMinDistance(No* partida, unordered_map<No*,bool> percorridos)
{
    // ALTERAR PARA MAIOR VALOR DA LINGUAGEM 
    double minDistance = 999999;
    No* destino;
    for (No *i = this->grafo->getNoRaiz(); i; i = i->getProxNo())
    {
        if(matrizDistancias[partida][i] < minDistance && !inPercorridos(i,percorridos)){
            minDistance = matrizDistancias[partida][i];
            destino = i;
        }
    }
    return destino;
}


unordered_map<No*, bool> Solucao::initHash(){
    unordered_map<No*, bool> hash;

    for(No* i = grafo->getNoRaiz(); i; i=i->getProxNo()){
        hash[i] = false;
    }
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
    No* galpao = this->grafo->getGalpao();

    if(!galpao){
        cout << "erro ao encontrar o galpao." << endl;
        return NULL;
    }

    unordered_map<No*,bool> percorridos = initHash();

    Grafo *guloso = new Grafo; 

    for(int i=0; i < caminhoes; i++){
        No* destino = findMinDistance(galpao, percorridos);
        percorridos[destino] = true;
        double distancia = matrizDistancias[galpao][destino];
        guloso->insertAresta(galpao, destino, distancia) ? cout << "true" << endl : cout << "false" << endl;
    }
    guloso->imprime();

}


Grafo* Solucao::gulosoRandomizadoAdaptativo()
{

}


Grafo* Solucao::gulosoRandomizadoAdaptativoReativo()
{

}

