#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct unidade {
	
	char unidade[2];
	struct unidade *top, *bottom;
	struct diretorio *inidir;
};

struct diretorio {
	
	char nomedir[30], data[10], hora[8];
	struct diretorio *head, *tail, *ppai;
	struct Arquivo *larq;
};

struct arquivo {
	
	char nomearq[30], data[10], hora[8];
	struct arquivo *prox;
	struct conteudoarq *conteudoarq;
};

struct conteudoarq {
	
	int letras;
	struct conteudoarq *top, *bottom;
	struct linha *linha;
};

struct linha {
	
	char letra;
	struct linha *ant, *prox;
};

typedef struct unidade Unidade;
typedef struct diretorio Diretorio;
typedef struct arquivo Arquivo;
typedef struct conteudoarq ConteudoArq;
typedef struct linha Linha;

void exibeDir(Diretorio *D, int *qtdearq, int *tamtotal) {
	
	int tam;
	Arquivo *aux = D -> larq;
	ConteudoArq *auxC;
	
	if(D != NULL) {
		
		printf("%s %s <DIR>\t%s\n", D -> data, D -> hora, D -> nomedir);
		if(aux != NULL) {
			
			while(aux != NULL) {
				
				tam = 0;
				auxC = aux -> conteudoarq;
				while(auxC != NULL) {
					
					tam += auxC -> letras;
					auxC = auxC -> bottom;
				}
				printf("%s %s \t%d %s\n", aux -> data, aux -> hora, tam, aux -> nomearq);
				*qtdearq++;
				*tamtotal += tam;
				aux = aux -> prox;	
			}
		}
		exibeDir(D -> head, &*qtdearq, &*tamtotal);
		exibeDir(D -> tail, &*qtdearq, &*tamtotal);	
	}
}

void buscaUnidade(Unidade *U, char comand[]) {
	
	Unidade *aux = U;
	
	if(aux != NULL) {
		
		while(strcmp(aux -> unidade, comand) && aux -> bottom != NULL)
			aux = aux -> bottom;
		if(!strcmp(aux -> unidade, comand))
			executaDir(U, aux -> inidir, comand);
		else
			printf("O sistema nao pode encontrar a unidade especificada.\n");
	}
	else
		printf("O sistema nao pode encontrar a unidade especificada.\n");		
}

void mostraDir(Diretorio *D) {
	
	int qtdearq = 0, totaltam = 0;
	Diretorio *aux;
	
	aux = D -> head;
	if(aux != NULL)
		exibeDir(aux, &qtdearq, &totaltam);
	
	printf("%d Arquivo(s)\t%d bytes\n", qtdearq, totaltam);
}

void limpaTela(void) {
	
	system("cls");
}

void mudaDir(Diretorio *D, char comand[]) {
	
	int i;
	char nomeaux[100], flag = 0;
	Diretorio *aux;
	
	if(D -> head == NULL)
		printf("O sistema nao pode encontrar o caminho especificado.\n");
	else {
		
		aux = D -> head;
		for(i = 3; i < strlen(comand); i++)
			nomeaux[i] = comand[i];
		while(aux != NULL && !strcmp(aux -> nomedir, nomeaux))
			aux	= aux -> tail;
		if(aux != NULL)
			executaDir(aux);
		else
			printf("O sistema nao pode encontrar o caminho especificado.\n");		
	}		
}

Diretorio *novoDiretorioU(void) {
	
	Diretorio *nova = (Diretorio*)malloc(sizeof(Diretorio));
	
	nova -> larq = NULL;
	nova -> head = NULL;
	nova -> ppai = NULL;
	strcpy(nova -> nomedir, "");
	strcpy(nova -> data, "");
	strcpy(nova -> hora, "");
	nova -> tail = NULL;
	
	return nova;
}

Unidade *criaUniNova(char unidade[]) {
	
	Unidade *nova = (Unidade*)malloc(sizeof(Unidade));
	
	nova -> top = NULL;
	strcpy(nova -> unidade, unidade);
	nova -> inidir = novoDiretorioU();
	nova -> bottom = NULL;
	
	return nova;
}

void criaUni(Unidade **U, char comand[]) {
	
	int i;
	char uni[2];
	Unidade *aux, *nova; 
	
	for(i = 3; i < strlen(comand); i++)
		uni[i - 3] = comand[i];
	nova = criaUniNova(uni);
	if(*U == NULL)
		*U = nova;
	else {
		
		aux = *U;
		while(aux -> bottom != NULL)
			aux = aux -> bottom;
		aux -> bottom = nova;
		nova -> top = aux;	
	}	
}	

Diretorio *criaDirNova(char diretorio[]) {
	
	Diretorio *nova = (Diretorio*)malloc(sizeof(Diretorio));;
	
	nova -> larq = NULL;
	nova -> head = NULL;
	nova -> ppai = NULL;
	strcpy(nova -> nomedir,diretorio);
	strcpy(nova -> data, __DATE__);
	strcpy(nova -> hora, __TIME__);
	nova -> tail = NULL;
	
	return nova;
}

void criaDir(Diretorio **D, char comand[]) {
	
	int i;
	char dir[100];
	Diretorio *aux, *nova;
	
	for(i = 3; i < strlen(comand); i++)
		dir[i - 3],comand[i];
	nova = criaDirNova(dir);
	if((*D) -> head == NULL) {
		
		(*D) -> head = nova;
		nova -> ppai = *D;
	}
	else {
		
		aux = (*D) -> head;
		while(aux -> tail != NULL)
			aux = aux -> tail;
		aux -> tail = nova;
		nova -> ppai = aux -> ppai;	
	}	
}

Arquivo *criaArqNova(char arquivo[]) {
	
	Arquivo *nova = (Arquivo*)malloc(sizeof(Arquivo));
	
	strcpy(nova -> nomearq, arquivo);
	strcpy(nova -> data, __DATE__);
	strcpy(nova -> hora, __TIME__);
	nova -> prox = NULL;
	nova -> conteudoarq = NULL;
	
	return nova;
}

void criaArq(Diretorio *D, char comand[]){
	
	int i;
	char arq[100];
	Arquivo *aux, *nova;
	
	for(i = 6; i < strlen(comand); i++)
		arq[i - 6] = comand[i];
	nova = criaArqNova(arq);
	if(D -> larq == NULL)
		D -> larq = nova;
	else {
		
		aux = D -> larq;
		while(aux -> prox != NULL)
			aux = aux -> prox;
		aux -> prox = nova;	
	}		
}

Linha *criaLetraNova(char letra) {
	
	Linha *nova = (Linha*)malloc(sizeof(Linha));;
	
	nova -> ant = NULL;
	nova -> letra = letra;
	nova -> prox = NULL;
	
	return 0;
}

Linha *criaLinhaLetras(char linha[]) {
	
	int i;
	Linha *cabeca, *aux, *aux2, *nova = criaLetraNova(linha[0]);
	
	cabeca = nova;
	aux = cabeca -> prox;
	aux2 = cabeca;
	for(i = 1; i < strlen(linha); i++){
		
		nova = criaLetraNova(linha[i]);
		aux = nova;
		aux -> ant = aux2;
		aux2 -> prox = aux;
		aux = aux -> prox;
		aux2 = aux2 -> prox;
	} 
	
	return cabeca;
}

ConteudoArq *criaLinhaNova2(char linha[]) {
	
	ConteudoArq *nova = (ConteudoArq*)malloc(sizeof(ConteudoArq));;
	
	nova -> top = NULL;
	nova -> letras = strlen(linha);
	nova -> linha = criaLinhaLetras(linha);
	nova -> bottom = NULL;
	
	return nova;
}

void criaLinhaNova(Arquivo *C, char linha[]) {
	
	ConteudoArq *aux, *nova;
	
	nova = criaLinhaNova2(linha);
	if(C -> conteudoarq == NULL)
		C -> conteudoarq = nova;
	else {
		
		aux = C;
		while(aux -> bottom != NULL)
			aux = aux -> bottom;
		aux -> bottom = nova;
		nova -> top = aux;	
	}	
}

void insereLinhas(Diretorio *D, char comand[]) {
	
	int i;
	char arq[100], linha[1000];
	Arquivo *aux;
	
	for(i = 9; i < strlen(comand); i++)
		arq[i - 9] = comand[i];
	aux = D -> larq;
	while(aux != NULL && strcmp(aux -> nomearq, arq))
		aux = aux -> prox;
	if(aux == NULL)
		printf("O sistema nao pode encontrar o arquivo especificado.\n");
	else {
		
		scanf(" %[^\n]", linha);
		while(!strcmp(linha, "0")) {
			
			criaLinhaNova(aux, linha);
			scanf(" %[^\n]", linha);
		}	
	}
}

void excluiLinhas(Arquivo *A) {
	
	ConteudoArq *C, *auxC;
	Linha *L, *auxL;
	
	C = A -> conteudoarq;
	while(C -> bottom != NULL)
		C = C -> bottom;
	while(C != NULL) {
		
		auxC = C -> top;	
		L = C -> linha;
		while(L -> prox != NULL)
			L = L -> prox;	
		while(L != NULL) {
			
			auxL = L -> ant;
			auxL -> prox = NULL;
			free(L);
			L = auxL;
		}
		auxC -> linha = NULL;
		free(C);
		C = auxC;
	}
}

void deletaArq(Diretorio **D, char comand[]) {
	
	int i;
	Arquivo *A, *auxA = NULL;
	char arq[100];
	
	for(i = 4; i < strlen(comand); i++)
		arq[i - 4] = comand[i];
	A = (*D) -> larq;	
	while(A != NULL && !strcmp(A -> nomearq, arq)) {
		
		auxA = A;
		A = A -> prox;	
	}	
	if(A == NULL)
		printf("O sistema nao pode encontrar o arquivo especificado.\n");
	else {
		
		excluiLinhas(A);
		if(auxA == NULL)
			(*D) -> larq = A -> prox;
		if(A -> prox == NULL)
			auxA -> prox = NULL;	
		else
			auxA -> prox = A -> prox;
		A -> conteudoarq = NULL; 	
		free(A);				
	}		
}

void removeArqs(Arquivo *A) {
	
	Arquivo *auxA = NULL;
	
	while(A != NULL) {
		
		auxA = A -> prox;
		excluiLinhas(A);
		A -> conteudoarq = NULL;
		free(A);
		A = auxA;	
	}
}

void removeDirs(Diretorio *D) {
	
	Diretorio *auxD = NULL;
	
	while(D != NULL) {
		
		auxD = D -> tail;
		if(D -> larq != NULL)
			removeArqs(D -> larq);
		D -> larq = NULL;
		if(D -> head != NULL)
			removeDirs(D -> head);
		D -> head = NULL;
		free(D);
		D = auxD;
	}	
}

void removeDir(Diretorio **D, char comand[]) {
	
	int i;
	char dir[100];
	Diretorio *aux = (*D) -> head, *aux2 = NULL;
	
	for(i = 3; i < strlen(comand); i++)
		dir[i - 3] = comand[i];
	while(aux != NULL && !strcmp(aux -> nomedir, dir)) {
		
		aux2 = aux;
		aux = aux -> tail;
	}
	if(aux == NULL)
		printf("O sistema nao pode encontrar o diretorio especificado.\n");
	else {
		
		if(aux -> larq != NULL)
			removeArqs(aux -> larq);
		aux -> larq = NULL;
		if(aux -> head != NULL)
			removeDirs(aux -> head);
		aux -> head = NULL;
		if(aux2 == NULL)
			(*D) -> head = aux -> head;
		if(aux -> tail == NULL)
			aux2 -> tail = NULL;
		else
			aux2 -> tail = aux -> tail;
		aux -> ppai = NULL;	
		free(aux);				
	}	
}

void removeUni(Unidade **U, char comand[]) {
	
	Unidade *auxU;
	
	if((*U) -> inidir -> larq != NULL)
		removeArqs(&(*U) -> inidir -> larq);
	(*U) -> inidir -> larq = NULL;
	if((*U) -> inidir -> head != NULL)
		removeDirs(&(*U) -> inidir -> head);
	(*U) -> inidir -> head = NULL;
	auxU = (*U) -> inidir;
	free(auxU);
	(*U) -> inidir = NULL;
	if((*U) -> top == NULL)
		auxU = (*U) -> bottom;
	else if((*U) -> bottom == NULL)
		auxU = (*U) -> top;
	else {
		
		auxU = (*U) -> top;
		auxU -> bottom = (*U) -> bottom;
	}
	free(U);	
}

void mostraArq(Diretorio *D, char comand[]) {
	
	int i;
	char arq[100];
	Arquivo *aux = D -> larq;
	ConteudoArq *auxC;
	Linha *auxL;
	
	for(i = 5; i < strlen(comand); i++)
		arq[i - 5] = comand[i];
	while(aux != NULL && !strcmp(aux -> nomearq, arq))
		aux = aux -> prox;
	if(aux == NULL)
		printf("O sistema nao pode encontrar o arquivo especificado.\n");
	else {
		
		auxC = aux -> conteudoarq;
		while(auxC != NULL) {
			
			auxL = auxC -> linha;
			while(auxL != NULL) {
				
				printf("%c", auxL -> letra);
				auxL = auxL -> prox;
			}
			printf("\n");
			auxC = auxC -> bottom;
		}
	}			
}

void dosKey(void) {
	
	int i = 1, op;
	char linha[100];
	FILE *hist = fopen("historico.txt","r");
	
	op = getch();
	while(op != 63)
		op = getch();
	while(!feof(hist)) {
		
		fscanf(hist, " %[^\n]", linha);
		printf("%d: %s\n", i, linha);
		i++;
	}
	fclose(hist);
}

Linha *copiaLinha(Linha *aux) {
	
	Linha *nova, *auxL;
	
	while(aux != NULL) {
		
		Linha *auxL2 = (Linha*)malloc(sizeof(Linha));
		
		auxL2 -> ant = aux -> ant;
		auxL2 -> letra = aux -> letra;
		auxL2 -> prox = aux -> prox;
		auxL = auxL2;
		if(auxL -> ant == NULL)
			nova = auxL;
		free(auxL2);	
		auxL = auxL -> prox;	
		aux = aux -> prox;
	}
	
	return nova;
}

ConteudoArq *copiaConteudo(ConteudoArq *aux) {
	
	ConteudoArq *nova, *auxC;
	
	while(aux != NULL) {
		
		ConteudoArq *auxC2 = (ConteudoArq*)malloc(sizeof(ConteudoArq));
		
		auxC2 -> top = aux -> top;
		auxC2 -> letras = aux -> letras;
		auxC2 -> linha = copiaLinha(aux->linha);
		auxC2 -> bottom = aux -> bottom;
		auxC = auxC2;
		if(auxC -> top == NULL)
			nova = auxC;
		free(auxC2);
		auxC = auxC -> bottom;
		aux = aux -> bottom;	
	}	 
	
	return nova;
}

Arquivo *criaCopiaArq(Arquivo *A) {
	
	Arquivo *nova = (Arquivo*)malloc(sizeof(Arquivo));
	
	strcpy(nova -> nomearq, A -> nomearq);
	strcpy(nova -> data, A -> data);
	strcpy(nova -> hora, A -> hora);
	nova -> prox = A -> prox;
	nova -> conteudoarq = copiaConteudo(A -> conteudoarq);
	
	return nova;
}

Diretorio *buscaUniDest(Unidade *U, char dest[]) {
	
	Unidade *aux = U;
	
	if(aux != NULL) {
		
		while(strcmp(aux -> unidade, dest) && aux -> bottom != NULL)
			aux = aux -> bottom;
		if(!strcmp(aux -> unidade, dest))
			return aux -> inidir;
		else
			return NULL;
	}
	else
		return NULL;
}

Diretorio *buscaDirDest(Diretorio *D, char dest[]) {
	
	Diretorio *aux = D -> head;
	
	if(aux != NULL) {
		
		while(strcmp(aux -> nomedir, dest) && aux -> tail != NULL)
			aux = aux -> tail;
		if(!strcmp(aux -> nomedir, dest))
			return aux;
		else
			return NULL;
	}
	else
		return NULL;
}

void insereArqCopiado(Diretorio **dirDest, Arquivo *arqCopiado) {
	
	Arquivo *aux = (*dirDest) -> larq, *aux2 = NULL;
	
	while(aux != NULL) {
		
		aux2 = aux;
		aux = aux -> prox;
	}
	if(aux2 == NULL)
		(*dirDest) -> larq = arqCopiado;
	else
		aux2 -> prox = arqCopiado;	
}

void copiaArq(Unidade *U, Diretorio *D, char comand[]) {
	
	int i, j;
	char arq[100], dest[100];
	Arquivo *aux = D -> larq, *arqCopiado;
	Unidade *auxU = U;
	Diretorio *dirDest;
	
	for(i = 6; comand[i] != ' '; i++)
		arq[i - 6] = comand[i];
	while(aux != NULL && !strcmp(aux -> nomearq, arq))
		aux = aux -> prox;
	if(aux == NULL)
		printf("O sistema nao pode encontrar o arquivo especificado.\n");
	else {
		
		arqCopiado = criaCopiaArq(aux);
		while(auxU -> top != NULL)
			auxU = auxU -> top;
		for(i = i + 1, j = 0; comand[i] != '\\'; i++, j++)
			dest[j] = comand[i];	
		dirDest = buscaUniDest(U, dest);
		if(dirDest != NULL) {
			
			while(dirDest != NULL && i != strlen(comand)) {
				
				dest[0] = '\0';
				for(i = i + 1, j = 0; comand[i] != '\\'; i++, j++)
					dest[j] = comand[i];
				dirDest = buscaDirDest(dirDest, dest);
			}
			if(dirDest != NULL)
				insereArqCopiado(&dirDest, arqCopiado);
			else
				printf("O sistema nao pode encontrar o destino especificado.\n");
		}	
		else
			printf("O sistema nao pode encontrar o destino especificado.\n");
	}	
}

void contaOc(Arquivo *A, char txt[]) {
	
	int i, cont = 0;
	ConteudoArq *auxC = A -> conteudoarq;
	Linha *auxL;
	
	while(auxC != NULL) {
		
		auxL = auxC -> linha;
		while(auxL != NULL) {
			
			for(i = 0; i < strlen(txt) && txt[i] == auxL -> letra && auxL != NULL; i++)
				auxL = auxL -> prox;		
			if(i == strlen(txt))
				cont++;
		}
		auxC = auxC -> bottom;
	}
	printf("%d ocorrencias\n", cont);
}

void ignoraMN(Arquivo *A, char txt[]) {
	
	int i, cont = 0;
	ConteudoArq *auxC = A -> conteudoarq;
	Linha *auxL;
	
	while(auxC != NULL) {
		
		auxL = auxC -> linha;
		while(auxL != NULL) {
			
			for(i = 0; i < strlen(txt) && (txt[i] == auxL -> letra || txt[i] == toupper(auxL -> letra)) && auxL != NULL; i++)
				auxL = auxL -> prox;		
			if(i == strlen(txt))
				cont++;
		}
		auxC = auxC -> bottom;
	}
	printf("%d ocorrencias\n", cont);
}

void contaLin(Arquivo *A, char txt[]) {
	
	int i, cont, contL = 0;
	ConteudoArq *auxC = A -> conteudoarq;
	Linha *auxL;
	
	while(auxC != NULL) {
		
		cont = 0;
		auxL = auxC -> linha;
		while(auxL != NULL) {
			
			for(i = 0; i < strlen(txt) && txt[i] == auxL -> letra && auxL != NULL; i++)
				auxL = auxL -> prox;		
			if(i == strlen(txt))
				cont++;
		}
		if(cont > 0)
			contL++;
		auxC = auxC -> bottom;
	}
	printf("%d linhas\n", contL);
}

void mostraN(Arquivo *A, char txt[]) {
	
	int i, cont, contL = 0;
	ConteudoArq *auxC = A -> conteudoarq;
	Linha *L, *auxL;
	
	while(auxC != NULL) {
		
		cont = 0;
		L = auxL = auxC -> linha;
		while(auxL != NULL) {
			
			for(i = 0; i < strlen(txt) && txt[i] == auxL -> letra && auxL != NULL; i++)
				auxL = auxL -> prox;		
			if(i == strlen(txt))
				cont++;
		}
		if(cont == 0) {
			
			while(L != NULL){
				
				printf("%c", L -> letra);
				L = L -> prox;
			}
			printf("\n");
		}
		auxC = auxC -> bottom;
	}
	printf("%d linhas\n", contL);
}

void encontraCad(Diretorio *D, char comand[]) {
	
	int i, j;
	char op[2], txt[100], arq[100];
	Arquivo *aux = D -> larq;
	
	for(i = 5, j = 0; j < 2; i++, j++)
		op[j] = comand[i];
	for(i = 9, j = 0; comand[i] != '"'; i++, j++)
		txt[j] = comand[i];
	for(i += 2, j = 0; i < strlen(comand); i++, j++)
		arq[j] = comand[i];
	while(aux != NULL && !strcmp(arq, aux -> nomearq))
		aux = aux -> prox;						
	if(aux == NULL)
		printf("O sistema nao pode encontrar o arquivo especificado.\n");
	else {
		
		if(op[1] == 'C')
			contaOc(aux, txt);
		else if(op[1] == 'I')
			ignoraMN(aux, txt);
		else if(op[1] == 'N')
			contaLin(aux, txt);
		else if(op[1] == 'V')
			mostraN(aux, txt);
	}					
}	

void executaComandoU(Unidade **U, char comand[]) {
	
	if(comand[1] == ':')
		buscaUnidade(*U, comand);
	else if(!strcmp(comand, "Dir"))
		mostraDir((*U) -> inidir);
	else if(!strcmp(comand,"Cls"))
		limpaTela();
	else if(comand[0] == 'C')
		mudaDir((*U) -> inidir, comand);
	else if(comand[0] == 'M')
		criaUni(&(*U),comand);	
	else if(comand[0] == 'R')
		removeUni(&(*U), comand);
	else if(!strcmp(comand, "Doskey"))
		dosKey();
	else
		printf("'%s' nao e reconhecido como um comando interno ou externo, um programa operavel ou um arquivo em lotes.\n",comand);											
}	

void executaComandoD(Unidade *U, Diretorio **D, char comand[]) {
	
	if(!strcmp(comand,"Dir"))
		mostraDir(*D);
	else if(!strcmp(comand, "Cls"))
		limpaTela();
	else if(comand[0] == 'C' && comand[1] == 'D')
		mudaDir(*D, comand);	
	else if(comand[0] == 'M' && comand[1] == 'D')
		criaDir(&(*D), comand);	
	else if(comand[0] == 'R')
		removeDir(&(*D), comand);
	else if(comand[0] == 'M' && comand[1] == 'K')
		criaArq(*D, comand);
	else if(comand[0] == 'C' && strlen(comand) == 4)
		copiaArq(U, *D, comand);	
	else if(comand[0] == 'C' && comand[5] == 'c')
		insereLinhas(*D, comand);
	else if(comand[0] == 'D')
		deletaArq(&(*D), comand);
	else if(comand[0] == 'T')
		mostraArq(*D, comand);
	else if(comand[0] == 'F')
		encontraCad(*D, comand);	
	else if(!strcmp(comand, "Doskey"))
		dosKey();	
	else
		printf("'%s' nao e reconhecido como um comando interno ou externo, um programa operavel ou um arquivo em lotes.\n",comand);					
}

void executaUni(Unidade *U) {
	
	FILE *hist = fopen("historico.txt","a");
	char comand[100];
	
	scanf(" %[^\n]", comand);
	fputs(comand, hist);
	fclose(hist);
	while(strcmp(comand,"exit")) {
		
		FILE *hist = fopen("historico.txt","a");
		executaComandoU(&U, comand);
		scanf(" %[^\n]", comand);
		fputs(comand, hist);
		fclose(hist);	
	}
}

void executaDir(Unidade *U, Diretorio *D) {
	
	FILE *hist = fopen("historico.txt","a");
	char comand[100];
	
	scanf(" %[^\n]", comand);
	fputs(comand, hist);
	fclose(hist);
	while(strcmp(comand,"exit")) {
		
		FILE *hist = fopen("historico.txt","a");
		executaComandoD(U, &D, comand);
		scanf(" %[^\n]", comand);
		fputs(comand, hist);
		fclose(hist);	
	}
}

int main(void) {
	
	FILE *hist = fopen("historico.txt","w");
	Unidade *U = NULL;
	
	fclose(hist);
	executaUni(U);
	remove("historico.txt");
	
	return 0;
}