#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "solverSAT.h"

void displayDIMACS(struct grid *grid, char *filename);

int calcul_somme(struct grid *grid, struct group group, FILE * fichier);
int calcul_somme_size_3(struct grid *grid, struct group group, struct clause_somme* clause_somme);
int calcul_somme_recursif(struct grid *grid, struct group group, struct clause_somme* clause_somme);
int calcul_somme_init(struct grid *grid, struct group group, FILE * fichier);
