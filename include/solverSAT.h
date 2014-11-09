#ifndef  solverSAT_INC
#define  solverSAT_INC
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "parseFile.h"

char call_glucose(char *filename_input, char *filemane_output, char *glucose_exe);

char fill_grid_with_result(char *filename, struct grid *grid);

char parse_line_result(char *line, struct grid *grid);

unsigned int bijection_triplet(unsigned int x, unsigned int y, unsigned int value, struct grid *grid);

void bijection_triplet_inverse(unsigned int *x, unsigned int *y, unsigned int *value, unsigned int number, struct grid *grid);

char check_grid(struct grid *grid);

char check_group(struct group group);

char check_square(struct square *square, struct grid *grid);
#endif
