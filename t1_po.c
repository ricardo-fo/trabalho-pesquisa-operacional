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
	float coeficientes[20];
	char tipo_rest[3];
	float termo_ind;
} Restricao;

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

	float funcao_objetivo[qntdd_var], auxiliar;

	/* Input da função objetivo */
	printf("\nFuncao objetivo\n");
	for(i = 0; i < qntdd_var; i++)
	{
		printf("Coeficiente da variavel x%d: ", i);
		while(scanf("%f", &auxiliar) != 1){
			while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
			printf("Coeficiente da variavel x%d: ", i);
		}
		while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
		funcao_objetivo[i] = (-1) * auxiliar;
	}

	Restricao restricoes[qntdd_rest];

	/* Input dos dados das restrições */
	for(i = 1; i < qntdd_rest; i++)
	{
		printf("\nRestricao %d:\n", i + 1);

		/* Input dos coeficientes da variavel*/
		for(j = 0; j < qntdd_var; j++)
		{
			printf("Coeficiente da variavel x%d: ", j);
			while(scanf("%f", &restricoes[i].coeficientes[j]) != 1){
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
			scanf(" %3s", restricoes[i].tipo_rest);
			while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
		}while(strcmp(restricoes[i].tipo_rest, "==") != 0 
			&& strcmp(restricoes[i].tipo_rest, "<=") != 0 
			&& strcmp(restricoes[i].tipo_rest, ">=") != 0);

		/* Input do termo independente */
		printf("Termo independente: ");
		while(scanf("%f", &restricoes[i].termo_ind) != 1){
			while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
			fprintf(stderr, "ERRO: input passado nao corresponde a um numero real.\n\n");
			printf("Termo independente: ");
		}
		while((buffer = getchar()) != '\n'); // limpeza do buffer do teclado.
	}

	return 0;
}
