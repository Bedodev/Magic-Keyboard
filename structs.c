#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"



int remove_aux(trie_node_t *node, char *key, int index, trie_t *trie)
{
	if (node == NULL) return 0;
	if (index == (int)strlen(key) - 1) {
		if (node->end_of_word == 1) {
			node->end_of_word = 0;
		}
		if (node->n_children == 0 && trie->root != node) {
			free(node->children);
			free(node);
			trie->nNodes--;
			return 2;
		} else return 1;
	}
	int kc = key[index + 1] - 'a';
	int ret = remove_aux(node->children[kc], key, index + 1, trie);
	if (ret == 2) {
		node->children[kc] = NULL;
		node->n_children--;
		if (node->n_children == 0 && trie->root != node) {
			free(node->children);
			free(node);
			trie->nNodes--;
			return 2;
		} else return 1;
	} else return 1;

}

trie_node_t *trie_create_node(trie_t *trie)
{
	trie_node_t *node = (trie_node_t *)malloc(sizeof(trie_node_t));
	node->end_of_word = 0;
	node->n_children = 0;
	node->freq = 0;
	node->children = (trie_node_t **)malloc(sizeof(trie_node_t *) * ALPHABET_SIZE);
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		node->children[i] = NULL;
	trie->nNodes++;
	return node;
}

trie_t *trie_create(int data_size, int alphabet_size, char *alphabet)
{
	trie_t *trie = (trie_t *)malloc(sizeof(trie_t));
	trie->root = trie_create_node(trie);
	trie->size = 0;
	trie->data_size = data_size;
	trie->alphabet_size = alphabet_size;
	trie->alphabet = malloc(sizeof(char) * alphabet_size);
	memcpy(trie->alphabet, alphabet, sizeof(char) * alphabet_size);
	return trie;
}

void trie_insert(trie_t *trie, char *key)
{
	int l = strlen(key);
	trie_node_t *node = trie->root;
	for (int i = 0; i < l; ++i) {
		int index = key[i] - 'a';
		if (node->children[index] == NULL) {
			node->children[index] = trie_create_node(trie);
			if (i == l - 1) {
				node->children[index]->end_of_word = 1;
				node->children[index]->freq++;
			}
			node->n_children++;
			node = node->children[index];
		} else {
			if (i == l - 1) {
				node->children[index]->end_of_word = 1;
				node->children[index]->freq++;
			}
			node = node->children[index];
		}
	}
}


void trie_remove(trie_t *trie, char *key)
{
	remove_aux(trie->root, key, -1, trie);
}

void free_aux(trie_node_t *node) //it no work
{
	if (node == NULL) return;
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		free_aux(node->children[i]);
	free(node->children);
	free(node);
}

void trie_free(trie_t **pTrie) //mem leak here
{
	trie_t *trie = *pTrie;
	trie_node_t *node = trie->root;
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		if (node->children[i] != NULL)
			free_aux(node->children[i]);
	free(node->children);
	free(node);
	free(trie->alphabet);
	free(trie);
	*pTrie = NULL;
}