/******************************************************************************
** Student name: 	Quang Dao
** Student number: 	S3687103
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "drive.h"
#include "invector.h"
#include "pop.h"
#include "gene.h"

int main(int argc, char *argv[])
{
	FILE * fp;
	InVTable invt;
	Pop_list * list;
	int i;
	int numAllelesInt = 0;
	int popSizeInt = 0;
	int numGenInt = 0;

	#ifdef DEBUG
	test_minfn();
	test_pcbmill();
	#endif

	/* The only point at which srand should be called */
	srand(SRAND_SEED);

	/* Check if the arguments are correct */
	if(argc != CMD_ARG_MAX)
	{
		printf("Incorrect number of arguments. \n\n");
		return EXIT_FAILURE;
	}

	/* Check and open input file to read */
	fp = fopen(argv[inputFile], "r");
	if(fp == NULL)
	{
		printf("Unable to open file. \n\n");
		return EXIT_FAILURE;
	}

	/* Create InVector Table from input file */
	invector_init(&invt);
	readInvector(&invt, fp);
	fclose(fp);

	/* Init population */
	pop_init(&list);

	/* Check gennType argument and set appropriate functions  */
	if(strcmp(argv[geneType], "minfn") == 0)
	{
		pop_set_fns(list, create_minfn_chrom, mutate_minfn, crossover_minfn, eval_minfn);
		numAllelesInt = strtol(argv[alleleSize], NULL, 10);
	}
	else if(strcmp(argv[geneType], "pcbmill") == 0)
	{
		pop_set_fns(list, create_pcbmill_chrom, mutate_pcbmill, crossover_pcbmill, eval_pcbmill);
		if (strtol(argv[alleleSize], NULL, 10) <= invt.tot) {
			numAllelesInt = strtol(argv[alleleSize], NULL, 10);
		} else {
			printf("Allele size can not larger then number of vector in the input file\n");
			printf("Allele size is assigned to: %d\n", invt.tot);
			numAllelesInt = invt.tot;
		}
	}
	else
	{
		printf("Unknown gene type. \n\n");
		return EXIT_FAILURE;
	}

	/* Check and open output file to write */
	fp = fopen(argv[outputFile], "w");
	if(fp == NULL)
	{
		printf("Unable to open file. \n\n");
		return EXIT_FAILURE;
	}

	/* Read population size and number of generation from command line arguments */
	popSizeInt = strtol(argv[popSize], NULL, 10);
	numGenInt = strtol(argv[numGen], NULL, 10);

	/* Create appropriate population and calculate fitness of nodes */
	createInitialPopulation(list, popSizeInt, numAllelesInt);
	calculateFitness(list, &invt);

	/* Print out the fittest node to output file */
	printf("Gen:   0 ");
	pop_print_fittest(list);
	printGeneToFile(fittestPopNode(list)->gene,fp);
	printf("\n");

	/* Print out the population
	printList(list);*/

	/* Create generations of population, mutate and crossover and then print out
	** the fittest node to console and to output file */
	for (i = 0; i < numGenInt - 1; i++)
	{
		list = mutateAndCrossOverPopulation(list, &invt);
		calculateFitness(list, &invt);
		printf("Gen:   %d ", i+1);
		pop_print_fittest(list);
		printGeneToFile(fittestPopNode(list)->gene,fp);
		printf("\n");
		/*
		printList(list);
		*/
	}

	freePop(list);

	/* Close output file */
	fclose(fp);

	return EXIT_SUCCESS;
}

/* Test for minfn */
void test_minfn(void)
{
	Gene * gene = gene_create_rand_gene(TEST_ALLELE_LEN, create_minfn_chrom);
	Gene * geneM = mutate_minfn(gene);
	Gene * gene1 = gene_create_rand_gene(TEST_ALLELE_LEN, create_minfn_chrom);
	Gene * gene2 = gene_create_rand_gene(TEST_ALLELE_LEN, create_minfn_chrom);
	Gene * gene3 = crossover_minfn(gene1, gene2);

	printf("MINFN gene:\n");
	gene_print(gene);

	printf("Mutate: ");
	printf("\n");
	gene_print(geneM);

	gene_free(gene);
	gene_free(geneM);

	printf("MINFN genes:\n");
	gene_print(gene1);
	gene_print(gene2);

	printf("Crossover:\n");
	gene_print(gene3);

	printf("\n");
	gene_free(gene1);
	gene_free(gene2);
	gene_free(gene3);
}

/* Test for pcbmill */
void test_pcbmill(void)
{
	Gene * gene = gene_create_rand_gene(TEST_ALLELE_LEN, create_pcbmill_chrom);
	Gene * geneM = mutate_pcbmill(gene);
	Gene * gene1 = gene_create_rand_gene(TEST_ALLELE_LEN, create_pcbmill_chrom);
	Gene * gene2 = gene_create_rand_gene(TEST_ALLELE_LEN, create_pcbmill_chrom);
	Gene * gene3 = crossover_pcbmill(gene1, gene2);


	printf("PCBMILL gene:\n");
	gene_print(gene);

	printf("Mutate: ");
	printf("\n");
	gene_print(geneM);
	gene_free(gene);
	gene_free(geneM);

	printf("PCBMILL genes:\n");
	gene_print(gene1);
	gene_print(gene2);

	printf("Crossover:\n");
	gene_print(gene3);

	printf("\n");
	gene_free(gene1);
	gene_free(gene2);
	gene_free(gene3);

}
