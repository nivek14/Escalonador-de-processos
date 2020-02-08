#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"


int quantum = 10;
int control = -1;
int processoFila;

// criando fila
Head* createFil(){

	Head *head = (Head*) malloc(sizeof(Head));

	head->qtd = 0;
	head->cab = NULL;

	return head;
	
}

// inserindo na fila
void insereFil(Head* head, int chegada, int tempo, int memoria, int prioridade, int flag, int observado, int lancamento){
	
	Node *node = (Node*) malloc(sizeof(Node));
	
	node->chegada = chegada;
	node->tempo = tempo;
	node->memoria = memoria;
	node->prioridade = prioridade;
	node->flag = flag;
	node->tempoObservado = observado;
	node->lancamento = lancamento;
	
	node->prox = head->cab;
	head->cab = node;
	head->qtd++;
	
	printf("processo inserido, prioridade do processo e %d\n", node->prioridade);
	printf("processo inserido, tempo do processo e %d\n", node->tempo);

}

// verificando se a fila está vazia
int FilVazia(Head* head){
	
	if(head->qtd == 0)
		return 1;
	else
		return 0;

}

// printa uma fila
void printFil(Head* head){
	
	int i = 0;

	if(FilVazia(head) == 1){
		printf("Fila vazia\n");
		return;
	}

	Node *p = head->cab;

	while(p != NULL){
		printf("processo %d\n", i);
		printf("chegada do processo %d: %d\n", i , p->chegada);
		printf("tempo do processo %d: %d\n", i , p->tempo);
		printf("memoria do processo %d: %d\n", i , p->memoria);
		printf("prioridade do processo %d: %d\n", i , p->prioridade);
		printf("flag do processo %d: %d\n", i , p->flag);
		printf("tempo observado do processo %d: %d\n", i , p->tempoObservado);
		printf("lancamento do processo %d: %d\n", i , p->lancamento);
		printf("\n");
		i++;
		p = p->prox;
	}

}

// função q remove a cabeça da fila
void removeFil(Head* head){
	
	if(FilVazia(head) != 1){
		Node *p = head->cab;
		head->cab = p->prox;
		free(p);
		head->qtd--;
	}
	
}


Node* atPos(Head* head, int pos){
	
	if(pos >= 0 && pos < head->qtd){
		Node* node = head->cab;
		
		int i;
		for(i=0;i<pos;i++)
			node = node->prox;
		
		return node;
	}
	
	return NULL;

}


// função q remove elemento especifico da fila 		
void removeElemento(Head* head, int pos){
	
	if(pos == 0){
		removeFil(head);
	}
	
	else{
		Node* current = atPos(head,pos);
	
		if(current != NULL){
			Node* previous = atPos(head, pos-1);
			previous->prox = current->prox;
			free(current);
		}
	}
}

int numeroDeProcessos(Head* head){
	
	Node *p = head->cab;
	int i = 0;
	
	while(p != NULL){
		i++;
		p = p->prox;
	}

	return i;
	
}

// função que ajuda no gerenciamento da memória do sistema
Head* maisMemoria(Head* head, int mem){
	
	Node *p = head->cab;
	Head *nova;
	
	nova = createFil();
	
	printf("printando fila ao chegar na funcao\n");
	printFil(head);
	
	while(p != NULL){
		
		if(p->memoria <= mem){
			insereFil(nova,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
		}
		else{
			printf("removido por causa da memória %d %d\n", p->memoria, p->chegada);
			removeFil(head);
		}
		
		p = p->prox;
	
	}
	
	printf("printando fila ao sair da funcao\n");
	printFil(nova);
	
	return nova;
	
}
			
// cpu do sistema
void CpuControl(Head* head, Head* cpu, int nCpus, Head* aux, int memSis){
	
	Node *p = head->cab;
	int i=0,j=0,m = memSis;
	
	// passando n processos de uma determinada prioridade para as cpus
	while(i < nCpus){
		
		if(p->memoria <= memSis){
			insereFil(cpu,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			memSis -= p->memoria;
			removeFil(head);
			i++;
			if(p->prox == NULL)
				break;
			else
				p = p->prox;
			
		}
		
		else if(p->memoria > memSis){
			printf("memoria atual %d, processo precisa de %d\n", memSis, p->memoria);
			break;
		}
		
	}
	
	p = head->cab;
	
	/* aumentando em 1 o tempo dos processos que não foram 
	 * executados pelas cpus e em mais 10 no tempo observado.
	 */
	while(p != NULL){
		
		p->lancamento += 1;
		p->tempoObservado += 10;
		p = p->prox;
	
	}
	
	printFil(head);
	
	Node *pp = cpu->cab;
	
	// executando os processos nas cpus
	while(pp != NULL){
			
			printf("%d\n", pp->tempo);
			
			for(i=0;i<quantum;i++){
				
				printf("executando processo da fila %d\n", pp->prioridade);
				
				pp->tempo -= 1;
				pp->tempoObservado += 1;
				
				if(pp->tempo == 0)
					break;
				
			}		
			
			if(pp->tempo > 0 && control == 0)
				pp->prioridade += 1;
			else
				pp->prioridade -= 1;
			
			j++;
			pp = pp->prox;

	}
	
	processoFila = numeroDeProcessos(cpu);
	
	pp = cpu->cab;
	
	// passando os processos da cpu para uma fila auxiliar
	for(i=0;i<processoFila;i++){

		insereFil(aux,pp->chegada,pp->tempo,pp->memoria,pp->prioridade,pp->flag,pp->tempoObservado,pp->lancamento);
		
		if(pp->prox == NULL)
			break;
		else
			pp = pp->prox;
			
	}
	
	// removendo processos já executados das cpus
	while(processoFila != 0){
		removeFil(cpu);
		processoFila--;
	}
	
	memSis = m;
	
	// se existirem processos na fila original executamos novamente as cpus
	if(FilVazia(head) != 1){
		
		processoFila = numeroDeProcessos(head);

		CpuControl(head,cpu,processoFila,aux,memSis);
	
	}
	
	else{
		
		processoFila = numeroDeProcessos(aux);
		
		pp = aux->cab;
		
		while(pp != NULL){
			
			insereFil(head,pp->chegada,pp->tempo,pp->memoria,pp->prioridade,pp->flag,pp->tempoObservado,pp->lancamento);
			
			pp = pp->prox;
		
		}
		
		// removendo processos da fila auxiliar
		while(processoFila != 0){
			removeFil(aux);
			processoFila--;
		}
		
	}
			
}

//======================================================================

// Round-Robin

// prioridade == 0
void PrioridadeZero(Head* head, Head* um, int cpus, int memSis, Head* cpu, Head* aux, Head* comp){
	
	// o controlo nos indica se temos que passar para uma fila de menor ou maior prioridade
	// se control == 0, passamos para uma fila de menor prioriade
	// se control == 1, passamos para uma fila de maior prioridade
	control = 0;
	
	printf("FILA ZERO\n");
	printFil(head);
	
	// chamando a execução da cpu
	CpuControl(head,cpu,cpus,aux,memSis);

	Node *p = head->cab;
	
	printFil(head);
	
	// percorrendo a lista atualizada depois da execução da cpu
	while(p != NULL){
		// verificando se o processo tem tempo 0, se tiver devemos remove-lo pois já foi executado
		if(p->tempo == 0){
			insereFil(comp,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		// passando processos que não foram executados para a nova fila de prioridade
		else{
			insereFil(um,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		p = p->prox;
	}
	
}
	
// prioridade == 1	
void PrioridadeUm(Head* head,Head* zero, Head* dois, int cpus, int memSis, Head* cpu, Head* aux, Head* comp){
	
	// o control será menos 1 quando não tivermos começado com processos de prioridade 0
	if(control == -1)
		control = 0;
		
	printf("FILA UM\n");
	printFil(head);
	
	// chamando cpu
	CpuControl(head,cpu,cpus,aux,memSis);
	
	Node *p = head->cab;
	
	// percorrendo fila atualizada
	while(p != NULL){
		// tempo igual a 0 removemos o processo pois foi executado
		if(p->tempo == 0){
			insereFil(comp,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		// se não entrar no primeiro if significa que o tempo é maior que 0, então verificamos se o control é 0 ou 1
		// se o control for 0 passamos para uma fila de menor prioridade
		else if(control == 0){
			insereFil(dois,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		// control igual a 1 passamos para uma fila de maior prioridade
		else if(control == 1){
			insereFil(zero,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		p = p->prox;
	}	
	
}

// prioridade == 2
void PrioridadeDois(Head* head,Head* um, Head* tres, int cpus, int memSis, Head* cpu, Head* aux, Head* comp){
	
	// caso control igual a -1 colocamos ele em 0
	if(control == -1)
		control = 0;
	
	printf("FILA DOIS\n");
	printFil(head);
	
	// chamando cpu
	CpuControl(head,cpu,cpus,aux,memSis);
	
	Node *p = head->cab;
	
	// percorrendo fila atualizada
	while(p != NULL){
		// removendo processo já executado
		if(p->tempo == 0){
			insereFil(comp,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		// passando para uma fila de menor prioridade
		else if(control == 0){
			insereFil(tres,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		// passando para uma fila de maior prioridade
		else if(control == 1){
			insereFil(um,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		
		p = p->prox;
	}
		
}

// prioridade == 3
void PrioridadeTres(Head* head,Head* dois, Head* quatro, int cpus, int memSis, Head* cpu, Head* aux, Head* comp){
	
	// colocando 0 para o control
	if(control == -1)
		control = 0;
		
	printf("FILA TRES\n");
	printFil(head);
	
	// chamando cpu
	CpuControl(head,cpu,cpus,aux,memSis);
	
	Node *p = head->cab;
	
	// percorrendo fila atualizada
	while(p != NULL){
		// removendo processo já executado
		if(p->tempo == 0){
			insereFil(comp,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		// passando processo para uma fila de menor prioridade
		else if(control == 0){
			insereFil(quatro,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		// passando processo para uma fila de maio prioridade
		else if(control == 1){
			insereFil(dois,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		p = p->prox;
	}
		
}

// prioridade == 4
void PrioridadeQuatro(Head* head, Head* tres, int cpus, int memSis, Head* cpu, Head* aux, Head* comp){
	
	/* 	quando a função round-robin chama a função de prioridade 4 colocamos o control em 1
	 	fazemos isso porque agora sabemos que devemos colocar os processos que ainda não foram
	  	em sua fila original, então começamos a decrementar a prioridade até que o processo chegue em
	  	sua fila original.
	*/
	control = 1;
	
	printf("FILA QUATRO\n");
	printFil(head);
	
	// chamando cpu
	CpuControl(head,cpu,cpus,aux,memSis);
	
	Node *p = head->cab;
	
	// percorrendo fila atualizada
	while(p != NULL){
		// removendo processo já executado
		if(p->tempo == 0){
			insereFil(comp,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		// passando processos que não foram executados para uma fila de maior prioridade
		else{
			insereFil(tres,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			removeFil(head);
		}
		p = p->prox;
	}
		
}

void RoundRobin(Head* zero, Head* um, Head* dois, Head* tres, Head* quatro, int cpus, int memSis, Head* cpu, Head* aux, Head* comp){
	
	/* 	a função round-robin basicamente faz a chamada das funções que controlam as filas
		essas chamadas são em ordem, ou seja primeiro a fila 0 será verificada, veremos 
		se ela possui algum processo, se tiver iremos executa-la, caso não tiver será informado
		que essa fila não possui processos e passaremos para a próxima fila.*/

	int z,u,d,t,q;
	
	printf("Execução da fila 0\n");
	z = FilVazia(zero);
	if(z == 1)
		printf("sem processos de prioridade 0\n");
	else
		PrioridadeZero(zero,um,cpus,memSis,cpu,aux,comp);
	
	printf("Execução da fila 1\n");
	u = FilVazia(um);
	if(u == 1)
		printf("sem processos de prioridade 1\n");
	else
		PrioridadeUm(um,zero,dois,cpus,memSis,cpu,aux,comp);
	
	printf("Execução da fila 2\n");
	d = FilVazia(dois);
	if(d == 1)
		printf("sem processos de prioridade 2\n");
	else
		PrioridadeDois(dois,um,tres,cpus,memSis,cpu,aux,comp);
	
	printf("Execução da fila 3\n");
	t = FilVazia(tres);
	if(t == 1)
		printf("sem processos de prioridade 3\n");
	else
		PrioridadeTres(tres,dois,quatro,cpus,memSis,cpu,aux,comp);
	
	printf("Execução da fila 4\n");
	q = FilVazia(quatro);
	if(q == 1)
		printf("sem processos de prioridade 0\n");
	else
		PrioridadeQuatro(quatro,tres,cpus,memSis,cpu,aux,comp);
	
}

//======================================================================

// função que le o arquivo de entrada e cria a fila com todos os processos
Head* FilaPrincipal(){
	
	FILE *arq;
	int cheg,temp,mem,prior,i=0;
	Head* head;
	
	head = createFil();

	//abrindo o arquivo_frase em modo "somente leitura"
	 if((arq = fopen("ex.txt", "r")) == NULL){
        printf("Erro ao abrir o arquivo.\n");
    }
  
	//enquanto não for fim de arquivo o looping será executado
	//e será impresso o texto
	while(fscanf(arq,"%d %d %d %d\n", &cheg, &temp, &mem, &prior) != EOF){
		insereFil(head,cheg,temp,mem,prior,i,temp,cheg);
		i++;
	}
	
	//fechando o arquivo
	fclose(arq);
	
	return head;
  
}


// função que irá filtras os processos, colocando-os em suas filas corretas
void FilasDePrioridades(Head* head,Head** zero, Head** um, Head** dois, Head** tres, Head** quatro,int nCpus){
	
	/*	aqui iremos pegar a fila que possui todos os processos lidos do arquivo de entrada
		e será realizado um filtro dos processos, onde iremos coloca-los em suas filas de
		prioridade corretas.*/


	 Node* p = head->cab;
	 
	 *zero = createFil();
	 *um = createFil();
	 *dois = createFil();
	 *tres = createFil();
	 *quatro = createFil();
	 
	 while(p != NULL){
	 	// colocando os processos de prioridade 0 na fila 0
		if(p->prioridade == 0)
			 insereFil(*zero,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
		// colocando os processos de prioridade 1 na fila 1
		else if(p->prioridade == 1)
			  insereFil(*um,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
		// colocando os processos de prioridade 2 na fila 2
		else if(p->prioridade == 2)
			  insereFil(*dois,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
		// colocando os processos de prioridade 3 na fila 3	
		else if(p->prioridade == 3)
			  insereFil(*tres,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
		// colocando os processos de prioridade 4 na fila 4	
		else if(p->prioridade == 4)
			  insereFil(*quatro,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
			  		  
		p = p->prox;
			  
	}
		 
}

// pegando os dados corretos e colocando em uma fila final
Head* filaFinal(Head* original, Head* processado){
	
	Head *final;
	Node *p = original->cab;
	Node *pp = processado->cab;
	int i = 0;
	
	final = createFil();
	
	while(pp != NULL){
		
		if(pp->flag == p->flag){
			insereFil(final,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,pp->tempoObservado,pp->lancamento);
			//removeElemento(original,i);
			pp = pp->prox;
		}
		else if(p->prox == NULL){
			i=0;
			p = original->cab;
		}
		else{
			i++;
			p = p->prox;
		}
	}
	
	return final;
}


void escreveSaida(Head* head){
	
	FILE *pont_arq;
	Node *p = head->cab;
	
	pont_arq = fopen("saida.txt", "w");
	  
	if(pont_arq == NULL){
		printf("Erro na abertura do arquivo!");
	 }
	// chegada, lançamento, duração projetada, duração observada 
	while(p != NULL){
		fprintf(pont_arq,"%d" " " "%d" " " "%d" " " "%d" "\n", p->chegada, p->lancamento, p->tempo, p->tempoObservado);
		p = p->prox;
	}
	
	fclose(pont_arq);
	
}

// organizando a fila final na ordem dos processos de chegada
Head* organizaFila(Head* head){
	
	Node *p = head->cab;
	Head *nova;
	int cont=0,i;
	
	nova = createFil();
	
	while(p != NULL){
		cont++;
		p = p->prox;
	}
	
	p = head->cab;
	
	for(i=cont-1;i>=0;i--){
		while(p != NULL){
			if(p->flag == i){
				insereFil(nova,p->chegada,p->tempo,p->memoria,p->prioridade,p->flag,p->tempoObservado,p->lancamento);
				p = head->cab;
				break;
			}
			p = p->prox;
		}
	}
	
	return nova;
	
}
