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

#include <stdio.h> // uso: inputs e outputs.
#include <string.h> // uso: strcmp();
#include <ctype.h> // uso: tolower();
#include <stdbool.h>

#define PRECISAO 1000

int inicializar_valor_inteiro(char *);
void receber_tipo_problema(char *);
void inicializar_objetivo(double *, char *, int, int);
void inicializar_matriz_problema(int, int, double[][*], int, int, double *);
void print_matriz(int, int, double[][*], double *, double *, double *, int *);

void simplex(int, int, double[][*], double *, double *, double *, int *);
void tornar_viavel(int, int, double[][*], double *, double *, double *, int *);
void otimizar_resultado(int, int, double[][*], double *, double *, double *, int *);
void pivotamento(int, int, double[][*], double *, double *, double *, int *, int, int);

int main()
{
	int qntdd_var, qntdd_rest, larg_matriz, comp_matriz, i; // quantidade de variáveis e quantidade de restrições.
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
	int indices[comp_matriz];
	for(i = 0; i < comp_matriz; i++) indices[i] = i;

	print_matriz(larg_matriz, comp_matriz, matriz_problema, solucoes, &solucao_objetivo, objetivos, indices);

	simplex(larg_matriz, comp_matriz, matriz_problema, solucoes, &solucao_objetivo, objetivos, indices);

	print_matriz(larg_matriz, comp_matriz, matriz_problema, solucoes, &solucao_objetivo, objetivos, indices);

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
				printf("Coeficiente da variavel x%d: ", j);
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

		/* Input do termo independente */
		printf("Termo independente: ");
		while (scanf("%lf", &solucoes[i]) != 1) {
			while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
			printf("Termo independente: ");
		}
		while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.

		if(strcmp(tipo_restricao, "==") == 0) {
			//TODO
		} else if (strcmp(tipo_restricao, ">=") == 0) {
			for (j = 0; j < qntdd_var; j++)
			{
				matriz_problema[i][j] *= -1;
			}
			solucoes[i] *= -1;
		}
	}
}

void print_matriz(int larg_matriz, int comp_matriz, double matriz[][comp_matriz], double * solucoes, double * solucao_objetivo, double * objetivo, int * indices)
{
	int i, j;

	printf("Base\t");
	for (i = 0; i < comp_matriz; i++) {
		printf("x%d\t", indices[i]);
	}
	printf("Solucao\n");

	printf("z\t");
	for (i = 0; i < comp_matriz; i++) {
		printf("%.2lf\t", objetivo[i]);
	}
	printf("%.2lf\n", (*solucao_objetivo));

	for (i = 0; i < larg_matriz; i++) {
		printf("x%d\t", comp_matriz - larg_matriz + i);
		for (j = 0; j < comp_matriz; j++) {
			printf("%.2lf\t", matriz[i][j]);
		}
		printf("%.2lf\n", solucoes[i]);
	}
	printf("\n\n");
}

void simplex(int larg_matriz, int comp_matriz, double matriz[][comp_matriz], double * solucoes, double * solucao_objetivo, double * objetivo, int * indices)
{
    bool is_solved = false;
    bool continuar;
    int i;
    while(!is_solved) {
        continuar = true;
        for(i = 0; i < larg_matriz; i++){
            if(solucoes[i] < 0) {
                tornar_viavel(larg_matriz, comp_matriz, matriz, solucoes, solucao_objetivo, objetivo, indices);
                continuar = false;
                break;
            }
        }
        if(!continuar){
            continue;
        }
        for(i = 0; i < larg_matriz; i++){
            if(objetivo[i] < 0) {
                otimizar_resultado(larg_matriz, comp_matriz, matriz, solucoes, solucao_objetivo, objetivo, indices);
                continuar = false;
                break;
            }
        }
        if(!continuar){
            continue;
        }
        is_solved = true;
    }
}

void tornar_viavel(int larg_matriz, int comp_matriz, double matriz[][comp_matriz], double * solucoes, double * solucao_objetivo, double * objetivo, int * indices) {
    int i;
    int menor_larg = 0;
    for(i = 1; i < larg_matriz; i++) {
        if(solucoes[i] < solucoes[menor_larg]) menor_larg = i;
    }
    printf("menor_larg = %d\n", menor_larg);
    int menor_comp = -1;
    for(i = 0; i < comp_matriz; i++) {
        if(matriz[menor_larg][i] != 0 && objetivo[i] / matriz[menor_larg][i] > 0) {
            if(menor_comp == -1) {
                menor_comp = i;
            } else if(objetivo[i] / matriz[i][menor_comp] < objetivo[menor_comp] / matriz[menor_larg][menor_comp]) {
                menor_comp = i;
            }
        }
    }

    pivotamento(larg_matriz, comp_matriz, matriz, solucoes, solucao_objetivo, objetivo, indices, menor_larg, menor_comp);
}

void otimizar_resultado(int larg_matriz, int comp_matriz, double matriz[][comp_matriz], double * solucoes, double * solucao_objetivo, double * objetivo, int * indices) {
    int i;
    int menor_comp = 0;
    for(i = 1; i < comp_matriz; i++) {
        if(objetivo[i] < objetivo[menor_comp]) menor_comp = i;
    }

    int menor_larg = -1;
    for(i = 0; i < larg_matriz; i++) {
        if(matriz[i][menor_comp] != 0 && solucoes[i] / matriz[i][menor_comp] > 0) {
            if(menor_larg == -1) {
                menor_larg = i;
            } else if(solucoes[i] / matriz[i][menor_comp] < solucoes[menor_larg] / matriz[menor_larg][menor_comp]) {
                menor_larg = i;
            }
        }
    }

    pivotamento(larg_matriz, comp_matriz, matriz, solucoes, solucao_objetivo, objetivo, indices, menor_larg, menor_comp);
}

void pivotamento(int larg_matriz, int comp_matriz, double matriz[][comp_matriz], double * solucoes, double * solucao_objetivo, double * objetivo, int * indices, int x, int y) {
    printf("Interacao %d: sai x%d, entra x%d\n", 0, x, y);
    double pivo = matriz[x][y];
    int i, j;
    for(i = 0; i < comp_matriz; i++) {
        matriz[x][i] /= pivo;
    }
    solucoes[x] /= pivo;

    double multiplicar = objetivo[y];

    for(i = 0; i < comp_matriz; i++) {
        objetivo[i] += multiplicar * matriz[x][i] * (-1);

    }
    (*solucao_objetivo) += multiplicar * solucoes[x] * (-1);

    for(i = 0; i < larg_matriz; i++) {
        if(x == i) continue;
        multiplicar = matriz[i][y];
        for(j = 0; j < comp_matriz; j++) {
            matriz[i][j] += multiplicar * matriz[x][j] * (-1);
        }
        solucoes[i] += multiplicar * solucoes[x] * (-1);
    }
	print_matriz(larg_matriz, comp_matriz, matriz, solucoes, solucao_objetivo, objetivo, indices);
}
