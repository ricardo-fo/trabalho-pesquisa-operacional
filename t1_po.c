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
#include <stdlib.h>
#include <stdbool.h>

#define PRECISAO 100000

int inicializar_valor_inteiro(char *);
void receber_tipo_problema(char *);
void inicializar_objetivo(double *, char *);
void inicializar_matriz(double[][*], double *, double *);
void print_matriz(double[][*], double *, double *, int *);
void print_solucao(double *, int *);

void simplex(double[][*], double *, double *, int *);
void tornar_viavel(double[][*], double *, double *, int *);
void otimizar_resultado(double[][*], double *, double *, int *);
void pivotamento(double[][*], double *, double *, int *, int, int);

void analizar_recursos(double[][*], double *, double *);
void analizar_objetivos(double[][*], double *, double *, int *);

int qntdd_var, qntdd_rest;
int larg_matriz, comp_matriz, larg_real, comp_real;
double solucao_objetivo;

int main()
{
	int i,j; // quantidade de variáveis e quantidade de restrições.
	char problema[4];

	qntdd_var = inicializar_valor_inteiro("variaveis");
	qntdd_rest = inicializar_valor_inteiro("restricoes");

	larg_matriz = qntdd_rest;
	comp_matriz = qntdd_rest + qntdd_var;

	larg_real = larg_matriz * 2;
	comp_real = comp_matriz + larg_matriz;

	double objetivos[comp_real];
	for(i = 0; i < comp_real; i++) objetivos[i] = 0;

	double solucoes[larg_real];
	for(i = 0; i < larg_real; i++) solucoes[i] = 0;

	double matriz[larg_real][comp_real];
	for(i = 0; i < larg_real; i++) {
		for(j = 0; j < comp_real; j++) matriz[i][j] = 0;
	}

	int indices[comp_real];
	for(i = 0; i < comp_real; i++) indices[i] = i + qntdd_var;

	double solucao_objetivo = 0;

	receber_tipo_problema(problema);

	inicializar_objetivo(objetivos, problema);

	inicializar_matriz(matriz, objetivos, solucoes);

	double objetivos_iniciais[comp_real];
	for(i = 0; i < comp_real; i++) objetivos_iniciais[i] = objetivos[i];

	double solucoes_iniciais[larg_real];
	for(i = 0; i < larg_real; i++) solucoes_iniciais[i] = solucoes[i];

	printf("\nQuadro inicial\n");
	print_matriz(matriz, objetivos, solucoes, indices);

	simplex(matriz, objetivos, solucoes, indices);

	print_solucao(solucoes, indices);

	printf("\nAnalise de Sensibilidade\n");
	analizar_recursos(matriz, solucoes, solucoes_iniciais);
	analizar_objetivos(matriz, objetivos, objetivos_iniciais, indices);

	return 0;
}

int inicializar_valor_inteiro(char * msg) {
	int valor;
	char buffer;
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
	int i;
	char buffer;
	/* Input do tipo de problema */
	printf("\n");
	do {
		printf("Tipo de problema (max/min): ");
		scanf(" %3[a-xA-X]", problema); // fomatação para leitura de apenas letras minusculas.
		while (getchar() != '\n'); // limpeza do buffer do teclado.

		for (i = 0; i < 4; i++) { // conversão para letras minusculas.
			problema[i] = tolower(problema[i]);
		}
	} while (strcmp(problema, "max") != 0 && strcmp(problema, "min") != 0);
}

void inicializar_objetivo(double * objetivos, char * problema) {
	int i;
	char buffer;
	/* Input da função objetivo */
	printf("\nFuncao objetivo\n");
	for (i = 0; i < qntdd_var; i++) {
		printf("Coeficiente da variavel x%d: ", i);
		while (scanf("%lf", &objetivos[i]) != 1) {
			while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
			printf("Coeficiente da variavel x%d: ", i);
		}
		while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.

		if(strcmp(problema, "max") == 0) {
			objetivos[i] *= -1;
			if(abs((long)(objetivos[i] * PRECISAO)) == 0) objetivos[i] = abs(objetivos[i]);
		}
	}
}

void inicializar_matriz(double matriz[larg_real][comp_real], double * objetivos, double * solucoes) {
	int i, j, k = 0;
	char buffer;
	bool igual_igual = false;
	char tipo_restricao[3];
	/* Input dos dados das restrições */
	for (i = 0; i < qntdd_rest; i++)
	{
		if(!igual_igual) {
			printf("\nRestricao %d:\n", i + 1 - k);

			/* Input dos coeficientes da variavel*/
			for (j = 0; j < qntdd_var; j++)
			{
				printf("Coeficiente da variavel x%d: ", j);
				while (scanf("%lf", &matriz[i][j]) != 1) {
					while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
					fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
					printf("Coeficiente da variavel x%d: ", j);
				}
				while ((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			}

			/* Input do tipo de restricao */
			do {
				printf("Opcoes de restricao:\n==\n<=\n>=\n");
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
		}

		matriz[i][qntdd_var + i] = 1;

		if(!igual_igual && strcmp(tipo_restricao, "==") == 0) {
			for (j = 0; j < qntdd_var; j++) matriz[i + 1][j] = matriz[i][j];
			solucoes[i + 1] = solucoes[i];
			
			igual_igual = true;
			larg_matriz++;
			comp_matriz++;
			qntdd_rest++;
			k++;
		} else if (igual_igual || strcmp(tipo_restricao, ">=") == 0) {
			for (j = 0; j < qntdd_var; j++)
			{
				matriz[i][j] *= -1;
				if(abs((long)(matriz[i][j] * PRECISAO)) == 0) matriz[i][j] = abs(matriz[i][j]);
			}
			solucoes[i] *= -1;
			if(abs((long)(solucoes[i] * PRECISAO)) == 0) solucoes[i] = abs(solucoes[i]);
			
			igual_igual = false;
		}
	}
}

void print_matriz(double matriz[larg_matriz][comp_real], double * objetivos, double * solucoes, int * indices)
{
	int i, j;

	printf("Base\t");
	for (i = 0; i < comp_matriz; i++) printf("x%d\t", i);
	printf("Solucao\n");

	printf("z\t");
	for (i = 0; i < comp_matriz; i++) printf("%.2lf\t", objetivos[i]);
	printf("%.2lf\n", solucao_objetivo);

	for (i = 0; i < larg_matriz; i++) {
		printf("x%d\t", indices[i]);
		for (j = 0; j < comp_matriz; j++) printf("%.2lf\t", matriz[i][j]);
		printf("%.2lf\n", solucoes[i]);
	}
}

void print_solucao(double * solucoes, int * indices) {
	printf("Solucao: %.2lf\n", solucao_objetivo);
	int i, j;

	for(i = 0; i < qntdd_var; i++) {
		for(j = 0; j < larg_matriz; j++) {
			if(i == indices[j]) {
				printf("x%d: %.2lf\n", i, solucoes[j]);
				break;
			}
		}
	}
}

void simplex(double matriz[larg_matriz][comp_real], double * objetivos, double * solucoes, int * indices)
{
    bool is_solved = false;
    bool continuar;
    int i;

    while(!is_solved) {
        continuar = true;

        for(i = 0; i < larg_matriz; i++){
            if(solucoes[i] < 0) {
                tornar_viavel(matriz, objetivos, solucoes, indices);
                continuar = false;
                break;
            }
        }
        if(!continuar) continue;

        for(i = 0; i < comp_matriz; i++){
            if(objetivos[i] < 0) {
                otimizar_resultado(matriz, objetivos, solucoes, indices);
                continuar = false;
                break;
            }
        }
        if(!continuar) continue;
        is_solved = true;
    }
}

void tornar_viavel(double matriz[larg_matriz][comp_real], double * objetivos, double * solucoes, int * indices) {
    int i;
    int menor_larg = 0;
    for(i = 1; i < larg_matriz; i++) if(solucoes[i] < solucoes[menor_larg]) menor_larg = i;

    int menor_comp = -1;
    for(i = 0; i < comp_matriz; i++) {
        if(abs((long)(matriz[menor_larg][i] * PRECISAO)) != 0 && abs((long)((objetivos[i] / matriz[menor_larg][i]) * PRECISAO)) != 0) {
            if(menor_comp == -1) menor_comp = i;
            else if(objetivos[i] / matriz[i][menor_comp] < objetivos[menor_comp] / matriz[menor_larg][menor_comp]) menor_comp = i;
        }
    }

    pivotamento(matriz, objetivos, solucoes, indices, menor_larg, menor_comp);
}

void otimizar_resultado(double matriz[larg_matriz][comp_real], double * objetivos, double * solucoes, int * indices) {
    int i;
    int menor_comp = 0;
    for(i = 1; i < comp_matriz; i++) {
        if(objetivos[i] < objetivos[menor_comp]) menor_comp = i;
    }

    int menor_larg = -1;
    for(i = 0; i < larg_matriz; i++) {
        if(abs((long)(matriz[i][menor_comp] * PRECISAO)) != 0 && (long)((solucoes[i] / matriz[i][menor_comp]) * PRECISAO) > 0) {
            if(menor_larg == -1) menor_larg = i;
			else if(solucoes[i] / matriz[i][menor_comp] < solucoes[menor_larg] / matriz[menor_larg][menor_comp]) menor_larg = i;
        }
    }

    pivotamento(matriz, objetivos, solucoes, indices, menor_larg, menor_comp);
}

void pivotamento(double matriz[larg_matriz][comp_real], double * objetivos, double * solucoes, int * indices, int x, int y) {
    static int interacao = 1;
    int i, j;
	double pivo, multiplicar;

	if(x == -1 || y == -1) {
		fprintf(stderr, "ERRO: Matriz sem solucao.\n\n");
		exit(1);
	}

	printf("Interacao %d: sai x%d, entra x%d\n", interacao++, indices[x], y);

	indices[x] = y;
    pivo = matriz[x][y];

    for(i = 0; i < comp_matriz; i++) matriz[x][i] /= pivo;
    solucoes[x] /= pivo;

    multiplicar = objetivos[y];
    for(i = 0; i < comp_matriz; i++) {
        objetivos[i] += multiplicar * matriz[x][i] * (-1);
		if(abs((long)(objetivos[i] * PRECISAO)) == 0) objetivos[i] = abs(objetivos[i]);
    }
    solucao_objetivo += multiplicar * solucoes[x] * (-1);
	if(abs((long)(solucao_objetivo * PRECISAO)) == 0) solucao_objetivo = abs(solucao_objetivo);

    for(i = 0; i < larg_matriz; i++) {
        if(x == i) continue;

        multiplicar = matriz[i][y];
        for(j = 0; j < comp_matriz; j++) {
            matriz[i][j] += multiplicar * matriz[x][j] * (-1);
			if(abs((long)(matriz[i][j] * PRECISAO)) == 0) matriz[i][j] = abs(matriz[i][j]);
        }
        solucoes[i] += multiplicar * solucoes[x] * (-1);
		if(abs((long)(solucoes[i] * PRECISAO)) == 0) solucoes[i] = abs(solucoes[i]);
    }
	
	print_matriz(matriz, objetivos, solucoes, indices);
}

void analizar_recursos(double matriz[larg_matriz][comp_real], double * solucoes, double * solucoes_iniciais) {
	double valor;
	int i, j;

	printf("\nRecursos\n\n");
	for(i = qntdd_var; i < comp_matriz; i++) printf("x%d\t\t", i);
	printf("\n");

	for(i = 0; i < larg_matriz; i++) {
		for(j = qntdd_var; j < comp_matriz; j++) {
			if(abs((long)(matriz[i][j] * PRECISAO)) != 0) {
				valor = solucoes[i] / matriz[i][j] * (-1);
				valor += solucoes_iniciais[j - qntdd_var];
				
				if(matriz[i][j] > 0) printf("x%d>=%.0lf\t\t", j, valor);
				else printf("x%d<=%.0lf\t\t", j, valor);
			} else printf("-\t\t");
		}
		printf("\n");
	}
}

void analizar_objetivos(double matriz[larg_matriz][comp_real], double * objetivos, double * objetivos_iniciais, int * indices) {
	double valor;
	int i, j;

	printf("\nCoeficientes do Objetivo\n\n");
	for(i = 0; i < qntdd_rest; i++) printf("c%d\t\t", indices[i]);
	printf("\n");

	for(j = qntdd_var; j < comp_matriz; j++) {
		for(i = 0; i < larg_matriz; i++) {
			
			if(abs((long)(matriz[i][j] * PRECISAO)) != 0) {
				valor = objetivos[j] / matriz[i][j] * (-1);
				valor += objetivos_iniciais[indices[i]] * (-1);
				
				if(matriz[i][j] > 0) printf("c%d>=%.0lf\t\t", indices[i], valor);
				else printf("c%d<=%.2lf\t\t", indices[i], valor);
			} else printf("-\t\t");
		}
		printf("\n");
	}
}
