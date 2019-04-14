#ifndef	_UTIL_
#define _UTIL_

#include "tadGrafo.h"
#define INFINITO 99999
#define LIMPA system("clear");
#define PAUSA setbuf(stdin, NULL); getchar();

typedef int boolean;

/* */
/* @param    :*/
/* @return   :*/
boolean leitura(Controle *controle, char url[]);

/* */
/* @param    :*/
/* @return   :*/
int **algoritmo_de_prim(int qtdeVertices, int **matriz_bi, int inicio);

/* */
/* @param    :*/
/* @return   :*/
void cria_Prim(Grafo *gr, int **matriz, int de);

/* */
/* @param    :*/
/* @return   :*/
int conta_Aeroportos(FILE *arq);

/* */
/* @param    :*/
/* @return   :*/
void cria_Dot_Voos(Grafo *gr);

/* */
/* @param    :*/
/* @return   :*/
void cria_Dot_Rotas(Grafo *gr);

/* */
/* @param    :*/
/* @return   :*/
int **conversor_matriz_tri_bi(Grafo *gr);

/* */
/* @param    :*/
/* @return   :*/
int *caminho_de_para(Grafo *gr, Aeroporto *origem, Aeroporto *destino, int **matriz_bi, int *duracao);

/* */
/* @param    :*/
/* @return   :*/
void voo_sem_escala(Grafo *gr, Aeroporto *origem, int id);

/* */
/* @param    :*/
/* @return   :*/
int *aeroportos_Fora_De_Servico(Grafo *gr, Aeroporto *origem, Aeroporto *destino, int *tam_vetor_trajeto);

#endif