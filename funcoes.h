
typedef struct estrutura {

	int coluna_e;
	int linha_e;

	int tam_colunas;
	int tam_linhas;

	int jogador_no_momento;

	struct estrutura * proximo;

}estrutura;

typedef struct tam_alterado {

	int tam_alterado_a;
	int tam_alterado_b;

}tam_alterado;

void get_linhas_colunas(int autom, int *linhas, int *colunas);