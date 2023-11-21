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
    this->noRaiz = NULL;
    this->digrafo = false;
}

/**
 * Destructor
 */
Grafo::~Grafo()
{
    No *noAux = getNoRaiz();
    No *noAnterior;
    while (noAux != NULL)
    {
        noAnterior = noAux->getProxNo();
        delete (noAux);
        noAux = noAnterior;
    }
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
No *Grafo::procurarNoPeloId(int idFindNo, bool anteiror = false)
{

    // bool anterior serve para caso necessario, retorne o anterior ao nó que contem id = id;

    No *noAux = noRaiz;
    No *noAuxAnterior = NULL;

    while (noAux != NULL)
    {
        if (noAux->getIdNo() == idFindNo)
        {
            break;
        }
        noAuxAnterior = noAux;
        noAux = noAux->getProxNo();
    }
    if (anteiror)
        return noAuxAnterior;
    else
        return noAux;
}


No *Grafo::insereNo(int idNo, float x, float y, int demanda = 0)
{
    No *procurado = procurarNoPeloId(idNo, 0);

    if (procurado == NULL)
    {
        No *novoNo = new No(idNo, x, y, demanda);

        if (this->noRaiz == NULL)
        {
            this->noRaiz = novoNo;
        }
        else
        {
            novoNo->setProxNo(noRaiz);
            noRaiz = novoNo;
        }
        return novoNo;
    }
    else
    {
        return procurado;
    }
}


No *Grafo::insereNo(No* novoNo)
{
    if (this->noRaiz == NULL)
    {
        this->noRaiz = novoNo;
    }
    else
    {
        novoNo->setProxNo(noRaiz);
        noRaiz = novoNo;
    }
    return novoNo;
}


void Grafo::imprime()
{

    No *aux = this->noRaiz;
    while (aux)
    {
        cout << " | " << endl
             << " V " << endl;
        cout << " " << aux->getIdNo();
        aux->imprimeArestas();
        aux = aux->getProxNo();
    }
    cout << " | " << endl
         << " = " << endl;
}


bool Grafo::removeNo(int idNo, bool isDigrafo)
{

    No *anterior = this->procurarNoPeloId(idNo, 1);
    No *removido;
    // caso onde o nó é a raiz;
    if (anterior == NULL && this->getNoRaiz()->getIdNo() == idNo)
    {
        removido = this->getNoRaiz();
        this->noRaiz = removido->getProxNo();
    }
    // caso onde o no está no meio da lista
    else if (anterior->getProxNo() != NULL)
    {
        removido = anterior->getProxNo();
        anterior->setProxNo(removido->getProxNo());
    }
    // nao encontrou nó a ser removido.
    else
    {
        cout << "Nó de id " << idNo << " não esta no grafo." << endl
             << endl;
        return false;
    }

    if (removido)
    {
        Aresta *aux = removido->getPrimeiraAresta();

        // remove a aresta do removido com cada no vizinho dele
        while (aux)
        {
            aux->getNoDestino()->removeAresta(removido);
            aux = aux->getProxAresta();
        }

        delete removido;
        cout << "Nó de id " << idNo << " removido com sucesso!" << endl
             << endl;
        return true;
    }
    return false;
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

bool Grafo::insertAresta(No* NoOrigem, No* destino, double pesoAresta){
    No* NoDestino = this->insereNo(destino->getIdNo(), destino->getX(), destino->getY(), destino->getDemanda());
    if(this->procurarNoPeloId(NoOrigem->getIdNo()) == NULL){
        this->insereNo(NoOrigem);
        this->imprime();
    }
    if(this->procurarNoPeloId(NoDestino->getIdNo()) == NULL){
        this->insereNo(NoDestino);
    }
    if (NoOrigem->insereArestaNo(NoDestino, pesoAresta) && NoDestino->insereArestaNo(NoOrigem, pesoAresta))
    {
        this->numAresta++;
        return true;
    }
    return false;
}


bool Grafo::insertAresta(int idNoOrigem, int idNoDestino, float pesoAresta)
{
    No *noFonte, *noDestino;

    if (idNoOrigem == idNoDestino)
        return false;

    noFonte = procurarNoPeloId(idNoOrigem, 0);

    if (noFonte == NULL)
    {
        // noFonte = this->insereNo(idNoOrigem, 0);
        this->incOrdem();
    }

    noDestino = procurarNoPeloId(idNoDestino, 0);

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

    No *origem = this->procurarNoPeloId(idNoOrigem, 0);
    No *destino = this->procurarNoPeloId(idNoDestino, 0);

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


int Grafo::AtualizaNumAresta()
{
    No *noAuxiliar = noRaiz;
    this->numAresta = 0;

    while (noAuxiliar != NULL)
    {
        this->numAresta += noAuxiliar->getGrau();
        noAuxiliar = noAuxiliar->getProxNo();
    }

    if (this->digrafo)
        return this->numAresta;
    return this->numAresta / 2; // PERGUNTAR PQ DIVIDE POR 2
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

    No *noAux = noRaiz;
    while (noAux != NULL)
    {
        if (noAux->getGrauEntrada() > inputDegree)
        {
            inputDegree = noAux->getGrauEntrada();
        }
        noAux = noAux->getProxNo();
    }
    return inputDegree;
}


int Grafo::getGrauSaida()
{
    int outputDegree = 0;

    No *noAux = noRaiz;
    while (noAux != NULL)
    {
        if (noAux->getGrauSaida() > outputDegree)
        {
            outputDegree = noAux->getGrauSaida();
        }
        noAux = noAux->getProxNo();
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

        for (No *coluna = this->getNoRaiz(); coluna != NULL; coluna = coluna->getProxNo())
        {
            for (No *linha = this->getNoRaiz(); linha != NULL; linha = linha->getProxNo())
            {

                matriz[linha->getIdNo()][coluna->getIdNo()] = infinito;
            }
        }
        return matriz;
    };

    auto preencheHash = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        for (No *no = this->getNoRaiz(); no != NULL; no = no->getProxNo())
        {
            matriz[no->getIdNo()][no->getIdNo()] = 0;
            for (Aresta *aresta = no->getPrimeiraAresta(); aresta != NULL; aresta = aresta->getProxAresta())
            {

                matriz[no->getIdNo()][aresta->getNoDestino()->getIdNo()] = aresta->getPeso();
            }
        }
        return matriz;
    };

    auto procuraMenorCaminho = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        for (No *k = this->getNoRaiz(); k != NULL; k = k->getProxNo())
        {
            for (No *linha = this->getNoRaiz(); linha != NULL; linha = linha->getProxNo())
            {
                for (No *coluna = this->getNoRaiz(); coluna != NULL; coluna = coluna->getProxNo())
                {
                    matriz[linha->getIdNo()][coluna->getIdNo()] = min(matriz[linha->getIdNo()][coluna->getIdNo()], matriz[linha->getIdNo()][k->getIdNo()] + matriz[k->getIdNo()][coluna->getIdNo()]);
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

        for (No *coluna = this->getNoRaiz(); coluna != NULL; coluna = coluna->getProxNo())
        {
            for (No *linha = this->getNoRaiz(); linha != NULL; linha = linha->getProxNo())
            {

                matriz[linha->getIdNo()][coluna->getIdNo()] = infinito;
            }
        }
        return matriz;
    };

    auto preencheHash = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        for (No *no = this->getNoRaiz(); no != NULL; no = no->getProxNo())
        {
            matriz[no->getIdNo()][no->getIdNo()] = 0;
            for (Aresta *aresta = no->getPrimeiraAresta(); aresta != NULL; aresta = aresta->getProxAresta())
            {

                matriz[no->getIdNo()][aresta->getNoDestino()->getIdNo()] = aresta->getPeso();
            }
        }
        return matriz;
    };

    auto procuraMenorCaminho = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        for (No *k = this->getNoRaiz(); k != NULL; k = k->getProxNo())
        {
            for (No *linha = this->getNoRaiz(); linha != NULL; linha = linha->getProxNo())
            {
                for (No *coluna = this->getNoRaiz(); coluna != NULL; coluna = coluna->getProxNo())
                {
                    matriz[linha->getIdNo()][coluna->getIdNo()] = min(matriz[linha->getIdNo()][coluna->getIdNo()], matriz[linha->getIdNo()][k->getIdNo()] + matriz[k->getIdNo()][coluna->getIdNo()]);
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


void Grafo::fechoTransitivoIndireto(int idNo)
{
    if (this->digrafo)
    {
        No *procurado = this->procurarNoPeloId(idNo);
        if (procurado)
        {
            cout << "Fecho transitivo indireto do vertice " << idNo << " : ";

            No *percorre = this->getNoRaiz();

            int vetor[this->getOrdem()] = {0};
            vetor[0] = procurado->getIdNo();
            int tam = 1;
            bool reiniciar = false;

            while (percorre)
            {
                if (!estarNoVetor(vetor, percorre->getIdNo(), tam))
                {
                    if (arestaNoVetor(vetor, percorre, tam))
                    {
                        vetor[tam] = percorre->getIdNo();
                        tam = tam + 1;
                        percorre = this->getNoRaiz();
                        reiniciar = true;
                    }
                }
                if (reiniciar == false)
                    percorre = percorre->getProxNo();
                else
                    reiniciar = false;
            }
            imprimeVetor(vetor, tam);
        }
        else
        {
            cout << "O no " << idNo << " nao esta no grafo." << endl;
        }
    }
}


int Grafo::Djkstra(int idNoinicio, int idNofim)
{

    auto preencheHash = [](int idNoinicio, unordered_map<int, int> distance, No *no)
    {
        auto infinito = numeric_limits<double>::infinity();
        while (no)
        {
            distance[no->getIdNo()] = infinito;
            no = no->getProxNo();
        }
        distance[idNoinicio] = 0;
    };

    No *inicio = procurarNoPeloId(idNoinicio);
    No *fim = procurarNoPeloId(idNofim);

    if (!inicio || !fim)
    {
        cout << "o no " << idNoinicio << " e/ou " << idNofim << " nao estao no grafo.";
        return -1;
    }

    unordered_map<int, int> distance;
    unordered_map<int, bool> percorrido;

    preencheHash(idNoinicio, distance, this->getNoRaiz());

    Aresta *aresta = inicio->getPrimeiraAresta();
    No *no = this->getNoRaiz();

    while (no)
    {
        while (aresta)
        {
            int idNo = aresta->getNoDestino()->getIdNo();

            if (distance[idNo] > distance[idNo] + aresta->getPeso())
            {
                distance[idNoinicio] = aresta->getPeso();
            }

            aresta = aresta->getProxAresta();
        }
        no = no->getProxNo();
    }

    for (const auto &pair : distance)
    {
        std::cout << "Chave: " << pair.first << ", Valor: " << pair.second << std::endl;
    }

    return distance[idNofim];
}


int* Grafo::Floyd(int idNoinicio, int idNofim)
{
    auto preencheHashMatrizInfinito = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        int infinito = 9999;
        // int infinito = 9999999;
        // int infinito = numeric_limits<int>::infinity();  
        // cout << infinito << endl;

        for (No *coluna = this->getNoRaiz(); coluna != NULL; coluna = coluna->getProxNo())
        {
            for (No *linha = this->getNoRaiz(); linha != NULL; linha = linha->getProxNo())
            {
                matriz[linha->getIdNo()][coluna->getIdNo()] = infinito;
            }
        }
        return matriz;
    };

    auto preencheHash = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        for (No *no = this->getNoRaiz(); no != NULL; no = no->getProxNo())
        {
            matriz[no->getIdNo()][no->getIdNo()] = 0;
            for (Aresta *aresta = no->getPrimeiraAresta(); aresta != NULL; aresta = aresta->getProxAresta())
            {

                matriz[no->getIdNo()][aresta->getNoDestino()->getIdNo()] = aresta->getPeso();
            }
        }
        return matriz;
    };

    auto imprimeMatriz = [this](unordered_map<int, unordered_map<int, int>> matriz)
    {
        cout << "   ";
        for (No *no = this->getNoRaiz(); no != NULL; no = no->getProxNo())
        {
            cout << no->getIdNo() << " ";
        }
        cout << endl
             << "   ";
        for (No *no = this->getNoRaiz(); no != NULL; no = no->getProxNo())
        {
            cout << "-"
                 << " ";
        }
        cout << endl;

        for (No *coluna = this->getNoRaiz(); coluna != NULL; coluna = coluna->getProxNo())
        {
            cout << coluna->getIdNo() << "| ";
            for (No *linha = this->getNoRaiz(); linha != NULL; linha = linha->getProxNo())
            {

                cout << matriz[linha->getIdNo()][coluna->getIdNo()] << " ";
            }
            cout << endl;
        }
    };

    auto procuraMenorCaminho = [this](unordered_map<int, unordered_map<int, int>> matriz, int idNoinicio, int idNofim, int* n)
    {
        int* caminho = new int[this->ordem];
        caminho[(*n)++] = idNoinicio;
        for (No *k = this->getNoRaiz(); k != NULL; k = k->getProxNo())
        {
            for (No *linha = this->getNoRaiz(); linha != NULL; linha = linha->getProxNo())
            {
                for (No *coluna = this->getNoRaiz(); coluna != NULL; coluna = coluna->getProxNo())
                {
                    if (matriz[linha->getIdNo()][coluna->getIdNo()] > matriz[linha->getIdNo()][k->getIdNo()] + matriz[k->getIdNo()][coluna->getIdNo()]){
                        matriz[linha->getIdNo()][coluna->getIdNo()] = matriz[linha->getIdNo()][k->getIdNo()] + matriz[k->getIdNo()][coluna->getIdNo()];   
                        if(idNoinicio == linha->getIdNo() && idNofim == coluna->getIdNo()){
                            caminho[(*n)++] = k->getIdNo();
                        }
                    }
                    // matriz[linha->getIdNo()][coluna->getIdNo()] = min(matriz[linha->getIdNo()][coluna->getIdNo()], matriz[linha->getIdNo()][k->getIdNo()] + matriz[k->getIdNo()][coluna->getIdNo()]);
                }
            }
        }
        cout << "O menor caminho: " << matriz[idNoinicio][idNofim] << endl;
        caminho[(*n)++] = idNofim;
        return caminho;
    };

    auto imprimeCaminho = [this](int *caminhoMinimo, int n, int idNoinicio, int idNofim)
    {
        cout << "Caminho Minimo de "<< idNoinicio << " para " << idNofim << " eh ";

        for(int i=0; i < n; i++){
            cout << " [" << caminhoMinimo[i] << "]"; 
        }
        cout << endl;
    };

    No *inicio = procurarNoPeloId(idNoinicio);
    No *fim = procurarNoPeloId(idNofim);

    if (!inicio || !fim)
    {
        cout << "o no " << idNoinicio << " e/ou " << idNofim << " nao estao no grafo.";
        return {};
    }

    unordered_map<int, unordered_map<int, int>> matriz;  

    matriz = preencheHashMatrizInfinito(matriz);
    matriz = preencheHash(matriz);
    int n = 0;
    int* caminhoMinimo = procuraMenorCaminho(matriz, idNoinicio, idNofim, &n);
    // imprimeMatriz(matriz);
    imprimeCaminho(caminhoMinimo, n, idNoinicio, idNofim);
    return caminhoMinimo;
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

}


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

    
}

No* Grafo::getGalpao()
{
    for(No* i = this->getNoRaiz(); i; i=i->getProxNo())
        if(i->getDemanda() == 0)
            return i;
    return NULL;
}

