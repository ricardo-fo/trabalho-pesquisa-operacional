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

int inicializar_valor_inteiro(char *);
void receber_tipo_problema(char *);
void inicializar_objetivo(double *, char *, int, int);
void inicializar_matriz_problema(int, int, double[][*], int, int, double *);
void print_matriz(int, int, double[][*], double *, double, double *);

int main()
{
	int qntdd_var, qntdd_rest, larg_matriz, comp_matriz; // quantidade de variáveis e quantidade de restrições.
	char problema[4];

	qntdd_var = inicializar_valor_inteiro("variaveis");
	qntdd_rest = inicializar_valor_inteiro("restricoes");

	receber_tipo_problema(problema);

	larg_matriz = qntdd_rest;
	comp_matriz = qntdd_rest + qntdd_var;

	double objetivos[comp_matriz];
	inicializar_objetivo(objetivos, problema, qntdd_var, qntdd_rest);

	double matriz_problema[larg_matriz][comp_matriz];
	double solucoes[qntdd_rest];
	double solucao_objetivo = 0.0;
	inicializar_matriz_problema(larg_matriz, comp_matriz, matriz_problema, qntdd_var, qntdd_rest, solucoes);

	print_matriz(larg_matriz, comp_matriz, matriz_problema, solucoes, solucao_objetivo, objetivos);

	return 0;
}

int inicializar_valor_inteiro(char * msg) {
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

void receber_tipo_problema(char * problema) {
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

void inicializar_objetivo(double * objetivo, char * problema, int qntdd_var, int qntdd_rest) {
	int i;
	char buffer;
	/* Input da função objetivo */
	printf("\nFuncao objetivo\n");
	for (i = 0; i < qntdd_var; i++) {
		printf("Coeficiente da variavel x%d: ", i);
		while (scanf("%lf", &objetivo[i]) != 1) {
			while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
			printf("Coeficiente da variavel x%d: ", i);
		}
		while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.

		if(strcmp(problema, "max") == 0) {
			objetivo[i] *= -1;
		}
	}

	for (i = 0; i < qntdd_rest; i++) {
		objetivo[i + qntdd_var] = 0;
	}
}

void inicializar_matriz_problema(int l, int c, double matriz_problema[][c], int qntdd_var, int qntdd_rest, double * solucoes) {
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
			while (scanf("%lf", &matriz_problema[i][j]) != 1) {
				while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
				fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
				printf("Coeficiente da variavel x%d: ", i);
			}
			while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
		}
		
		for (j = qntdd_var; j < c; j++) {
			if (qntdd_var + i == j) {
				matriz_problema[i][j] = 1;
			}
			else {
				matriz_problema[i][j] = 0;
			}
		}
		
		char tipo_restricao[3];
		/* Input do tipo de restricao */
		do {
			printf("\nOpcoes de restricao:\n==\n<=\n>=\n");
			printf("Tipo de restricao: ");
			scanf(" %3s", tipo_restricao);
			while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
		} while (strcmp(tipo_restricao, "==") != 0 && strcmp(tipo_restricao, "<=") != 0 && strcmp(tipo_restricao, ">=") != 0);

		if(strcmp(tipo_restricao, "==") == 0) {
			//TODO
		} else if (strcmp(tipo_restricao, ">=") == 0) {
			for (j = 0; j < qntdd_var; j++)
			{
				matriz_problema[i][j] *= -1;
			}
		}
		
		/* Input do termo independente */
		printf("Termo independente: ");
		while (scanf("%lf", &solucoes[i]) != 1) {
			while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
			printf("Termo independente: ");
		}
		while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
	}
}

void print_matriz(int larg_matriz, int comp_matriz, double matriz[][comp_matriz], double * solucoes, double solucao_objetivo, double * objetivo)
{
	int i, j;

	printf("Base\t");
	for (i = 0; i < comp_matriz; i++) {
		printf("x%d\t", i);
	}
	printf("Solucao\n");

	printf("z\t");
	for (i = 0; i < comp_matriz; i++) {
		printf("%.2lf\t", objetivo[i]);
	}
	printf("%.2lf\n", solucao_objetivo);

	for (i = 0; i < larg_matriz; i++) {
		printf("x%d\t", comp_matriz - larg_matriz + i);
		for (j = 0; j < comp_matriz; j++) {
			printf("%.2lf\t", matriz[i][j]);
		}
		printf("%.2lf\n", solucoes[i]);
	}
}