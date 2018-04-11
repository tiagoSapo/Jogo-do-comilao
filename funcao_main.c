#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "funcoes.h"
#include "mostrar.h"

int main() {

	FILE *f, *file_save, *file_open;

	int linhas, colunas, celulas;
	int i, j; /* iterações dos ciclos for */
	int jogada, linha_escolhida, indice_coluna, coluna_escolhida; /* linha/coluna_escolhida -> linha/coluna escolhida pelo jogador */
	int **p, **temp;
	
	int jogador = 0;	/* saber qual é o jogador (0 -> Jogador A | 1 -> Jogador B) */
	int flag = 0;		/* saber se a celula já foi comida*/
	int guardado = 0;   /* saber se o jogo foi guardado ou não (0 -> não / 1 -> sim) */
	int indice_linha = 1;	/* indice da linha = linha + 1 de forma ao mapa começar a partir do 1 e não a partir do 0 */
	int posicao_estrutura = 1; /* quantas estruturas já foram preenchidas */
	
	int save_arr[4]; /* guarda valores do jogo para ser retomado mais tarde */
	int fwrite_arr[5];
	int tamanho_arr[2]; /* saber se o jogador ja aumentou o tamanho */

	char answer;
	int resposta_auto, autom; /* Jogo automatico*/

	estrutura lista[100];
	estrutura *pe = & lista[0]; /* pe -> ponteiro para estrutura */

	//FILES
	/* Abre ficheiro para exportar sucessões -> texto .txt */
	f = fopen("estados.txt", "wt");	
	if (f == NULL) {
		printf("\nERRO: Nao foi possivel criar o ficheiro 'estados.txt'!\n");
		return 1;
	}
		
	/*Mostra mensagem de abertura do jogo*/
	mostra_intro();

	/* Pergunta se continua jogo anterior */
	printf("Pretende continuar o jogo anterior? (s - sim / outra tecla - nao): ");
	scanf(" %c", &answer);

	if (answer == 's') {
		
		file_open = fopen("save.bin", "rb");

		if (file_open == NULL) {
			printf("\nERRO: O ficheiro com a continuacao do jogo ('save.bin') nao foi encontrado!\nA iniciar novo jogo...\n");
			Sleep(1000);
		}
		else
		{
			fread(&resposta_auto, sizeof(int), 1, file_open);
			fread(save_arr, sizeof(save_arr), 4, file_open);
			
			linhas = save_arr[0];
			colunas = save_arr[1];
			jogador = save_arr[2];
			posicao_estrutura = save_arr[3];
			
			for (i = 0; i < 100; i++) {
				
				fread(fwrite_arr, sizeof(fwrite_arr), 5, file_open);

				lista[i].coluna_e = fwrite_arr[0];
				lista[i].linha_e = fwrite_arr[1];

				lista[i].tam_colunas = fwrite_arr[2];
				lista[i].tam_linhas = fwrite_arr[3];

				lista[i].jogador_no_momento = fwrite_arr[4];
				
				if (i != 99) {
					lista[i].proximo = &lista[i + 1];
				}
				else
				{
					lista[i].proximo = 0;
				}				
			}
			fread(tamanho_arr, sizeof(tamanho_arr), 2, file_open);			
			
			guardado = 1;
		}
	}
	
	if (guardado == 0) {

		/* Jogo automático */
		printf("\nCom quem queres jogar?\n1 - Jogar com um amigo;\n2 - Jogar com a CPU.\n");

		do
		{
			printf("-> ");
			scanf("%d", &resposta_auto);

			if (resposta_auto == 1) {
				printf("\nA comecar um jogo manual ...\n");
				Sleep(1000);
				break;
			}
			else if (resposta_auto == 2) {
				printf("\nA comecar um jogo com a CPU ...\n");
				Sleep(1000);
				autom = 1;
				break;
			}
			else
			{
				printf("ERRO: O valor %d e invalido!\n", resposta_auto);
			}

		} while (1);

		/* Recebe o numero de linhas e colunas */
		get_linhas_colunas(autom, &linhas, &colunas);

		/* Preenche listas a zero */
		tamanho_arr[0] = 0;
		tamanho_arr[1] = 0;

		for (i = 0; i < 100; i++) {
			lista[i].coluna_e = 0;
			lista[i].linha_e = 0;

			lista[i].tam_colunas = colunas;
			lista[i].tam_linhas = linhas;

			lista[i].jogador_no_momento = 0;

			if (i != 99) {
				lista[i].proximo = &lista[i + 1];
			}
			else
			{
				lista[i].proximo = 0;
			}

		}
	}
	/* Alocar memoria dinâmica (malloc) */
	p = malloc(linhas * sizeof(int *));
	for (i = 0; i < linhas; i++) {
		p[i] = malloc(colunas * sizeof(int));
	}
	celulas = linhas * colunas;
			
	if (guardado == 0) {
		printf("\nAVISO: Foi criada uma tabela com %d linhas e %d colunas!\n\n", linhas, colunas);

		/* Preenche o array 2d a zeros  */
		for (i = 0; i < linhas; i++) {
			for (j = 0; j < colunas; j++) {
				p[i][j] = 0;
			}
		}
		p[linhas - 1][colunas - 1] = 2; /* Celula com veneno (0 - preenchida (*), 1 - vazia (-), 2 - veneno (x))*/

	}
	else
	{
		for (i = 0; i < linhas; i++) {
			fread(p[i], sizeof(int), colunas, file_open);
		}
		
		fclose(file_open);

	}

	/* Jogo começa aqui */
	do {

		printf("- "); fprintf(f, "- ");

		for (indice_coluna = 1; indice_coluna < colunas + 1; indice_coluna++) {
			printf(" %d", indice_coluna); fprintf(f, " %d", indice_coluna);
		}

		indice_coluna--;
		printf("\n"); fprintf(f, "\n");
		
		for (indice_linha = 1, i = 0; i < linhas; i++) {

			printf("%d| ", indice_linha); fprintf(f, "%d| ", indice_linha);
			indice_linha++;

			for (j = 0; j < colunas; j++) {

				if (p[i][j] == 0) {
					printf("* "); fprintf(f, "* ");
				}
				else if (p[i][j] == 1) {
					printf("- "); fprintf(f, "- ");
				}
				else {
					printf("x"); fprintf(f, "x ");
				}
			}
			printf("\n"); fprintf(f, "\n");
		}
		indice_linha--;
		fprintf(f, "\n");		

		if (jogador == 0) {
			/* Mostra as instruções */
			mostra_manual();

			printf("Jogador A -> ");
			scanf("%d", &jogada);
			printf("\n");
		}
		else
		{
			if (resposta_auto == 2) {
				jogada = 1;
			}
			else
			{
				/* Mostra as instruções */
				mostra_manual();

				printf("Jogador B -> ");
				scanf("%d", &jogada);
				printf("\n");
			}
		}

		if (jogada == 1) {

			if (jogador == 2) {

				do {
					linha_escolhida = (rand() % linhas) + 1;
					coluna_escolhida = (rand() % colunas) + 1;

					if (p[linha_escolhida - 1][coluna_escolhida - 1] != 1) {
						break;
					}

				} while (1);

				printf("\nA CPU esta a pensar...\n");
				Sleep(3000);
				printf("\n\n");

				jogador = 2;
			}
			else
			{

				do {
					printf("Coluna = ");
					scanf("%d", &coluna_escolhida);

					if (coluna_escolhida >= 1 && coluna_escolhida <= indice_coluna)
						break;
					else
						printf("ERRO: Valor da coluna invalido! (Apenas valores entre 1 e %d)\n", indice_coluna);

				} while (1);

				do {

					printf("Linha = ");
					scanf("%d", &linha_escolhida);

					if (linha_escolhida >= 1 && linha_escolhida <= indice_linha)
						break;
					else
						printf("ERRO: Valor da linha invalido! (Apenas valores entre 1 e %d)\n", indice_linha);

				} while (1);
			}

			/* Cria historial do jogo */
			lista[posicao_estrutura].jogador_no_momento = jogador;
			lista[posicao_estrutura].coluna_e = coluna_escolhida;
			lista[posicao_estrutura].linha_e = linha_escolhida;
			posicao_estrutura++;

			printf("AVISO: Foi escolhida a posicao [%d][%d] do mapa.\n\n", linha_escolhida, coluna_escolhida);

			if (p[linha_escolhida - 1][coluna_escolhida - 1] == 1) {
				printf("\nERRO: Jogada Invalida -> Celula ja foi comida!\n\n");
				flag = 1;
			}

			for (i = 0; i < linha_escolhida; i++) {

				for (j = 0; j < coluna_escolhida; j++) {
					p[i][j] = 1;
				}

			}

			/* Se apanhou o veneno o jogador perde */
			if (p[linhas - 1][colunas - 1] == 1) {
				
				if (jogador == 0) {
					printf("AVISO: O jogador A apanhou veneno!\n"); fprintf(f, "\nAVISO: O jogador A apanhou veneno!\n");

					if (resposta_auto == 2) {
						printf("O jogador A perdeu!\n"); fprintf(f, "O jogador A perdeu!\n");
						printf("\n\nO jogador CPU ganhou.\n\n"); fprintf(f, "\n\nO jogador B ganhou.\n\n");
					}
					else
					{
						printf("O jogador A perdeu!\n"); fprintf(f, "O jogador A perdeu!\n");
						printf("\n\nO jogador B ganhou, PARABENS!!!\n\n"); fprintf(f, "\n\nO jogador B ganhou, PARABENS!!!\n\n");
					}					
					return 0;
				}
				else if(jogador == 2)
				{
					printf("AVISO: O jogador CPU apanhou veneno!\n"); fprintf(f, "\nAVISO: O jogador CPU apanhou veneno!\n");
					printf("O jogador CPU perdeu!\n"); fprintf(f, "O jogador CPU perdeu!\n");
					printf("\n\nO jogador A ganhou, PARABENS!!!\n\n"); fprintf(f, "\n\nO jogador A ganhou, PARABENS!!!\n\n");
					return 0;
				}
				else
				{
					printf("AVISO: O jogador B apanhou veneno!\n"); fprintf(f, "\nAVISO: O jogador B apanhou veneno!\n");
					printf("O jogador B perdeu!\n"); fprintf(f, "O jogador B perdeu!\n");
					printf("\n\nO jogador A ganhou, PARABENS!!!\n\n"); fprintf(f, "\n\nO jogador A ganhou, PARABENS!!!\n\n");
					return 0;
				}

			}

			/* Flag = celula já foi comida */
			if (flag == 0) {

				if (jogador == 0) {

					if (resposta_auto == 2) {
						jogador = 2;
					}
					else
					{
						jogador = 1;
					}
				}
				else {
					jogador = 0;
				}

			}
			else
			{
				flag = 0;
			}
				
			fprintf(f, "\nJa foram feitas estas jogadas:\n");

			for (i = 0; i < posicao_estrutura; i++) {

				if (lista[i].jogador_no_momento == 0) {
					fprintf(f, "-> A[%d][%d] ", lista[i].linha_e, lista[i].coluna_e);
				}
				else if (lista[i].jogador_no_momento == 2) {
					fprintf(f, "-> CPU[%d][%d] ", lista[i].linha_e, lista[i].coluna_e);
				}
				else
				{
					fprintf(f, "-> B[%d][%d] ", lista[i].linha_e, lista[i].coluna_e);
				}

			}
			fprintf(f, "\n\n");
		}
		else if (jogada == 2) {

			if (jogador == 0) {
				
				if (tamanho_arr[0] == 0) {
					printf("\nAVISO: O jogador A aumentou o tabuleiro!\n\n");

					temp = malloc((linhas + 1) * sizeof(int *));
					for (i = 0; i < (linhas + 1); i++) {
						temp[i] = malloc((colunas + 1) * sizeof(int));
					}

					/* TEMP a zeros */
					for (i = 0; i < linhas + 1; i++) {
						for (j = 0; j < colunas + 1; j++) {
							temp[i][j] = 0;
						}
					}

					/* TEMP fica com valores de P */
					for (i = 0; i < linhas; i++) {
						for (j = 0; j < colunas; j++) {
							temp[i][j] = p[i][j];
							if (temp[i][j] == 2) {
								temp[i][j] = 0;
							}
						}
					}

					temp[linhas][colunas] = 2; /*2 na ultima linhas coluna*/

					for (i = 0; i < linhas; i++)
						p[i] = temp[i][0];

					for (i = 0; i < linhas; i++) {
						p[i] = temp[i];
					}
					p = temp;

					linhas++; colunas++; indice_coluna++; indice_linha++;
					tamanho_arr[0] = 1; /* indica que este jogador já aumentou o tamanho do array */
					jogador = 1; /* É a vez do outro jogador */
										
				}
				else
				{
					printf("\nERRO: O jogador A ja aumentou o tabuleiro!\n\n");					
				}
			}

			else
			{
				if (tamanho_arr[1] == 0) {
					printf("\nAVISO: O jogador B aumentou o tabuleiro!\n\n");

					temp = malloc((linhas + 1) * sizeof(int *));
					for (i = 0; i < (linhas + 1); i++) {
						temp[i] = malloc((colunas + 1) * sizeof(int));
					}

					/* TEMP a zeros */
					for (i = 0; i < linhas + 1; i++) {
						for (j = 0; j < colunas + 1; j++) {
							temp[i][j] = 0;
						}
					}

					/* TEMP fica com valores de P */
					for (i = 0; i < linhas; i++) {
						for (j = 0; j < colunas; j++) {
							temp[i][j] = p[i][j];
							if (temp[i][j] == 2) {
								temp[i][j] = 0;
							}
						}
					}

					temp[linhas][colunas] = 2; /*2 na ultima linhas coluna*/

					for (i = 0; i < linhas; i++)
						p[i] = temp[i][0];

					for (i = 0; i < linhas; i++) {
						p[i] = temp[i];
					}
					p = temp;

					linhas++; colunas++; indice_coluna++; indice_linha++;
					tamanho_arr[1] = 1; /* indica que este jogador já aumentou o tamanho do array */
					jogador = 0; /* É a vez do outro jogador */
				}
				else
				{
					printf("\nERRO: O jogador B ja aumentou o tabuleiro!\n\n");
					jogador = 1;
				}

				jogada = 0;
			}

		}
		else if (jogada == 3) {
			printf("\nAVISO: O jogo foi guardado!\n");

			/* Abre ficheiro para guardar jogo a fim de ser retomado mais tarde -> binario .bin */
			file_save = fopen("save.bin", "wb");
			if (file_save == NULL) {
				printf("Erro: Nao foi possivel criar o ficheiro 'save.bin' para guardar o jogo!\n");
				return 1;
			}
			fwrite(&resposta_auto, sizeof(int), 1, file_save);

			save_arr[0] = linhas;
			save_arr[1] = colunas;
			save_arr[2] = jogador;
			save_arr[3] = posicao_estrutura;
						
			fwrite(save_arr, sizeof(save_arr), 4, file_save);

			for (i = 0; i < 100; i++) {
				
				fwrite_arr[0] = lista[i].coluna_e;
				fwrite_arr[1] = lista[i].linha_e;

				fwrite_arr[2] = lista[i].tam_colunas;
				fwrite_arr[3] = lista[i].tam_linhas;

				fwrite_arr[4] = lista[i].jogador_no_momento;

				fwrite(fwrite_arr, sizeof(fwrite_arr), 5, file_save);
			}

			fwrite(tamanho_arr, sizeof(tamanho_arr), 2, file_save);

			/* Escreve as celulas do mapa */
			for (i = 0; i < linhas; i++) {
				fwrite(p[i], sizeof(int), colunas, file_save);
			}			
			
			fclose(file_save);
			return 0;
		}
		else if (jogada == 4) {
			
			printf("\nJa foram feitas estas jogadas:\n");

			for (i = 0; i < posicao_estrutura; i++) {

				if (lista[i].jogador_no_momento == 0) {
					printf("-> A[%d][%d] ", lista[i].linha_e, lista[i].coluna_e);
				}
				else if (lista[i].jogador_no_momento == 2) {
					printf("-> CPU[%d][%d] ", lista[i].linha_e, lista[i].coluna_e);
				}
				else
				{
					printf("-> B[%d][%d] ", lista[i].linha_e, lista[i].coluna_e);
				}

			}
			printf("\n");
		}
		else
		{
			printf("ERRO: O valor %d e invalido!\n\n", jogada);
		}
				
	} while (1);

	fclose(f);

	free(temp);
	free(p);
	free(pe);
	free(lista);
	return 0;
}
