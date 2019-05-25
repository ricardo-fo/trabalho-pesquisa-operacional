/*
 * 1º Trabalho de Pesquisa Operacional.
 * Integrantes do grupo:
 * Nome: Alexandre Saura.
 * Nome: Fábio Thomaz.
 * Nome: Ricardo Oliveira.
 * Profº.: José Fontebasso Neto.
 * Discplina: Pesquisa Operacional - I.
 * Universidade  Católica de Santos - Maio de 2019.
*/
#include <stdio.h> // uso: inputs e outputs.
#include <string.h> // uso: strcmp();
#include <ctype.h> // uso: tolower();

/* Estrutura para armazenar as informações das restrições*/
typedef struct {
	float coeficientes[20];
	char tipo_rest[3]; // tipo de restrição.
	float termo_ind; // termo independente.
} funcaoObjetivo;

int main()
{
	int qntdd_var, qntdd_rest, i, j; // quantidade de variáveis e quantidade de restrições.
	char problema[4], buffer;

	/* Input da quantidade de variáveis */
	/* --> ARRUMAR: ACEITANDO DECIMAIS COMO INPUT */
	do{
		printf("Informe a quantidade de variaveis (maximo 20): ");
		if(scanf("%d", &qntdd_var) != 1)
		{
			while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero inteiro.\n\n");
			continue;
		}
	}while(qntdd_var <= 0 || qntdd_var > 20);

	/* Input da quantidade de restrições */
	/* --> ARRUMAR: ACEITANDO DECIMAIS COMO INPUT */
	do{
		printf("Informe a quantidade de restricoes (maximo 20): ");
		if(scanf("%d", &qntdd_rest) != 1)
		{
			while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero inteiro.\n\n");
			continue;
		}
	}while(qntdd_rest <= 0 || qntdd_rest > 20);

	/* Input do tipo de problema */
	do{
		printf("\nTipo de problema (max/min): ");
		scanf(" %3[a-xA-X]", problema); // fomatação para leitura de apenas letras minusculas.
		while(getchar() != '\n'); // limpeza do buffer do teclado.

		for(i = 0; i < 4; i++){ // conversão para letras minusculas.
			problema[i] = tolower(problema[i]);
		}
	}while(strcmp(problema, "max") != 0 && strcmp(problema, "min") != 0);

	float coeficientes[qntdd_var]; // coeficientes.
	funcaoObjetivo funcoes[qntdd_rest]; // dados das restricoes.

	/* Input da função objetivo */
	printf("\nFuncao objetivo\n");
	for(i = 0; i < qntdd_var; i++)
	{
		printf("Coeficiente da variavel x%d: ", i);
		while(scanf("%f", &coeficientes[i]) != 1){
			while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
			printf("Coeficiente da variavel x%d: ", i);
		}
		while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
	}

	/* Input dos dados das restrições */
	for(i = 0; i < qntdd_rest; i++)
	{
		printf("\nRestricao %d:\n", i + 1);

		/* Input dos coeficientes da variavel*/
		for(j = 0; j < qntdd_var; j++)
		{
			printf("Coeficiente da variavel x%d: ", j);
			while(scanf("%f", &funcoes[i].coeficientes[j]) != 1){
				while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
				fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
				printf("Coeficiente da variavel x%d: ", i);
			}
			while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
		}
		/* Input do tipo de restricao */
		do{
			printf("\nOpcoes de restricao:\n==\n<=\n>=\n");
			printf("Tipo de restricao: ");
			scanf(" %3s", funcoes[i].tipo_rest);
			while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
		}while(strcmp(funcoes[i].tipo_rest, "==") != 0 
			&& strcmp(funcoes[i].tipo_rest, "<=") != 0 
			&& strcmp(funcoes[i].tipo_rest, ">=") != 0);

		/* Input do termo independente */
		printf("Termo independente: ");
		while(scanf("%f", &funcoes[i].termo_ind) != 1){
			while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
			printf("Termo independente: ");
		}
		while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
	}

	/*********************************************************************/
	printf("\n......................................................................................................\n");
	printf("\nIMPRESSAO PARA TESTE:\n");
	for(i = 0; i < qntdd_rest; i++){
		printf("\nRestricao %d:", i + 1);
		for(j = 0; j < qntdd_var; j++){
			printf("\nCoeficiente da variavel x%d: %.3f", j, funcoes[i].coeficientes[j]);
		}
		printf("\nTipo de restricao: %s", funcoes[i].tipo_rest);
		printf("\nTermo independente: %.3f\n", funcoes[i].termo_ind);
	}
	/*********************************************************************/
	return 0;
}
