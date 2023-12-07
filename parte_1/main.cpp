#include <fstream>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "Grafo.h"
#include "No.h"
#include "Aresta.h"


Grafo *leitura(ifstream &input_file, int directed, int weightedEdge, int weightedNode) {

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Grafo*graph = new Grafo(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo

    if (!graph->getWeightedEdge() && !graph->getWeightedNode()) {

        while (input_file >> idNodeSource >> idNodeTarget) {

            graph->insertAresta(idNodeSource, idNodeTarget, 0);

        }

    } else if (graph->getWeightedEdge() && !graph->getWeightedNode()) {

        int edgeWeight;

        while (input_file >> idNodeSource >> idNodeTarget >> edgeWeight) {

            graph->insertAresta(idNodeSource, idNodeTarget, edgeWeight);

        }

    } else if (graph->getWeightedNode() && !graph->getWeightedEdge()) {

        int nodeSourceWeight, nodeTargetWeight;

        while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertAresta(idNodeSource, idNodeTarget, 0);
            graph->procurarNoPeloId(idNodeSource)->setPeso(nodeSourceWeight);
            graph->procurarNoPeloId(idNodeTarget)->setPeso(nodeTargetWeight);

        }

    } else if (graph->getWeightedNode() && graph->getWeightedEdge()) {

        int nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertAresta(idNodeSource, idNodeTarget, edgeWeight);
            graph->procurarNoPeloId(idNodeSource)->setPeso(nodeSourceWeight);
            graph->procurarNoPeloId(idNodeTarget)->setPeso(nodeTargetWeight);

        }

    }

    return graph;
}

Grafo *leituraInstancia(ifstream &input_file, int directed, int weightedEdge, int weightedNode) {

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    int numEdges;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Grafo *graph = new Grafo(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    while (input_file >> idNodeSource >> idNodeTarget) {

        graph->insertAresta(idNodeSource, idNodeTarget, 0);

    }

    return graph;
}

int menu() {

    int selecao;
    cout << endl;
    cout << "   ------  MENU ------" << endl;
    cout << "[1] Fecho transitivo direto de um vertice" << endl;
    cout << "[2] Fecho transitivo indireto de um vertice" << endl;
    cout << "[3] Caminho Minimo entre dois vertices - Dijkstra " << endl;
    cout << "[4] Caminho Minimo entre dois vertices - Floyd" << endl;
    cout << "[5] Arvore Geradora Minima - Prim" << endl;
    cout << "[6] Arvore Geradora Minima - Kruskal " << endl;
    cout << "[7] Caminhamento em profundidade " << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}

void selecionar(int selecao, Grafo *graph, ofstream &output_file) {

    switch (selecao) {

        //Fecho transitivo direto de um vértice ;
        case 1: {
            int id;
            cout << "Digite o ID do vertice para seu fecho transitivo direto: ";
            cin >> id;
            graph->fechoTransitivoDireto(output_file, id);
            break;
        }
            //Fecho transitivo indireto de um vértice;
        case 2: {
            int id;
            cout << "Digite o ID do vertice para seu fecho transitivo indireto: ";
            cin >> id;
            graph->fechoTransitivoIndireto(output_file, id);
            break;
        }
            // Djkstra
            // Caminho Mínimo entre dois vértices - Dijkstra ;
        case 3: {
            int id1, id2;
            cout << "Digite o vértice de inicio: ";
            cin >> id1;
            cout << "Digite o vértice final: ";
            cin >> id2;
            float distancia = graph->Dijkstra(id1, id2);
            cout << "A distancia entre " << id1 << " e " << id2 << " é de: " << distancia << endl;
            output_file << "A distancia entre " << id1 << " e " << id2 << " é de: " << distancia << endl;
            break;
        }

        //Floyd 
        case 4: {
            int id_one, id_two;
            cout << "Digite o id do vertice de origem: ";
            cin >> id_one;
            cout << "Digite o id do vertice de destino: ";
            cin >> id_two;
            graph->Floyd(output_file, id_one, id_two);
            break;
        }
            //Árvore Geradora Mínima - Prim;
        case 5: {
            cout << "Árvore Geradora Mínima - Prim" << endl;
            graph->prim(output_file, graph->getNoRaiz()->getIdNo());
            break;
        }
            //Árvore Geradora Mínima - Kruskal;
        case 6: {
            //graph->(output_file, graph->getVerticeInduzido());
            cout << "a ser implementado" << endl;
            break;
        }
            //Caminhamento em profundidade
        case 7: {
            // output_file.close();
            int id;
            cout << "Digite o vértice de inicio: ";
            cin >> id;
            graph->caminhoEmProfundidade(output_file, id);
        }
        default: {
            cout << "Exit!!!" << endl;
        }

    }
}

int mainMenu(ofstream &output_file, Grafo *graph) {

    int selecao = 1;

    while (selecao != 0) {
        // system("clear");
        selecao = menu();

        if (output_file.is_open())
            selecionar(selecao, graph, output_file);

        else
            cout << "Unable to open the output_file" << endl;


        output_file << endl;

    }

    return 0;
}

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc != 6) {

        cout
                << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> "
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
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    Grafo *graph;
    if (input_file.is_open()) {
        auto start = chrono::steady_clock::now();
        graph = leitura(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
        auto end = chrono::steady_clock::now();

        cout << "Demorou  "
                << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << " ms para ler o arquivo de entrada." << endl;

        if (!output_file.is_open())
        {
            cout << "Erro ao abrir arquivo de saída" << endl;
            return 1;
        }
        //graph->imprime(output_file);
        // graph->printGraphDot(output_file);

    } else
        cout << "Unable to open " << argv[1];

    mainMenu(output_file, graph);

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}
