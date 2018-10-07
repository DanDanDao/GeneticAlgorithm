/******************************************************************************
** Student name: 	...
** Student number: 	...
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "pop.h"

Boolean pop_init(Pop_list **pop){
	/* TO DO */
	Pop_list * list = safeMalloc(sizeof(*list));
	list->head = NULL;
	list->count = 0;
	list->create_rand_chrom = NULL;
	list->mutate_gene = NULL;
	list->crossover_genes = NULL;
	list->evaluate_fn = NULL;
	*pop = list;


	return TRUE;
}

void pop_set_fns(Pop_list *p,CreateFn cf,MutateFn mf,CrossOverFn cof,EvalFn ef){
	/* TO DO */
	p->create_rand_chrom = cf;
	p->mutate_gene = mf;
	p->crossover_genes = cof;
	p->evaluate_fn = ef;
}

void pop_print_fittest(Pop_list *p){
	/* TO DO */

}

/* TO DO - other functions as appropriate */
void insertNode(Pop_list * list, int numAlleles)
{
	Pop_node * node = safeMalloc(sizeof(*node));
	node->next = NULL;
	node->gene = gene_create_rand_gene(numAlleles, list->create_rand_chrom);

	node->next = list->head;
	list->head = node;
	list->count++;
}

void calculateFitness(Pop_list * list, InVTable * invt) {
	double totalFitness = 0;

	Pop_node * node = list->head;
	while(node != NULL) {
		gene_calc_fitness(node->gene, list->evaluate_fn, invt);
		totalFitness += node->gene->fitness;
		node = node->next;
	}

	node = list->head;
	while(node != NULL) {
		gene_normalise_fitness(node->gene, totalFitness);
		node = node->next;
	}
}

void printList(Pop_list * list)
{
	double totalFitness = 0;
	Pop_node * node = list->head;

	while(node != NULL)
	{
		gene_print(node->gene);
		totalFitness += node->gene->fitness;
		node = node->next;
	}
	printf("Total Fitness: %f \n\n", totalFitness);
}
