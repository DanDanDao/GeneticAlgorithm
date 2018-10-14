/******************************************************************************
** Student name: 	Quang Dao
** Student number: 	S3687103
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "pop.h"

/* Function to init a population */
Boolean pop_init(Pop_list **pop)
{
	Pop_list * p = safeMalloc(sizeof(*p));
	p->head = NULL;
	p->count = 0;
	p->create_rand_chrom = NULL;
	p->mutate_gene = NULL;
	p->crossover_genes = NULL;
	p->evaluate_fn = NULL;
	*pop = p;
	return TRUE;
}

/* Function to set appropriate functions that are used in a population */
void pop_set_fns(Pop_list *p,CreateFn cf,MutateFn mf,CrossOverFn cof,EvalFn ef)
{
	p->create_rand_chrom = cf;
	p->mutate_gene = mf;
	p->crossover_genes = cof;
	p->evaluate_fn = ef;
}

/* Function to print out the fittest node in a population */
void pop_print_fittest(Pop_list *p)
{
	int i;
	Pop_node * node;
	node = fittestPopNode(p);

	printf("chrom: ");
	for(i = 0; i < node->gene->num_alleles; i++)
	{
		printf("%d", node->gene->chromosome[i]);
		if(i < node->gene->num_alleles - 1)
		printf(", ");
	}
	printf(" fit: %.3f", node->gene->fitness);
	printf(" raw: %.3f \n", node->gene->raw_score);
}

/* Function to print out the population into output file */
void printPopulationToFile(Pop_list * p, FILE * fp)
{
	Pop_node * node;
	Gene * fittestGene;

	node = p->head;
	fittestGene = node->gene;
	while(node != NULL)
	{
		if(node->gene->fitness > fittestGene->fitness)
		{
			fittestGene = node->gene;
		}
		node = node->next;
	}
	printGeneToFile(fittestGene, fp);
}

/* Function to create a new population */
void createInitialPopulation(Pop_list * p, int popSize, int alleleSize)
{
	int i;

	for(i = 0; i < popSize; i++)
	{
		Pop_node * node = createPopNode(alleleSize, p->create_rand_chrom);
		insertNode(p, node);
	}
}

/* Function to create a new node */
Pop_node * createPopNode(int alleleSize, CreateFn cf)
{
	Pop_node * node = safeMalloc(sizeof(*node));
	node->gene = gene_create_rand_gene(alleleSize, cf);
	node->next = NULL;

	return node;
}

/* Function to insert node into a population */
void insertNode(Pop_list * p, Pop_node * node)
{
	node->next = p->head;
	p->head = node;
	p->count++;
}

/* Function to calculate fitness of a population */
void calculateFitness(Pop_list * list, InVTable * invt)
{
	Pop_node * node;
	double totalFitness;

	totalFitness = 0;
	node = list->head;
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

/* Function to print out a population */
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

/* Function to mutate and crossover a population */
Pop_list * mutateAndCrossOverPopulation(Pop_list * p, InVTable * invt)
{
	Pop_list * newPopulation;
	int i;

	if (!pop_init(&newPopulation))
	{
		return NULL;
	}
	pop_set_fns(newPopulation, p->create_rand_chrom, p->mutate_gene, p->crossover_genes, p->evaluate_fn);

	sortPopulation(p);
	insertNode(newPopulation, clonePopNode(fittestPopNode(p)));
	for(i = 0; i < p->count - 1; i++)
	{
		Pop_node * newNode = safeMalloc(sizeof(* newNode));
		newNode->gene = p->crossover_genes(randomPopNode(p)->gene, randomPopNode(p)->gene);
		newNode->next = NULL;

		/* Mutate gene at the MUTATE_RATE percentage which is 5% of the time */
		if(rand() % 100 < MUTATE_RATE)
		{
			Gene * gene = p->mutate_gene(newNode->gene);
			gene_free(newNode->gene);
			newNode->gene = gene;
		}
		insertNode(newPopulation, newNode);
	}
	return newPopulation;
}

/* Function to sort population */
void sortPopulation(Pop_list * p)
{
	Boolean swap = TRUE;
	while(swap)
	{
		Pop_node * previous, * current;

		swap = FALSE;
		previous = NULL;
		current = p->head;
		while(current != NULL)
		{
			if(previous != NULL && current != NULL && previous->gene->fitness < current->gene->fitness){
				swap = TRUE;
				break;
			}
			previous = current;
			current = current->next;
		}
		if(swap)
		{
			swapPopNode(previous, current);
		}
	}
}

/* Function to swap nodes */
void swapPopNode(Pop_node * previous, Pop_node * current)
{
	Gene * gene = previous->gene;
	previous->gene = current->gene;
	current->gene = gene;
}

/* Function to find fittest node in a population */
Pop_node * fittestPopNode(Pop_list * p)
{
	Pop_node * node, * fittestNode;

	node = p->head;
	fittestNode = node;
	while(node != NULL)
	{
		if(node->gene->fitness > fittestNode->gene->fitness)
		{
			fittestNode = node;
		}
		node = node->next;
	}
	return fittestNode;
}

/* Function to clone a node in */
Pop_node * clonePopNode(Pop_node * node)
{
	Pop_node * clone = safeMalloc(sizeof(*clone));
	clone->next = NULL;
	clone->gene = safeMalloc(sizeof(*clone->gene));
	clone->gene->num_alleles = node->gene->num_alleles;
	clone->gene->raw_score = node->gene->raw_score;
	clone->gene->fitness = node->gene->fitness;
	clone->gene->chromosome = safeMalloc(sizeof(*clone->gene->chromosome) * clone->gene->num_alleles);
	memcpy(clone->gene->chromosome, node->gene->chromosome, sizeof(*clone->gene->chromosome) * clone->gene->num_alleles);
	return clone;
}

Pop_node * randomPopNode(Pop_list * p)
{
	Pop_node * node, * randomNode;
	double randomFitnessValue, fitnessSum;

	randomFitnessValue = fRand(0,1);
	fitnessSum = 0;
	randomNode = NULL;
	node = p->head;
	while(node != NULL && randomNode == NULL)
	{
		fitnessSum += node->gene->fitness;
		if (fitnessSum >= randomFitnessValue || node->next == NULL)
		{
			randomNode = node;
		}
		node = node->next;
	}
	return randomNode;
}

/* Function to free a population */
void freePop(Pop_list * p)
{
	Pop_node * node = p->head;
	while(node != NULL)
	{
		Pop_node * temp = node;
		node = node->next;
		freePopNode(temp);
	}
	free(p);
}

/* Function to free node */
void freePopNode(Pop_node * node)
{
	if(node->gene != NULL)
	{
		gene_free(node->gene);
	}
	free(node);
}

/* Function to get random value in a range */
double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}
