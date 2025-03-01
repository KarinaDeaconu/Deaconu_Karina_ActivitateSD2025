#include<stdio.h>
#include<stdlib.h>

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
	printf("Id: %d \n", m.id);
	printf("Numar km: %d \n", m.nrKm);
	printf("Model: %s \n", m.model);
	printf("Pret: %f \n", m.pret);
	printf("Tip motor: %c \n", m.tipMotor);

	// printf("Id: %d, Numar km: %d, Model: %s, Pret: %f, Tip motor: %c", m.id, m.nrKm, m.model, m.pret, m.tipMotor);
}

void modifica_Pret(struct Masina* m, float pretNou) {
	if (pretNou > 0)
		m->pret = pretNou; // (*m).pret
}

void dezalocare(struct Masina* m) {
	free(m->model);
	m->model = NULL;
}

int main() {
	struct Masina masina;
	masina = initializare(1, 50000, "Logan", 3000.50, 'B');
	afisare(masina);
	modifica_Pret(&masina, 4000);
	afisare(masina);
	dezalocare(&masina);
	return 0;
}