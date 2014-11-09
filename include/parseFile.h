#ifndef  parseFile_INC
#define  parseFile_INC

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "header.h"

struct grid *parse_file(char *file_name);

void parse_line_group(char *line, struct grid *grid);

void parse_line_value(char *line, struct grid *grid);

void parse_line_relation(char *line, struct grid *grid);

void parse_file_type(FILE * fd, struct grid *grid, void (*parse_line) (char *, struct grid *));

char parse_file_information(FILE * fd, unsigned int *nb_row, unsigned int *nb_column, char *type, unsigned int *nb_symbole);

char generate_all_square(struct grid *grid, unsigned int nb_row, unsigned int nb_column);

void delete_grid(struct grid *grid);

void empty_grid(struct grid *grid);

void empty_group(struct group group);

char *read_line(FILE * fd, char *eof);

struct square *get_square(unsigned int x, unsigned int y, struct grid *grid);

unsigned int count_characters(char *str, char character);

void generate_group(char type, struct grid *grid);
#endif
