#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <unordered_map>
#include "../Headers/Grafo.h"

using namespace std;
typedef vector<No *> NodeVector;
// typedef EdgeSearchInfo EdgeInfo;

//------------------------ Construtors and Destrutors ------------------------

Grafo::Grafo()
{
    this->numAresta = 0;
    this->ordem = 0;
    this->digrafo = false;
}

/**
 * Destructor
 */
Grafo::~Grafo()
{
    hashNo.clear();
}

//----------------------------------- Seters -----------------------------------

/**
 * Funcao incrementa Ordem do grafo em um.
 */
void Grafo::incOrdem()
{
    this->ordem++;
}

/**
 * Funcao decrementa Ordem do grafo em um.
 */
void Grafo::decOrdem()
{
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
No *Grafo::procurarNoPeloId(int idFindNo)
{
    return hashNo[idFindNo];
}


No *Grafo::insereNo(int idNo, float x, float y, int demanda)
{
    No *procurado = procurarNoPeloId(idNo);

    if (procurado == NULL)
    {
        No *novoNo = new No(idNo, x, y, demanda);
        hashNo[idNo] = novoNo;
        this->incOrdem();
        return novoNo;
    }
    else
    {
        return procurado;
    }
}


No *Grafo::insereNo(No* novoNo)
{
    if(!hashNo[novoNo->getIdNo()]){
        hashNo[novoNo->getIdNo()] = novoNo;
        this->incOrdem();
    }
    return novoNo;
}


void Grafo::imprime()
{
    for (const auto& no : hashNo) {
        cout << " | " << endl
            << " V " << endl;
        cout << " " << no.first;
        no.second->imprimeArestas();
    }
    cout << " | " << endl
        << " = " << endl;
}


bool Grafo::removeNo(int idNo, bool isDigrafo)
{
    if(hashNo[idNo]){
        hashNo.erase(idNo);
        this->decOrdem();
        return true;
    } 
    else{
        cout << "Nó de id " << idNo << " não esta no grafo." << endl
            << endl;
        return false;
    }
}


void Grafo::removeAresta(No *origem, No *destino)
{
    Aresta *delAresta = origem->getPrimeiraAresta();
    Aresta *anterior = NULL;

    if (delAresta)
    {

        // percorre as arestas de origem até encontrar a aresta com destino
        while (delAresta->getProxAresta() && delAresta->getNoDestino() != destino)
        {
            anterior = delAresta;
            delAresta = delAresta->getProxAresta();
        }

        // nao encontrou a aresta com no destino
        if (delAresta->getProxAresta() == NULL && delAresta->getNoDestino() != destino)
        {
            exit(1);
        }
        else
        {
            // A aresta do destino é a primeira, troca-se o ponteiro ultima aresta
            if (delAresta == origem->getPrimeiraAresta())
            {
                origem->setPrimeiraAresta(delAresta->getProxAresta());
            }

            // A aresta do destino é a ultima, troca-se o ponteiro ultima aresta
            if (delAresta == origem->getUltimaAresta())
            {
                origem->setUltimaAresta(anterior);
            }

            anterior->setProxAresta(delAresta->getProxAresta());

            delete delAresta;
        }
    }
}

No* Grafo::insertAresta(No* NoOrigem, No* destino, double pesoAresta){
    No* NoDestino = this->insereNo(destino->getIdNo(), destino->getX(), destino->getY(), destino->getDemanda());
    if(this->procurarNoPeloId(NoOrigem->getIdNo()) == NULL){
        this->insereNo(NoOrigem);
        // this->imprime();
    }
    if(this->procurarNoPeloId(NoDestino->getIdNo()) == NULL){
        this->insereNo(NoDestino);
    }
    if (NoOrigem->insereArestaNo(NoDestino, pesoAresta) && NoDestino->insereArestaNo(NoOrigem, pesoAresta))
    {
        this->numAresta++;
        return NoDestino;
    }
    return NoDestino;
}


bool Grafo::insertAresta(int idNoOrigem, int idNoDestino, float pesoAresta)
{
    No *noFonte, *noDestino;

    if (idNoOrigem == idNoDestino)
        return false;

    noFonte = procurarNoPeloId(idNoOrigem);

    if (noFonte == NULL)
    {
        // noFonte = this->insereNo(idNoOrigem, 0);
        this->incOrdem();
    }

    noDestino = procurarNoPeloId(idNoDestino);

    if (noDestino == NULL)
    {
        // noDestino = this->insereNo(idNoDestino, 0);
        this->incOrdem();
    }

    if (this->digrafo)
    {
        if (noFonte->insereArestaNo(noDestino, pesoAresta))
        {
            this->numAresta++;
            return true;
        }
    }
    else
    {
        if (noFonte->insereArestaNo(noDestino, pesoAresta) && noDestino->insereArestaNo(noFonte, pesoAresta))
        {
            this->numAresta++;
            return true;
        }
    }

    return false;
}


bool Grafo::removeAresta(int idNoOrigem, int idNoDestino, bool isDirected)
{
    // Procura o nó origem e busca na sua lista de arestas o idNoDestino
    // Se encontrar, remove. Se não, retorna zero

    No *origem = this->procurarNoPeloId(idNoOrigem);
    No *destino = this->procurarNoPeloId(idNoDestino);

    if (origem && destino)
    {
        if (isDirected)
        {
            if (origem->verificaRemoveAresta(destino))
            {
                this->decrementaNumAresta();
                return true;
            }
        }
        else
        {
            if (origem->verificaRemoveAresta(destino) && destino->verificaRemoveAresta(origem))
            {
                this->decrementaNumAresta();
                return true;
            }
        }
    }

    return false;
}

bool Grafo::decrementaNumAresta()
{
    if (this->isDigraph())
    {
        if (this->numAresta > 0)
            this->numAresta--;
        return true;
    }
    else
    {
        if (this->numAresta >= 2)
            this->numAresta -= 2;
        return true;
    }
    return false;
}


int Grafo::getNumAresta()
{
    return this->numAresta;
}


int Grafo::getOrdem()
{
    return this->ordem;
}


int Grafo::getGrauEntrada()
{
    int inputDegree = 0;

    for (const auto& no : hashNo) {
        if (no.second->getGrauEntrada() > inputDegree)
            inputDegree = no.second->getGrauEntrada(); 
    }
    return inputDegree;
}


int Grafo::getGrauSaida()
{
    int outputDegree = 0;

    for (const auto& no : hashNo) {
        if (no.second->getGrauSaida() > outputDegree)
            outputDegree = no.second->getGrauSaida(); 
    }
    return outputDegree;
}


string Grafo::getGrau()
{
    int inputDegree = 0;
    int outputDegree = 0;
    std::stringstream auxSS;
    auxSS.str(string());
    inputDegree = getGrauEntrada();
    outputDegree = getGrauSaida();

    if (isDigraph())
    {
        auxSS << "O grau de ENTRADA do grafo é " << inputDegree << " \ne o de SAIDA e " << outputDegree << ".";
    }
    else
    {
        auxSS << "O GRAU do grafo e " << outputDegree << ".";
    }

    return auxSS.str();
}


bool Grafo::isDigraph()
{
    return this->digrafo;
}


void Grafo::fechoTransitivoDireto(int idNo) // imprimindo a coluna
{
    double infinito = 99999;
    auto preencheHashMatrizInfinito = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        auto infinito = 99999;
        for (const auto& coluna : hashNo) {
            for (const auto& linha : hashNo) {
                matriz[linha.first][coluna.first] = infinito;
            }   
        }
        return matriz;
    };

    auto preencheHash = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        for (const auto& no : hashNo) {
            matriz[no.first][no.first] = 0;
            for (Aresta *aresta = no.second->getPrimeiraAresta(); aresta != NULL; aresta = aresta->getProxAresta())
            {
                matriz[no.first][aresta->getNoDestino()->getIdNo()] = aresta->getPeso();
            }
        }
        return matriz;
    };

    auto procuraMenorCaminho = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        for (const auto& k : hashNo) {
            for (const auto& linha : hashNo) {
                for (const auto& coluna : hashNo) {
                    matriz[linha.first][coluna.first] = min(matriz[linha.first][coluna.first], matriz[linha.first][k.first] + matriz[k.first][coluna.first]);
                }
            }
        }
        
        return matriz;
    };

    unordered_map<int, unordered_map<int, int>> matriz;
    matriz = preencheHashMatrizInfinito(matriz);
    matriz = preencheHash(matriz);
    matriz = procuraMenorCaminho(matriz);

    cout << "Fecho transitivo direto do vertice " << idNo << " : ";
    for (auto linha : matriz)
    {
        if (linha.first == idNo)
        {

            for (auto coluna : linha.second)
            {
                if (matriz[linha.first][coluna.first] < infinito)
                    // imprimir indice da linha e coluna
                    cout << coluna.first << " ";
            }
            cout << endl;
            break; // Para sair do loop após imprimir a linha desejada, se for único
        }
    }
}


void Grafo::fechoTransitivoIndiretoFunc(int idNo) // imprimindo a linha
{
    double infinito = 99999;
    auto preencheHashMatrizInfinito = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        auto infinito = 99999;
        for (const auto& coluna : hashNo) {
            for (const auto& linha : hashNo) {
                matriz[linha.first][coluna.first] = infinito;
            }
        }
        return matriz;
    };

    auto preencheHash = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        for (const auto& no : hashNo) {
            for (Aresta *aresta = no.second->getPrimeiraAresta(); aresta != NULL; aresta = aresta->getProxAresta())
            {
                matriz[no.first][aresta->getNoDestino()->getIdNo()] = aresta->getPeso();
            }
        }
        return matriz;
    };

    auto procuraMenorCaminho = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        for (const auto& k : hashNo) {
            for (const auto& linha : hashNo) {
                for (const auto& coluna : hashNo) {
                    matriz[linha.first][coluna.first] = min(matriz[linha.first][coluna.first], matriz[linha.first][k.first] + matriz[k.first][coluna.first]);
                }
            }    
        }
        return matriz;
    };

    unordered_map<int, unordered_map<int, int>> matriz;
    matriz = preencheHashMatrizInfinito(matriz);
    matriz = preencheHash(matriz);
    matriz = procuraMenorCaminho(matriz);

    cout << "Fecho transitivo indireto do vertice " << idNo << " : ";
    for (auto coluna : matriz)
    {
        if (coluna.first == idNo)
        {

            for (auto linha : coluna.second)
            {
                if (matriz[linha.first][coluna.first] < infinito)
                    // imprimir indice da linha e coluna
                    cout << linha.first << " ";
            }
            cout << endl;
            break; // Para sair do loop após imprimir a linha desejada, se for único
        }
    }
}


void Grafo::auxFechoTransitivoDireto(No *no, int *percorridos, int tam, int *n)
{
    if (!no)
        exit(1); // se no nao existe, para a recusividade

    Aresta *aresta = no->getPrimeiraAresta();
    while (aresta)
    {
        if (!estarNoVetor(percorridos, aresta->getNoDestino()->getIdNo(), *n))
        {
            percorridos[(*n)++] = aresta->getNoDestino()->getIdNo();
            // cout << " [ " << aresta->getNoDestino()->getIdNo() << " , " << aresta->getNoDestino()->getPeso() << " ] ";
            if (aresta->getProxAresta()->getNoDestino())
                auxFechoTransitivoDireto(aresta->getProxAresta()->getNoDestino(), percorridos, tam, n);
        }
        aresta = aresta->getProxAresta();
    }
}


bool Grafo::estarNoVetor(int vetor[], int idNo, int tam)
{
    for (int i = 0; i < tam; i++)
        if (vetor[i] == idNo)
            return true;

    return false;
}


bool Grafo::arestaNoVetor(int vetor[], No *noAtual, int tam)
{
    Aresta *aresta = noAtual->getPrimeiraAresta();
    while (aresta)
    {
        if (estarNoVetor(vetor, aresta->getNoDestino()->getIdNo(), tam))
            return true;
        aresta = aresta->getProxAresta();
    }
    return false;
}


void Grafo::imprimeVetor(int vetor[], int tam)
{
    for (int i = 0; i < tam; i++)
    {
        cout << vetor[i] << ", ";
    }
    cout << endl;
}


Grafo* Grafo::prim(int idNo)
{
    auto preenchePercorridos = [](No* no, int* percorridos, int *n){
        for(Aresta* aresta = no->getPrimeiraAresta(); aresta!= NULL; aresta = aresta->getProxAresta()){
            percorridos[(*n)++] = aresta->getNoDestino()->getIdNo();
        }
        return percorridos;
    };

    auto verificaInPercorridos = [](int* percorridos, int n, int idno){
        for(int i=0; i<n; i++){
            if(percorridos[i] == idno)
                return true;
        }
        return false;
    };

    auto arestaMenorPeso = [this](No* no){
        Aresta* menorPeso = no->getPrimeiraAresta();
        
        for(Aresta* percorre=no->getPrimeiraAresta(); percorre!= NULL; percorre = percorre->getProxAresta()){
            if(percorre->getPeso() < menorPeso->getPeso()){
                menorPeso = percorre;
            }
        }
        return menorPeso;
    };

    No* no = this->procurarNoPeloId(idNo);

    if(!no){
        cout << "o no não está no grafo." << endl;
        return NULL; 
    }

    int* percorridos;
    Grafo* arvoreGeradoraMinima = new Grafo();
    // arvoreGeradoraMinima->insereNo(no->getIdNo(), no->getPeso());

    for(int i=0; i<this->getOrdem(); i++){
        Aresta *menorPeso = arestaMenorPeso(no);
        if(!menorPeso)
            break;
        no = menorPeso->getNoDestino(); 
        // arvoreGeradoraMinima->insereNo(no->getIdNo(), no->getPeso());        
    }

    arvoreGeradoraMinima->imprime();
    return NULL;
}

// double Grafo::somaPesoArestas(){
//     double soma = 0;
//     for(No *n = this->getNoRaiz(); n; n=n->getProxNo())
//         for(Aresta *a = n->getPrimeiraAresta(); a; a=a->getProxAresta())
//             soma += a->getPeso();
//     return soma;
// }

Grafo* Grafo::caminhoEmProfundidade(int idNo)
{
    // auto empilha = [](No* no, No *pilha){
    //     no->setProxNo(pilha);
    //     pilha = no;
    // };

    // auto inserePercorridos = [](No *no, No *percorridos){
    //     no->setProxNo(percorridos);
    //     percorridos = no;
    // };

    // auto percorrido = [](No no, No percorridos){ 
    //     while(&percorridos != NULL){
    //         if(&percorridos == &no) return true;
    //         percorridos = *percorridos.getProxNo();  
    //     }
    //     return false;
    // };


    // No* no = procurarNoPeloId(idNo);

    // if(!no){
    //     cout << "O no " << idNo << " nao esta no grafo";
    //     return NULL;
    // }

    // No* pilha;
    // No* percorridos;
    // Grafo* caminhoProfundidade = new Grafo(this->isDigraph());

    // while(pilha){
    //     if(!percorrido(*no, *percorridos)){
    //         caminhoProfundidade->insertAresta()
    //         for(Aresta *adjacentes=no->getPrimeiraAresta(); adjacentes; adjacentes=adjacentes->getProxAresta()){
    //             empilha(adjacentes->getNoDestino(),pilha);
    //         }
    //     }
    //     else{

    //     }
    //     no = pilha;
    //     pilha = pilha->getProxNo();
    //     inserePercorridos(no, percorridos);
    // }

return NULL;    
}

No* Grafo::getGalpao()
{
    for (const auto& no : hashNo) {
        if(no.second->getDemanda() == 0)
            return no.second;
    }
    return NULL;
}  