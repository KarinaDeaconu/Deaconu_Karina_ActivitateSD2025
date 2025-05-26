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
	struct Nod* dr;
	struct Nod* st;
};
typedef struct Nod Nod;

Masina citirMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m;
	aux = strtok(buffer, sep);
	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m.model = malloc(strlen(aux) + 1);
	strcpy_s(m.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);

	m.serie = *strtok(NULL, sep);
	return m;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** nod, Masina masinaNoua) {
	if ((*nod) != NULL) {
		if (masinaNoua.id > (*nod)->info.id) {
			adaugaMasinaInArbore(&(*nod)->dr, masinaNoua);
		}
		else if (masinaNoua.id < (*nod)->info.id) {
			adaugaMasinaInArbore(&(*nod)->st, masinaNoua);
		}
	}
	else {
		(*nod) = (Nod*)malloc(sizeof(Nod));
		(*nod)->info = masinaNoua;
		(*nod)->st = NULL;
		(*nod)->dr = NULL;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* nod = NULL;
	while (!feof(f)) {
		adaugaMasinaInArbore(&nod, citirMasinaDinFisier(f));
	}
	fclose(f);
	return nod;
}

void afisarePreOrdineRSD(Nod* radacina) {
	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisarePreOrdineRSD(radacina->st);
		afisarePreOrdineRSD(radacina->dr);
	}
}

void afisareInOrdineSRD(Nod* radacina) {
	if (radacina != NULL) {
		afisareInOrdineSRD(radacina->st);
		afisareMasina(radacina->info);
		afisareInOrdineSRD(radacina->dr);
	}
}

void afisarePostOrdineSDR(Nod* radacina) {
	if (radacina != NULL) {
		afisarePostOrdineSDR(radacina->st);
		afisarePostOrdineSDR(radacina->dr);
		afisareMasina(radacina->info);
	}
}

void afisareMasiniDinArbore(Nod* radacina) {
	printf("Afisare inordine (SRD):\n");
	afisareInOrdineSRD(radacina);

	printf("\nAfisare preordine (RSD):\n");
	afisarePreOrdineRSD(radacina);

	printf("\nAfisare postordine (SDR):\n");
	afisarePostOrdineSDR(radacina);
}

void dezalocareArboreDeMasini(Nod** radacina) {
	if (*radacina != NULL) {
		dezalocareArboreDeMasini(&(*radacina)->st);
		dezalocareArboreDeMasini(&(*radacina)->dr);
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free((*radacina));
		*radacina = NULL;
	}
}

int determinarNumarNoduri(Nod* radacina) {
	int nrNoduri = 0;
	if (radacina != NULL) {
		nrNoduri += determinarNumarNoduri(radacina->st);
		nrNoduri += determinarNumarNoduri(radacina->dr);
		nrNoduri += 1;
	}
	return nrNoduri;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina != NULL) {
		int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);
		return 1 + (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr);
	}
	return 0;
}

//Nod* stergeNod(Nod* radacina, int id) {
//	if (radacina == NULL)return 0;
//	if (id < radacina->info.id) {
//		radacina->st = stergeNod(radacina->st, id);
//	}
//	else if (id > radacina->info.id) {
//		radacina->dr = stergeNod(radacina->dr, id);
//	}
//	else {
//		if (radacina->st == NULL) {
//			Nod* aux = radacina->dr;
//			free(radacina->info.model);
//			free(radacina->info.numeSofer);
//			free(radacina);
//			return aux;
//		}
//		else if (radacina->dr == NULL) {
//			Nod* aux = radacina->st;
//			free(radacina->info.model);
//			free(radacina->info.numeSofer);
//			free(radacina);
//			return aux;
//		}
//		//
//	}
//	return radacina;
//}

Nod* subarboreMaiMare(Nod* radacina) {
	if (radacina != NULL) {
		int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);
		return inaltimeSt > inaltimeDr ? radacina->st : radacina->dr;
	}
	return 0;
}

int nrNoduriInSubordine(Nod* radacina) {
	if (radacina != NULL) {
		int nrSt = determinarNumarNoduri(radacina->st);
		int nrDr = determinarNumarNoduri(radacina->dr);
		return nrSt + nrDr;
	}
	return 0;
}

Nod* subarboreCuMaiMulteNoduri(Nod* radacina) {
	if (radacina != NULL) {
		int nrSt = determinarNumarNoduri(radacina->st);
		int nrDr = determinarNumarNoduri(radacina->dr);
		return nrSt > nrDr ? radacina->st : radacina->dr;
	}
	return 0;
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	
	printf("Afisare SRD: \n");
	afisareInOrdineSRD(arbore);

	printf("Numar noduri: %d\n", determinarNumarNoduri(arbore));

	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(arbore));

	//arbore = stergeNod(arbore, 5);
	//printf("Afisare SRD dupa stergere: \n");
	//afisareInOrdineSRD(arbore);

	Nod* a1 = subarboreMaiMare(arbore);
	printf("Subarborele cu inaltimea mai mare: %d\n", a1->info.id);

	printf("Numarul de noduri aflate in subordine: %d\n", nrNoduriInSubordine(arbore));

	Nod* a2 = subarboreCuMaiMulteNoduri(arbore);
	printf("Subarbore cu mai multi descendenti: %d\n", a2->info.id);

	dezalocareArboreDeMasini(&arbore);
	return 0;
}