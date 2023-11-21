#include "../Headers/No.h"

using namespace std;

No::No(int idNo, int x, int y) {
    this->idNo = idNo;
    this->x = x;
    this->y = y;
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


bool No::procuraAresta(Aresta *aresta, No *noDestino)
{
    if(aresta != NULL){
        if(aresta->getNoDestino() == noDestino) return true;
        return this->procuraAresta(aresta->getProxAresta(), noDestino);
    }   
    return false;
}

Aresta* No::getArestaAnterior(No *noDestino)
{   
    // se existe a primeira aresta
    if(this->getPrimeiraAresta()){
        // retorna a aresta anterior à aresta onde o noDestino == noDestino(parametro)
        return this->auxGetArestaAnterior(this->getPrimeiraAresta(), noDestino);
    }
    else return NULL;
}

Aresta* No::auxGetArestaAnterior(Aresta *aresta, No *noDestino)
{
    if(aresta->getProxAresta() != NULL){
        if(aresta->getProxAresta()->getNoDestino() == noDestino) return aresta;
    }
    return aresta;
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
    aresta->~Aresta();
    this->decrementaGrauSaida();
    destino->decrementaGrauEntrada();
    return true;

}

bool No::insereArestaNo(No *noDestino, float pesoAresta)
{
    if(this->getPrimeiraAresta() == NULL) // NÃO EXISTE NENHUMA ARESTA NESSE NO 
    {  
        Aresta *aresta = new Aresta(noDestino, NULL, pesoAresta);
        this->setPrimeiraAresta(aresta);
        this->setUltimaAresta(aresta);
        this->incrementaGrauSaida();
        noDestino->incrementaGrauEntrada();
        return true;
    }
    else
    {
        if(!this->procuraAresta(this->getPrimeiraAresta(), noDestino))
        {   
            Aresta *aresta = new Aresta(noDestino, NULL, pesoAresta);
            Aresta *ultimaAresta = this->getUltimaAresta();
            ultimaAresta->setProxAresta(aresta);
            this->setUltimaAresta(aresta);
            this->incrementaGrauSaida();
            noDestino->incrementaGrauEntrada();
            return true;
        }
    }
    return false;
}

void No::removeAresta(No *noRemovido)
{
    Aresta *aresta = this->getPrimeiraAresta();

    // caso a aresta a ser removida seja a primeira
    if(aresta->getNoDestino() == noRemovido)
    {
        this->setPrimeiraAresta(aresta->getProxAresta());
        aresta->~Aresta();
    }
    else{
        // passa pra proxima aresta
        Aresta *anterior = aresta;
        aresta = aresta->getProxAresta();
        while(aresta)
        {
            if(aresta->getNoDestino() == noRemovido)
            {
                anterior->setProxAresta(aresta->getProxAresta());
                delete aresta;
            }
            anterior = aresta;
            aresta = aresta->getProxAresta();
        }    
    }
    
}

Aresta* No::verificaNoAresta(No *no)
{
    Aresta *aresta = this->getPrimeiraAresta();
    if(aresta){
        if(aresta->getNoDestino() == no){
            return aresta;
        }
    }
    return NULL;
}

void No::imprimeArestas(){
    Aresta *aux = this->primeiraAresta;

    while(aux){
        cout << " -> " <<  "[ " << aux->getNoDestino()->getIdNo() << " | " << aux->getPeso() << " ]";
        aux = aux->getProxAresta();
    }
    cout << endl;
}

bool No::in_percorridos(No percorridos [], int tam){
    for(int i=0; i<tam; i++){
        if(this->getIdNo() == percorridos[i].getIdNo()){
            return true;
        }
    }
    return false;
}