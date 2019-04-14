#ifndef	_UTIL_
#define _UTIL_

#include "tadGrafo.h"
#define INFINITO 99999
#define LIMPA system("clear");
#define PAUSA setbuf(stdin, NULL); getchar();

typedef int boolean;

/* Funcao responsavel por ler todos os dados necessarios do arquivo, e armazenar esses dados, nas estruturas certas.*/
/* @param    : Recebe de parametro a variavel de controle, e o url do arquivo a ser lido. */
/* @return   : Retorna false se houve um erro na leitura do arquivo e retorna true caso a leitura tenha tenha sido um sucesso. */
boolean leitura(Controle *controle, char url[]);

/* Funcao responsavel por mostrar a arvore geradora minima de acordo com o vertice desejado.*/
/* @param    : Recebe de parametro a quantidade de vertices que tem no grafo, a matriz bidimensional com o menor custo e o id do vertice raiz desejado.*/
/* @return   : Retorna uma matriz resposta com a arvore resultante.*/
int **algoritmo_de_prim(int qtdeVertices, int **matriz_bi, int inicio);

/* Funcao responsavel por criar arquivos .dot do grafo de prim. */
/* @param    : Recebe o grafo de parametro. */
/* @return   : void*/
void cria_Prim(Grafo *gr, int **matriz, int de);

/* Funcao responsavel por contar a quantidade de vertices que tem no arquivo txt.*/
/* @param    : Ponteiro para o arquivo.*/
/* @return   : int - A quantidade de aeroportos existentes.*/
int conta_Aeroportos(FILE *arq);

/* Funcao responsavel por criar arquivos .dot do grafo de voos. */
/* @param    : Recebe o grafo de parametro. */
/* @return   : void*/
void cria_Dot_Voos(Grafo *gr);

/* Funcao responsavel por criar arquivos .dot do grafo de rotas. */
/* @param    : Recebe o grafo de parametro. */
/* @return   : void*/
void cria_Dot_Rotas(Grafo *gr);

/* Transforma a matriz tridimensional em uma matriz bidimensional, sera pego a menor duracao*/
/* @param    : Recebe um grafo.*/
/* @return   : int** - Retorna a matriz bidimensional com o menor custo.*/
int **conversor_matriz_tri_bi(Grafo *gr);

/* Funcao responsavel por achar o menor caminho de um aeroporto origem para um destino.*/
/* @param    : Recebe um grafo, um aeroporto origem, um aeroporto destino, a matriz com menor custo e uma variavel de duracao que sera modificada aqui dentro para ser usada fora.*/
/* @return   : int* - Retorna o vetor com os ids.*/
int *caminho_de_para(Grafo *gr, Aeroporto *origem, Aeroporto *destino, int **matriz_bi, int *duracao);

/* Funcao que eh responsavel por criar um .dot para mostrar no graphviz os voos diretos de um determinado aeroporto. */
/* @param    : Recebe de parametro o grafo, o aeroporto de origem, e o ID do aeroporto de origem. */
/* @return   : void. */
void voo_sem_escala(Grafo *gr, Aeroporto *origem, int id);

/* Funcao responsavel por mostrar quais aeroportos que se ficarem fora de servico conseguiriam impedir voce de chegar da origem para o destino.*/
/* @param    : Recebe um grafo, um aeroporto origem, um aeroporto destino e uma variavel de tamanho que sera modificada aqui dentro para ser usada fora.*/
/* @return   : int* - Retorna um vetor com os indices desse aeroportos.*/
int *aeroportos_Fora_De_Servico(Grafo *gr, Aeroporto *origem, Aeroporto *destino, int *tam_vetor_trajeto);

#endif