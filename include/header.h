#ifndef  header_INC
#define  header_INC

struct relation
{
	struct square *neighbour;
	int state;					// 0 if square[i] < square[i]->a_relation[i]->neighbour et 1 sinon
};

struct square
{
	unsigned int x;
	unsigned int y;
	unsigned int value;
	struct relation a_relation[5];
	unsigned int size_a_relation;
#define EMPTY_VALUE 0
};
struct group
{
	struct square **a_square;
	unsigned int size;
	unsigned int value;
};

struct clause_somme
{
	unsigned int **clause_somme;
	unsigned int size_clause_some;
	unsigned int *tab_size;
	char **sens;
};

struct grid
{
	struct group *a_group;
	unsigned int size_a_group;
	struct square *a_all_square;
	unsigned int size_a_all_square;
	unsigned int nb_symbole;
	unsigned int nb_row;
	unsigned int nb_column;
};
#endif
