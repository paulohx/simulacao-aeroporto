#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

enum boolean{false = 0 ,true};

/* Funcao responsavel por ler todos os dados necessarios do arquivo, e armazenar esses dados, nas estruturas certas.*/
/* @param    : Recebe de parametro a variavel de controle, e o url do arquivo a ser lido. */
/* @return   : Retorna false se houve um erro na leitura do arquivo e retorna true caso a leitura tenha tenha sido um sucesso. */
boolean leitura(Controle *controle, char url[]){

	/* Criando um ponteiro arq de FILE, e abrindo  o arquivo para a leitura. */
	FILE *arq = fopen(url,"r");

	/* Variaveis auxiliares na leitura do arquivo. */
	char lixo[400];
	char abreviacao_cidade[4];
	char nome_cidade[70];
	char nome_aeroporto[50];

	char de[10];
	char para[10];
	char identificador[10];
	int  duracao;
	char horario[10];

	/*A quantidade de vertices no inicio e 0*/
	int qtdeVertices = 0;

	/* Verificando se houve algum erro ao abrir o arquivo. */
	if(arq == NULL){
		printf("Problema ao abrir o arquivo!\n");
		return(false);
	}

	/*Guarda a quantidade de vertices que foi verificado no arquivo*/
	qtdeVertices = conta_Aeroportos(arq);

	/*Criando os grafos de rotas e voos*/
	controle->rotas = cria_Grafo(qtdeVertices, false, false);
	controle->voos  = cria_Grafo(qtdeVertices, true,  true);
	controle->vetor_rotas = (Aeroporto*)malloc(qtdeVertices * sizeof(Aeroporto));

	/*Seta o ponteiro para o comeco do arquivo*/
	rewind(arq);	

	/*Comeca a ler o arquivo novamente*/	
	while(!feof(arq)){

		switch(fgetc(arq)){
			
			/*Achou o caractere '#'*/			
			case '#':

	 			fscanf(arq,"%[^\n]s",lixo);
	 			fgetc(arq);
				
	 		break;

	 		/*Achou o caractere '$'*/
	 		case '$':

	 			/*Loop infinito para ser generico, PARA quando achar o '$'*/
	 			for (int i = 0; i < qtdeVertices; i++){

	 				/*Le o \n apos o '$'*/
					fgetc(arq);

					/*Le a abreviacao do nome da cidade*/
					fscanf(arq,"%s", abreviacao_cidade);

					/*Espaco apos a abreviacao*/
					fgetc(arq);

					/*Le o nome da cidade, nome do estado*/
					fscanf(arq,"%[^(]s", nome_cidade);

					/*Espaco apos o nome do estado*/
					fgetc(arq);

					/*Le o nome do aeroporto que esta entre parenteses*/
					fscanf(arq,"%[^)]s", nome_aeroporto);

					/*Aloca espaco de memoria para uma estrutura de aeroporto*/
					Aeroporto *aeroporto = (Aeroporto *) malloc(sizeof(Aeroporto));

					/* Preenchendo dados do aeroporto lido. */
					aeroporto->id            = i;
					aeroporto->qtdeVooDePara = 0;
					aeroporto->vetor_Conexao = NULL;
					strcpy(aeroporto->abreviacao_cidade, abreviacao_cidade);
					strcpy(aeroporto->nome_cidade,       nome_cidade);
					strcpy(aeroporto->nome_aeroporto,    nome_aeroporto);

					/*O vertice ira apontar para esse aeroporto*/
					controle->rotas->arestas[i][i] = *aeroporto;
					controle->voos->arestas[i][i]  = *aeroporto;
					controle->vetor_rotas[i]	   = *aeroporto;

				}

				/*Consome o ) do ultimo vertice*/
				fgetc(arq);
				/*Consome o \n do ultimo vertice*/
				fgetc(arq);
				fgetc(arq);
				fgetc(arq);

			break;

			/*Achou o caractere '!'*/
	 		case '!':

	 			/*Chega aqui apos encontrar o '!'
				  Comeca a ler as rotas dos aeroportos
				*/

				/*Loop infinito para ser generico, PARA quando achar o '!'*/
				while(1){
					/*Le o \n depois da exclamacao*/
					fgetc(arq);

					/*Le a conexao DE*/
					fscanf(arq, "%s", de);

					/*Achou o local de parada que e o '!', entao o loop sera quebrado*/
					if (strcmp(de, "!") == 0){
						break;
					}

					/*Espaco apos o DE*/
					fgetc(arq);

					/*Le a conexao PARA*/
					fscanf(arq, "%s", para);

					/*Le o \n*/
					fgetc(arq);

					int indice1 = -1;
					int indice2 = -1;

					/*Procurando a conexao DE*/
					for (int i = 0; i < qtdeVertices; i++){
						if (strcmp(controle->vetor_rotas[i].abreviacao_cidade, de) == 0){
							indice1 = i;
							break;	
						}
					}

					/*Procurando a conexao PARA*/
					for (int i = 0; i < qtdeVertices; i++){
						if (strcmp(controle->vetor_rotas[i].abreviacao_cidade, para) == 0){
							indice2 = i;
							break;	
						}
					}


					insere_Rotas(controle->voos, indice1, indice2);					
					insere_Rotas(controle->rotas, indice1, indice2);
					
				}

	 		break;

	 		/*Achou o caractere '%'*/
	 		case '%':

	 			while (1){

		 			/*Le o \n depois da porcentagem*/
					fgetc(arq);

					/*Le o identificador do voo*/
					fscanf(arq, "%s", identificador);

					/*Achou o local de parada que e o '%', entao o loop sera quebrado*/
					if (strcmp(identificador, "%") == 0){
						break;
					}

					/*Espaco apos o identificador*/
					fgetc(arq);

					/*Le a conexao DE*/
					fscanf(arq, "%s", de);

					/*Espaco apos a conexao DE*/
					fgetc(arq);

					/*Le a conexao PARA*/
					fscanf(arq, "%s", para);

					/*Espaco apos a conexao PARA*/
					fgetc(arq);

					/*Le a duracao do voo*/
					fscanf(arq, "%d", &duracao);

					/*Espaco apos a duracao*/
					fgetc(arq);

					/*Le a duracao em forma de horario*/
					fscanf(arq, "%[^,]s", horario);

					int indice1 = -1;
					int indice2 = -1;

					/*Procurando a conexao DE*/
					for (int i = 0; i < qtdeVertices; i++){

						/* Verificando se a abreviacao na posicao i eh a mesma que a variavel de. */
						if (strcmp(controle->vetor_rotas[i].abreviacao_cidade, de) == 0){
							
							/* Se for a mesma, eh guardado o indice, na variavel. */
							indice1 = i;
							break;	
						}
					}

					/*Procurando a conexao PARA*/
					for (int i = 0; i < qtdeVertices; i++){

						/* Verificando se a abreviacao na posicao i eh a mesma que a variavel para. */
						if (strcmp(controle->vetor_rotas[i].abreviacao_cidade, para) == 0){

							/* Se for a mesma, eh guardado o indice, na variavel. */
							indice2 = i;
							break;	
						}
					}

					/* variavel indice3 recebe a quantidade de voos da posicao da matriz indice1 e indice2*/
					int indice3 = controle->voos->arestas[indice1][indice2].qtdeVooDePara;

					/* incrementando +1, para cada voo achado para essas posicoes. */
					controle->voos->arestas[indice1][indice2].qtdeVooDePara += 1;

					/* realocando o cubo de memoria. */
					controle->voos->arestas[indice1][indice2].vetor_Conexao = (Conexao *) realloc(controle->voos->arestas[indice1][indice2].vetor_Conexao, controle->voos->arestas[indice1][indice2].qtdeVooDePara * sizeof(Conexao));
					
					/* o cubo de memoria na posicao [indice1, indice2, indice3], recebendo a duracao lida. */
					controle->voos->arestas[indice1][indice2].vetor_Conexao[indice3].duracao = duracao;
					
					/* o cubo de memoria na posicao [indice1, indice2, indice3], recebendo o identificador do voo. */
					strcpy(controle->voos->arestas[indice1][indice2].vetor_Conexao[indice3].identificador_voo, identificador);

					/* o cubo de memoria na posicao [indice1, indice2, indice3], recebendo a duracao em outro formato. */
					strcpy(controle->voos->arestas[indice1][indice2].vetor_Conexao[indice3].duracao_horario,   horario);
					
				}

	 		break;
		}
	}

	/*Fecha o arquivo*/
	fclose(arq);

	/* pegando a matriz lida, e convertendo ela para uma matriz com o menor custo. */
	controle->matriz_bi = conversor_matriz_tri_bi(controle->voos);

	system("clear");

	/*Deu tudo certo na leitura do arquivo*/
	return(true);
}

/* Funcao responsavel por mostrar a arvore geradora minima de acordo com o vertice desejado.*/
/* @param    : Recebe de parametro a quantidade de vertices que tem no grafo, a matriz bidimensional com o menor custo e o id do vertice raiz desejado.*/
/* @return   : Retorna uma matriz resposta com a arvore resultante.*/
int **algoritmo_de_prim(int qtdeVertices, int **matriz_bi, int inicio){

	/*Guarda os vertices ja verificados*/		
	boolean *verticesVerificados = (boolean *) malloc(qtdeVertices * sizeof(boolean));

	/*Guarda a duracao relativa para cada vertice em cada iteracao*/
	int *duracaoRelativa = (int *) malloc(qtdeVertices * sizeof(int));

	/*Guarda os nos vizinhos de cada no grafo da arvore final*/
	int *nosVizinhos = (int *) malloc(qtdeVertices * sizeof(int));

	/*Inicializacao de variaveis*/
	for (int i = 0; i < qtdeVertices; i++) {

		verticesVerificados[i] = false;
		nosVizinhos[i]         = 0;
		duracaoRelativa[i]     = INFINITO;
	}

	duracaoRelativa[0] = 0;

	/*Ponto que sera a raiz da arvore resultante*/
	int pontoAvaliado = (inicio - 1);

	/*Inicio do Prim*/
	for (int contadorPontosAvaliados = 0; contadorPontosAvaliados < qtdeVertices; contadorPontosAvaliados++) {

		for (int contadorVizinhos = 0; contadorVizinhos < qtdeVertices; contadorVizinhos++) {
			
			/*Verifica se o no a ser avaliado nessa iteracao ja foi avaliado anteriormente, se sim nao ira fazer nada*/
			if ((verticesVerificados[contadorVizinhos]) || (contadorVizinhos == pontoAvaliado)) {continue;}

			/*
				Verifica se na matriz ha algum valor na coluna que seja > 0. Caso afirmativo, significa que ha uma aresta entre estes dois pontos.
				Verifica se o peso da aresta entre os dois nos e menor que a atual duracaoRelativa do no vizinho.
				Caso correto, a duracaoRelativa do no viizho ao que esta sendo avaliado no momento sera atualizada pelo valor dessa no 
					duracao avaliada ate o pontoAvaliado
			*/
			if ((matriz_bi[pontoAvaliado][contadorVizinhos] > 0) &&
				((matriz_bi[pontoAvaliado][contadorVizinhos] < duracaoRelativa[contadorVizinhos]))) {

				duracaoRelativa[contadorVizinhos] = matriz_bi[pontoAvaliado][contadorVizinhos];
				nosVizinhos[contadorVizinhos]     = pontoAvaliado;
			}
		}

		/*Marca o vertice de pontoAvaliado como um vertice ja verificado*/
		verticesVerificados[pontoAvaliado] = true;

		/*Preparacao para selecao do proximo vertice a ser avalido*/
		pontoAvaliado = 0;
		int duracaoComparada = INFINITO;

		/*Selecao do proximo vertice a ser avalido*/
		for (int contador = 1; contador < qtdeVertices; ++contador) {
			
			/*Se o vertice a ser verificado ja foi verificado anteriormente (true) passa para a proxima iteracao*/
			if (verticesVerificados[contador]) {continue;}

			/*
				Se a duracao relativa desse ponto for menor que a do ponto avaliado assume-se esse novo ponto como o ponto avaliado
				Ao final da iteracao, sera selecionado o ponto com menor duracaoRelativa.
			*/

			if (duracaoRelativa[contador] < duracaoComparada){
				duracaoComparada = duracaoRelativa[contador];
				pontoAvaliado    = contador;
			}
		}
	}	

	int **matrizResposta = (int **) malloc(qtdeVertices * sizeof(int *));
	for (int i = 0; i < qtdeVertices; i++) {
		matrizResposta[i] = (int *) malloc(qtdeVertices * sizeof(int));
		for (int j = 0; j < qtdeVertices; j++) {
			matrizResposta[i][j] = 0;
		}
	}

	/*Criacao da matrizResposta com a arvore resultante do Algoritmo de Prim*/
	for (int i = 1; i < qtdeVertices; i++) {

		matrizResposta[i][nosVizinhos[i]] = matriz_bi[i][nosVizinhos[i]];
		matrizResposta[nosVizinhos[i]][i] = matriz_bi[i][nosVizinhos[i]];
	}

	return matrizResposta;
}

/* Funcao responsavel por criar arquivos .dot do grafo de prim. */
/* @param    : Recebe o grafo de parametro. */
/* @return   : void*/
void cria_Prim(Grafo *gr, int **matriz, int de){

	FILE *arq;
	arq = fopen("prim.dot","w");

	if(arq == NULL){
		printf("Ocorreu um erro na leitura do arquivo\n");
		exit(1);
	}

	fprintf(arq,"graph Arvore\n{\n");
	fprintf(arq,"\tlabel = \"""Arvore Geradora Minima [%s]""\";\n",gr->arestas[de-1][de-1].abreviacao_cidade);

	for (int i = 0; i < gr->nro_vertices; i++){

		if(i==0) {fprintf(arq,"\t%s[color=red];\n",        gr->arestas[i][i].abreviacao_cidade);}
		if(i==1) {fprintf(arq,"\t%s[color=blue];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==2) {fprintf(arq,"\t%s[color=green];\n",      gr->arestas[i][i].abreviacao_cidade);}
		if(i==3) {fprintf(arq,"\t%s[color=cyan];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==4) {fprintf(arq,"\t%s[color=black];\n",      gr->arestas[i][i].abreviacao_cidade);}
		if(i==5) {fprintf(arq,"\t%s[color=yellow];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==6) {fprintf(arq,"\t%s[color=violet];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==7) {fprintf(arq,"\t%s[color=orange];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==8) {fprintf(arq,"\t%s[color=gray];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==9) {fprintf(arq,"\t%s[color=purple];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==10){fprintf(arq,"\t%s[color=pink];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==11){fprintf(arq,"\t%s[color=tomato];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==12){fprintf(arq,"\t%s[color=gold];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==13){fprintf(arq,"\t%s[color=salmon];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==14){fprintf(arq,"\t%s[color=magenta];\n",    gr->arestas[i][i].abreviacao_cidade);}
		if(i==15){fprintf(arq,"\t%s[color=brown];\n",      gr->arestas[i][i].abreviacao_cidade);}
		if(i==16){fprintf(arq,"\t%s[color=dimgray];\n",    gr->arestas[i][i].abreviacao_cidade);}
		if(i==17){fprintf(arq,"\t%s[color=darkorange];\n", gr->arestas[i][i].abreviacao_cidade);}
		if(i==18){fprintf(arq,"\t%s[color=limegreen];\n",  gr->arestas[i][i].abreviacao_cidade);}
		if(i==19){fprintf(arq,"\t%s[color=darkviolet];\n", gr->arestas[i][i].abreviacao_cidade);}
		if(i==20){fprintf(arq,"\t%s[color=aquamarine];\n", gr->arestas[i][i].abreviacao_cidade);}
		if(i==21){fprintf(arq,"\t%s[color=peru];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==22){fprintf(arq,"\t%s[color=coral];\n",      gr->arestas[i][i].abreviacao_cidade);}
	}

	for(int i = 0; i < gr->nro_vertices; i++) {

		for(int j = i; j < gr->nro_vertices; j++) {

			if(gr->matriz_Rotas[i][j] > 0) {

				fprintf(arq,"\t%s -- %s",gr->arestas[i][i].abreviacao_cidade,gr->arestas[j][j].abreviacao_cidade);
				if(i==0) {fprintf(arq,"[label = ""%d""][color=red];\n",        matriz[i][j]);}
				if(i==1) {fprintf(arq,"[label = ""%d""][color=blue];\n",       matriz[i][j]);}
				if(i==2) {fprintf(arq,"[label = ""%d""][color=green];\n",      matriz[i][j]);}
				if(i==3) {fprintf(arq,"[label = ""%d""][color=cyan];\n",       matriz[i][j]);}
				if(i==4) {fprintf(arq,"[label = ""%d""][color=black];\n",      matriz[i][j]);}
				if(i==5) {fprintf(arq,"[label = ""%d""][color=yellow];\n",     matriz[i][j]);}
				if(i==6) {fprintf(arq,"[label = ""%d""][color=violet];\n",     matriz[i][j]);}
				if(i==7) {fprintf(arq,"[label = ""%d""][color=orange];\n",     matriz[i][j]);}
				if(i==8) {fprintf(arq,"[label = ""%d""][color=gray];\n",       matriz[i][j]);}
				if(i==9) {fprintf(arq,"[label = ""%d""][color=purple];\n",     matriz[i][j]);}
				if(i==10){fprintf(arq,"[label = ""%d""][color=pink];\n",       matriz[i][j]);}
				if(i==11){fprintf(arq,"[label = ""%d""][color=tomato];\n",     matriz[i][j]);}
				if(i==12){fprintf(arq,"[label = ""%d""][color=gold];\n",       matriz[i][j]);}
				if(i==13){fprintf(arq,"[label = ""%d""][color=salmon];\n",     matriz[i][j]);}
				if(i==14){fprintf(arq,"[label = ""%d""][color=magenta];\n",    matriz[i][j]);}
				if(i==15){fprintf(arq,"[label = ""%d""][color=brown];\n",      matriz[i][j]);}
				if(i==16){fprintf(arq,"[label = ""%d""][color=dimgray];\n",    matriz[i][j]);}
				if(i==17){fprintf(arq,"[label = ""%d""][color=darkorange];\n", matriz[i][j]);}
				if(i==18){fprintf(arq,"[label = ""%d""][color=limegreen];\n",  matriz[i][j]);}
				if(i==19){fprintf(arq,"[label = ""%d""][color=darkviolet];\n", matriz[i][j]);}
				if(i==20){fprintf(arq,"[label = ""%d""][color=aquamarine];\n", matriz[i][j]);}
				if(i==21){fprintf(arq,"[label = ""%d""][color=peru];\n",       matriz[i][j]);}
				if(i==22){fprintf(arq,"[label = ""%d""][color=coral];\n",      matriz[i][j]);}
			}
		}
	}

	fprintf(arq,"}");
	fclose(arq);
}

/* Funcao responsavel por contar a quantidade de vertices que tem no arquivo txt.*/
/* @param    : Ponteiro para o arquivo.*/
/* @return   : int - A quantidade de aeroportos existentes.*/
int conta_Aeroportos(FILE *arq){

	char lixo[400];
	char abreviacao[4];
	char cidade[70];
	char aeroporto[50];

	int qtdeVertices = 0;

	/*Le o arquivo uma primeira vez para saber quantos vertices vai ter na matriz*/
	while(!feof(arq)){

		switch(fgetc(arq)){
			
			case '#':

				/*Le ate o \n*/
				fscanf(arq,"%[^\n]s",lixo);

				/*Le o \n pulando para a proxima linha*/
				fgetc(arq);
				
			break;

			/*Achou o caractere '$'*/
			case '$':

				/*Loop infinito para ser generico, PARA quando achar o '$'*/
				while(1){
					
					/*Le o \n apos o '$'*/
					fgetc(arq);

					/*Le a abreviacao do nome da cidade*/
					fscanf(arq,"%s", abreviacao);

					/*Espaco apos a abreviacao*/
					fgetc(arq);

					/*Achou o local de parada que e o '$', entao o loop sera quebrado*/
					if (strcmp(abreviacao, "$") == 0){
						break;
					}

					/*Le o nome da cidade, nome do estado*/
					fscanf(arq,"%[^(]s", cidade);

					/*Espaco apos o nome do estado*/
					fgetc(arq);

					/*Le o nome do aeroporto que esta entre parenteses*/
					fscanf(arq,"%[^)]s", aeroporto);

					/*Acrescenta a variavel de vertices*/
					qtdeVertices += 1;
				}

				/*Apos ter achado o '$' nao precisa ler mais, entao retorna o numero de vertices*/
				return qtdeVertices;

			break;
		}
	}
}

/* Funcao responsavel por criar arquivos .dot do grafo de voos. */
/* @param    : Recebe o grafo de parametro. */
/* @return   : void*/
void cria_Dot_Voos(Grafo *gr){

	/* Criando um ponteiro arq de FILE*/
	FILE *arq;

	/* Criando um arquivo para a escrita. */
	arq = fopen("voos.dot","w");

	/* Verificando se houve algum erro ao abrir o arquivo. */
	if(arq == NULL){
		printf("Ocorreu um erro na leitura, ou abertura do arquivo\n");
		exit(1);
	}

	/* Aqui sera escrito no arquivo, no formato que o graphviz eh aceito, para depois vir a ser gerado um digrafo.*/
	
	/* Aqui eh informado que eh um digrafo. */
	fprintf(arq,"digraph Voos\n{\n");

	/* Aqui eh informado o nome do digrafo. */
	fprintf(arq,"\tlabel = \"""VOOS""\";\n");


	/* for responsavel por criar os vertices do digrafo. */
	for (int i = 0; i < gr->nro_vertices; i++){

		/* Aqui, cada vertice ira receber a sua respectiva abreviacao, e uma cor diferente um do outro. */
		if(i==0) {fprintf(arq,"\t%s[color=red];\n",        gr->arestas[i][i].abreviacao_cidade);}
		if(i==1) {fprintf(arq,"\t%s[color=blue];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==2) {fprintf(arq,"\t%s[color=green];\n",      gr->arestas[i][i].abreviacao_cidade);}
		if(i==3) {fprintf(arq,"\t%s[color=cyan];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==4) {fprintf(arq,"\t%s[color=black];\n",      gr->arestas[i][i].abreviacao_cidade);}
		if(i==5) {fprintf(arq,"\t%s[color=yellow];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==6) {fprintf(arq,"\t%s[color=violet];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==7) {fprintf(arq,"\t%s[color=orange];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==8) {fprintf(arq,"\t%s[color=gray];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==9) {fprintf(arq,"\t%s[color=purple];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==10){fprintf(arq,"\t%s[color=pink];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==11){fprintf(arq,"\t%s[color=tomato];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==12){fprintf(arq,"\t%s[color=gold];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==13){fprintf(arq,"\t%s[color=salmon];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==14){fprintf(arq,"\t%s[color=magenta];\n",    gr->arestas[i][i].abreviacao_cidade);}
		if(i==15){fprintf(arq,"\t%s[color=brown];\n",      gr->arestas[i][i].abreviacao_cidade);}
		if(i==16){fprintf(arq,"\t%s[color=dimgray];\n",    gr->arestas[i][i].abreviacao_cidade);}
		if(i==17){fprintf(arq,"\t%s[color=darkorange];\n", gr->arestas[i][i].abreviacao_cidade);}
		if(i==18){fprintf(arq,"\t%s[color=limegreen];\n",  gr->arestas[i][i].abreviacao_cidade);}
		if(i==19){fprintf(arq,"\t%s[color=darkviolet];\n", gr->arestas[i][i].abreviacao_cidade);}
		if(i==20){fprintf(arq,"\t%s[color=Aquamarine];\n", gr->arestas[i][i].abreviacao_cidade);}
		if(i==21){fprintf(arq,"\t%s[color=peru];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==22){fprintf(arq,"\t%s[color=coral];\n",      gr->arestas[i][i].abreviacao_cidade);}			
	}
	

	/* For que ira percorrer todas as posicoes do cubo de memoria, buscando todas as conexoes dos voos. */	
	for(int i = 0; i < gr->nro_vertices; i++) {

		for(int j = 0; j < gr->nro_vertices; j++) {

			for (int k = 0; k < gr->arestas[i][j].qtdeVooDePara; k++) {

				/* Aqui eh realizado a ligacao entre dois vertices. */
				fprintf(arq,"\t%s -> %s",gr->arestas[i][i].abreviacao_cidade,gr->arestas[j][j].abreviacao_cidade);
				
				/* Aqui eh colocado as mesmas cores dos vertices, para suas respectivas ligacoes. */
				if(i==0) {fprintf(arq,"[label = \"""(%s)""\"][color=red];\n",        gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==1) {fprintf(arq,"[label = \"""(%s)""\"][color=blue];\n",       gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==2) {fprintf(arq,"[label = \"""(%s)""\"][color=green];\n",      gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==3) {fprintf(arq,"[label = \"""(%s)""\"][color=cyan];\n",       gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==4) {fprintf(arq,"[label = \"""(%s)""\"][color=black];\n",      gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==5) {fprintf(arq,"[label = \"""(%s)""\"][color=yellow];\n",     gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==6) {fprintf(arq,"[label = \"""(%s)""\"][color=violet];\n",     gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==7) {fprintf(arq,"[label = \"""(%s)""\"][color=orange];\n",     gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==8) {fprintf(arq,"[label = \"""(%s)""\"][color=gray];\n",       gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==9) {fprintf(arq,"[label = \"""(%s)""\"][color=purple];\n",     gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==10){fprintf(arq,"[label = \"""(%s)""\"][color=pink];\n",       gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==11){fprintf(arq,"[label = \"""(%s)""\"][color=tomato];\n",     gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==12){fprintf(arq,"[label = \"""(%s)""\"][color=gold];\n",       gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==13){fprintf(arq,"[label = \"""(%s)""\"][color=salmon];\n",     gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==14){fprintf(arq,"[label = \"""(%s)""\"][color=magenta];\n",    gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==15){fprintf(arq,"[label = \"""(%s)""\"][color=brown];\n",      gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==16){fprintf(arq,"[label = \"""(%s)""\"][color=dimgray];\n",    gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==17){fprintf(arq,"[label = \"""(%s)""\"][color=darkorange];\n", gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==18){fprintf(arq,"[label = \"""(%s)""\"][color=limegreen];\n",  gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==19){fprintf(arq,"[label = \"""(%s)""\"][color=darkviolet];\n", gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==20){fprintf(arq,"[label = \"""(%s)""\"][color=Aquamarine];\n", gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==21){fprintf(arq,"[label = \"""(%s)""\"][color=peru];\n",       gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
				if(i==22){fprintf(arq,"[label = \"""(%s)""\"][color=coral];\n",      gr->arestas[i][j].vetor_Conexao[k].duracao_horario);}
			}
		}
	}

	/* finalizando o arquivo.DOT. */
	fprintf(arq,"}");

	/* fechando o arquivo. */
	fclose(arq);
}

/* Funcao responsavel por criar arquivos .dot do grafo de rotas. */
/* @param    : Recebe o grafo de parametro. */
/* @return   : void*/
void cria_Dot_Rotas(Grafo *gr){


	FILE *arq;
	arq = fopen("rotas.dot","w");

	if(arq == NULL){
		printf("Ocorreu um erro na leitura do arquivo\n");
		exit(1);
	}

	fprintf(arq,"graph Rotas\n{\n");
	fprintf(arq,"\tlabel = \"""GRAFO DE ROTAS""\";\n");
		
	for (int i = 0; i < gr->nro_vertices; i++){

		if(i==0) {fprintf(arq,"\t%s[color=red];\n",        gr->arestas[i][i].abreviacao_cidade);}
		if(i==1) {fprintf(arq,"\t%s[color=blue];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==2) {fprintf(arq,"\t%s[color=green];\n",      gr->arestas[i][i].abreviacao_cidade);}
		if(i==3) {fprintf(arq,"\t%s[color=cyan];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==4) {fprintf(arq,"\t%s[color=black];\n",      gr->arestas[i][i].abreviacao_cidade);}
		if(i==5) {fprintf(arq,"\t%s[color=yellow];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==6) {fprintf(arq,"\t%s[color=violet];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==7) {fprintf(arq,"\t%s[color=orange];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==8) {fprintf(arq,"\t%s[color=gray];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==9) {fprintf(arq,"\t%s[color=purple];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==10){fprintf(arq,"\t%s[color=pink];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==11){fprintf(arq,"\t%s[color=tomato];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==12){fprintf(arq,"\t%s[color=gold];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==13){fprintf(arq,"\t%s[color=salmon];\n",     gr->arestas[i][i].abreviacao_cidade);}
		if(i==14){fprintf(arq,"\t%s[color=magenta];\n",    gr->arestas[i][i].abreviacao_cidade);}
		if(i==15){fprintf(arq,"\t%s[color=brown];\n",      gr->arestas[i][i].abreviacao_cidade);}
		if(i==16){fprintf(arq,"\t%s[color=dimgray];\n",    gr->arestas[i][i].abreviacao_cidade);}
		if(i==17){fprintf(arq,"\t%s[color=darkorange];\n", gr->arestas[i][i].abreviacao_cidade);}
		if(i==18){fprintf(arq,"\t%s[color=limegreen];\n",  gr->arestas[i][i].abreviacao_cidade);}
		if(i==19){fprintf(arq,"\t%s[color=darkviolet];\n", gr->arestas[i][i].abreviacao_cidade);}
		if(i==20){fprintf(arq,"\t%s[color=aquamarine];\n", gr->arestas[i][i].abreviacao_cidade);}
		if(i==21){fprintf(arq,"\t%s[color=peru];\n",       gr->arestas[i][i].abreviacao_cidade);}
		if(i==22){fprintf(arq,"\t%s[color=coral];\n",      gr->arestas[i][i].abreviacao_cidade);}	
	}

	for(int i = 0; i < gr->nro_vertices; i++){

		for(int j = 1; j < gr->nro_vertices; j++){

			if(gr->matriz_Rotas[i][j] == 1){

				fprintf(arq,"\t%s -- %s",gr->arestas[i][i].abreviacao_cidade,gr->arestas[j][j].abreviacao_cidade);
				if(i==0) {fprintf(arq,"[color=red];\n");}
				if(i==1) {fprintf(arq,"[color=blue];\n");}
				if(i==2) {fprintf(arq,"[color=green];\n");}
				if(i==3) {fprintf(arq,"[color=cyan];\n");}
				if(i==4) {fprintf(arq,"[color=black];\n");}
				if(i==5) {fprintf(arq,"[color=yellow];\n");}
				if(i==6) {fprintf(arq,"[color=violet];\n");}
				if(i==7) {fprintf(arq,"[color=orange];\n");}
				if(i==8) {fprintf(arq,"[color=gray];\n");}
				if(i==9) {fprintf(arq,"[color=purple];\n");}
				if(i==10){fprintf(arq,"[color=pink];\n");}
				if(i==11){fprintf(arq,"[color=tomato];\n");}
				if(i==12){fprintf(arq,"[color=gold];\n");}
				if(i==13){fprintf(arq,"[color=salmon];\n");}
				if(i==14){fprintf(arq,"[color=magenta];\n");}
				if(i==15){fprintf(arq,"[color=brown];\n");}
				if(i==16){fprintf(arq,"[color=dimgray];\n");}
				if(i==17){fprintf(arq,"[color=darkorange];\n");}
				if(i==18){fprintf(arq,"[color=limegreen];\n");}
				if(i==19){fprintf(arq,"[color=darkviolet];\n");}
				if(i==20){fprintf(arq,"[color=aquamarine];\n");}
				if(i==21){fprintf(arq,"[color=peru];\n");}
				if(i==22){fprintf(arq,"[color=coral];\n");}
			}
		}
	}
	
	fprintf(arq,"}");
	fclose(arq);
}

/* Transforma a matriz tridimensional em uma matriz bidimensional, sera pego a menor duracao*/
/* @param    : Recebe um grafo.*/
/* @return   : int** - Retorna a matriz bidimensional com o menor custo.*/
int **conversor_matriz_tri_bi(Grafo *gr){

	int **matriz_bi;
	matriz_bi = (int**) malloc(gr->nro_vertices * sizeof(int*));	

	for (int i = 0; i < gr->nro_vertices; i++) {
		matriz_bi[i] = (int*) malloc(gr->nro_vertices * sizeof(int));
	    for (int j = 0; j < gr->nro_vertices; j++) {

    		/*Iniciando variaveis*/
			matriz_bi[i][j] = INFINITO;
    	}
	}

	int valor_menor=0;

	for (int i = 0; i < gr->nro_vertices; i++){
		for (int j = 0; j < gr->nro_vertices; j++){
			for (int k = 0; k < gr->arestas[i][j].qtdeVooDePara; k++){
				if(gr->arestas[i][j].vetor_Conexao[k].duracao < valor_menor || valor_menor == 0) {
					valor_menor = gr->arestas[i][j].vetor_Conexao[k].duracao;		
					matriz_bi[i][j] = valor_menor;
				}
			}
			valor_menor = 0;
		}
	}

	return(matriz_bi);
}

/* Funcao responsavel por achar o menor caminho de um aeroporto origem para um destino.*/
/* @param    : Recebe um grafo, um aeroporto origem, um aeroporto destino, a matriz com menor custo e uma variavel de duracao que sera modificada aqui dentro para ser usada fora.*/
/* @return   : int* - Retorna o vetor com os ids.*/
int *caminho_de_para(Grafo *gr, Aeroporto *origem, Aeroporto *destino, int **matriz_bi, int *duracao){

	int *trajeto = (int*)malloc(gr->nro_vertices*sizeof(int));
	for (int i = 0; i < gr->nro_vertices; i++){
		trajeto[i] = -1;
	}

	/*Caso base 1: Nao ha um intermediario entre a origem e o destino*/
	if((gr->arestas[origem->id][destino->id].qtdeVooDePara > 0 )){
		trajeto[0] = gr->arestas[origem->id][origem->id].id;
		trajeto[1] = gr->arestas[destino->id][destino->id].id;
		*duracao   = matriz_bi[origem->id][destino->id]; 
		return(trajeto);

	/*Caso base 2: A origem e o destino sao iguais*/
	}else if((gr->arestas[origem->id][origem->id].id == gr->arestas[destino->id][destino->id].id)){
		printf("\nNao existe voo para a cidade que voce ja esta ¬¬!\n");
		trajeto[0] = gr->arestas[origem->id][origem->id].id;
		trajeto[1] = gr->arestas[destino->id][destino->id].id;
		*duracao   = 0;
		return(trajeto);
	}

	boolean primeiro = true;
	int menor  = 0;
	int indice;

	/*Caso base 3: Ha um aeroporto no meio do caminho, pega o que tem menor duracao*/
	for (int i = 0; i < gr->nro_vertices; ++i){

		for (int j = 0; j < gr->nro_vertices; ++j){
 						
 						/*A*/												/*E*/
			if ((gr->arestas[origem->id][j].qtdeVooDePara > 0) && (gr->arestas[j][destino->id].qtdeVooDePara > 0)){

				if(primeiro){
									/*C*/
					menor    = matriz_bi[origem->id][j] + matriz_bi[j][destino->id];
					indice   = j;
					primeiro = false;

				}else{

					if((matriz_bi[origem->id][j] + matriz_bi[j][destino->id] ) < menor){
						indice = j;
						menor  = matriz_bi[origem->id][j] + matriz_bi[j][destino->id];
					}
				}
			}
		}
	}

	/*A duracao vai ser a variavel soma de menor caminho*/
	*duracao = menor;

	/*Preenche as 3 posicoes do vetor com a origem, a meiuca e o destino*/
	trajeto[0] = gr->arestas[origem->id][origem->id].id;
	trajeto[1] = gr->arestas[indice][indice].id;
	trajeto[2] = gr->arestas[destino->id][destino->id].id;

	/*Retorna o vetor*/
	return(trajeto);
}

/* Funcao que eh responsavel por criar um .dot para mostrar no graphviz os voos diretos de um determinado aeroporto. */
/* @param    : Recebe de parametro o grafo, o aeroporto de origem, e o ID do aeroporto de origem. */
/* @return   : void. */
void voo_sem_escala(Grafo *gr, Aeroporto *origem, int id){

	/* Criando um ponteiro arq de FILE*/
	FILE *arq;

	/* Criando um arquivo para a escrita. */
	arq = fopen("voosdiretos.dot","w");

	/* Verificando se houve algum erro ao abrir o arquivo. */
	if(arq == NULL){
		printf("Ocorreu um erro na leitura do arquivo\n");
		exit(1);
	}


	/* Aqui sera escrito no arquivo, no formato que o graphviz eh aceito, para depois vir a ser gerado um digrafo.*/
	
	/* Aqui eh informado que eh um digrafo. */
	fprintf(arq,"digraph Voos\n{\n");

	/* Aqui eh informado o nome do digrafo. */
	fprintf(arq,"\tlabel = \"""VOOS DIRETOS [%s]""\";\n", gr->arestas[id-1][id-1].abreviacao_cidade);

		

	for (int i = 0; i < gr->nro_vertices; i++) {

		for (int j = 0; j < gr->nro_vertices; j++) {

			if(gr->arestas[i][j].qtdeVooDePara > 0) {

				if(i == origem->id) {

					if(j==0) {fprintf(arq,"\t%s[color=red];\n",        gr->arestas[j][j].abreviacao_cidade);}
					if(j==1) {fprintf(arq,"\t%s[color=blue];\n",       gr->arestas[j][j].abreviacao_cidade);}
					if(j==2) {fprintf(arq,"\t%s[color=green];\n",      gr->arestas[j][j].abreviacao_cidade);}
					if(j==3) {fprintf(arq,"\t%s[color=cyan];\n",       gr->arestas[j][j].abreviacao_cidade);}
					if(j==4) {fprintf(arq,"\t%s[color=black];\n",      gr->arestas[j][j].abreviacao_cidade);}
					if(j==5) {fprintf(arq,"\t%s[color=yellow];\n",     gr->arestas[j][j].abreviacao_cidade);}
					if(j==6) {fprintf(arq,"\t%s[color=violet];\n",     gr->arestas[j][j].abreviacao_cidade);}
					if(j==7) {fprintf(arq,"\t%s[color=orange];\n",     gr->arestas[j][j].abreviacao_cidade);}
					if(j==8) {fprintf(arq,"\t%s[color=gray];\n",       gr->arestas[j][j].abreviacao_cidade);}
					if(j==9) {fprintf(arq,"\t%s[color=purple];\n",     gr->arestas[j][j].abreviacao_cidade);}
					if(j==10){fprintf(arq,"\t%s[color=pink];\n",       gr->arestas[j][j].abreviacao_cidade);}
					if(j==11){fprintf(arq,"\t%s[color=tomato];\n",     gr->arestas[j][j].abreviacao_cidade);}
					if(j==12){fprintf(arq,"\t%s[color=gold];\n",       gr->arestas[j][j].abreviacao_cidade);}
					if(j==13){fprintf(arq,"\t%s[color=salmon];\n",     gr->arestas[j][j].abreviacao_cidade);}
					if(j==14){fprintf(arq,"\t%s[color=magenta];\n",    gr->arestas[j][j].abreviacao_cidade);}
					if(j==15){fprintf(arq,"\t%s[color=brown];\n",      gr->arestas[j][j].abreviacao_cidade);}
					if(j==16){fprintf(arq,"\t%s[color=dimgray];\n",    gr->arestas[j][j].abreviacao_cidade);}
					if(j==17){fprintf(arq,"\t%s[color=darkorange];\n", gr->arestas[j][j].abreviacao_cidade);}
					if(j==18){fprintf(arq,"\t%s[color=limegreen];\n",  gr->arestas[j][j].abreviacao_cidade);}
					if(j==19){fprintf(arq,"\t%s[color=darkviolet];\n", gr->arestas[j][j].abreviacao_cidade);}
					if(j==20){fprintf(arq,"\t%s[color=Aquamarine];\n", gr->arestas[j][j].abreviacao_cidade);}
					if(j==21){fprintf(arq,"\t%s[color=peru];\n",       gr->arestas[j][j].abreviacao_cidade);}
					if(j==22){fprintf(arq,"\t%s[color=coral];\n",      gr->arestas[j][j].abreviacao_cidade);}
				}
			}
		}
		
	}

	/* For que ira percorrer todas as posicoes do cubo de memoria, buscando todas as conexoes dos voos. */	
	for(int i = 0; i < gr->nro_vertices; i++) {

		for(int j = 0; j < gr->nro_vertices; j++) {

			if(gr->arestas[i][j].qtdeVooDePara > 0) {

				/* Aqui eh realizado a ligacao entre dois vertices. */
				if(i == origem->id) {

					/* Aqui eh colocado as mesmas cores dos vertices, para suas respectivas ligacoes. */
					fprintf(arq,"\t%s -> %s",gr->arestas[i][i].abreviacao_cidade,gr->arestas[j][j].abreviacao_cidade);
					if(j==0) {fprintf(arq,"[label = \"""(%d)""\"][color=red];\n",        gr->arestas[i][j].qtdeVooDePara);}
					if(j==1) {fprintf(arq,"[label = \"""(%d)""\"][color=blue];\n",       gr->arestas[i][j].qtdeVooDePara);}
					if(j==2) {fprintf(arq,"[label = \"""(%d)""\"][color=green];\n",      gr->arestas[i][j].qtdeVooDePara);}
					if(j==3) {fprintf(arq,"[label = \"""(%d)""\"][color=cyan];\n",       gr->arestas[i][j].qtdeVooDePara);}
					if(j==4) {fprintf(arq,"[label = \"""(%d)""\"][color=black];\n",      gr->arestas[i][j].qtdeVooDePara);}
					if(j==5) {fprintf(arq,"[label = \"""(%d)""\"][color=yellow];\n",     gr->arestas[i][j].qtdeVooDePara);}
					if(j==6) {fprintf(arq,"[label = \"""(%d)""\"][color=violet];\n",     gr->arestas[i][j].qtdeVooDePara);}
					if(j==7) {fprintf(arq,"[label = \"""(%d)""\"][color=orange];\n",     gr->arestas[i][j].qtdeVooDePara);}
					if(j==8) {fprintf(arq,"[label = \"""(%d)""\"][color=gray];\n",       gr->arestas[i][j].qtdeVooDePara);}
					if(j==9) {fprintf(arq,"[label = \"""(%d)""\"][color=purple];\n",     gr->arestas[i][j].qtdeVooDePara);}
					if(j==10){fprintf(arq,"[label = \"""(%d)""\"][color=pink];\n",       gr->arestas[i][j].qtdeVooDePara);}
					if(j==11){fprintf(arq,"[label = \"""(%d)""\"][color=tomato];\n",     gr->arestas[i][j].qtdeVooDePara);}
					if(j==12){fprintf(arq,"[label = \"""(%d)""\"][color=gold];\n",       gr->arestas[i][j].qtdeVooDePara);}
					if(j==13){fprintf(arq,"[label = \"""(%d)""\"][color=salmon];\n",     gr->arestas[i][j].qtdeVooDePara);}
					if(j==14){fprintf(arq,"[label = \"""(%d)""\"][color=magenta];\n",    gr->arestas[i][j].qtdeVooDePara);}
					if(j==15){fprintf(arq,"[label = \"""(%d)""\"][color=brown];\n",      gr->arestas[i][j].qtdeVooDePara);}
					if(j==16){fprintf(arq,"[label = \"""(%d)""\"][color=dimgray];\n",    gr->arestas[i][j].qtdeVooDePara);}
					if(j==17){fprintf(arq,"[label = \"""(%d)""\"][color=darkorange];\n", gr->arestas[i][j].qtdeVooDePara);}
					if(j==18){fprintf(arq,"[label = \"""(%d)""\"][color=limegreen];\n",  gr->arestas[i][j].qtdeVooDePara);}
					if(j==19){fprintf(arq,"[label = \"""(%d)""\"][color=darkviolet];\n", gr->arestas[i][j].qtdeVooDePara);}
					if(j==20){fprintf(arq,"[label = \"""(%d)""\"][color=Aquamarine];\n", gr->arestas[i][j].qtdeVooDePara);}
					if(j==21){fprintf(arq,"[label = \"""(%d)""\"][color=peru];\n",       gr->arestas[i][j].qtdeVooDePara);}
					if(j==22){fprintf(arq,"[label = \"""(%d)""\"][color=coral];\n",      gr->arestas[i][j].qtdeVooDePara);}
				}
			}
		}
	}

	/* finalizando o arquivo.DOT. */
	fprintf(arq,"}");
	
	/* fechando o arquivo. */
	fclose(arq);
}

/* Funcao responsavel por mostrar quais aeroportos que se ficarem fora de servico conseguiriam impedir voce de chegar da origem para o destino.*/
/* @param    : Recebe um grafo, um aeroporto origem, um aeroporto destino e uma variavel de tamanho que sera modificada aqui dentro para ser usada fora.*/
/* @return   : int* - Retorna um vetor com os indices desse aeroportos.*/
int *aeroportos_Fora_De_Servico(Grafo *gr, Aeroporto *origem, Aeroporto *destino, int *tam_vetor_trajeto) {

	int *trajeto = NULL;
	int indice = 0;


	for (int j = 0; j < gr->nro_vertices; j++) {
						
		if ((gr->arestas[origem->id][j].qtdeVooDePara > 0) && (gr->arestas[j][destino->id].qtdeVooDePara > 0)) {

			trajeto = (int *) realloc(trajeto, (indice + 1) * sizeof(int));
			trajeto[indice] = j;
			indice++;
		}
	}

	*tam_vetor_trajeto = indice;
	
	return(trajeto);
}