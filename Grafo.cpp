#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include "Grafo.h"

using namespace std;
typedef vector<No*> NodeVector;
// typedef EdgeSearchInfo EdgeInfo;

//------------------------ Construtors and Destrutors ------------------------

/**
* Grafo Construtor.
*
* @param isDigrafo (new valor)
*/
Grafo::Grafo(bool isDigrafo, bool weightArc, bool weightNo) {
   this->numAresta = 0;
   this->ordem = 0;
   this->noRaiz = NULL;
   this->digrafo = isDigrafo;
   this->weigthNo = weightNo;
   this->weightArc = weightArc;
}

/**
* Destructor
*/
Grafo::~Grafo() {
   No *noAux = getNoRaiz();
   No *noAnterior;
   while ( noAux != NULL ) {
       noAnterior = noAux->getProxNo();
       delete( noAux);
       noAux = noAnterior;
   }
}

//----------------------------------- Seters -----------------------------------

/**
* Funcao incrementa Ordem do grafo em um.
*/
void Grafo::incOrdem() {
   this->ordem++;
}

/**
* Funcao decrementa Ordem do grafo em um.
*/
void Grafo::decOrdem() {
   this->ordem--;
}

//------------------------------ Funcoes do No ------------------------------

/**
* EauxSSa funcao busca o No pelo nome.
* Para achar o No, faça grafo->getNoAux()
*
* @param nome (no nome)
* @return true - se achar no.
*         false - se Nao achar.
*/
No *Grafo::procurarNoPeloId(int idFindNo) {
    No *noAux = noRaiz;
    No *noAuxAnterior = NULL;

    while ( noAux != NULL ) {

        if ( noAux->getIdNo() == idFindNo ) {
            return noAux;
        }
        if ( noAux->getProxNo() == NULL ) {
            return NULL;
        }
        noAuxAnterior = noAux;
        noAux = noAux->getProxNo();
    }
    return NULL;
}

/**
* A funcao insere o No caso ele nao existe ou atualiza seu peso.
*
* @param nome
* @param peso (peso do No)
*/
No *Grafo::insereNo(int idNo, int peso) {

    No* procurado = procurarNoPeloId(idNo);

    if (procurado == NULL ) {
        No *novoNo = new No(idNo, peso);

        if (noRaiz == NULL){
            noRaiz = novoNo;
        }else{
            novoNo->setProxNo(noRaiz);
            noRaiz = novoNo;
        }
        return novoNo;
    }
   else
    {
        procurado->setOrdemMaisUm();
        return procurado;
    }
}

/**
* Remove No de um grafo
*
* @param nome (no nome)
*/
bool Grafo::removeNo(int idNo, bool isDigrafo) {
   // Pesquisa o No a ser excluido
   // Remove todas as arestas/arcos onde este nó ocorre
   // Remove o no

    No *no = this->procurarNoPeloId(idNo);

    if(no){
        No *noAux = this->getNoRaiz();
        No *anterior = NULL;
        int contador = 0;
        bool achou = false;

        if(this->getNoRaiz() == no){
            this->noRaiz = this->noRaiz->getProxNo();
        }

        if(this->isDigraph()){
            while(noAux && (contador < no->getGrau() || !achou)){

                contador += noAux->verificaRemoveAresta(no);
                
                if(noAux == no){
                    achou == true;
                    anterior->setProxNo(noAux->getProxNo());
                }                
                anterior = noAux;
                noAux = noAux->getProxNo();
            }
        }
        else{
            while(noAux && (contador < no->getGrau() || !achou)){
                contador += noAux->verificaRemoveAresta(no);
                contador += no->verificaRemoveAresta(noAux);
                cout << "foi?" << endl;           
                if(noAux == no){
                    achou == true;
                    anterior->setProxNo(noAux->getProxNo());
                }     
                anterior = noAux;
                noAux = noAux->getProxNo();
            }
        }
        // Caso queira implementar o int grau no private do Grafo.h
        // if(no->getGrau() == this->getOrdem()){
        //     this->verificaOrdem();
        // }
        this->decOrdem();
        return true;
    }
   
   return false;
}

/*
Caso queira implementar o int grau no private do Grafo.h
void Grafo::verificaGrau(){
    
    this->grau = this->getNoRaiz()->getGrau();

    No *aux = this->getNoRaiz()->getProxNo();

    while(aux){
        if(this->grau < aux->getGrau()) this->grau = aux->getGrau();
        aux = aux->getProxNo();
    }
}
*/

/**
* Insere a aresta de A para B se não adjacencia nao existir,
* caso exista, a funcao atualiza o peso
*
* @param noFonteNome ()
* @param noDestinoNome ()
* @param pesoAresta ()
*/
bool Grafo::insertAresta(int idNoOrigem, int idNoDestino, int pesoAresta, bool weigthArc, bool isDirected) {

   //Verifica se já existem os dois nós com esses ids no grafo CHECK
   //Se ja existem, eé so inserir a aresta em cada um se o grafo for nao direcionado ou apenas de
   //   origem pra destino, caso o grafo seja orientado.
   //Se para algum ou ambos os ids não existe o no no grafo, e preciso inserir nos com esses ids antes de incluir a aresta
    No *noFonte, *noDestino;

    noFonte = procurarNoPeloId(idNoOrigem);    
    noDestino = procurarNoPeloId(idNoDestino);

    if(noFonte == NULL) {
        noFonte = this->insereNo(idNoOrigem,0);
        this->incOrdem();
    }
    
    if(noDestino == NULL){
        noDestino = this->insereNo(idNoDestino,0);
        this->incOrdem();
    } 

    if(isDirected)
    {
        if(this->criaAresta(noFonte, noDestino, pesoAresta))
        { 
            this->numAresta ++;
            return true;
        }
    }
    else
    {
        if(this->criaAresta(noFonte, noDestino, pesoAresta) && this->criaAresta(noDestino, noFonte, pesoAresta))
        {
            this->numAresta ++;
            cout << "noFonte->getGrau() : " << noFonte->getGrau() << endl;
            return true;
        }
    }   

    return false;
}

bool Grafo::criaAresta(No *noFonte, No *Destino, int pesoAresta)
{   
    if(noFonte->getPrimeiraAresta() == NULL) // ARESTA NAO EXISTE
    {  
        Aresta *aresta = new Aresta(Destino, NULL, pesoAresta);
        noFonte->setPrimeiraAresta(aresta);
        noFonte->setUltimaAresta(aresta);
        noFonte->incrementaGrauSaida();
        Destino->incrementaGrauEntrada();
        return true;
    }
    else
    {
        if(!noFonte->procuraAresta(noFonte->getPrimeiraAresta(), Destino))
        {
            Aresta *aresta = new Aresta(Destino, NULL, pesoAresta);
            Aresta *ultimaAresta = noFonte->getUltimaAresta();
            ultimaAresta->setProxAresta(aresta);
            noFonte->setUltimaAresta(aresta);
            noFonte->incrementaGrauSaida();
            Destino->incrementaGrauEntrada();
            return true;
        }
    }
    return false;
}



/**
* This function search in a no the edges that points to the same destiny no.\n
* If 'removeAll' is TRUE, all of these edges will be removed, only 1 otherwise. (multigraph)\n
* If 'considerarPeso' is TRUE, only the edge with peso equals to 'peso' will be removed.\n
* If 'considerarPeso' is FALSE, don't matter the valor paauxSSed as parameter in 'peso'.
*
* @param *noDestino (the no that refers to the edges that will be removed)
* @param *noOrigem (the no that refers to the edges that will be removed)
* @param isDirected (flag to inform graph or digraph)
* @param peso (peso)
*
* @return int (numero arestas excluidas)
*/
bool Grafo::removeAresta(int idNoOrigem, int idNoDestino, bool isDirected) {
// Procura o nó origem e busca na sua lista de arestas o idNoDestino
// Se encontrar, remove. Se não, retorna zero

    No *origem = this->procurarNoPeloId(idNoOrigem);
    No *destino = this->procurarNoPeloId(idNoDestino);

    if(origem && destino)
    {            
        if(isDirected)
        {
            if(origem->verificaRemoveAresta(destino)){
                this->decrementaNumAresta();
                return true;
            }
        }
        else
        {
            if(origem->verificaRemoveAresta(destino) && destino->verificaRemoveAresta(origem)){
                this->decrementaNumAresta();
                return true;
            }
        }
    }

    return false;
}


bool Grafo::decrementaNumAresta(){
    if(this->isDigraph()){
        if(this->numAresta > 0)
        this->numAresta--;
        return true;
    }
    else{
        if(this->numAresta >= 2)
        this->numAresta-=2;
        return true;
    }
    return false;
}


//--- Caracteristica do Grafo ---

/**
* Retorna o numero de aresta do grafo.
*/

// IRRELEVANTE

int Grafo::getNumAresta() {
    No *noAuxiliar = noRaiz;
    int numAresta = 0;

    while ( noAuxiliar != NULL ) {
        numAresta += noAuxiliar->getGrau();
        noAuxiliar = noAuxiliar->getProxNo();
    }

    // this->numAresta = numeroArestas / 2;

    if(this->digrafo) return numAresta;
    return numAresta / 2; // PERGUNTAR PQ DIVIDE POR 2
}

int Grafo::getNumAresta2() {
    return this->numAresta;
}

/**
* Retorna 'rootNode'.
*/
No *Grafo::getNoRaiz() {
   return this->noRaiz;
}


/**
* Retorna ordem do grafo.
*/
int Grafo::getOrdem() {
   return this->ordem;
}

/**
* Retorna Aresta Aux.
*/
// Aresta* Grafo::getArestaAux() {
//    return this->arestaAux;
// }

/**
* Retorna grau de entrada.
*/
int Grafo::getGrauEntrada() {
   int inputDegree = 0;

   No* noAux = noRaiz;
   while ( noAux != NULL ) {
       if ( noAux->getGrauEntrada() > inputDegree ) {
           inputDegree = noAux->getGrauEntrada();
       }
       noAux = noAux->getProxNo();
   }
   return inputDegree;
}

/**
* Retorna grau de saida.
*/
int Grafo::getGrauSaida() {
   int outputDegree = 0;

   No* noAux = noRaiz;
   while ( noAux != NULL ) {
       if ( noAux->getGrauSaida() > outputDegree ) {
           outputDegree = noAux->getGrauSaida();
       }
       noAux = noAux->getProxNo();
   }
   return outputDegree;
}

/**
* Retorna grau do grafo.
*/

string Grafo::getGrau() {
    int inputDegree = 0;
    int outputDegree = 0;
    std::stringstream auxSS;
    auxSS.str(string());
    inputDegree = getGrauEntrada();
    outputDegree = getGrauSaida();

    if ( isDigraph() ) {
        auxSS << "O grau de ENTRADA do grafo é " << inputDegree << " \ne o de SAIDA e " << outputDegree << ".";
    } else {
        auxSS << "O GRAU do grafo e " << outputDegree << ".";
    }

    return auxSS.str();
}

/**
* @return true - se grafo e digrafo.
*        false - se grafo NAO e digrafo.
*/
bool Grafo::isDigraph() {
   return this->digrafo;
}
/**
* @param idNo (id do nó)
* @return No* - uma "lista" de nós que pertencem ao fecho transitivo do nó de id == idNo. Nessa lista o ponteiro proxNo de cada Nó aponta para o proximo Nó
*/

Aresta* Grafo::fechoTransitivo(int idNo)
{
    if(this->digrafo)
    {
        No *noProcurado = this->procurarNoPeloId(idNo);
        
        if(noProcurado){
            Aresta *pAresta = noProcurado->getPrimeiraAresta();

            cout << "Fecho Transitivo do vertice " << idNo << " : ";
            
            if(pAresta){

                while(pAresta){
                    cout << " [ " << pAresta->getNoDestino()->getIdNo() << " , " << pAresta->getNoDestino()->getPeso() << " ] ";
                    
                    pAresta = pAresta->getProxAresta();   
                }
                cout << endl;
                return noProcurado->getPrimeiraAresta();
            }
            else cout << "O vertice " << idNo << " eh isolado.";
        }
        else cout << "O vertice " << idNo << " nao esta no grafo.";
    }
    else cout << "O grafo nao é direcionado.";
    cout << endl;
    return NULL;
}

void Grafo::fechoTransitivoIndireto(int idNo)
{
    if(this->digrafo)
    {
        if(this->procurarNoPeloId(idNo))
        {
            Aresta* vetArestas = this->fechoTransitivo(idNo);

            cout << "Fecho Transitivo indireto do vertice " << idNo << " --> \t";
            while(vetArestas)
            {
                this->fechoTransitivo(vetArestas->getNoDestino()->getIdNo());
                vetArestas = vetArestas->getProxAresta();
                cout << "\t\t\t\t\t\t";
            }
            cout << endl;
        }
        else cout << "O vertice " << idNo << " nao esta no grafo." << endl;
    }
    else cout << "O grafo nao é direcionado." << endl;


}