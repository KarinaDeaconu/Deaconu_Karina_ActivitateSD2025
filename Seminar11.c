#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara
typedef struct NodLP NodLP;
typedef struct NodLS NodLS;

struct NodLS {
	NodLP* nodPrincipal;
	NodLS* next;
};

struct NodLP {
	Masina info;
	NodLS* vecini;
	NodLP* next;
};

void inserareLP(NodLP** cap, Masina m) {
	NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
	nou->info = m;
	nou->next = NULL;
	nou->vecini = NULL;

	if (*(cap) == NULL) {
		*cap = nou;
	}
	else {
		NodLP* temp = (*cap);
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareLS(NodLS** cap, NodLP* nodPrincipal) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->nodPrincipal = nodPrincipal;
	nou->next = (*cap);
	(*cap) = nou;
}

NodLP* cautaNodDupaID(NodLP* cap, int id) {
	while (cap != NULL && cap->info.id != id) {
		cap = cap->next;
	}
	return cap;
}

void inserareMuchie(NodLP* cap, int idStart, int idStop) {
	NodLP* nodStart = cautaNodDupaID(cap, idStart);
	NodLP* nodStop = cautaNodDupaID(cap, idStop);
	if (nodStart && nodStop) {
		inserareLS(&nodStart->vecini, nodStop);
		inserareLS(&nodStop->vecini, nodStart); 
	}
}

void* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodLP* cap = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		inserareLP(&cap, m);
	}
	fclose(f);
	return cap;
}

void citireMuchiiDinFisier(NodLP* cap, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	int idStart;
	int idStop;
	while (!feof(f)) {
		fscanf(f, "%d %d", &idStart, &idStop);
		inserareMuchie(cap, idStart, idStop);
	}
	fclose(f);
}

void dezalocareListaAdiacentaLS(NodLS* cap) {
	while (cap != NULL) {
		NodLS* temp = cap->next;
		free(cap);
		cap = temp;
	}
}

void dezalocareNoduriGraf(NodLP** cap) {
	while ((*cap) != NULL) {
		NodLP* temp = (*cap)->next;
		dezalocareListaAdiacentaLS((*cap)->vecini);
		free((*cap)->info.model);
		free((*cap)->info.numeSofer);
		(*cap)->vecini = NULL;
		free((*cap));
		(*cap) = temp;
	}
}

NodLP* citesteGrafMasini(const char* numeFisierMasini, const char* numeFisierMuchii) {
	NodLP* graf = citireNoduriMasiniDinFisier(numeFisierMasini);
	citireMuchiiDinFisier(graf, numeFisierMuchii);
	return graf;
}

int main() {
	NodLP* graf = citesteGrafMasini("masini.txt", "muchii.txt");

	return 0;
}