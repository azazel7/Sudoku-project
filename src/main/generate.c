#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("[-] I need a file\n");
		return 0;
	}

	char *filename = argv[1];
	int size = atoi(argv[2]);
	int line = size;
	int column = size;
	int symbol = size;
	FILE *fichier = NULL;
	fichier = fopen(filename, "w+");
	if(fichier != NULL)
	{
		fprintf(stdout, "%d %d %d \n", symbol, line, column);

		for(int i = 1; i <= size; i++)
		{
			printf("u ");
			for(int j = 1; j <= size; j++)
			{
				fprintf(stdout, "(%d,%d) ", i, j);
			}
			fprintf(stdout, "\n");
		}
		for(int i = 1; i <= size; i++)
		{
			printf("u ");
			for(int j = 1; j <= size; j++)
			{
				fprintf(stdout, "(%d,%d) ", j, i);
			}
			fprintf(stdout, "\n");
		}

		int square = sqrt(size);
		for(int k = 0; k < square; k++)
		{
			for(int l = 0; l < square; l++)
			{
				printf("u ");
				for(int i = 1; i <= square; i++)
				{
					for(int j = 1; j <= square; j++)
					{
						fprintf(stdout, "(%d,%d) ", square * k + i, square * l + j);
					}
				}
				fprintf(stdout, "\n");
			}
		}

	}

	fclose(fichier);
	return EXIT_SUCCESS;
}
