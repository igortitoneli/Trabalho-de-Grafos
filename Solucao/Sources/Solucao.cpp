#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include "../Headers/Solucao.h"

using namespace std;

Solucao::Solucao(string txt){
    this->grafo = lerArquivo(txt);
}


Solucao::~Solucao(){
    delete this->grafo;
}


Grafo* Solucao::lerArquivo(string txt)
{   
    ifstream arquivo("./arquivos/" + txt);

    if(!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo " << txt << endl;
        return NULL;
    }

    int dimension = -1;
    string linha;
    while(getline(arquivo, linha)) {
        // Usa std::istringstream para analisar cada linha
        istringstream ss(linha);

        string chave;
        ss >> chave;

        if (chave == "DIMENSION") {
            ss >> chave;
            ss >> dimension;
        } else if (chave == "CAPACITY") {
            ss >> chave;
            ss >> this->capacidade;
            break;
        }
    }
    
    for (int i = 0; i < dimension; ++i) {
        istringstream ss(linha);

        int idno, x, y;
        ss >> idno >> x >> y;

        this->grafo->insereNo(idno, x, y);  
        
    }
    grafo->imprime();
    arquivo.close();
    return 0;
}
