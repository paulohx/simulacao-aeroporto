#ifndef	_TADGRAFO_
#define _TADGRAFO_

/* Criando o tipo boolean. */
typedef int boolean;

/*Estrutura de conexao, usada quando e o grafo de voos*/
typedef struct {

	/* variavel para armazenar a duracao de um determinado voo. */
    int  duracao;

    /* variavel para armazenar o identificador de um determinado voo. */
    char identificador_voo[10];

    /* variavel para armazenar a duracao em um formato diferente. */
    char duracao_horario[6];

}Conexao;

/*Estrutura do aeroporto que sera usada na matriz*/
typedef struct {

	/* variavel para armazenar o ID de uma determinada cidade/aeroporto. */
    int  id;

    /* variavel para armazenar a abreviacao da cidade. */
	char abreviacao_cidade[4];
	
	/* variavel par armazenar o nome inteiro da cidade. */
	char nome_cidade[70];

	/* variavel para armazenar o nome do aeroporto*/
	char nome_aeroporto[50];

	/* variavel para armazenar a quantidade de voos que ele tem para um determinado lugar*/
	int qtdeVooDePara;

	/* vetor de conexoes, que nada mais eh que, a construcao de uma terceira dimensao, fazendo com que a matriz vire um cubo de memoria*/
	Conexao *vetor_Conexao;
	
}Aeroporto;

/*Estrutura do grafo*/
typedef struct {

	/* variavel responsavel por fazer o controle se o grafo/digrafo eh ou nao ponderado (se tem pesos ou nao). */
	int eh_ponderado;

	/* variavel responsavel por fazer o controle se eh um grafo ou um digrafo. */
    int eh_digrafo;

    /* variavel responsavel por armazenar a quantidade de vertices que este grafo possui. */
	int nro_vertices;

	/* matriz de inteiros, responsavel por armazenar as ligacoes das rotas existentes. */
	int       **matriz_Rotas;

	/* matriz de aeroportos, responsavel por armazenar os dados dos aeroportos. */
	Aeroporto **arestas;

}Grafo;

/*Estrutura de controle para os grafos, eles compartilham os mesmos vertices*/
typedef struct {

	/* grafo de rotas, responsavel por conter os dados das rotas*/
    Grafo *rotas;

	/* grafo de voos, responsavel por conter os dados dos voos*/
    Grafo *voos;

    /* matriz bi dimencional, usada em um algoritmo, e responsavel por conter o menor caminho de cada voo. */
    int **matriz_bi;

    /* vetor de rotas, responsavel por armazenar as rotas de voos, para ter um controle mais facil. */
    Aeroporto *vetor_rotas;

}Controle;

/* Funcao responsavel por alocar a variavel de controle e colocar seus atributos nulos*/
/* @param    : Nao recebe nada de parametro*/
/* @return   : Retorna a variavel de controle alocada na memoria*/
Controle *cria_controle();

/* Funcao responsavel por criar um grafo ou digrafo, e alocar suas matrizes */
/* @param    : Ela recebe por parametro, a quantidade de vertices que vai ser usado para
			   alocar as matrizes deste grafo/digrafo, e tambem recebe duas variaveis 
			   boolean de controle, para ver se eh um grafo ou um digrafo e 
			   se o mesmo vai ser ponderado ou nao (se vai ter peso ou nao). */
/* @return   : Grafo* - Retorna um grafo que foi criado, com suas matrizes ja alocadas, 
			   sendo a matriz de aeroportos, que aqui mesmo ja eh iniciada suas variaveis,
			   e a tambem a matriz de inteiros que eh preenchida por 0. */
Grafo *cria_Grafo(int nro_vertices, boolean eh_ponderado, boolean eh_digrafo);

/* Funcao que eh responsavel por liberar da memoria a variavel de controle do programa. */
/* @param    : Recebe a variavel controle, que vai ser liberada da memoria. */
/* @return   : Controle* - Retorna NULL. */
Controle *libera_Controle(Controle *controle);

/* Funcao que eh responsavel por liberar o grafo da memoria. */
/* @param    : Recebe um grafo de parametro, para ser liberado da memoria. */
/* @return   : Grafo* - Retorna NULL. */
Grafo *libera_Grafo(Grafo *gr);

/* Funcao que eh responsavel por liberar a matriz de aeroportos da memoria. */
/* @param    : Recebe por parametro a quantidade de vertices existentes e tambem a matriz de aeroportos. */
/* @return   : Aeroporto** - Retorna NULL. */
Aeroporto **libera_Matriz_Aeroporto(int nro_vertices, Aeroporto **matriz);


/* Funcao que eh responsavel por liberar a matriz de inteiros da memoria. */
/* @param    : Recebe por parametro a quantidade de vertices existentes e tambem a matriz de inteiros. */
/* @return   : int** - Retornando NULL. */
int **libera_Matriz_Inteiro(int nro_vertices, int **matriz);


/* Funcao que eh responsavel por inserir rotas no grafo. */
/* @param    : Recebe de parametro o grafo, recebe tanbem o indice de origem
			   e recebe o indice de destino. */
/* @return   : boolean - Returna false, se o grafo estiver nulo ou ele estiver com algum problema,
						 ou retorna true caso tenha inserido as rotas com sucesso. */
boolean insere_Rotas(Grafo *gr, int origem, int destino);

#endif
