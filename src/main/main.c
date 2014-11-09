#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "parseFile.h"
#include "solverSAT.h"

void solve_sudoku(char *file_name);
void print_grid(struct grid *grid);
void displayDIMACS(struct grid *grid, char *filename);

void test_bijection(void)
{
	struct grid grid;
	grid.nb_symbole = 9;
	unsigned int init = 1, offset = grid.nb_symbole;
	unsigned int x, y, z, number;
	for(unsigned int i = init; i < init + offset; i++)
	{
		for(unsigned int j = init; j < init + offset; j++)
		{
			for(unsigned int k = init; k < init + offset; k++)
			{
				number = bijection_triplet(i, j, k, &grid);
				bijection_triplet_inverse(&x, &y, &z, number, &grid);
				if(i != x || j != y || k != z)
				{
					printf("[-](%u,%u,%u) -> %u -> (%u,%u,%u)\n", i, j, k, number, x, y, z);
				}
				else
				{
					printf("Good\n");
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{

	printf("[i] Beginning\n");
	unsigned int x, y, value, number;
	struct grid grid;
	grid.nb_symbole = 4;
	if(argc < 2)
	{
		printf("Usage : %s <file1> <file2> ...\n", argv[0]);
		return 0;
	}

	for(int i = 1; i < argc; i++)
	{
		solve_sudoku(argv[i]);
	}


	printf("[i] End\n");
	return 0;
}

void print_grid(struct grid *grid)
{
	struct square *square = NULL;
	for(int x = 1; x <= grid->nb_column; x++)
	{
		if(x == 1)
		{
			printf("-");
		}
		printf("----");
	}
	printf("\n");
	for(int y = 1; y <= grid->nb_row; y++)
	{
		for(int x = 1; x <= grid->nb_column; x++)
		{
			if(x == 1)
			{
				printf("|");
			}
			if((square = get_square(x, y, grid)) != NULL)
			{
				printf(" %d |", square->value);
			}
		}
		printf("\n");
		for(int x = 1; x <= grid->nb_column; x++)
		{
			if(x == 1)
			{
				printf("-");
			}
			printf("----");
		}
		printf("\n");
	}
	/*
	 * for(int i = 0; i < grid->size_a_all_square; i++)
	 * {
	 * printf("[%u] (%u,%u) = %u\n", i, grid->a_all_square[i].x, grid->a_all_square[i].y, grid->a_all_square[i].value);
	 * }
	 * for(int i = 0; i < grid->size_a_group; i++)
	 * {
	 * struct group group = grid->a_group[i];
	 * if(group.value > 0)
	 * {
	 * printf ( "%u ", group.value );
	 * }
	 * for(int j = 0; j < group.size; j++)
	 * {
	 * struct square *square = group.a_square[j];
	 * printf ( "(%u,%u) ", square->x, square->y );
	 * }
	 * printf ( "\n" );
	 * }
	 */
}

void solve_sudoku(char *file_name)
{
	printf("[i] Work on %s\n", file_name);
	struct grid *grid = NULL;
	if((grid = parse_file(file_name)) == NULL)
	{
		printf("[-] Error to parse grid\n");
		return;
	}
	/*
	 * print_grid(grid);
	 */
	displayDIMACS(grid, "sortieDimacs");
	call_glucose("sortieDimacs", "sortieGlucose", "./gluco");
	if(!fill_grid_with_result("sortieGlucose", grid))
	{
		if(check_grid(grid) == 0)
		{
			printf ( "[-] Error in the solution\n" );
		}
		print_grid(grid);
	}
	else
	{
		printf("[-] There is no solution\n");
	}
	delete_grid(grid);
	grid = NULL;
}
