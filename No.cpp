#include "No.h"

using namespace std;

No::No() {
    // this->idNo = -1; 
    this->pesoNo = 0;
    this->grauEntrada = 0;
    this->grauSaida = 0;
    this->proxNo = NULL;
    this->primeiraAresta = NULL;
}

No::No(int name) {
    this->idNo = name;
    this->pesoNo = 0;
    this->grauEntrada = 0;
    this->grauSaida = 0;
    this->proxNo = NULL;
    this->primeiraAresta = NULL;
}

No::No(int name, int peso) {
    this->idNo = name;
    this->pesoNo = peso;
    this->grauEntrada = 0;
    this->grauSaida = 0;
    this->proxNo = NULL;
    this->primeiraAresta = NULL;
}

No::~No() {
    Aresta* auxEdge = getPrimeiraAresta();
    while ( auxEdge != NULL ) {
        Aresta* auxNextEdge = auxEdge->getProxAresta();
        delete( auxEdge );
        auxEdge = auxNextEdge;       
    }
    // delete( searchInfo );
}

void No::setIdNo(int valor) {
    this->idNo = valor;
}

void No::setPeso(int valor) {
    this->pesoNo = valor;
}
int No::getPeso() {
    return this->pesoNo;
}

int No::getGrau() {
    return this->grauSaida;
}
void No::incGrau() {
    this->grauEntrada++;
    this->grauSaida++;
}
void No::decGrau() {
    this->grauEntrada--;
    this->grauSaida--;
}

int No::getGrauEntrada() {
    return this->grauEntrada;
}
void No::incrementaGrauEntrada() {
    this->grauEntrada++;
}
void No::decrementaGrauEntrada() {
    this->grauEntrada--;
}

int No::getGrauSaida() {
    return this->grauSaida;
}

void No::incrementaGrauSaida() {
    this->grauSaida++;
}
void No::decrementaGrauSaida() {
    this->grauSaida--;
}

void No::setProxNo(No *novoNo) {
    this->proxNo = novoNo;
}

No* No::getProxNo(){
    return this->proxNo;
}

int No::getIdNo(){
    return this->idNo;
}

void No::setPrimeiraAresta(Aresta *novaAresta) {
    this->primeiraAresta = novaAresta;
}

void No::setUltimaAresta(Aresta *novaAresta) {
    this->ultimaAresta = novaAresta;
}

Aresta* No::getPrimeiraAresta(){
    return this->primeiraAresta;
}

Aresta* No::getUltimaAresta(){
    return this->ultimaAresta;
}

void No::setOrdemMaisUm(){
    this->pesoNo++;
}

bool No::procuraAresta(Aresta *aresta, No *noDestino)
{
    if(aresta != NULL){
        if(aresta->getNoDestino() == noDestino) return true;
        return this->procuraAresta(aresta->getProxAresta(), noDestino);
    }   
    return false;
}

Aresta* No::procuraAresta(No* noProcurado)
{
    Aresta *aresta = this->getPrimeiraAresta();
    Aresta *arestaAnterior = NULL;

    while(aresta)
    {
        if(aresta->getNoDestino() == noProcurado) break;
        arestaAnterior = aresta;
        aresta = aresta->getProxAresta();
    }

    return arestaAnterior;
}

bool No::verificaRemoveAresta(No *destino)
{
    Aresta *aresta = this->procuraAresta(destino);

    if(aresta == NULL){
        // Como implementar um try catch aqui stenio? ta dando segmentation fault
        Aresta *proxima = this->getPrimeiraAresta();
        if(proxima){
            if(proxima->getProxAresta()){
                this->setPrimeiraAresta(proxima->getProxAresta());
            }
            else{
                this->setPrimeiraAresta(NULL);
                this->setUltimaAresta(NULL);
            }
        }
    }
    else if(aresta == this->getUltimaAresta()){
        cout << "Os vertices nao estao conectados." << endl;
        return false;
    }
    else
    {  
        // Como implementar um try catch aqui stenio? ta dando segmentation fault

        Aresta *proximaAresta = aresta->getProxAresta();
        if(proximaAresta != this->getUltimaAresta()){
            aresta->setProxAresta(proximaAresta->getProxAresta());
        }
        else{
            this->setUltimaAresta(aresta);
            aresta->setProxAresta(NULL);
        }
    }
    delete [] aresta;
    this->decrementaGrauSaida();
    destino->decrementaGrauEntrada();
    return true;

}