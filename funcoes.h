typedef struct headd{
	int qtd;
	struct nodee *cab;
}Head;

typedef struct nodee{
	int chegada;
	int tempo;
	int memoria;
	int prioridade;
	int tempoObservado;
	int lancamento;
	int flag;
	int m;
	struct nodee* prox;
}Node;

Head* createFil();
void insereFil(Head* head, int chegada, int tempo, int memoria, int prioridade, int flag, int observado, int lancamento);
int FilVazia(Head* head);
void printFil(Head* head);
void removeFil(Head* head);
Node* atPos(Head* head, int pos);
void removeElemento(Head* head, int pos);
int numeroDeProcessos(Head* head);
Head* maisMemoria(Head* head, int mem);
void CpuControl(Head* head, Head* cpu, int nCpus, Head* aux, int memSis);
void PrioridadeZero(Head* head, Head* um, int cpus, int memSis, Head* cpu, Head* aux, Head* comp);
void PrioridadeUm(Head* head,Head* zero, Head* dois, int cpus, int memSis, Head* cpu, Head* aux, Head* comp);
void PrioridadeDois(Head* head,Head* um, Head* tres, int cpus, int memSis, Head* cpu, Head* aux, Head* comp);
void PrioridadeTres(Head* head,Head* dois, Head* quatro, int cpus, int memSis, Head* cpu, Head* aux, Head* comp);
void PrioridadeQuatro(Head* head, Head* tres, int cpus, int memSis, Head* cpu, Head* aux, Head* comp);
void RoundRobin(Head* zero, Head* um, Head* dois, Head* tres, Head* quatro, int cpus, int memSis, Head* cpu, Head* aux, Head* comp);
Head* FilaPrincipal();
void FilasDePrioridades(Head* head,Head** zero, Head** um, Head** dois, Head** tres, Head** quatro,int nCpus);
Head* filaFinal(Head* original, Head* processado);
void escreveSaida(Head* head);
Head* organizaFila(Head* head);
