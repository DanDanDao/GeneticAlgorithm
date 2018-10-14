/******************************************************************************
** Student name: 	Quang Dao
** Student number: 	S3687103
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "gene.h"

/* Function to create new pcbmill chromosome */
int *create_pcbmill_chrom(int numAlleles)
{
	int index = 0, randomValue = rand() % numAlleles;
	int *pcbmill_chrom = safeMalloc(sizeof(*pcbmill_chrom) * numAlleles);

	while (index < numAlleles)
	{
		while (contains(pcbmill_chrom, index, randomValue) == TRUE)
		{
			randomValue = rand() % numAlleles;
		}
		pcbmill_chrom[index] = randomValue;
		index++;
	}
	return pcbmill_chrom;
}

/* Function to create new minfn chromosome */
int * create_minfn_chrom(int numAlleles)
{
	int * chrom = safeMalloc(sizeof(*chrom)*numAlleles);
	int i;

	for (i = 0; i < numAlleles; i++) {
		chrom[i] = rand() % (MINFN_MAX + 1);
	}
	return chrom;
}

/* Function to mutate pcbmill gene */
Gene * mutate_pcbmill(Gene *g)
{
	int num1, num2, temp, i;
	Gene * gene = malloc(sizeof(Gene));
	* gene = * g;
	gene->chromosome = malloc(sizeof(int)*g->num_alleles);
	for(i = 0; i < gene->num_alleles; i++)
	{
		gene->chromosome[i] = g->chromosome[i];
	}
	#ifdef DEBUG
	num1 = 2;
	num2 = 4;
	#else
	num1 = (rand() % gene->num_alleles) + 1;
	num2 = (rand() % gene->num_alleles) + 1;
	while(num1==num2)
	{
		num2 = (rand() % gene->num_alleles) + 1;;
	}
	#endif
	temp = gene->chromosome[num1];
	gene->chromosome[num1] = gene->chromosome[num2];
	gene->chromosome[num2] = temp;
	return gene;
}

/* Function to mutate minfn gene */
Gene * mutate_minfn(Gene *g)
{
	int i, num;
	Gene * gene = malloc(sizeof(Gene));
	* gene = * g;
	gene->chromosome = malloc(sizeof(int)*g->num_alleles);
	for(i = 0; i < gene->num_alleles; i++)
	{
		gene->chromosome[i] = g->chromosome[i];
	}
	#ifdef DEBUG
	num = 2;
	#else
	num = (rand() % gene->num_alleles) + 1;
	#endif
	gene->chromosome[num] = rand() % (MINFN_MAX + 1);
	return gene;
}

/* Function to crossover pcbmill gene */
Gene * crossover_pcbmill(Gene *g1, Gene *g2)
{
	int  i, j, k;
	int value, index;
	Gene * gene = gene_create_rand_gene(g1->num_alleles, create_minfn_chrom);
	int num1 = (rand() % gene->num_alleles) + 1;
	int num2 = (rand() % gene->num_alleles) + 1;
	while(num1 >= num2)
	{
		num2 = (rand() % gene->num_alleles) + 1;;
	}
	#ifdef DEBUG
	k=0;
	gene->chromosome[0] = g1->chromosome[2];
	gene->chromosome[1] = g1->chromosome[3];
	gene->chromosome[2] = g1->chromosome[4];
	for (i = 3; i < gene->num_alleles; i++)
	{
		for (j = 0; j < g2->num_alleles; j++)
		{
			value = g2->chromosome[j];
			index = find_index(gene->chromosome, g2->num_alleles, value);
			if (index == -1)
			{
				gene->chromosome[i] = g2->chromosome[j];
			}
		}
	}
	#else
	for (k = 0; k < num2-num1; k++) {
		gene->chromosome[k] = g1->chromosome[num1+k];
	}
	for (i = num2+1; i < gene->num_alleles; i++)
	{
		for (j = 0; j < g2->num_alleles; j++)
		{
			value = g2->chromosome[j];
			index = find_index(gene->chromosome, g2->num_alleles, value);
			if (index == -1)
			{
				gene->chromosome[i] = g2->chromosome[j];
			}
		}
	}
	#endif
	return gene;
}

/* Function to crossover minfn gene */
Gene * crossover_minfn(Gene *g1, Gene *g2)
{
	Gene * gene = gene_create_rand_gene(g1->num_alleles, create_minfn_chrom);
	int i;
	for(i = 0; i < gene->num_alleles/2; i++)
	{
		gene->chromosome[i] = g1->chromosome[i];
	}
	for(i = gene->num_alleles/2; i < gene->num_alleles; i++)
	{
		gene->chromosome[i] = g2->chromosome[i];
	}
	return gene;
}

/* Function to evaluate pcbmill gene */
double eval_pcbmill(InVTable *invt, Gene *gene)
{
	double totalRawScore;
	int i;
	totalRawScore = 0;

	for (i = 0; i < invt->tot; i++)
	{
		int * vector = invt->table[i];
		double rawScore;
		int j;

		rawScore = 0;
		for (j = 0; j < gene->num_alleles; j++)
		{
			rawScore += gene->chromosome[j] * vector[j];
		}
		rawScore -= vector[invt->width - 1];
		totalRawScore += fabs(rawScore);
	}

	return totalRawScore;
}

/* Function to evaluate minfn gene */
double eval_minfn(InVTable *invt, Gene *gene)
{
	double totalRawScore;
	int i;
	totalRawScore = 0;

	for (i = 0; i < invt->tot; i++)
	{
		int * vector = invt->table[i];
		double rawScore;
		int j;

		rawScore = 0;
		for (j = 0; j < gene->num_alleles; j++)
		{
			rawScore += gene->chromosome[j] * vector[j];
		}
		rawScore -= vector[invt->width - 1];
		totalRawScore += fabs(rawScore);
	}
	return totalRawScore;
}

/* Function to create a new gene */
Gene * gene_create_rand_gene(int numAlleles, CreateFn create_chrom)
{
	Gene * gene = malloc(sizeof(Gene));
	gene->num_alleles = numAlleles;
	gene->chromosome = create_chrom(numAlleles);
	gene->raw_score = 0.0;
	gene->fitness = 0.0;
	return gene;
}

/* Function to calculate gene fitness */
void gene_calc_fitness(Gene *gene, EvalFn evaluate_fn, InVTable *invTab)
{
	gene->raw_score = evaluate_fn(invTab, gene);
	gene->fitness =  gene_get_fitness(gene);
}

/* Function to normalise gene fitness */
void gene_normalise_fitness(Gene *gene, double total_fitness)
{
	gene->fitness /= total_fitness;
}

/* Function to free gene */
void gene_free(Gene *gene)
{
	free(gene->chromosome);
	free(gene);
}

/* Function to get gene fitness */
double gene_get_fitness(Gene *gene)
{
	/* TO DO */
	return 1/(gene->raw_score+1);
}

/* Function to print out gene */
void gene_print(Gene *gene)
{
	int i;
	printf("chrom: ");
	for(i = 0; i < gene->num_alleles; i++)
	{
		printf("%d", gene->chromosome[i]);
		if(i < gene->num_alleles - 1)
		printf(", ");
	}
	printf(" fit: %.3f", gene->fitness);
	printf(" raw: %.3f \n", gene->raw_score);
}

/* Function to find chromosome */
int find_index(int a[], int num_elements, int value)
{
	int i;
	for (i=0; i<num_elements; i++)
	{
		if (a[i] == value)
		{
			return(value);  /* it was found */
		}
	}
	return(-1);  /* if it was not found */
}

/* Safe Malloc */
void * safeMalloc(size_t size)
{
	void * p = malloc(size);

	if(p == NULL)
	{
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	return p;
}

/* Function to print out gene to output file */
void printGeneToFile(Gene * gene, FILE * fp)
{
	int i;
	if (fp == NULL) {
		printf("I couldn't open file for writing.\n");
		exit(0);
	}

	fprintf(fp, "chrom: ");
	for(i = 0; i < gene->num_alleles; i++)
	{
		fprintf(fp, "%d", gene->chromosome[i]);
		if(i < gene->num_alleles - 1)
		fprintf(fp, ", ");
	}
	fprintf(fp, " fit: %.3f", gene->fitness);
	fprintf(fp, " raw: %.3f \n", gene->raw_score);
}

Boolean contains(int *array, int arraySize, int value)
{
	int i = 0;

	while (i < arraySize)
	{
		if (array[i] == value)
		{
			return TRUE;
		}
		i++;
	}
	return FALSE;
}
