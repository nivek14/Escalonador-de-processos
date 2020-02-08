#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"


int main(){
	
	// ponteiros para listas que foram usadas no simulador
	Head *h,*zero,*um,*dois,*tres,*quatro,*cpu,*aux,*comp,*final;

	// entradas do usuário
	int cpus,memSis;
	
	printf("insira o número de cpus: ");
	scanf("%d", &cpus);
	
	printf("informe a memoria do sistema: ");
	scanf("%d", &memSis);
	
	cpu = createFil(); // criando um fila vazia que representa as cpus
	aux = createFil(); // criando uma fila que servirá como auxiliar no processo de execução das cpus 
	comp = createFil();// fila que receberá os dados dos processos já executados 
	h = FilaPrincipal();
	printf("\n\n");
	printFil(h);
	h = maisMemoria(h,memSis);
	printFil(h);
	if(FilVazia(h) == 1)
		return 0;
	FilasDePrioridades(h,&zero,&um,&dois,&tres,&quatro,cpus);
	
	while((FilVazia(zero) && FilVazia(um) && 
			FilVazia(dois) && FilVazia(tres) && 
			FilVazia(quatro)) != 1){
		RoundRobin(zero,um,dois,tres,quatro,cpus,memSis,cpu,aux,comp);
	}
	
	if((FilVazia(zero) && FilVazia(um) && 
		FilVazia(dois) && FilVazia(tres) && 
		FilVazia(quatro)) == 1)
		printf("TODOS OS PROCESSOS FORAM PROCESSADOS\n");
		
	printFil(comp);
	
	printf("\n\n");
	
	printFil(h);
	
	
	final = filaFinal(h,comp);
	
	printf("PRINTANDO FILA FINAL COM OS DADOS CORRETOS\n");
	printFil(final);
	
	final = organizaFila(final);
	
	printf("PRINTANDO FILA FINAL COM OS DADOS CORRETOS E ORGANIZADOS\n");
	printFil(final);
	
	escreveSaida(final);
	 
	return 0;
	
}