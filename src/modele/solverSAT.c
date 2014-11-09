#include "solverSAT.h"

char call_glucose(char *filename_input, char *filemane_output, char *glucose_executable)
{
	unsigned int size_command_line = 100 + 17 + strlen(glucose_executable) + strlen(filemane_output) + strlen(glucose_executable);
	char *command_line = malloc(sizeof(char) * size_command_line);
	if(command_line == NULL)
	{
		return -1;
	}
	memset(command_line, 0, size_command_line * sizeof(char));
	sprintf(command_line, "%s %s > %s 2>/dev/null", glucose_executable, filename_input, filemane_output);
	system(command_line);
	free(command_line);
	command_line = NULL;
	return 0;
}

char fill_grid_with_result(char *filename, struct grid *grid)
{
	char eof = 0, retour = 0;
	char *line = NULL;
	FILE *file = fopen(filename, "r");

	if(file == NULL)
	{
		return -1;
	}
	while(eof == 0)
	{
		line = read_line(file, &eof);
		if(line != NULL)
		{

			if(line[0] == 's')
			{
				if(strstr(line, "UNSATISFIABLE") != NULL)
				{
					retour = -1;
					free(line);
					break;
				}
			}
			else if(line[0] == 'v')
			{
				parse_line_result(line, grid);
			}
			free(line);
		}
	}
	return retour;
}

char parse_line_result(char *line, struct grid *grid)
{
	unsigned int x, y, value, number;
	struct square *square = NULL;

	while(line != NULL)
	{
		line = strstr(line, " ");
		if(line != NULL)
		{
			line++;
			if(line[0] == '-')
			{
				continue;
			}
			else if(line[0] != '0')
			{
				sscanf(line, "%u", &number);
				bijection_triplet_inverse(&x, &y, &value, number, grid);
				square = get_square(x, y, grid);
				if(square != NULL)
				{
					square->value = value;
				}
			}
		}
	}
	return 0;
}

unsigned int bijection_triplet(unsigned int x, unsigned int y, unsigned int value, struct grid *grid)
{
	assert(x > 0);
	assert(y > 0);
	assert(value > 0);
	assert(x <= grid->nb_symbole);
	assert(y <= grid->nb_symbole);
	assert(value <= grid->nb_symbole);

	return (x - 1) * grid->nb_symbole * grid->nb_symbole + (y - 1) * grid->nb_symbole + (value - 1) + 1;
}

void bijection_triplet_inverse(unsigned int *x, unsigned int *y, unsigned int *value, unsigned int number, struct grid *grid)
{
	assert(number > 0);
	assert(grid != NULL);
	assert(grid->nb_symbole > 0);
	number--;
	*value = number % grid->nb_symbole;
	*y = ((number - (*value)) % (grid->nb_symbole * grid->nb_symbole)) / grid->nb_symbole;
	*x = (number - (*value) - (*y)) / (grid->nb_symbole * grid->nb_symbole);
	*y = (*y) + 1;
	*x = (*x) + 1;
	*value = (*value) + 1;
}

char check_grid(struct grid *grid)
{
	for(int i = 0; i < grid->size_a_group; i++)
	{
		if(check_group(grid->a_group[i]) == 0)
		{
			return 0;
		}
	}
	for(int i = 0; i < grid->size_a_all_square; i++)
	{
		if(check_square(&(grid->a_all_square[i]), grid) == 0)
		{
			return 0;
		}
	}
	return 1;
}

char check_group(struct group group)
{
	unsigned int sum = 0;
	if(group.value != EMPTY_VALUE)
	{
		for(int i = 0; i < group.size; i++)
		{
			sum += group.a_square[i]->value;
		}
		if(sum != group.value)
		{
			return 0;
		}
	}
	for(int i = 0; i < group.size; i++)
	{
		for(int j = 0; j < group.size; j++)
		{
			if(i != j)
			{
				if(group.a_square[i]->value == group.a_square[j]->value)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

char check_square(struct square *square, struct grid *grid)
{
	if(square->value == EMPTY_VALUE || square->value > grid->nb_symbole)
	{
		return 0;
	}
	char i = 0;
	int state;
	struct square *neig = NULL;
	while(square->a_relation[i].state != -1)
	{
		state = square->a_relation[i].state;
		neig = square->a_relation[i].neighbour;
		if(state == 0 && neig->value < square->value)
		{
			return 0;
		}
		else if(state == 1 && neig->value > square->value)
		{
			return 0;
		}
		i++;
	}
	return 1;
}
