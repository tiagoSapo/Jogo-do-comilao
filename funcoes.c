#include <stdio.h>
#include "funcoes.h"

void get_linhas_colunas(int autom, int *linhas, int *colunas) {
	int linhasf, colunasf;
	char resposta0, resposta1;

	do {

		do {

			printf("\nNumero de linhas (4 a 8): ");
			scanf("%d", &linhasf);

			if (linhasf > 3 && linhasf < 9)
				break;
			else
				printf("Valores invalidos!\n");

		} while (1);

		do {

			printf("\nNumero de colunas (6 a 10): ");
			scanf("%d", &colunasf);

			if (colunasf > 5 && colunasf < 11)
				break;
			else
				printf("Valores invalidos!\n");

		} while (1);

		printf("\nJogador A concorda? (s - sim / outra tecla - nao): ");
		scanf(" %c", &resposta0);

		if (autom == 1) {
			break;
		}
		printf("Jogador B concorda? (s - sim / outra tecla - nao): ");
		scanf(" %c", &resposta1);

		if (resposta0 == 's' && resposta1 == 's')
			break;
		else
			printf("Os jogadores nao estao de acordo!\n");

	} while (1);

	*linhas = linhasf;
	*colunas = colunasf;
}
