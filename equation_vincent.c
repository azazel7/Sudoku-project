#include "equation.h"
#include "header.h"


void displayDIMACS(struct grid *grid, char *filename)
{
	FILE *fichier = NULL;
	fichier = fopen(filename, "w+");
	fprintf(fichier, "cccccccccccccccccccccc\n");
	struct square *square = NULL;
	int nb_ligne = 0;
	int test_line = 1;
	// Clauses : Each group contains all symbols just once    
	for(int i = 0; i < grid->size_a_group; i++)
	{
		for(int k = 1; k <= grid->nb_symbole; k++)
		{
			for(int j = 0; j < grid->a_group[i].size; j++)
			{
				test_line = 0;
				/*
				 * If the size of the group is equal to the number of symbols 
				 */
				if(grid->a_group[i].size == grid->nb_symbole)
				{
					square = grid->a_group[i].a_square[j];
					//fprintf(fichier, "%d%d%d ",square->x, square->y,k);
					fprintf(fichier, "%d ", bijection_triplet(square->x, square->y, k, grid));
					test_line = 1;
				}
			}
			if(test_line == 1)
			{
				fprintf(fichier, "0\n");
				nb_ligne++;
			}

		}
	}

	fprintf(fichier, "ccccccccccccccccccc\n");

	// Clauses : We impose values for all boxes define with values in the file
	for(int i = 0; i < grid->size_a_group; i++)
	{
		for(int j = 0; j < grid->a_group[i].size; j++)
		{
			square = grid->a_group[i].a_square[j];
			if(square->value != EMPTY_VALUE)
			{
				fprintf(fichier, "%d 0\n", bijection_triplet(square->x, square->y, square->value, grid));
				//fprintf(fichier, "%d%d%d 0\n",square->x, square->y, square->value);
				nb_ligne++;
			}
		}
	}

	//Clauses : A box contain only one digit
	for(int i = 1; i <= grid->nb_column; i++)
	{
		for(int j = 1; j <= grid->nb_row; j++)
		{
			for(int k = 1; k <= grid->nb_symbole; k++)
			{
				for(int l = k + 1; l <= grid->nb_symbole; l++)
				{

					fprintf(fichier, "-%d -%d 0\n", bijection_triplet(i, j, k, grid), bijection_triplet(i, j, l, grid));
					//fprintf(fichier, "-%d%d%d -%d%d%d 0\n",i,j,k,i,j,l);
					nb_ligne++;
				}
			}
		}
	}
	fprintf(fichier, "ccccccccccccccccccc\n");

	// Sudoku comparison

	// We run through all boxex for all possible values
	for(int i = 1; i <= grid->nb_column; i++)
	{
		for(int j = 1; j <= grid->nb_row; j++)
		{
			for(int k = 1; k <= grid->nb_symbole; k++)
			{

				struct square *square = get_square(i, j, grid);
				int l = 0;

				//While the box contains relations
				while(square->a_relation[l].state != -1)
				{
					fprintf(fichier, "-%d ", bijection_triplet(i, j, k, grid));
					//fprintf(fichier, "-%d%d%d ",i,j,k); 
					int i_bis = k;
					// If the first member is inferior to the second one 
					if(square->a_relation[l].state == 0)
					{
						// We display for example 222 223 224...229
						while(i_bis + 1 <= grid->nb_symbole)
						{
							fprintf(fichier, "%d ", bijection_triplet(square->a_relation[l].neighbour->x, square->a_relation[l].neighbour->y, i_bis + 1, grid));
							// fprintf(fichier, "%d%d%d ",square->a_relation[l].neighbour->x, square->a_relation[l].neighbour->y, i_bis+1);
							i_bis++;
						}
					}
					// If the first member is superiorinferior to the second one 
					else if(square->a_relation[l].state == 1)
					{
						while(i_bis - 1 > 0)
						{
							fprintf(fichier, "%d ", bijection_triplet(square->a_relation[l].neighbour->x, square->a_relation[l].neighbour->y, i_bis - 1, grid));
							//fprintf(fichier, "%d%d%d ",square->a_relation[l].neighbour->x, square->a_relation[l].neighbour->y, i_bis-1);
							i_bis--;
						}

					}
					l++;
					fprintf(fichier, "0\n");
					nb_ligne++;
				}


			}
		}
	}
	fprintf(fichier, "ccccccccccccccccccccccccccccccccccccccc\n");

	// Management of killer-sudoku    
	for(int i = 0; i < grid->size_a_group; i++)
	{
		if(grid->a_group[i].value != 0)
		{
			nb_ligne += calcul_somme(grid, grid->a_group[i], fichier);
		}
	}


	fseek(fichier, 0, SEEK_SET);
	// We write in the first line of the DIMACS file
	fprintf(fichier, "p cnf %d %d \n", grid->nb_symbole * 111, nb_ligne);
	fclose(fichier);
}


// This function helps me  
int transformation(int x, int y, int z)
{
	return x * 100 + y * 10 + z;
}

int calcul_somme(struct grid *grid, struct group group, FILE * fichier)
{
	int somme = group.value;
	int taille = group.size;
	int nb_symbole = grid->nb_symbole;
	int nb_lignes = 0;
	int nb_succes = 0;
	unsigned int **toto = malloc(100 * sizeof(unsigned int *));
	int tab[taille];
	int first_value = 1;

	int idx_idx = 0;
	if(taille >= 3)
	{
		return calcul_somme_init(grid, group, fichier);	
	}
  /*----------------------------------------------------------------*/
	/*
	 * Storage of all possible combination in a 2d table 
	 */

	for(int i = 0; i < taille; i++)
	{
		tab[i] = 1;
	}

	while(tab[taille - 1] != nb_symbole + 1)
	{
		tab[0]++;
		for(int i = 0; i < taille; i++)
		{
			if((tab[i] == nb_symbole + 1) && (i != (taille - 1)))
			{
				tab[i] = 1;
				tab[i + 1]++;
			}
		}
		int res = 0;


		for(int i = 0; i < taille; i++)
		{
			res = res + tab[i];
		}

		if(res == somme && tab[taille - 1] != nb_symbole + 1)
		{
			int test_unitaire = 1;
			for(int i = 0; i < taille - 1; i++)
			{
				for(int j = i + 1; j < taille; j++)
				{
					if(tab[i] == tab[j])
					{
						test_unitaire = 0;
					}
				}
			}
			if(test_unitaire == 1)
			{
				toto[nb_succes] = malloc(taille * sizeof(unsigned int));
				for(int i = 0; i < taille; i++)
				{
					group.a_square[i]->value = tab[i];
					//toto[nb_succes][i]=transformation(group.a_square[i]->x, group.a_square[i]->y, group.a_square[i]->value);
					toto[nb_succes][i] = bijection_triplet(group.a_square[i]->x, group.a_square[i]->y, group.a_square[i]->value, grid);
				}
				nb_succes++;
			}
		}

	}
  /*------------------------*/

	/*
	 * Transformation FND -> FNC 
	 */

	int tab_idx[nb_succes];

	// Filling of 0 in the table

	for(int i = 0; i < nb_succes; i++)
	{
		tab_idx[i] = 0;
	}

	int test_unit = 1;
	while(tab_idx[0] != taille)
	{
		for(int i = 0; i < nb_succes - 1; i++)
		{
			test_unit = 1;
			for(int j = i + 1; j < nb_succes; j++)
			{
				if(toto[i][tab_idx[i]] == toto[j][tab_idx[j]])
				{
					test_unit = 0;
				}
			}
			if(test_unit == 1)
			{
				fprintf(fichier, "%d ", toto[i][tab_idx[i]]);
			}
		}
		fprintf(fichier, "%d ", toto[nb_succes - 1][tab_idx[nb_succes - 1]]);


		fprintf(fichier, "0 \n");
		nb_lignes++;
		tab_idx[nb_succes - 1]++;

		for(int i = nb_succes - 1; i > 0; i--)
		{
			if(tab_idx[i] == taille)
			{
				tab_idx[i] = 0;
				tab_idx[i - 1]++;
			}
		}
	}

	for(int i = 0; i < nb_succes; i++)
	{
		free(toto[i]);
	}

	free(toto);
	return nb_lignes;
}

int calcul_somme_init(struct grid *grid, struct group group, FILE * fichier)
{
	unsigned int size;
	struct clause_somme* clause_somme = NULL;
	clause_somme = malloc(sizeof(struct clause_somme));
	clause_somme->clause_somme = NULL;
	clause_somme->size_clause_some = 0;
	clause_somme->tab_size = NULL;
	clause_somme->sens = NULL;
	calcul_somme_recursif(grid, group, clause_somme);
	for(unsigned int i = 0; i < clause_somme->size_clause_some; i++)
	{
		for(int j = 0;  j < clause_somme->tab_size[i]; j++)
		{
			if(clause_somme->sens[i][j] < 0)
			{
				fprintf(fichier, "-");
			}
			fprintf(fichier, "%d ", clause_somme->clause_somme[i][j]);
		}
		fprintf(fichier, "0\n");
	}
	size = clause_somme->size_clause_some;
	for(unsigned int i = 0; i < clause_somme->size_clause_some; i++)
	{
		free(clause_somme->clause_somme[i]);
		free(clause_somme->sens[i]);
	}
	free(clause_somme->clause_somme);
	free(clause_somme->sens);
	free(clause_somme->tab_size);
	free(clause_somme);
	return size;
}
int calcul_somme_recursif(struct grid *grid, struct group group, struct clause_somme* clause_somme)
{
	int first_value = 1;
	unsigned int nb_line_add = 0;
	unsigned int position_courante = clause_somme->size_clause_some;
	if(group.size > 3)
	{
		for(first_value = 1; first_value < grid->nb_symbole; first_value++)
		{
			group.size--;
			group.a_square++;
			group.value -= first_value;
			calcul_somme_recursif(grid, group, clause_somme);
			group.value += first_value;
			group.a_square--;
			group.size++;
			//compter le nb de ligne ajouté
			for(;position_courante < clause_somme->size_clause_some; position_courante++)
			{
				//Pour chacune de ces lignes, ajouter une case, et y mettre
				clause_somme->clause_somme[position_courante] = 
				realloc(clause_somme->clause_somme[position_courante], (clause_somme->tab_size[position_courante] + 1)*sizeof(unsigned int));
				clause_somme->sens[position_courante] =
				realloc(clause_somme->sens[position_courante], (clause_somme->tab_size[position_courante] + 1)*sizeof(char));
				clause_somme->tab_size[position_courante] += 1;
				//La bijection
				clause_somme->clause_somme[position_courante][clause_somme->tab_size[position_courante] - 1] =
				bijection_triplet(group.a_square[0]->x, group.a_square[0]->y, first_value, grid);
				clause_somme->sens[position_courante][clause_somme->tab_size[position_courante] - 1] = -1;
			}
		}
	}
	else if(group.size == 3)
	{
		calcul_somme_size_3(grid, group, clause_somme);	
	}
}

int calcul_somme_size_3(struct grid *grid, struct group group, struct clause_somme* clause_somme)
{
	int somme = group.value;
	int taille = group.size;
	int nb_symbole = grid->nb_symbole;
	int nb_lignes = 0;
	int nb_succes = 0;
	unsigned int **toto = malloc(100 * sizeof(unsigned int *));
	int tab[taille];
	int first_value = 1;
	int tab_bis[2];

	int idx_idx = 0;

	while(first_value != nb_symbole + 1)
	{
		nb_succes = 0;
		tab_bis[0] = 1;
		tab_bis[1] = 1;
		while(tab_bis[1] != nb_symbole + 1)
		{
			tab_bis[0]++;
			for(int i = 0; i < 2; i++)
			{
				if((tab_bis[i] == nb_symbole + 1) && (i != 1))
				{
					tab_bis[i] = 1;
					tab_bis[i + 1]++;
				}
			}
			int res = 0;


			for(int i = 0; i < 2; i++)
			{
				res = res + tab_bis[i];
			}

			if(res == somme - first_value && tab_bis[1] != nb_symbole + 1)
			{
				int test_unitaire = 1;
				for(int i = 0; i < 1; i++)
				{
					for(int j = i + 1; j < 2; j++)
					{
						if(tab_bis[i] == tab_bis[j])
						{
							test_unitaire = 0;
						}
					}
				}
				if(test_unitaire == 1)
				{
					toto[nb_succes] = malloc(2 * sizeof(unsigned int));
					for(int i = 0; i < 2; i++)
					{
						group.a_square[i + 1]->value = tab_bis[i];
						toto[nb_succes][i] = bijection_triplet(group.a_square[i + 1]->x, group.a_square[i + 1]->y, group.a_square[i + 1]->value, grid);
					}
					nb_succes++;
				}
			}

		}

		int tab_idx[nb_succes];
		for(int i = 0; i < nb_succes; i++)
		{
			tab_idx[i] = 0;
		}

		int test_unit = 1;
		//Add line
		unsigned int position_courante;
		while(tab_idx[0] != 2 && nb_succes != 0)
		{
			clause_somme->clause_somme = realloc(clause_somme->clause_somme, (clause_somme->size_clause_some + 1)*sizeof(unsigned int*));
			clause_somme->sens = realloc(clause_somme->sens, (clause_somme->size_clause_some + 1)*sizeof(char*));
			clause_somme->tab_size = realloc(clause_somme->tab_size, (clause_somme->size_clause_some + 1)*sizeof(unsigned int));
			clause_somme->sens[clause_somme->size_clause_some] = NULL;
			clause_somme->clause_somme[clause_somme->size_clause_some] = NULL;
			clause_somme->tab_size[clause_somme->size_clause_some] = 0;
			clause_somme->size_clause_some++;
			position_courante = clause_somme->size_clause_some - 1;

			//Ajout le pt fixe puis les deux autres
			clause_somme->clause_somme[position_courante] = 
				realloc(clause_somme->clause_somme[position_courante], (clause_somme->tab_size[position_courante] + 1)*sizeof(unsigned int));
			clause_somme->sens[position_courante] =
				realloc(clause_somme->sens[position_courante], (clause_somme->tab_size[position_courante] + 1)*sizeof(char));
			clause_somme->tab_size[position_courante] += 1;
			//La bijection
			clause_somme->clause_somme[position_courante][clause_somme->tab_size[position_courante] - 1] =
				bijection_triplet(group.a_square[0]->x, group.a_square[0]->y, first_value, grid);
			clause_somme->sens[position_courante][clause_somme->tab_size[position_courante] - 1] = -1;
			for(int i = 0; i < nb_succes - 1; i++)
			{
				test_unit = 1;
				for(int j = i + 1; j < nb_succes; j++)
				{
					if(toto[i][tab_idx[i]] == toto[j][tab_idx[j]])
					{
						test_unit = 0;
					}
				}
				if(test_unit == 1)
				{
					//ajouter une case, et on écrit la valeur
					//fprintf(fichier, "%d ", toto[i][tab_idx[i]]);
					clause_somme->clause_somme[position_courante] = 
					realloc(clause_somme->clause_somme[position_courante], (clause_somme->tab_size[position_courante] + 1)*sizeof(unsigned int));
					clause_somme->sens[position_courante] = 
					realloc(clause_somme->sens[position_courante], (clause_somme->tab_size[position_courante] + 1)*sizeof(char));
					clause_somme->tab_size[position_courante] += 1;
					//La bijection
					clause_somme->clause_somme[position_courante][clause_somme->tab_size[position_courante] - 1] =toto[i][tab_idx[i]];
					clause_somme->sens[position_courante][clause_somme->tab_size[position_courante] - 1] = 1;
				}
			}

					//ajouter une case, et on écrit la valeur

			//Pour chacune de ces lignes, ajouter une case, et y mettre
			clause_somme->clause_somme[position_courante] = 
			realloc(clause_somme->clause_somme[position_courante], (clause_somme->tab_size[position_courante] + 1)*sizeof(unsigned int));
			clause_somme->sens[position_courante] = realloc(clause_somme->sens[position_courante], (clause_somme->tab_size[position_courante] + 1)*sizeof(char));
			clause_somme->tab_size[position_courante] += 1;
			//La bijection
			clause_somme->clause_somme[position_courante][clause_somme->tab_size[position_courante] - 1] =toto[nb_succes - 1][tab_idx[nb_succes - 1]];
			clause_somme->sens[position_courante][clause_somme->tab_size[position_courante] - 1] = 1;

			
			nb_lignes++;
			tab_idx[nb_succes - 1]++;

			for(int i = nb_succes - 1; i > 0; i--)
			{
				if(tab_idx[i] == 2)
				{
					tab_idx[i] = 0;
					tab_idx[i - 1]++;
				}
			}
		}
		for(int i = 0; i < nb_succes; i++)
		{
			free(toto[i]);
		}

		first_value++;
	}
	free(toto);
	return nb_lignes;
}
