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

struct Heap {
	int lungime;
	int nrELemente;
	Masina* masini;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	Heap nou;
	nou.lungime = lungime;
	nou.nrELemente = 0;
	nou.masini = (Masina*)malloc(sizeof(Masina) * lungime);
	return nou;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int fiuStanga = pozitieNod * 2 + 1;
	int fiuDreapta = pozitieNod * 2 + 2;
	int pozMax = pozitieNod;
	if (fiuStanga < heap.nrELemente && heap.masini[fiuStanga].id > heap.masini[pozMax].id) {
		pozMax = fiuStanga;
	}
	if (fiuDreapta < heap.nrELemente && heap.masini[fiuDreapta].id > heap.masini[pozMax].id) {
		pozMax = fiuDreapta;
	}
	if (pozMax != pozitieNod) {
		Masina aux = heap.masini[pozMax];
		heap.masini[pozMax] = heap.masini[pozitieNod];
		heap.masini[pozitieNod] = aux;
		if (pozMax <= (heap.nrELemente - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Heap h = initializareHeap(10);
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		h.masini[h.nrELemente++] = m;
	}
	for (int i = (h.nrELemente - 2) / 2; i >= 0; i--) {
		filtreazaHeap(h, i);
	}
	fclose(f);
	return h;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrELemente; i++) {
		afisareMasina(heap.masini[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrELemente; i < heap.lungime; i++) {
		afisareMasina(heap.masini[i]);
	}
}

Masina extrageMasina(void* h) {
	Masina gol = ((Heap*)h)->masini[0];
	((Heap*)h)->masini[0] = ((Heap*)h)->masini[((Heap*)h)->nrELemente - 1];
	((Heap*)h)->masini[((Heap*)h)->nrELemente - 1] = gol;
	((Heap*)h)->nrELemente--;
	for (int i = (((Heap*)h)->nrELemente - 2) / 2; i >= 0; i--) {
		filtreazaHeap(*((Heap*)h), i);
	}
	return gol;
}


void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->masini[i].model);
		free(heap->masini[i].numeSofer);
	}
	free(heap->masini);
	heap->masini = NULL;
	heap->lungime = 0;
	heap->nrELemente = 0;
}

int main() {
	Heap h = citireHeapDeMasiniDinFisier("masini_arbore.txt");
	afisareHeap(h);
	extrageMasina(&h);
	afiseazaHeapAscuns(h);
	dezalocareHeap(&h);
	return 0;
}