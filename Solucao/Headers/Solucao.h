#ifndef SOLUCAO_H
#define SOLUCAO_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include "../Headers/Aresta.h"
#include "../Headers/No.h"
#include "../Headers/Grafo.h"


using namespace std;

class Solucao{
    public:
        Solucao(string txt);
        ~Solucao();

        Grafo* guloso();
        Grafo* gulosoRandomizadoAdaptativo();
        Grafo* gulosoRandomizadoAdaptativoReativo();

    private:
        Grafo* lerArquivo(string txt);

        Grafo* grafo;
        int capacidade;
};


#endif // SOLUCAO_H
