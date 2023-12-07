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
#include <limits.h>
#include <cmath>
#include <iomanip>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

using namespace std;



Solucao::Solucao(string txt)
{
    lerArquivo(txt);
    construirArestas();
    construirMatriz();
    fim = this->grafo->getOrdem();
    inicio = this->galpao->getIdNo();
}   


Solucao::~Solucao()
{
    delete grafo;
    matrizDistancias.clear();
    rotas.clear();
    best.clear();
    candidatos.clear();
    stop.clear();
    percorridos.clear();
    total_rotas.clear();
    delete galpao;
}


void Solucao::lerArquivo(string txt)
{
    ifstream arquivo("./parte_2/Instances/" + txt + ".vrp.txt");

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
    for (const auto& i : grafo->getHashNo()) {
        for (const auto& j : grafo->getHashNo()) {
            if(i.first!=j.first){
                double difX = pow((i.second->getX() - j.second->getX()), 2);
                double difY = pow((i.second->getY() - j.second->getY()), 2);

                double pesoAresta = sqrt(difX + difY);
                // cout << pesoAresta << endl;
                i.second->insereArestaNo(j.second,pesoAresta);
            }    
        }        
    }
}


void Solucao::construirMatriz()
{
    for (const auto& i : grafo->getHashNo()) {
        for(Aresta *a = i.second->getPrimeiraAresta(); a; a=a->getProxAresta()){
            this->matrizDistancias[i.second->getIdNo()][a->getNoDestino()->getIdNo()] = a->getPeso(); 
        }     
    }
}


void Solucao::imprimeMatriz()
{
    
    cout << "   ";
    for (const auto& no : grafo->getHashNo()) {
        cout << no.first << "\t";
    }
    cout << endl
            << "   ";
    for (const auto& no : grafo->getHashNo()) {
        cout << "-" << "\t";    
    }

    cout << endl;
    for (const auto& coluna : grafo->getHashNo()) {
        cout << coluna.first << "|\t";
        for (const auto& linha : grafo->getHashNo()) {
            cout << fixed << setprecision(2);
            cout << this->matrizDistancias[linha.first][coluna.first] << "\t";
        }
        cout << endl;
    }
}


bool Solucao::inPercorridos(No* procurado, vector<int> percorridos)
{   
    int idProcurado = procurado->getIdNo(); 
    for(int no : percorridos){
        if(no == idProcurado)
            return true;
    }
    return false;
}


bool Solucao::noValido(No* destino , vector<int> percorridos, int i, unordered_map<int, rota> rotas) {  
    return (destino != NULL && 
            !inPercorridos(destino,percorridos) && 
            rotas[i].cargaAtual + destino->getDemanda() <= this->capacidade
        );
}


bool Solucao::noValidoReativo(No* destino , vector<int> percorridos, int i, unordered_map<int, rota> rotas) {  
    return (destino != NULL && 
            !inPercorridos(destino,percorridos) && 
            rotas[i].cargaAtual + destino->getDemanda() <= this->capacidade
            && verificaRaio(destino,percorridos)
        );
}


bool Solucao::verificaRaio(No* destino, vector<int> percorridos){
    for(int no : percorridos)
        if (matrizDistancias[destino->getIdNo()][no] < raio) 
            return false;
    return true;
}   


void Solucao::makeCandidatos(){    
    // para todas as rotas
    for(int i=0; i<this->caminhoes; i++)
    {
        // para todos os nós do grafo
        for (const auto& no : grafo->getHashNo()) {
            if(!this->inPercorridos(no.second,percorridos)){
                // (peso / distancia) entre o ultimo nó da rota e o nó do for
                if(this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][no.first] != 0){
                    
                    double heuristica = no.second->getDemanda() / this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][no.first];
                    
                    if(heuristica > candidatos[i].heuristica)
                    {
                        candidatos[i].no = no.second;
                        candidatos[i].heuristica = heuristica;
                    }
                }
            }
        }
    }
}


void Solucao::makeCandidatosRandomizado(float alpha){    
    // para todas as rotas

    for(int i=0; i<this->caminhoes; i++)
    {
        unordered_map<No*, double> listCandidatos;
        // para todos os nós do grafo
        for (const auto& no : grafo->getHashNo()) {
            if(!this->inPercorridos(no.second,percorridos)){
                // (peso / distancia) entre o ultimo nó da rota e o nó do for
                if(this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][no.first] != 0){
                    
                    double heuristica = no.second->getDemanda() / this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][no.first];
                    
                    listCandidatos[no.second] = heuristica;
                }
            }
        }
        pair<No*, double> NoEscolhido = ordena(listCandidatos, alpha); 
        candidatos[i].no = NoEscolhido.first;
        candidatos[i].heuristica = NoEscolhido.second; 
    }
}


pair<No*, double> Solucao::ordena(unordered_map<No*, double> listCandidatos, float alpha){
    
    vector<pair<No*, double>> list(listCandidatos.begin(), listCandidatos.end());

    // Ordenar o vetor com base nas chaves
    sort(list.begin(), list.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    }); 
    int antigoTamanho = list.size();
    int novoTamanho = (list.size() * alpha);

    if(novoTamanho == 0) novoTamanho ++;
    list.resize(novoTamanho);

    int numero_aleatorio = gerarNumeroAleatorio(0,novoTamanho);

    No* no = list.at(numero_aleatorio).first;

    double heuristica = list.at(numero_aleatorio).second; 
    pair<No*,double> NoEscolhido (no, heuristica);

    return NoEscolhido; 
}


void Solucao::atualizaCandidatos(No* alterado){
    // para todas as rotas
    for(int i=0; i<this->caminhoes; i++)
    {
        candidatos[i].heuristica = -1;
        candidatos[i].no = grafo->getGalpao();
        // para todos os nós do grafo
        double heuristica;
        for (const auto& no : grafo->getHashNo()) {
            // demanda / distancia entre o ultimo nó da rota e o nó do for
            heuristica = no.second->getDemanda() / this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][no.first];
            
            // cout << "no \t" << no.first << "\t percorrido? \t" << (!inPercorridos(no.second,percorridos) ? "true" :  "false") << endl;
            // cout << endl;
            // cout << "no \t" << no.first << "\t heu(" << heuristica << ") > ? cand("<< candidatos[i].heuristica << ") \t" << ((heuristica > candidatos[i].heuristica) ? "true" : "false") << endl;
            // cout << endl;
            // cout << "no \t" << no.first << "\t carga(" << rotas[i].cargaAtual << ") + demanda(" << no.second->getDemanda() << ") <= " << this->capacidade << "\t" << ((rotas[i].cargaAtual + no.second->getDemanda() <= this->capacidade) ? "true" : "false") << endl;
            // cout << endl;
            if(!inPercorridos(no.second,percorridos) &&
                heuristica > candidatos[i].heuristica &&
                rotas[i].cargaAtual + no.second->getDemanda() <= this->capacidade)
            {
                candidatos[i].no = no.second;
                candidatos[i].heuristica = heuristica;
            }
        }
    }
}


void Solucao::atualizaCandidatosRandomizado(float alpha){
    // para todas as rotas
    for(int i=0; i<this->caminhoes; i++)
    {
        unordered_map<No*, double> listCandidatos;
            
        candidatos[i].heuristica = 0;
        candidatos[i].no = grafo->getGalpao();

        // para todos os nós do grafo
        for (const auto& no : grafo->getHashNo()) {

            if(!inPercorridos(no.second,percorridos) &&
                rotas[i].cargaAtual + no.second->getDemanda() <= this->capacidade)
            {
                double heuristica = no.second->getDemanda() / this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][no.first] ;
                listCandidatos[no.second] = heuristica;
            }
        }
    
        if(!listCandidatos.empty()){
            pair<No*, double> NoEscolhido = ordena(listCandidatos, alpha); 
            candidatos[i].no = NoEscolhido.first;
            candidatos[i].heuristica = NoEscolhido.second; 
        }
    }
}


int Solucao::getCandidatos() {
    double max = -1;
    int cont = -1;
    for(int i=0; i < this->caminhoes; i++){
        // cout << max << " < " << "candidatos[" << i << "].heuristica - " << candidatos[i].heuristica << endl; 
        if(max < candidatos[i].heuristica){
            max = candidatos[i].heuristica;
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
            throw ExcecaoSemCandidatos(this);
            return true;
    }
    return false;
};


void Solucao::atualizaRota(int i, No* destino) {
    rotas[i].distancia += this->matrizDistancias[rotas[i].ultimoNo->getIdNo()][destino->getIdNo()];
    rotas[i].ultimoNo = destino;
    rotas[i].cargaAtual += destino->getDemanda();
    rotas[i].percurso.push_back(destino->getIdNo());
};


double Solucao::calcMedia(){
    double soma = 0;
    for(double rota : total_rotas){
        soma += rota;
    }
    if (!total_rotas.empty()) {
        double media = soma / total_rotas.size();
        cout << "Media: " << media << endl;
        return media;
    } else {
        cout << "Não foi possivel calcular media." << endl;
        return 0;
    }
    return 0;
}


double Solucao::desvioPadrao(double media){
    double soma = 0; 

    for(double valor : total_rotas){
        soma += pow((valor - media),2);
    }
    soma = soma / total_rotas.size();
    double desvioPadrao = sqrt(soma);
    cout << "Desvio Padrao: " << desvioPadrao << endl;
    return desvioPadrao;
}


void Solucao::mape(){
    
    double soma = 0;
    for(double valor : total_rotas){
        soma += (valor - optimal_value)/valor;
    }
    cout << "Mape: " << soma/total_rotas.size();

}


void Solucao::Estatisticas(){
    if(total_rotas.empty()){
        cout << "Nao foi possivel calcular Estatisticas." << endl;
        exit(1);
    }

    double media = this->calcMedia();
    desvioPadrao(media);
    mape();
}


void Solucao::imprimeRotas(bool completa = true){
    
    double total = 0;
    
    for(int i=0; i<this->caminhoes; i++){
        cout << "Rota " << i << " : < ";  
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
        cout << "% acima: " << ((total/this->optimal_value) - 1) * 100 << "%" << endl; 
        // Estatisticas();
    }
    else{
        cout << "Nao possui optimal_value" <<  endl;
    }
};


void Solucao::imprimeBest(){
    
    double total = 0;
    
    for(int i=0; i<this->caminhoes; i++){
        cout << "Rota " << i << " : < ";  
        for(int j=0; j<best.at(i).percurso.size(); j++){
            cout << best.at(i).percurso[j] << " ";
        }
        total += best.at(i).distancia;
        cout << ">  - distancia: " << best.at(i).distancia;
        cout << " - carga: " << best.at(i).cargaAtual << endl;
    }
    cout << "total - " << total << endl;
    if (this->optimal_value > 0){
        cout << "optimal_value: " << this->optimal_value << endl;
        cout << "% acima: " << ((total/this->optimal_value) - 1) * 100 << "%" << endl; 
        // Estatisticas();
    }
    else{
        cout << "Nao possui optimal_value" <<  endl;
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


int Solucao::gerarNumeroAleatorio(int inicio, int fim) {
    int numeroAleatorio = inicio + rand() % (fim - inicio);
    return numeroAleatorio;
}


void Solucao::escreveRotas(ofstream &output_file, bool completa, int iter){

    if(output_file.is_open()){
        double total = 0;

        output_file << "Iteracao  =  " << iter + 1 << endl << endl;
        
        for(int i=0; i<this->caminhoes; i++){
            output_file << "percurso rota " << i << " : < ";  
            for(int j=0; j<rotas.at(i).percurso.size(); j++){
                output_file << rotas.at(i).percurso[j] << " ";
            }
            total += rotas.at(i).distancia;
            output_file << ">  - distancia: " << rotas.at(i).distancia;
            output_file << " - carga: " << rotas.at(i).cargaAtual << endl;
        }
        output_file << "total - " << total << endl;
        if (this->optimal_value > 0){
            output_file << "optimal_value: " << this->optimal_value << endl;
            output_file << "% acima: " << ((total/this->optimal_value) - 1) * 100 << "%" << endl; 
        }
        else{
            output_file << "Nao possui optimal_value" <<  endl;
        }   
        output_file << endl << endl;
        output_file << "==========================================================" << endl << endl;
        
    }
    else {
        cout << "Erro ao abrir o arquivo para escrita." << endl;
    }
    

}


bool Solucao::VerificaVeracidade(){

    bool completa = true;
    bool list_percorridos[grafo->getOrdem()] = {false};

    for(int i=0; i<caminhoes; i++){        
        for(int no : rotas[i].percurso){
            if(list_percorridos[no-1] == true && no != 1){
                cout << "o no " << no << " se repete" << endl;
                completa = false;
            }
            list_percorridos[no-1] = true; 
        }
        if(rotas[i].cargaAtual > capacidade){
            cout << "rotas[" << i << "].cargaAtual " << rotas[i].cargaAtual << endl; 
            completa = false;
        } 
    }
    for(bool no : list_percorridos){
        if(no == false && no != 0) {
            cout << "falta o no " << no+1 << endl;
            completa = false;
        }
    }
    return completa;
}


unordered_map<int,rota> Solucao::verificaBest(){
    double total_rota = 0;
    for(int i=0; i<caminhoes; i++){
        total_rota += rotas[i].distancia;
    }
    soma_iter += total_rota;
    n_iter++;
    if(best_distancia > total_rota){
        best_distancia = total_rota;
        return rotas;
    }
    else{
        return best;
    }
}


void Solucao::initRotas(){
    for(int i=0; i<caminhoes; i++){
        rotas[i].cargaAtual = 0;
        rotas[i].ultimoNo = grafo->getGalpao();
        rotas[i].percurso = {1};
        rotas[i].distancia = 0;
    }
}


void Solucao::initBest(){
    
    for(int i=0; i<caminhoes; i++){
        best[i].cargaAtual = 0;
        best[i].ultimoNo = grafo->getGalpao();
        best[i].percurso = {1};
        best[i].distancia = DBL_MAX;
    }
}


unordered_map<int,rota> Solucao::guloso(ofstream &output_file)
{
    int i = 0;
    initRotas();
    while(i < this->caminhoes){
        No* destino = this->grafo->procurarNoPeloId(gerarNumeroAleatorio(inicio,fim));
        if(noValido(destino, percorridos, i, rotas)){
            percorridos.push_back(destino->getIdNo());
            atualizaRota(i, destino);
            i++;
        }
    }
    makeStop();
    
    // peso / distancia
    bool completa = true;
    while(percorridos.size() != this->grafo->getOrdem())
    {
        try{
            // imprimeRotas(true);
            atualizaCandidatos(candidatos[i].no);
            i = getCandidatos();
            atualizaRota(i, candidatos[i].no);
            percorridos.push_back(candidatos[i].no->getIdNo());
            stop = candidatos;
        }
        catch(const ExcecaoSemCandidatos& e){
            e.what();
            break;
        }
    }
    backToGalpao();
    
    completa = VerificaVeracidade();

    imprimeRotas(completa); 
    escreveRotas(output_file,completa,0);
    return this->rotas;
}


void Solucao::initVariables(){
    initRotas();
    percorridos = {1};
}


void Solucao::calculaBest(){
    
    double total_rota = 0;
    for(int i=0; i<caminhoes; i++){
        total_rota += rotas[i].distancia; 
    }

    if(total_rota < best_distancia){
        best = rotas;
        best_distancia = total_rota;
    }
}


pair<double,double> Solucao::gulosoRandomizadoAdaptativo(ofstream &output_file, float alpha, int maxIter)
{
    int iter = 0;
    bool completa = true;
    initBest();
    while(iter < maxIter){

        // inicializa randomicamente
        initVariables();

        int i = 0;
        while(i < this->caminhoes){
            No* destino = this->grafo->procurarNoPeloId(gerarNumeroAleatorio(inicio,fim));
            if(noValido(destino, percorridos, i, rotas)){
                percorridos.push_back(destino->getIdNo());
                atualizaRota(i, destino);
                i++;
            }
        }
        makeStop();
        
        // peso / distancia
        bool completa = true;
        while(percorridos.size() != this->grafo->getOrdem())
        {
            try{
                atualizaCandidatosRandomizado(alpha);
                i = getCandidatos();
                atualizaRota(i, candidatos[i].no);
                percorridos.push_back(candidatos[i].no->getIdNo());
                stop = candidatos;
            }
            catch(const ExcecaoSemCandidatos& e){
                break;
            }
        }
        backToGalpao();
        
        if(VerificaVeracidade()){
            best = verificaBest();
        }
        rotas.clear();
        candidatos.clear();
        stop.clear();
        percorridos.clear();
        iter++;
    }
    if(best[0].cargaAtual != 0){
        imprimeBest();
        pair<double,double> retorno;
        retorno.first = soma_iter/n_iter;
        retorno.second = best_distancia;
        // retorna media das iter e best
        return retorno;
    }

    pair<double,double> retorno;
    retorno.first = -1;
    retorno.second = -1;
    
    return retorno;
}


double Solucao::calculaFormulaRaio(){

    for(pair<int, No*> no : grafo->getHashNo()){
        int x = no.second->getX();
        int y = no.second->getY();
        int demanda = no.second->getDemanda();

        if(maiorX < x) maiorX = x;
        if(menorX > x) menorX = x;
        if(maiorY < y) maiorY = y;
        if(menorY > y) menorY = y;
        if(maiorDemanda < demanda) maiorDemanda = demanda;
        if(menorDemanda > demanda && demanda > 0) menorDemanda = demanda;
    }
    
    area = (maiorX - menorX) * (maiorY - menorY);
    double pi = M_PI;
    raio = sqrt((area/caminhoes)/pi);
    return raio;
}


double Solucao::getRaio(No* no){
    return (menorDemanda * raio)/(no->getDemanda());
}


void Solucao::atualizaProb(vector<float> alpha){
    vector<double> q(alpha.size());
    for(int i=0; i<alpha.size(); i++){
        for(double m : media[i]){
        }
        if(!media[i].empty()){
            double media_i = accumulate(media[i].begin(), media[i].end(), 0.0)/ media[i].size();
            q[i] = pow((best_distancia / media_i), 100);    
        }
        else{
        }
    }
    
    for(int i=0; i<alpha.size(); i++){
        if(q[i]){
            prob[i] = q[i]/accumulate(q.begin(), q.end(), 0.0);    
        }
    }
}


void Solucao::initVetores(int tam){
    for(int i=0; i<tam; i++){
        prob.push_back(double(100/tam)); 
        media.push_back({});
    }
}


int Solucao::getAlpha(){
    double amplificador = 100.0;
    int aleatorio = gerarNumeroAleatorio(0,prob.size()*amplificador);
    int i;
    double cont = 0;
    for(i=0; i<prob.size(); i++){
        cont += prob[i]*amplificador;
        if(aleatorio<=cont) break;
    }
    return i;
}


void Solucao::atualizaMedia(double newSolucao, int indexAlpha){
    media.at(indexAlpha).push_back(newSolucao);
}


unordered_map<int,rota> Solucao::gulosoRandomizadoAdaptativoReativo(ofstream &output_file, vector<float> alpha, int maxIter, int bloco)
{
    initBest();
    initVetores(alpha.size());

    unordered_map<int, rota> best_atual_rota;
    for(int i=0; i<caminhoes; i++){
        best_atual_rota[i].cargaAtual = 0;
        best_atual_rota[i].ultimoNo = grafo->getGalpao();
        best_atual_rota[i].percurso = {1};
        best_atual_rota[i].distancia = DBL_MAX;
    }
    double best_atual_dist = DBL_MAX;

    int i=1;
    while(i <= maxIter){
        if(i%bloco == 0){
            atualizaProb(alpha);
        }

        int indexAlpha = getAlpha();
        pair<double,double> newSolucao = gulosoRandomizadoAdaptativo(output_file, alpha[indexAlpha], 30);
        if(newSolucao.first != -1){
            atualizaMedia(newSolucao.first, indexAlpha);
            if(best_atual_dist > newSolucao.second){
                best_atual_dist = newSolucao.second;
                best_atual_rota = best;
            }
        }
        else{
            initBest();
        };
        i++;
    }
    best = best_atual_rota;
    imprimeBest();
    return best;
}
