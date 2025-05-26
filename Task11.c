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

struct Nod {
	Masina info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

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

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore != NULL) {
		int inaltimeSt = calculeazaInaltimeArbore(arbore->stanga);
		int inaltimeDr = calculeazaInaltimeArbore(arbore->dreapta);
		return 1 + (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr);
	}
	return 0;
}

int calculeazaGradEchilibru(Nod* arbore) {
	return calculeazaInaltimeArbore(arbore->stanga) - calculeazaInaltimeArbore(arbore->dreapta);
}

void rotireStanga(Nod** arbore) {
	Nod* aux = (*arbore)->dreapta;
	(*arbore)->dreapta = aux->stanga;
	aux->stanga = (*arbore);
	(*arbore) = aux;
}

void rotireDreapta(Nod** arbore) {
	Nod* aux = (*arbore)->stanga;
	(*arbore)->stanga = aux->dreapta;
	aux->dreapta = (*arbore);
	(*arbore) = aux;
}

void adaugaMasinaInArboreEchilibrat(Nod** arbore, Masina masinaNoua) {
	if ((*arbore) != NULL) {
		if ((*arbore)->info.id < masinaNoua.id) {
			adaugaMasinaInArboreEchilibrat(&((*arbore)->dreapta), masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&((*arbore)->stanga), masinaNoua);
		}
		int gradEchilibru = calculeazaGradEchilibru(*arbore);
		if (gradEchilibru == 2) {
			if (calculeazaGradEchilibru((*arbore)->stanga) == 1) {
				rotireDreapta(arbore);
			}
			else {
				rotireStanga(&((*arbore)->stanga));
				rotireDreapta(arbore);
			}
		}
		else if (gradEchilibru == -2) {
			if (calculeazaGradEchilibru((*arbore)->dreapta) != -1) {
				rotireDreapta(&((*arbore)->dreapta));
			}
			rotireStanga(arbore);
		}
	}
	else {
		(*arbore) = (Nod*)malloc(sizeof(Nod));
		(*arbore)->info = masinaNoua;
		(*arbore)->stanga = NULL;
		(*arbore)->dreapta = NULL;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* nod = NULL;
	while (!feof(file)) {
		adaugaMasinaInArboreEchilibrat(&nod, citireMasinaDinFisier(file));
	}
	fclose(file);
	return nod;
}

void afisareInOrdineSRD(Nod* radacina) {
	if (radacina != NULL) {
		afisareInOrdineSRD(radacina->stanga);
		afisareMasina(radacina->info);
		afisareInOrdineSRD(radacina->dreapta);
	}
}

void afisarePreOrdineRSD(Nod* radacina) {
	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisarePreOrdineRSD(radacina->stanga);
		afisarePreOrdineRSD(radacina->dreapta);
	}
}

void afisarePostOrdineSDR(Nod* radacina) {
	if (radacina != NULL) {
		afisarePostOrdineSDR(radacina->stanga);
		afisarePostOrdineSDR(radacina->dreapta);
		afisareMasina(radacina->info);
	}
}

void afisareArboreDeMasini(Nod* radacina) {
	printf("Afisare inordine (SRD):\n");
	afisareInOrdineSRD(radacina);

	printf("\nAfisare preordine (RSD):\n");
	afisarePreOrdineRSD(radacina);

	printf("\nAfisare postordine (SDR):\n");
	afisarePostOrdineSDR(radacina);
}

void dezalocareArboreDeMasini(Nod** radacina) {
	if (*radacina != NULL) {
		dezalocareArboreDeMasini(&(*radacina)->stanga);
		dezalocareArboreDeMasini(&(*radacina)->dreapta);
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free((*radacina));
		(*radacina) = NULL;
	}
}

Nod* getMasinaById(Nod* radacina, int id) {
	if (radacina != NULL) {
		if (id == radacina->info.id)
			return radacina;
		else if (id < radacina->info.id)
			return getMasinaById(radacina->stanga, id);
		else return getMasinaById(radacina->dreapta, id);
	}
	return 0;
}

// vector
void salvareInordineVector(Nod* radacina, Masina* vector, int* i) {
	if (radacina != NULL) {
		salvareInordineVector(radacina->stanga, vector, i);
		vector[*i] = radacina->info;
		(*i)++;
		salvareInordineVector(radacina->dreapta, vector, i);
	}
}

void salvarePreordineVector(Nod* radacina, Masina* vector, int* i) {
	if (radacina != NULL) {
		vector[*i] = radacina->info;
		(*i)++;
		salvarePreordineVector(radacina->stanga, vector, i);
		salvarePreordineVector(radacina->dreapta, vector, i);
	}
}

void salvarePostordineVector(Nod* radacina, Masina* vector, int* i) {
	if (radacina != NULL) {
		salvarePostordineVector(radacina->stanga, vector, i);
		salvarePostordineVector(radacina->dreapta, vector, i);
		vector[*i] = radacina->info;
		(*i)++;
	}
}

// lista simplu inlantuita
struct LS {
	Masina info;
	struct LS* next;
	struct LS* prev;
};
typedef struct LS LS;

void adaugareLS(LS** cap, Masina masinaNoua) {
	LS* nou = (LS*)malloc(sizeof(LS));
	nou->info = masinaNoua;
	nou->next = NULL;
	if ((*cap) != NULL) {
		LS* aux = (*cap);
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*cap = nou;
	}
}

void salvareInordineLS(Nod* radacina, LS** cap) {
	if (radacina != NULL) {
		salvareInordineLS(radacina->stanga, cap);
		adaugareLS(cap, radacina->info);
		salvareInordineLS(radacina->dreapta, cap);
	}
}

void salvarePreordineLS(Nod* radacina, LS** cap) {
	if (radacina != NULL) {
		adaugareLS(cap, radacina->info);
		salvarePreordineLS(radacina->stanga, cap);
		salvarePreordineLS(radacina->dreapta, cap);
	}
}

void salvarePostordineLS(Nod* radacina, LS** cap) {
	if (radacina != NULL) {
		salvarePostordineLS(radacina->stanga, cap);
		salvarePostordineLS(radacina->dreapta, cap);
		adaugareLS(cap, radacina->info);
	}
}

void afisareLS(LS* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void dezalocareLS(LS** cap) {
	while ((*cap) != NULL) {
		LS* temp = (*cap)->next;
		//free((*cap)->info.model);
		//free((*cap)->info.numeSofer);
		free((*cap));
		(*cap) = temp;
	}
}

// lsita dublu inlantuita
struct LD {
	LS* first;
	LS* last;
};
typedef struct LD LD;

void adaugareLD(LD* lista, Masina masinaNoua) {
	LS* p = (LS*)malloc(sizeof(LS));
	p->info = masinaNoua;
	p->next = NULL;
	p->prev = lista->last;
	if (lista->last != NULL)
	{
		lista->last->next = p;
	}
	else
	{
		lista->first = p;
	}
	lista->last = p;
}

void salvareInordineLD(Nod* radacina, LD* lista) {
	if (radacina != NULL) {
		salvareInordineLD(radacina->stanga, lista);
		adaugareLD(lista, radacina->info);
		salvareInordineLD(radacina->dreapta, lista);
	}
}

void salvarePreordineLD(Nod* radacina, LD* lista) {
	if (radacina != NULL) {
		adaugareLD(lista, radacina->info);
		salvarePreordineLD(radacina->stanga, lista);
		salvarePreordineLD(radacina->dreapta, lista);
	}
}

void salvarePostordineLD(Nod* radacina, LD* lista) {
	if (radacina != NULL) {
		salvarePostordineLD(radacina->stanga, lista);
		salvarePostordineLD(radacina->dreapta, lista);
		adaugareLD(lista, radacina->info);
	}
}

void afisareLD(LD lista) {
	LS* p = lista.first;
	while (p != NULL) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void dezalocareLD(LD* lista) {
	while (lista->first != NULL) {
		LS* aux = lista->first;
		//free(aux->info.model); 
		//free(aux->info.numeSofer);
		lista->first = lista->first->next;
		free(aux);
	}
	lista->last = NULL;
}

int main() {
	Nod* arbore=citireArboreDeMasiniDinFisier("masini.txt");
	afisareArboreDeMasini(arbore);

	// vector 
	Masina* vector = malloc(10 * sizeof(Masina));
	int index = 0;
	salvareInordineVector(arbore, vector, &index);
	printf("\n\nMasini salvate in vector: \n");
	for (int i = 0; i < index; i++)
		afisareMasina(vector[i]);
	free(vector);

	// LS
	LS* ls = NULL;
	salvarePostordineLS(arbore, &ls);
	printf("\n\nMasini salvate in lista simplu inlantuita: \n");
	afisareLS(ls);
	dezalocareLS(&ls);

	//LD
	LD ld;
	ld.first = NULL;
	ld.last = NULL;
	salvarePostordineLD(arbore, &ld);
	printf("\n\nMasini salvate in lista dublu inlantuita: \n");
	afisareLD(ld);
	dezalocareLD(&ld);

	dezalocareArboreDeMasini(&arbore);

	return 0;
}