#include<stdio.h>
#include<malloc.h>

struct Masina {
	int id;
	int nrKm;
	char* model;
	float pret;
	char tipMotor;
};

struct Masina initializare(int id, int nrKm, const char* model, float pret, char tipMotor) {
	struct Masina m;
	m.id = id;
	m.nrKm = nrKm;
	m.model = (char*)malloc(strlen(model) + 1);
	strcpy(m.model, model);
	m.pret = pret;
	m.tipMotor = tipMotor;
	return m;
}

void afisare(struct Masina m) {
	printf("Id: %d, nr km: %d, model: %s, pret: %f, tip motor: %c", m.id, m.nrKm, m.model, m.pret, m.tipMotor);
}

void afisareVector(struct Masina* vector, int nrElemente)
{
	for (int i = 0; i < nrElemente; i++)
	{
		afisare(vector[i]);
		printf("\n");
	}
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int* nrElementeCopiate)
{
	struct Masina* vectorNou; // declarare
	vectorNou = NULL; // initializare
	if (vector != NULL && (*nrElementeCopiate) > 0)
	{
		if (nrElemente < (*nrElementeCopiate))
		{
			*nrElementeCopiate = nrElemente;
		}
		vectorNou = malloc(sizeof(struct Masina) * (*nrElementeCopiate));
		for (int i = 0; i < (*nrElementeCopiate); i++)
		{
			vectorNou[i] = initializare(vector[i].id, vector[i].nrKm, vector[i].model, vector[i].pret, vector[i].tipMotor);
		}
	}
	return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrElemente)
{
	for (int i = 0; i < (*nrElemente); i++)
	{
		if ((*vector)[i].model != NULL)
		{
			free((*vector)[i].model); // dr + dp + dr + a
		}
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaMasiniIeftine(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune)
{
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].pret < prag)
		{
			(*dimensiune) += 1;
		}
	}
	int k = 0;
	*vectorNou = malloc(sizeof(struct Masina) * (*dimensiune));
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].pret < prag)
		{
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].model = malloc(sizeof(char) * (strlen(vector[i].model) + 1));
			strcpy_s((*vectorNou)[k].model, (strlen(vector[i].model) + 1), vector[i].model);
			k += 1;
		}
	}
}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* conditie)
{
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Masina m;
	m.id = -1;
	for (int i = 0; i < nrElemente; i++)
	{
		if (strcmp(vector[i].model, conditie) == 0)
		{
			return initializare(vector[i].id, vector[i].nrKm, vector[i].model, vector[i].pret, vector[i].tipMotor);
		}
	}

	return m;
}

int main()
{
	int n = 3;
	struct Masina* vec;
	vec = malloc(sizeof(struct Masina) * n);
	vec[0] = initializare(10, 100000, "Logan", 1000.4, 'D');
	vec[1] = initializare(11, 40000, "Passat", 2500.5, 'B');
	vec[2] = initializare(12, 200000, "Cyber Truck", 10000.0, 'E');
	afisareVector(vec, n);
	printf("\n");

	printf("Elemente copiate: \n");
	int nc = 1;
	struct Masina* vecNou = copiazaPrimeleNElemente(vec, n, &nc);
	afisareVector(vec, nc);
	printf("\n");
	 
	dezalocare(&vecNou, &nc);

	printf("Masini ieftine: \n");
	copiazaMasiniIeftine(vec, n, 3000.0, &vecNou, &nc);
	afisareVector(vecNou, nc);
	printf("\n");

	struct Masina v = getPrimulElementConditionat(vec, n, "Logan");
	if (v.id != -1)
	{
		printf("Masina gasita: ");
		afisare(v);
	}
	else printf("Nu s-a gasit masina cautata");

	dezalocare(&vec, &n);
	dezalocare(&vecNou, &nc);

	return 0;
}