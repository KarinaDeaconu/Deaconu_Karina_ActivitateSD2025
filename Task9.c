#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Pacient{
	int id;
	char* nume;
	int varsta;
	int gradUrgenta;
};
typedef struct Pacient Pacient;

struct Heap {
	int lungime;
	Pacient* pacienti;
	int nrPacienti;
};
typedef struct Heap Heap;

Pacient citirePacientDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Pacient p;
	aux = strtok(buffer, sep);
	p.id = atoi(aux);

	aux = strtok(NULL, sep);
	p.nume = malloc(strlen(aux) + 1);
	strcpy_s(p.nume, strlen(aux) + 1, aux);

	p.varsta = atoi(strtok(NULL, sep));
	p.gradUrgenta = atoi(strtok(NULL, sep));
	return p;
}

void afisarePacient(Pacient pacient) {
	printf("Id: %d\n", pacient.id);
	printf("Nume: %s\n", pacient.nume);
	printf("Varsta: %d\n", pacient.varsta);
	printf("Grad de urgenta: %d\n\n", pacient.gradUrgenta);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrPacienti = 0;
	heap.pacienti = (Pacient*)malloc(sizeof(Pacient) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int fiuSt = 2 * pozitieNod + 1;
	int fiuDr = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (fiuSt < heap.nrPacienti && heap.pacienti[fiuSt].gradUrgenta > heap.pacienti[pozMax].gradUrgenta) {
		pozMax = fiuSt;
	}
	if (fiuDr < heap.nrPacienti && heap.pacienti[fiuDr].gradUrgenta > heap.pacienti[pozMax].gradUrgenta) {
		pozMax = fiuDr;
	}
	if (pozMax != pozitieNod) {
		Pacient aux = heap.pacienti[pozMax];
		heap.pacienti[pozMax] = heap.pacienti[pozitieNod];
		heap.pacienti[pozitieNod] = aux;
		if (pozMax <= (heap.nrPacienti - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDePacientiDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(5);
	while (!feof(file)) {
		Pacient p = citirePacientDinFisier(file);
		if (p.id != 0)
			heap.pacienti[heap.nrPacienti++] = p;
	}
	for (int i = (heap.nrPacienti - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}
	fclose(file);
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrPacienti; i++)
		afisarePacient(heap.pacienti[i]);
}

void afisareHeapAscuns(Heap heap) {
	for (int i = heap.nrPacienti; i < heap.lungime; i++)
		afisarePacient(heap.pacienti[i]);
}

Pacient extragePacient(Heap* heap) {
	if (heap->nrPacienti > 0) {
		Pacient aux = heap->pacienti[0];
		heap->pacienti[0] = heap->pacienti[heap->nrPacienti - 1];
		heap->pacienti[heap->nrPacienti - 1] = aux;
		heap->nrPacienti--;
		for (int i = (heap->nrPacienti - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}
		return aux;
	}
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->nrPacienti; i++) {
		if (heap->pacienti[i].nume != NULL) {
			free(heap->pacienti[i].nume);
		}
	}
	free(heap->pacienti);
	heap->pacienti = NULL;
	heap->lungime = 0;
	heap->nrPacienti = 0;
}

int main() {
	Heap heap = citireHeapDePacientiDinFisier("pacienti.txt");
	
	printf("Toti pacientii: \n");
	afisareHeap(heap);

	printf("Pacientul cu cel mai mare grad de urgenta: \n");
	afisarePacient(extragePacient(&heap));
	
	dezalocareHeap(&heap);
	return 0;
}