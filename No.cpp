#include "No.h"

using namespace std;

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

bool No::procuraAresta(Aresta *aresta, No *noDestino)
{
    if(aresta != NULL){
        if(aresta->getNoDestino() == noDestino) return true;
        cout << No  q
    }   
    return false;
}