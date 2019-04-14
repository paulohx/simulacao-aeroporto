/*************************************************************************************************************/
/*                                       Software MecNosh                                                    */
/*                                                                                                           */
/*                 Desenvolvedores: Jefferson Marques Costa Alves - 0035328                                  */
/*                                  Paulo Henrique Xavier         - 0035450                                  */
/*                                                                                                           */
/*                                                                                                           */
/*  Escopo:                     Controle de Voos e Rotas Aereas                                              */  
/*          O controle de horarios das companhias aereas podem ser vistos atraves da modelagem de um grafo,  */
/*          no qual os vertices sao aeroportos e as arestas podem representar rotas (nao dirigidas) ou       */
/*          voos (dirigidas).                                                                                */
/*                                                                                                           */
/*  Como compilar: make                                                                                      */
/*  Como executar: ./main.exe voo.txt                                                                        */
/*************************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tadGrafo.h"
#include "util.h"

enum boolean{false = 0 ,true};

int main (int argc, char *argv[]) {

	/*Nao passou os argumentos corretamente*/
	if(argc != 2){
		system("xcowsay Argumentos invalidos! \" ;-;\"");
		return(0);
	}

	/*Cria uma estrutura controle para gerenciar os grafos*/
	Controle *controle = cria_controle();

	/*Caso houve algum erro na leitura do arquivo*/
	if(!leitura(controle, argv[1])){
		system("xcowsay Ocorreu um erro na leitura do arquivo!\" ;-;\"");
		return(0);
	}

	/*Variaveis auxiliares*/
	 
	char trajeto1[4], trajeto2[4], trajeto3[4];
	int opcao = 0, de = 0, para = 0, dot = 0, duracao, tam_vetor_trajeto;
	boolean loop = true;
	
	int **matriz;

	Grafo *Prim;

	int *trajeto;
	int *vetor_trajeto;
	

	/*Laco de repeticao infinito. E parado quando o usuario deseja sair do programa.*/
	while(loop){
		
		LIMPA

		printf("\n\n\n\n\n\n\n\n");
		printf("\t\t\t\t\t--------------------------------------------------------------------------------\n");
		printf("\t\t\t\t\t|                     JP - Controle de Voos e Rotas Aereas                     |\n");
		printf("\t\t\t\t\t-------------------------------------------------------------------------------\n");
		printf("\t\t\t\t\t-------------------------------------------------------------------------------\n");
		printf("\t\t\t\t\t| Qual operacao deseja realizar?                                               |\n");
		printf("\t\t\t\t\t-------------------------------------------------------------------------------\n");
		printf("\t\t\t\t\t|[1]-Representar os grafos visualmente;                                        |\n");
		printf("\t\t\t\t\t-------------------------------------------------------------------------------\n");
		printf("\t\t\t\t\t|[2]-Trajeto entre dois aeroportos escolhidos;                                 |\n");
		printf("\t\t\t\t\t-------------------------------------------------------------------------------\n");
		printf("\t\t\t\t\t|[3]-Mostrar, a partir de um aeroporto, seus voos diretos;                     |\n");
		printf("\t\t\t\t\t-------------------------------------------------------------------------------\n");
		printf("\t\t\t\t\t|[4]-A partir de um aeroporto origem e destino, mostrar seu menor caminho;     |\n");
		printf("\t\t\t\t\t-------------------------------------------------------------------------------\n");
		printf("\t\t\t\t\t|[5]-A partir de um aerporto, determinar se e possivel atingir outro;          |\n");
		printf("\t\t\t\t\t-------------------------------------------------------------------------------\n");
		printf("\t\t\t\t\t|[6]-Definir um aeroporto de partida, e passar por todos os outros aeroportos; |\n");
		printf("\t\t\t\t\t-------------------------------------------------------------------------------\n");
		printf("\t\t\t\t\t|[7]-Encerrar o programa;                                                      |\n");
		printf("\t\t\t\t\t-------------------------------------------------------------------------------\n");
		printf("\t\t\t\t\t --> ");
		scanf("%d", &opcao);

		switch(opcao){

			/*[1]-Representar os grafos visualmente;*/
			case 1:

				do{
					
					LIMPA

					printf("\n\n\n\n\n\n\n\n");
					printf("\t\t\t\t\t\t\t\t\tRepresentacao grafos\n");
					printf("\t\t\t\t\t\t\t\t\tVoce deseja ver qual grafo:\n");
					printf("\t\t\t\t\t\t\t\t\t[1]-Grafo de Rotas:\n");
					printf("\t\t\t\t\t\t\t\t\t[2]-Grafo de Voos:\n");
					printf("\t\t\t\t\t\t\t\t\t-->");
					scanf("%d", &dot);

					if(dot == 1 ){
						cria_Dot_Rotas(controle->rotas);
						system("dot -Tpng rotas.dot -o rotas.png");
						system("display rotas.png");
					}else if(dot == 2){
						cria_Dot_Voos(controle->voos);	
						system("dot -Tpng voos.dot -o voos.png");
						system("display voos.png");
						
					}

				}while(!((dot > 0) && (dot < 3)));

				PAUSA

			break;

			/*[2]-Trajeto entre dois aeroportos escolhidos;*/
			case 2:
				
				LIMPA


				printf("\n\n\n\n\n\n\n\n");
				printf("\t\t\t\t\t\t\tEscolha um ID de um aeroporto de destino e um de origem:\n");

				for(int i = 0 ; i < controle->rotas->nro_vertices; i++){
					printf("\t\t\t\t\t\t\t[%d]-%s\n", i+1, controle->vetor_rotas[i].abreviacao_cidade);
				}
				
				do{					
					printf("\n\t\t\t\t\t\t\tOrigem:");
					scanf("%d", &de);
				}while(!((de >= 1) && (de <= controle->rotas->nro_vertices)));

				do{
					printf("\n\t\t\t\t\t\t\tDestino:");
					scanf("%d", &para);
				}while(!((para >= 1) && (para <= controle->rotas->nro_vertices)));

				trajeto = caminho_de_para(controle->voos, &controle->voos->arestas[de-1][de-1], &controle->voos->arestas[para-1][para-1], controle->matriz_bi, &duracao);

				if(trajeto[2] == -1){
					strcpy(trajeto1,   controle->voos->arestas[trajeto[0]][trajeto[0]].abreviacao_cidade);
					strcpy(trajeto2, controle->voos->arestas[trajeto[1]][trajeto[1]].abreviacao_cidade);
				}else{
					strcpy(trajeto1,   controle->voos->arestas[trajeto[0]][trajeto[0]].abreviacao_cidade);
					strcpy(trajeto2, controle->voos->arestas[trajeto[1]][trajeto[1]].abreviacao_cidade);
					strcpy(trajeto3, controle->voos->arestas[trajeto[2]][trajeto[2]].abreviacao_cidade);
				}

				for (int i = 0; i < 10; i++){
					
					LIMPA

					if(trajeto[2] != -1){
						for(int j = 0; j <= i ; j++){printf("\t");}printf("_________________________           _____\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|    %s--->%s--->%s   \\          \\   \\__      _____\n",trajeto1, trajeto2, trajeto3);
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|                         \\__________\\   \\/_______\\___\\_____________\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|                          /         < /_/ JP .................... o`-.\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|_________________________/            `-----------,----,--------------'\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("                                                  /____/\n");
						sleep(1);
					}else{
						for(int j = 0; j <= i ; j++){printf("\t");}printf("_________________________           _____\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|      %s----->%s      \\          \\   \\__      _____\n",trajeto1, trajeto2);
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|                         \\__________\\   \\/_______\\___\\_____________\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|                          /         < /_/ JP .................... o`-.\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|_________________________/            `-----------,----,--------------'\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("                                                  /____/\n");
						sleep(1);
					}

				}
				
				LIMPA
				PAUSA

			break;

			/*[3]-Mostrar, a partir de um aeroporto, seus voos diretos;*/
			case 3:
				
				LIMPA

				printf("\n\n\n\n\n\n\n\n");
				printf("\t\t\t\t\t\t\tEscolha um ID de um aeroporto para ver se existem voos diretos:\n");

				for(int i = 0 ; i < controle->rotas->nro_vertices; i++){
					printf("\t\t\t\t\t\t\t[%d]-%s\n", i+1, controle->vetor_rotas[i].abreviacao_cidade);
				}
				
				do{			
					printf("\n\t\t\t\t\t\t\tQual eh o ID escolhido: ");
					scanf("%d", &de);
				}while(!((de >= 1) && (de <= controle->rotas->nro_vertices)));

				
				LIMPA

				voo_sem_escala(controle->voos, &controle->voos->arestas[de-1][de-1], de);

				system("dot -Tpng voosdiretos.dot -o voosdiretos.png");
				system("display voosdiretos.png");


			break;

			/*[4]-A partir de um aeroporto origem e destino, mostrar seu menor caminho;*/
			case 4:
				
				LIMPA

				printf("\n\n\n\n\n\n\n\n");
				printf("\t\t\t\t\t\t\tEscolha um ID de um aeroporto de destino e um de origem:\n");

				for(int i = 0 ; i < controle->rotas->nro_vertices; i++){
					printf("\t\t\t\t\t\t\t[%d]-%s\n", i+1, controle->vetor_rotas[i].abreviacao_cidade);
				}
					
				do{		
					printf("\n\t\t\t\t\t\t\tOrigem:");
					scanf("%d", &de);
				}while(!((de >= 1) && (de <= controle->rotas->nro_vertices)));

				do{
					printf("\n\t\t\t\t\t\t\tDestino:");
					scanf("%d", &para);
				}while(!((para >= 1) && (para <= controle->rotas->nro_vertices)));

				if(de == para){		
					
					LIMPA
					printf("\t\t\t\t\t\t\tNao existe voo para a cidade que voce ja esta ¬¬!\n");
					PAUSA

					break;
				}

				trajeto = caminho_de_para(controle->voos, &controle->voos->arestas[de-1][de-1], &controle->voos->arestas[para-1][para-1], controle->matriz_bi, &duracao);

				if(trajeto[2] == -1){
					strcpy(trajeto1, controle->voos->arestas[trajeto[0]][trajeto[0]].abreviacao_cidade);
					strcpy(trajeto2, controle->voos->arestas[trajeto[1]][trajeto[1]].abreviacao_cidade);
				}else{
					strcpy(trajeto1, controle->voos->arestas[trajeto[0]][trajeto[0]].abreviacao_cidade);
					strcpy(trajeto2, controle->voos->arestas[trajeto[1]][trajeto[1]].abreviacao_cidade);
					strcpy(trajeto3, controle->voos->arestas[trajeto[2]][trajeto[2]].abreviacao_cidade);
				}
					

				for (int i = 0; i < 10; i++){
					
					LIMPA

					if(trajeto[2] != -1){
						for(int j = 0; j <= i ; j++){printf("\t");}printf("_________________________           _____\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|    %s--->%s--->%s   \\          \\   \\__      _____\n",trajeto1, trajeto2, trajeto3);
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|                         \\__________\\   \\/_______\\___\\_____________\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|      Duracao:%d         /         < /_/ JP .................... o`-.\n", duracao);
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|________________________/            `-----------,----,--------------'\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("                                                 /____/\n");
						sleep(1);
					}else{
						for(int j = 0; j <= i ; j++){printf("\t");}printf("_________________________           _____\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|      %s----->%s      \\          \\   \\__      _____\n",trajeto1, trajeto2);
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|                         \\__________\\   \\/_______\\___\\_____________\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|      Duracao:%d         /         < /_/ JP .................... o`-.\n", duracao);
						for(int j = 0; j <= i ; j++){printf("\t");}printf("|________________________/            `-----------,----,--------------'\n");
						for(int j = 0; j <= i ; j++){printf("\t");}printf("                                                 /____/\n");
						sleep(1);
					}

				}

				PAUSA

			break;

			/*[5]-A partir de um aerporto, determinar se e possivel atingir outro;*/	
			case 5:				
				
				LIMPA

				printf("\n\n\n\n\n\n\n\n");
				printf("\t\t\t\t\t\t\tEscolha um ID de um aeroporto de destino e um de origem:\n");
				for(int i = 0 ; i < controle->voos->nro_vertices; i++){
					printf("\t\t\t\t\t\t\t[%d]-%s\n", i+1, controle->vetor_rotas[i].abreviacao_cidade);
				}

				do{		
					printf("\n\t\t\t\t\t\t\tOrigem:");
					scanf("%d", &de);
				}while(!((de >= 1) && (de <= controle->rotas->nro_vertices)));

				do{
					printf("\n\t\t\t\t\t\t\tDestino:");
					scanf("%d", &para);
				}while(!((para >= 1) && (para <= controle->rotas->nro_vertices)));
		
				vetor_trajeto = aeroportos_Fora_De_Servico(controle->voos, &controle->voos->arestas[de - 1][de - 1], &controle->voos->arestas[para - 1][para - 1], &tam_vetor_trajeto);
			 	printf("\n\t\t\t\t\t\t\tVoo de: [%s], para: [%s].",controle->voos->arestas[de - 1][de - 1].abreviacao_cidade, controle->voos->arestas[para - 1][para - 1].abreviacao_cidade);
			 	printf("\n\t\t\t\t\t\t\tVoos Alternativas: [");
			 	for (int i = 0; i < tam_vetor_trajeto; i++){

					if(vetor_trajeto[i] >= 0){
						if((i+1) < tam_vetor_trajeto){
							printf("%s, ", controle->voos->arestas[vetor_trajeto[i]][vetor_trajeto[i]].abreviacao_cidade);
						}else{
							printf("%s", controle->voos->arestas[vetor_trajeto[i]][vetor_trajeto[i]].abreviacao_cidade);
						}
					}		
				}
				printf("]\n");

				PAUSA				

			break;

			/*[6]-Definir um aeroporto de partida, e passar por todos os outros aeroportos;*/
			case 6:
				
				LIMPA

				printf("\n\n\n\n\n\n\n\n");
				printf("\t\t\t\t\t\t\tEscolha um ID de um aeroporto origem:\n");

				for(int i = 0 ; i < controle->rotas->nro_vertices; i++){
					printf("\t\t\t\t\t\t\t[%d]-%s\n", i+1, controle->vetor_rotas[i].abreviacao_cidade);
				}
					
				do{			
					printf("\n\t\t\t\t\t\t\tOrigem:");
					scanf("%d", &de);
				}while(!((de >= 1) && (de <= controle->rotas->nro_vertices)));

				setbuf(stdin, NULL);

				matriz = algoritmo_de_prim(controle->rotas->nro_vertices, controle->matriz_bi, de);
				
				Prim = cria_Grafo(controle->rotas->nro_vertices, 0, 0);
				Prim->matriz_Rotas = matriz;
				Prim->arestas      = controle->rotas->arestas;

				cria_Prim(Prim, matriz, de);

				system("dot -Tpng prim.dot -o prim.png");
				system("display prim.png");

				PAUSA

			break;

			/*[7]-Encerrar o programa;*/
			case 7:
				
				LIMPA
				loop = false;
				PAUSA

			break;

			/*Caso o usuario tenha digitado a opcao incorreta*/
			default:
				
				LIMPA
				system("xcowsay Opcao Invalida, por favor digite outro numero\n");
				PAUSA

			break;
		}

	}

	/*Libera todas as sub-estruturas e estrutura da memoria*/
	controle = libera_Controle(controle);

	return(0);
}