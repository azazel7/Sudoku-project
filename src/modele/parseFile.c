#include "parseFile.h"

struct grid *parse_file(char *file_name)
{
	struct grid *grid = NULL;
	FILE *fd = NULL;
	char type = 0;
	//Open file and create grid
	fd = fopen(file_name, "r");
	if(fd == NULL)
	{
		printf("[-] Unable to open file\n");
		return NULL;
	}
	if((grid = malloc(sizeof(struct grid))) == NULL)
	{
		fclose(fd);
		return NULL;
	}
	grid->a_group = NULL;
	grid->size_a_group = 0;
	grid->a_all_square = NULL;
	grid->size_a_all_square = 0;
	grid->nb_symbole = 0;

	parse_file_information(fd, &grid->nb_row, &grid->nb_column, &type, &grid->nb_symbole);
	generate_all_square(grid, grid->nb_row, grid->nb_column);
	generate_group(type, grid);

	parse_file_type(fd, grid, &parse_line_value);
	parse_file_type(fd, grid, &parse_line_group);
	parse_file_type(fd, grid, &parse_line_relation);
	fclose(fd);
	return grid;
}

void parse_line_relation(char *line, struct grid *grid)
{
	if(line[0] != 'o')
	{
		return;
	}

	unsigned int x1, y1, x2, y2;
	unsigned char indic;
	struct square *square1 = NULL, *square2 = NULL;

	while((line = strchr(line, '(')) != NULL)
	{
		if(sscanf(line, "(%u,%u) %c (%u,%u)", &x1, &y1, &indic, &x2, &y2) == 5)
		{
			square1 = get_square(x1, y1, grid);
			square2 = get_square(x2, y2, grid);
			square1->size_a_relation++;
			square2->size_a_relation++;
			if(indic == '>')
			{
				square1->a_relation[square1->size_a_relation - 1].neighbour = square2;
				square1->a_relation[square1->size_a_relation - 1].state = 1;
				square2->a_relation[square2->size_a_relation - 1].neighbour = square1;
				square2->a_relation[square2->size_a_relation - 1].state = 0;
			}
			else if(indic == '<')
			{
				square1->a_relation[square1->size_a_relation - 1].neighbour = square2;
				square1->a_relation[square1->size_a_relation - 1].state = 0;
				square2->a_relation[square2->size_a_relation - 1].neighbour = square1;
				square2->a_relation[square2->size_a_relation - 1].state = 1;
			}

		}
		else
		{
			printf("[-] Error parsing\n");
		}
		line++;
		line = strchr(line, '(') + 1;
	}
}

void parse_line_group(char *line, struct grid *grid)
{
	if(line[0] != 'u')
	{
		return;
	}

	unsigned int x, y;
	struct square *square = NULL;
	grid->size_a_group++;
	grid->a_group = realloc(grid->a_group, (grid->size_a_group) * sizeof(struct group));
	struct group *group = &grid->a_group[grid->size_a_group - 1];
	group->a_square = NULL;
	group->size = 0;
	group->value = EMPTY_VALUE;
	line = strchr(line, ' ');
	line++;
	if(line[0] != '(' && line[0] >= '0' && line[0] <= '9')
	{
		if(sscanf(line, "%u", &group->value) < 1)
		{
			group->value = EMPTY_VALUE;
		}
	}
	while((line = strchr(line, '(')) != NULL)
	{
		if(sscanf(line, "(%u,%u)", &x, &y) == 2)
		{
			group->size++;
			group->a_square = realloc(group->a_square, group->size * sizeof(struct square *));
			square = get_square(x, y, grid);
			group->a_square[group->size - 1] = square;
		}
		else
		{
			printf("[-] Error parsing\n");
		}
		line++;
	}
}

void parse_file_type(FILE * fd, struct grid *grid, void (*parse_line) (char *, struct grid *))
{
	rewind(fd);
	char eof = 0;
	char *line = NULL;
	while(eof == 0)
	{
		line = read_line(fd, &eof);
		parse_line(line, grid);
		free(line);
	}
}

void parse_line_value(char *line, struct grid *grid)
{
	if(line[0] != 's')
	{
		return;
	}
	if(count_characters(line, '(') > 1)
	{
		line[0] = 'u';
		parse_line_group(line, grid);
		return;
	}

	unsigned int offset_line = 0;
	unsigned int value = EMPTY_VALUE, x, y;
	struct square *square = NULL;
	if(sscanf(line, "s %u ", &value) < 1)
	{
		return;
	}

	while((line = strchr(line, '(')) != NULL)
	{
		if(sscanf(line, "(%u,%u)", &x, &y) == 2)
		{
			if((square = get_square(x, y, grid)) != NULL)
			{
				square->value = value;
			}
		}
		else
		{
			printf("[-] Error parsing\n");
		}
		line++;
	}
}

char parse_file_information(FILE * fd, unsigned int *nb_row, unsigned int *nb_column, char *type, unsigned int *nb_symbole)
{
	rewind(fd);
	char line[50] = { 0 };
	printf("[i] Parse informations\n");
	if(fgets(line, 50, fd) != NULL)
	{
		if(sscanf(line, "%d %d %d", nb_symbole, nb_row, nb_column) < 3)
		{
			return -1;
		}
	}
	if(fgets(line, 50, fd) == NULL)
	{
		return -1;
	}
	if(strstr(line, "jigsaw") != NULL)
	{
		*type = 1;
	}
	printf("[i] %d symboles, %d lines, %d column\n", *nb_symbole, *nb_row, *nb_column);
	return 0;
}

char generate_all_square(struct grid *grid, unsigned int nb_row, unsigned int nb_column)
{
	unsigned int index = 0;
	grid->a_all_square = malloc(sizeof(struct square) * nb_column * nb_row);
	if(grid->a_all_square == NULL)
	{
		return -1;
	}
	grid->size_a_all_square = nb_column * nb_row;
	for(int x = 1; x <= nb_column; x++)
	{
		for(int y = 1; y <= nb_row; y++)
		{
			grid->a_all_square[index].x = x;
			grid->a_all_square[index].y = y;
			grid->a_all_square[index].size_a_relation = 0;
			for(int i = 0; i < 5; i++)
			{
				grid->a_all_square[index].a_relation[i].state = -1;
			}
			grid->a_all_square[index].value = EMPTY_VALUE;
			index++;
		}
	}
	return 0;
}

void delete_grid(struct grid *grid)
{
	if(grid != NULL)
	{
		empty_grid(grid);
		free(grid);
	}
}

void empty_grid(struct grid *grid)
{
	if(grid != NULL)
	{
		if(grid->a_group != NULL)
		{
			for(int i = 0; i < grid->size_a_group; i++)
			{
				empty_group(grid->a_group[i]);
			}
			free(grid->a_group);
			grid->a_group = NULL;
			grid->size_a_group = 0;
		}
		if(grid->a_all_square != NULL)
		{
			free(grid->a_all_square);
			grid->a_all_square = NULL;
			grid->size_a_all_square = 0;
		}

	}
}

void empty_group(struct group group)
{
	if(group.a_square != NULL)
	{
		free(group.a_square);
	}
	group.a_square = NULL;
	group.size = 0;
}

char *read_line(FILE * fd, char *eof)
{
	short letter = 0;
	char *line = NULL;
	unsigned short size = 0;
	while((letter = fgetc(fd)) != '\n')
	{
		if(letter == EOF)
		{
			*eof = 1;
			break;
		}
		size++;
		line = realloc(line, size * sizeof(char));
		line[size - 1] = letter;
	}
	line = realloc(line, (size + 1) * sizeof(char));
	line[size] = 0;

	return line;
}

struct square *get_square(unsigned int x, unsigned int y, struct grid *grid)
{
	if(x > grid->nb_column || y > grid->nb_row || x <= 0 || y <= 0)
	{
		return NULL;
	}
	x--;
	y--;
	//In the file, x and y begin at 1
	return grid->a_all_square + (x * grid->nb_row + y);
}

unsigned int count_characters(char *str, char character)
{
	unsigned int count = 0;
	do
	{
		if(*str == character)
		{
			count++;
		}
	} while(*(str++));
	return count;
}

void generate_group(char type, struct grid *grid)
{
	struct group *group = NULL;
	struct square *square = NULL;
	int size = grid->nb_symbole;
	for(int i = 1; i <= size; i++)
	{
		grid->size_a_group++;
		grid->a_group = realloc(grid->a_group, (grid->size_a_group) * sizeof(struct group));
		group = &grid->a_group[grid->size_a_group - 1];
		group->a_square = NULL;
		group->size = 0;
		group->value = EMPTY_VALUE;
		for(int j = 1; j <= size; j++)
		{
			group->size++;
			group->a_square = realloc(group->a_square, group->size * sizeof(struct square *));
			square = get_square(i, j, grid);
			group->a_square[group->size - 1] = square;
		}
	}
	for(int i = 1; i <= size; i++)
	{
		grid->size_a_group++;
		grid->a_group = realloc(grid->a_group, (grid->size_a_group) * sizeof(struct group));
		group = &grid->a_group[grid->size_a_group - 1];
		group->a_square = NULL;
		group->size = 0;
		group->value = EMPTY_VALUE;
		for(int j = 1; j <= size; j++)
		{
			group->size++;
			group->a_square = realloc(group->a_square, group->size * sizeof(struct square *));
			square = get_square(j, i, grid);
			group->a_square[group->size - 1] = square;
		}
	}

	if(type != 1)				// ! Jigsaw
	{
		int square_root = sqrt(size);
		for(int k = 0; k < square_root; k++)
		{
			for(int l = 0; l < square_root; l++)
			{
				grid->size_a_group++;
				grid->a_group = realloc(grid->a_group, (grid->size_a_group) * sizeof(struct group));
				group = &grid->a_group[grid->size_a_group - 1];
				group->a_square = NULL;
				group->size = 0;
				group->value = EMPTY_VALUE;
				for(int i = 1; i <= square_root; i++)
				{
					for(int j = 1; j <= square_root; j++)
					{
						group->size++;
						group->a_square = realloc(group->a_square, group->size * sizeof(struct square *));
						square = get_square(square_root * k + i, square_root * l + j, grid);
						group->a_square[group->size - 1] = square;
					}
				}
			}
		}
	}
}
