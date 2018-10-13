/******************************************************************************
** Student name: 	...
** Student number: 	...
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#ifndef POP_H
#define POP_H

#include "gene.h"
#include "invector.h"

#define MUTATE_RATE 5

typedef struct pop_node {
	Gene *gene;
	struct pop_node *next;
} Pop_node;

typedef struct pop_list {
	/* the head of the population list */
	Pop_node *head;
	/* the size of the population */
	int count;
	/* pointers to functions to perform
 	 * gene chromosome creation, mutation,
 	 * crossover and evaluation */
	CreateFn create_rand_chrom;
	MutateFn mutate_gene;
	CrossOverFn crossover_genes;
	EvalFn evaluate_fn;
} Pop_list;

/* Create and initialise the population list to a safe state.
 * Returns TRUE on success */
Boolean pop_init(Pop_list **);

/* Set the function pointers of a population list */
void pop_set_fns(Pop_list *p,CreateFn cf,MutateFn mf,CrossOverFn cof,EvalFn ef);

/* This function causes details of the fittest gene of the generation to
 * be printed out.
 * The function also prints out the number of the current generation.
 * This function must not be able to access any generation value outside
 * of the function. */
void pop_print_fittest(Pop_list *p);

/* TO DO - other functions as appropriate */
void printPopulationToFile(Pop_list * p, FILE * fp);

void createInitialPopulation(Pop_list * p, int popSize, int alleleSize);

Pop_node * createPopNode(int alleleSize, CreateFn cf);

void insertNode(Pop_list * p, Pop_node * node);

void calculateFitness(Pop_list * list, InVTable * invt);

void printList(Pop_list * list);

Pop_list * mutateAndCrossOverPopulation(Pop_list * p, InVTable * invt);

void sortPopulation(Pop_list * p);

void swapPopNode(Pop_node * previous, Pop_node * current);

Pop_node * fittestPopNode(Pop_list * p);

Pop_node * clonePopNode(Pop_node * node);

Pop_node * randomPopNode(Pop_list * p);

void freePop(Pop_list * p);

void freePopNode(Pop_node * node);

double fRand(double fMin, double fMax);

#endif
