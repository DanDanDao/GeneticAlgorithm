/******************************************************************************
** Student name: 	...
** Student number: 	...
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "drive.h"
#include "invector.h"
#include "pop.h"
#include "gene.h"

#ifdef DEBUG
void test_pcbmill(void){
	/* TO DO */
	Gene * gene = gene_create_rand_gene(TEST_ALLELE_LEN, create_pcbmill_chrom);
	Gene * geneM = mutate_pcbmill(gene);
	Gene * gene1 = gene_create_rand_gene(TEST_ALLELE_LEN, create_pcbmill_chrom);
	Gene * gene2 = gene_create_rand_gene(TEST_ALLELE_LEN, create_pcbmill_chrom);
	Gene * gene3 = crossover_pcbmill(gene1, gene2);


	printf("PCBMILL gene:\n");
	/* TO DO - create a random pcbmill gene by calling create_rand_gene
	The length of the gene's chromosome should be TEST_ALLELE_LEN */
	/* TO DO - print the gene using gene_print */
	gene_print(gene);

	printf("Mutate: ");
	/* TO DO - create a mutant copy of the gene using mutate_pcbmill */
	/* TO DO - print the mutant gene using gene_print */
	printf("\n");
	gene_print(geneM);
	/* TO DO - free the original gene and the mutant */
	gene_free(gene);
	gene_free(geneM);

	printf("PCBMILL genes:\n");
	/* TO DO - create 2 random pcbmill 'parent' genes using calls
	to create_rand_gene
	The length of the gene's chromosome should be TEST_ALLELE_LEN */

	/* TO DO - print each gene */
	gene_print(gene1);
	gene_print(gene2);

	printf("Crossover:\n");
	/* TO DO produce a new gene by calling crossover_pcbmill
	with the parent genes */
	/* TO DO - print the new gene */
	gene_print(gene3);

	printf("\n");
	/* TO DO - free both parents and the child gene */
	gene_free(gene1);
	gene_free(gene2);
	gene_free(gene3);

}

void test_minfn(void){
	/* TO DO */
	Gene * gene = gene_create_rand_gene(TEST_ALLELE_LEN, create_minfn_chrom);
	Gene * geneM = mutate_minfn(gene);
	Gene * gene1 = gene_create_rand_gene(TEST_ALLELE_LEN, create_minfn_chrom);
	Gene * gene2 = gene_create_rand_gene(TEST_ALLELE_LEN, create_minfn_chrom);
	Gene * gene3 = crossover_minfn(gene1, gene2);

	printf("MINFN gene:\n");
	/* TO DO - create a random minfn gene by calling create_rand_gene
	The length of the gene's chromosome should be TEST_ALLELE_LEN */
	/* TO DO - print the gene */
	gene_print(gene);

	printf("Mutate: ");
	/* TO DO - create a mutant copy of the gene using mutate_minfn */
	/* TO DO - print the mutant gene */
	printf("\n");
	gene_print(geneM);

	/* TO DO - free the original gene and the mutant */
	gene_free(gene);
	gene_free(geneM);

	printf("MINFN genes:\n");
	/* TO DO - create 2 random minfn 'parent' genes using calls
	to create_rand_gene
	The length of the gene's chromosome should be TEST_ALLELE_LEN */

	/* TO DO - print each gene */
	gene_print(gene1);
	gene_print(gene2);

	printf("Crossover:\n");
	/* TO DO produce a new gene by calling crossover_minfn
	with the parent genes */
	/* TO DO - print the new gene */
	gene_print(gene3);

	printf("\n");
	/* TO DO - free both parents and the child gene */
	gene_free(gene1);
	gene_free(gene2);
	gene_free(gene3);
}
#endif


int main(int argc, char *argv[]){

	#ifdef DEBUG
	test_minfn();
	test_pcbmill();
	#else
	printf("Not DEBUG \n");
	#endif

	/* The only point at which srand should be called */
	srand(SRAND_SEED);

	return EXIT_SUCCESS;
}
