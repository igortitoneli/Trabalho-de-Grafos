#include <fstream>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "../Headers/Solucao.h"
#include "../Headers/No.h"
#include "../Headers/Aresta.h"
#include "../Headers/Grafo.h"

using namespace std;

Solucao *leitura(string arq) {

    //Criando objeto Solucao
    Solucao*graph = new Solucao(arq);
    return graph;
}

int menu() {

    int selecao;
    cout << endl;
    cout << "   ------  MENU ------" << endl;
    cout << "[1] Guloso" << endl;
    cout << "[2] Guloso Randomizado Adaptativo" << endl;
    cout << "[3] Guloso Randomizado Adaptativo Reativo" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}

void selecionar(int selecao, Solucao *graph, ofstream &output_file, float alpha = 0 , int maxIter = 0, vector<float> vectorAlphas = {0}) {

    switch (selecao) {
            // Guloso ;
        case 1: {
            graph->guloso(output_file);
            break;
        }
            // Guloso Randomizado Adaptativo;
        case 2: {
            graph->gulosoRandomizadoAdaptativo(output_file, alpha, maxIter);
            break;
        }
            // Guloso Randomizado Adaptativo Reativo
        case 3: {
            graph->gulosoRandomizadoAdaptativoReativo(output_file, vectorAlphas, maxIter, int(alpha));
            break;
        }
        default: {
            cout << "Exit!!!" << endl;
        }
    }
}

int mainMenu(ofstream &output_file, Solucao *graph, int selecao, float alpha = 0, int maxIter = 0, vector<float> vetorAlpha = {0}) {
    // system("clear");
    
    if (output_file.is_open()){

        selecionar(selecao, graph, output_file, alpha, maxIter, vetorAlpha);
    }
    else
        cout << "Unable to open the output_file" << endl;

    output_file << endl;


    return 0;
}

using namespace std;

int main(int argc, char const *argv[])
{
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    srand(static_cast<unsigned int>(seed));

    if (argc < 3) {
        cout
                << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <Func> <param1> <maxIter>"
                << endl;
        return 1;
    }
    if (argc == 4 && stoi(argv[3]) != 1) {
        cout
                << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <Func>"
                << endl;
        return 1;
    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    if (input_file_name.find("v") <= input_file_name.size()) {
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
    }
    
    //Abrindo arquivo de entrada
    ofstream output_file;
    output_file.open(argv[2], ios::out | ios::trunc);

    Solucao *graph;
    
    graph = leitura(input_file_name);
    
    if (!output_file.is_open())
    {
        cout << "Erro ao abrir arquivo de saída" << endl;
        return 1;
    }
    // graph->imprime(output_file);
    // graph->printGraphDot(output_file);
    

    // Chama o guloso
    if(argc == 4){
        mainMenu(output_file, graph, atoi(argv[3]));
    }
    // Chama o guloso randomizado
    else if(argc == 6){
        mainMenu(output_file, graph, atoi(argv[3]), atof(argv[4]), atoi(argv[5]));
    }
    else{
        vector<float> vectorAlphas;
        for(int i=6; i<argc; i++){
            vectorAlphas.push_back(atof(argv[i]));
        }
        mainMenu(output_file, graph, atoi(argv[3]), atof(argv[4]), atoi(argv[5]), vectorAlphas);
    }

    //Fechando arquivo de saídai
    output_file.close();

    return 0;
}
