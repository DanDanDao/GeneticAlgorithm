/******************************************************************************
** Student name: 	...
** Student number: 	...
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "gene.h"

int * create_pcbmill_chrom(int numAlleles){
	/* TO DO */
	int * chrom = malloc(sizeof(*chrom)*numAlleles);
	int i;

	for (i = 0; i < numAlleles; i++) {
		chrom[i] = rand() % (MINFN_MAX + 1);
	}
	return chrom;
}

int * create_minfn_chrom(int numAlleles){
	/* TO DO */
	int * chrom = malloc(sizeof(*chrom)*numAlleles);
	int i;

	for (i = 0; i < numAlleles; i++) {
		chrom[i] = rand() % (MINFN_MAX + 1);
	}
	return chrom;
}

Gene * mutate_pcbmill(Gene *g){
	/* TO DO */
	int num1, num2, temp;
	Gene * gene = malloc(sizeof(Gene));
	gene = g;
	num1 = (rand() % gene->num_alleles) + 1;
	num2 = (rand() % gene->num_alleles) + 1;
	while(num1==num2)
	{
		num2 = (rand() % gene->num_alleles) + 1;;
	}
	#ifdef DEBUG
	temp = gene->chromosome[2];
	gene->chromosome[2] = gene->chromosome[4];
	gene->chromosome[4] = temp;
	#else
	temp = gene->chromosome[num1];
	gene->chromosome[num1] = gene->chromosome[num2];
	gene->chromosome[num2] = temp;
	#endif
	return gene;
}

Gene * mutate_minfn(Gene *g){
	/* TO DO */
	int num;
	Gene * gene = malloc(sizeof(Gene));
	gene = g;
	#ifdef DEBUG
	num = 0;
	gene->chromosome[2] = (rand() % MINFN_MAX) + 1;
	#else
	num = (rand() % gene->num_alleles) + 1;
	gene->chromosome[num] = (rand() % MINFN_MAX) + 1;
	#endif
	return gene;
}

Gene * crossover_pcbmill(Gene *g1, Gene *g2){
	/* TO DO */
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

Gene * crossover_minfn(Gene *g1, Gene *g2){
	/* TO DO */
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

double eval_pcbmill(InVTable *invt, Gene *gene){
	/* TO DO */
	return 0.0;
}

double eval_minfn(InVTable *invt, Gene *gene){
	/* TO DO */
	return 0.0;
}

Gene * gene_create_rand_gene(int numAlleles, CreateFn create_chrom){
	/* TO DO */
	Gene * gene = malloc(sizeof(Gene));
	gene->num_alleles = numAlleles;
	gene->chromosome = malloc(sizeof(int)*numAlleles);
	gene->chromosome = create_chrom(numAlleles);
	gene->raw_score = 0.0;
	gene->fitness = 0.0;
	return gene;
}

void gene_calc_fitness(Gene *gene, EvalFn evaluate_fn, InVTable *invTab){
	/* TO DO */
}

void gene_normalise_fitness(Gene *gene, double total_fitness){
	/* TO DO */
}

void gene_free(Gene *gene){
	/* TO DO */
	free(gene->chromosome);
	free(gene);
}


double gene_get_fitness(Gene *gene){
	/* TO DO */
	return 0.0;
}

void gene_print(Gene *gene) {
	/* TO DO */
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
