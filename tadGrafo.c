#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tadGrafo.h"

enum boolean{false = 0, true};

/* Funcao responsavel por alocar a variavel de controle e colocar seus atributos nulos*/
/* @param    : Nao recebe nada de parametro*/
/* @return   : Retorna a variavel de controle alocada na memoria*/
Controle *cria_controle(){

	/* Alocando espaco na memoria para a variavel de controle. */
	Controle *controle    = (Controle*) malloc(sizeof(Controle));

	/* Setando null para o grafo de rotas. */
	controle->rotas       = NULL;

	/* Setando null para o grafo de voos. */
	controle->voos        = NULL;

	/* Setando null para a matriz bi. */
	controle->matriz_bi   = NULL;

	/* Setando null para o vetor de rotas. */
	controle->vetor_rotas = NULL;

	/* Retornando a variavel de controle. */
	return(controle);
}

/* Funcao responsavel por criar um grafo ou digrafo, e alocar suas matrizes */
/* @param    : Ela recebe por parametro, a quantidade de vertices que vai ser usado para
			   alocar as matrizes deste grafo/digrafo, e tambem recebe duas variaveis 
			   boolean de controle, para ver se eh um grafo ou um digrafo e 
			   se o mesmo vai ser ponderado ou nao (se vai ter peso ou nao). */
/* @return   : Grafo* - Retorna um grafo que foi criado, com suas matrizes ja alocadas, 
			   sendo a matriz de aeroportos, que aqui mesmo ja eh iniciada suas variaveis,
			   e a tambem a matriz de inteiros que eh preenchida por 0. */
Grafo *cria_Grafo(int nro_vertices, boolean eh_ponderado, boolean eh_digrafo){

	/* Alocando a estrutura Grafo*/
	Grafo *gr = (Grafo*) malloc(sizeof(Grafo));
	
	/* Verificando se não ouve erro na hora de alocar o Grafo*/
	if(gr != NULL){

		/* Guardando o numero de vertices dentro da estrutura*/
		gr->nro_vertices = nro_vertices;

		/* Feito uma verificacao para garantir que o valor sera sempre 0 e 1, ou seja, true or false*/
		gr->eh_ponderado = eh_ponderado;
		gr->eh_digrafo   = eh_digrafo;

		/*Criando as linhas da matriz de tamanho nro_vertices*/
		gr->arestas = (Aeroporto**) malloc(nro_vertices * sizeof(Aeroporto*));
		gr->matriz_Rotas = (int**) malloc(nro_vertices * sizeof(int*));
		for (int i = 0; i < nro_vertices; i++){

			/*Criando as colunas da matriz de tamanho nro_vertices*/
			gr->arestas[i] = (Aeroporto*) malloc(nro_vertices * sizeof(Aeroporto));
			gr->matriz_Rotas[i] = (int*) malloc(nro_vertices * sizeof(int));
		    for (int j = 0; j < nro_vertices; j++){

		    	/*Iniciando variaveis*/
		    	gr->arestas[i][j].id            = -1;
		    	gr->arestas[i][j].qtdeVooDePara = 0;
		    	gr->arestas[i][j].vetor_Conexao = NULL;

		    	gr->matriz_Rotas[i][j] = 0;
		    }
		}
	}

	/* Retorna o grafo criado*/
	return(gr);
}

/* Funcao que eh responsavel por liberar da memoria a variavel de controle do programa. */
/* @param    : Recebe a variavel controle, que vai ser liberada da memoria. */
/* @return   : Controle* - Retorna NULL. */
Controle *libera_Controle(Controle *controle){

	/* Chamando uma funcao para liberar a matriz de inteiros.*/
	controle->matriz_bi = libera_Matriz_Inteiro(controle->rotas->nro_vertices, controle->matriz_bi);
	
	/* Chamando uma funcao para liberar o grafo. */
	controle->rotas     = libera_Grafo(controle->rotas);
	
	/* Chamando uma funcao para liberar o grafo. */	
	controle->voos      = libera_Grafo(controle->voos);	
	
	/* Dando free no vetor de rotas da variavel de controle. */
	free(controle->vetor_rotas);
	
	/* Dando free na variavel de controle. */
	free(controle);

	/* Retornando NULL. */
	return NULL;
}

/* Funcao que eh responsavel por liberar o grafo da memoria. */
/* @param    : Recebe um grafo de parametro, para ser liberado da memoria. */
/* @return   : Grafo* - Retorna NULL. */
Grafo *libera_Grafo(Grafo *gr){
	/* Chamando uma funcao para liberar a matriz de inteiros. */
	gr->matriz_Rotas = libera_Matriz_Inteiro(gr->nro_vertices, gr->matriz_Rotas);

	/* Chamando uma funcao para liberar a matriz de aeroportos. */
	gr->arestas      = libera_Matriz_Aeroporto(gr->nro_vertices, gr->arestas);

	/* Dando free no grafo. */
	free(gr);

	/* Retornando NULL. */
	return NULL;
}

/* Funcao que eh responsavel por liberar a matriz de aeroportos da memoria. */
/* @param    : Recebe por parametro a quantidade de vertices existentes e tambem a matriz de aeroportos. */
/* @return   : Aeroporto** - Retorna NULL. */
Aeroporto **libera_Matriz_Aeroporto(int nro_vertices, Aeroporto **matriz){

	/* Dando free nas posicoes da matriz. */
	for (int i = 0; i < nro_vertices; i++){
		free(matriz[i]);
	}

	/* Dando free na matriz. */
	free(matriz);

	/* Retornando NULL. */
	return NULL;
}

/* Funcao que eh responsavel por liberar a matriz de inteiros da memoria. */
/* @param    : Recebe por parametro a quantidade de vertices existentes e tambem a matriz de inteiros. */
/* @return   : int** - Retornando NULL. */
int **libera_Matriz_Inteiro(int nro_vertices, int **matriz){

	/* Dando free nas posicoes da matriz. */
	for (int i = 0; i < nro_vertices; i++){
		free(matriz[i]);
	}

	/* Dando free na matriz. */
	free(matriz);

	/* Retornando NULL. */
	return NULL;
}

/* Funcao que eh responsavel por inserir rotas no grafo. */
/* @param    : Recebe de parametro o grafo, recebe tanbem o indice de origem
			   e recebe o indice de destino. */
/* @return   : boolean - Returna false, se o grafo estiver nulo ou ele estiver com algum problema,
						 ou retorna true caso tenha inserido as rotas com sucesso. */
boolean insere_Rotas(Grafo *gr, int origem, int destino){

	/* Verificando se o Grafo eh null*/
	if(gr == NULL){
		return false;
	}
	
	/* Setando 1, na matriz de rotas, na posicao [origem][destino]*/
	gr->matriz_Rotas[origem][destino] = 1;

	/* Se ele nao for digrafo*/
	if(!gr->eh_digrafo){
		
		/* Voce passa a ordem inversa, para nao ser digrafo, e coloca 1 para nao ficar nesse ciclo infinitamente*/
		gr->eh_digrafo = true;

		/* Chamando a funcao recursivamente para fazer o caminho de volta, caso ela seja um grafo*/
		insere_Rotas(gr, destino, origem);
	}

	/* Retorna 1 se deu tudo certo na insercao*/
	return true;
}