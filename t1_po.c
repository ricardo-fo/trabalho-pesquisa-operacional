/*
 * 1º Trabalho de Pesquisa Operacional.
 * Integrantes do grupo:
 * Nome: Alexandre Saura.
 * Nome: Fábio Thomaz.
 * Nome: Ricardo Oliveira.
 * Profº.: José Fontebasso Neto.
 * Disciplina: Pesquisa Operacional - I.
 * Universidade  Católica de Santos - Maio de 2019.
*/
/*
PASSOS:
- Inputs:
1 - Ler a quantidade de variáveis;
2 - Ler a quantidade de restrições;
3 - Ler o tipo de problema;
4 - Ler o(s) coeficiente(s) da função objetivo;
5 - Ler o(s) coeficiente(s) da(s) variável(is) da(s) restrição(ões);
6 - Ler o tipo de restrição;
7 - Ler o termo independente da(s) restrição(ões);

- Processamentos:
1 - Verificar se algum dado precisa ter seu sinal invertido;
2 - Colocar os dados dos inputs numa matriz;
3 - Verificar se a matriz é viável;
	3.1 - Se sim, vá para o passo 4;
	3.2 - Se não for, então deixe viável e prossiga para o passo 4;
4 - Otimize a matriz;

*/
#include <stdio.h> // uso: inputs e outputs.
#include <string.h> // uso: strcmp();
#include <ctype.h> // uso: tolower();

typedef struct {
	double coeficientes[20];
	char tipo_rest[3];
	double termo_ind;
} Restricao;

int inicializar_valor(char *);
void tipo_problema(char * problema);
void inicializar_objetivo(double *, int);
void inicializar_restricoes(int, int, Restricao *);
void zera_matriz(int, int, double[][*]);
void inicializar_matriz_problema(int, int, double[][*], Restricao *, int);
void print_matriz(int, int, double[][*]);
void inicializar_solucao(double *, int, Restricao *);

int main()
{
	int qntdd_var, qntdd_rest, l, c; // quantidade de variáveis e quantidade de restrições.
	char problema[4];

	qntdd_var = inicializar_valor("variaveis");
	qntdd_rest = inicializar_valor("restricoes");

	tipo_problema(problema);

	double funcao_objetivo[qntdd_var], auxiliar;
	inicializar_objetivo(funcao_objetivo, qntdd_var);

	Restricao restricoes[qntdd_rest];
	inicializar_restricoes(qntdd_rest, qntdd_var, restricoes);

	l = qntdd_rest;
	c = qntdd_rest + qntdd_var;

	double matriz_problema[l][c];
	zera_matriz(l, c, matriz_problema);
	print_matriz(l, c, matriz_problema);

	inicializar_matriz_problema(qntdd_rest, qntdd_rest + qntdd_var, matriz_problema, restricoes, qntdd_var);

	double solucoes[qntdd_rest];
	inicializar_solucao(solucoes, qntdd_rest, restricoes);

	return 0;
}

int inicializar_valor(char * msg) {
	char buffer;
	int valor;
	/* Input da quantidade de variáveis */
	do {
		printf("Informe a quantidade de %s (maximo 20): ", msg);
		if (scanf("%d", &valor) != 1)
		{
			while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero inteiro.\n\n");
			continue;
		}
	} while (valor <= 0 || valor > 20);

	return valor;
}

void tipo_problema(char * problema) {
	char buffer;
	int i;
	/* Input do tipo de problema */
	do {
		printf("\nTipo de problema (max/min): ");
		scanf(" %3[a-xA-X]", problema); // fomatação para leitura de apenas letras minusculas.
		while (getchar() != '\n'); // limpeza do buffer do teclado.

		for (i = 0; i < 4; i++) { // conversão para letras minusculas.
			problema[i] = tolower(problema[i]);
		}
	} while (strcmp(problema, "max") != 0 && strcmp(problema, "min") != 0);
}

void inicializar_objetivo(double * funcao_objetivo, int qntdd_var) {
	int i;
	char buffer;
	/* Input da função objetivo */
	printf("\nFuncao objetivo\n");
	for (i = 0; i < qntdd_var; i++)
	{
		printf("Coeficiente da variavel x%d: ", i);
		while (scanf("%lf", &funcao_objetivo[i]) != 1) {
			while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
			printf("Coeficiente da variavel x%d: ", i);
		}
		while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
	}
}

void inicializar_restricoes(int qntdd_rest, int qntdd_var, Restricao * restricoes) {
	int i, j;
	char buffer;
	/* Input dos dados das restrições */
	for (i = 0; i < qntdd_rest; i++)
	{
		printf("\nRestricao %d:\n", i + 1);

		/* Input dos coeficientes da variavel*/
		for (j = 0; j < qntdd_var; j++)
		{
			printf("Coeficiente da variavel x%d: ", j);
			while (scanf("%lf", &restricoes[i].coeficientes[j]) != 1) {
				while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
				fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
				printf("Coeficiente da variavel x%d: ", i);
			}
			while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
		}
		/* Input do tipo de restricao */
		do {
			printf("\nOpcoes de restricao:\n==\n<=\n>=\n");
			printf("Tipo de restricao: ");
			scanf(" %3s", restricoes[i].tipo_rest);
			while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
		} while (strcmp(restricoes[i].tipo_rest, "==") != 0
			&& strcmp(restricoes[i].tipo_rest, "<=") != 0
			&& strcmp(restricoes[i].tipo_rest, ">=") != 0);
		/* Input do termo independente */
		printf("Termo independente: ");
		while (scanf("%lf", &restricoes[i].termo_ind) != 1) {
			while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
			printf("Termo independente: ");
		}
		while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
	}
}

void zera_matriz(int l, int c, double matriz[][c]) {
	int i, j;
	for (i = 0; i < l; i++)
	{
		for (j = 0; j < c; j++) {
			matriz[i][j] = 0;
		}
	}
}

void inicializar_matriz_problema(int l, int c, double matriz_problema[][c], Restricao * restricoes, int qntdd_var) {
	int i, j;
	for (i = 0; i < l; i++)
	{
		if (strcmp(restricoes[i].tipo_rest, "==") == 0) {
			return;
		}
		else if (strcmp(restricoes[i].tipo_rest, ">=") == 0) {
			for (j = 0; j < qntdd_var; j++) {
				matriz_problema[i][j] = restricoes[i].coeficientes[j] * (-1);
			}
			for (j = qntdd_var; j < c; j++) {
				if (qntdd_var + i == j) {
					matriz_problema[i][j] = 1;
				}
				else {
					matriz_problema[i][j] = 0;
				}
			}
		}
		else if (strcmp(restricoes[i].tipo_rest, "<=") == 0) {
			for (j = 0; j < qntdd_var; j++) {
				matriz_problema[i][j] = restricoes[i].coeficientes[j];
			}
			for (j = qntdd_var; j < c; j++) {
				if (qntdd_var + i == j) {
					matriz_problema[i][j] = 1;
				}
				else {
					matriz_problema[i][j] = 0;
				}
			}
		}
	}
}

void print_matriz(int l, int c, double matriz[][c])
{
	int i, j;
	for (i = 0; i < l; i++) {
		for (j = 0; j < c; j++) {
			printf("%lf ", matriz[i][j]);
		}
		printf("\n");
	}
}

void inicializar_solucao(double * solucoes, int qntdd_rest, Restricao * restricoes)
{
	int i;
	for (i = 0; i < qntdd_rest; i++) {
		if (strcmp(restricoes[i].tipo_rest, "==") == 0) {
			return;
		}
		else if (strcmp(restricoes[i].tipo_rest, ">=") == 0) {
			solucoes[i] = restricoes[i].termo_ind * (-1);
		}
		else if (strcmp(restricoes[i].tipo_rest, "<=") == 0) {
			solucoes[i] = restricoes[i].termo_ind;
		}
	}
}
