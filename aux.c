#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"

#define MAX_CHAR 256

short command_handler(char *command)
{
	if (strcmp(command, "INSERT") == 0)
		return 1;
	else if (strcmp(command, "LOAD") == 0)
		return 2;
	else if (strcmp(command, "REMOVE") == 0)
		return 3;
	else if (strcmp(command, "AUTOCORRECT") == 0)
		return 4;
	else if (strcmp(command, "AUTOCOMPLETE") == 0)
		return 5;
	else if (strcmp(command, "EXIT") == 0)
		return 0;
	else if (strcmp(command, "PRINT") == 0)
		return 6;
	else
		return 404;
}

void insert_helper(trie_t *trie)
{
	char *key = (char *)malloc(MAX_CHAR);
	scanf("%s", key);
	trie_insert(trie, key);
	free(key);
}

void load_helper(trie_t *trie)
{
	char *path = (char *)malloc(MAX_CHAR);
	scanf("%s", path);
	FILE *fp = fopen(path, "r");
	if (fp == NULL)
		printf("File not found!\n");
	char *key = (char *)malloc(MAX_CHAR);
	while (fscanf(fp, "%s", key) != EOF)
		trie_insert(trie, key);
	free(key);
	free(path);
	fclose(fp);
}

void remove_helper(trie_t *trie)
{
	char *key = (char *)malloc(MAX_CHAR);
	scanf("%s", key);
	trie_remove(trie, key);
	free(key);
}

void autocorrect_helper(trie_node_t *node, char *init, int pos, int max_dif, int dif_len, int *verbose)
{
	if (pos == (int)strlen(init)) {
		if (node->end_of_word == 1 && dif_len <= max_dif){
			printf("%s\n", init);
			*verbose = 1;
		}
		return;
	}
	if (node == NULL)
		return;
	if (dif_len > max_dif)
		return;
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		if (node->children[i] != NULL) {
			int kc = init[pos] - 'a';
			char *new_init = (char *)malloc(MAX_CHAR);
			strcpy(new_init, init);
			new_init[pos] = 'a' + i;
			if (kc == i)
				autocorrect_helper(node->children[i], new_init, pos + 1, max_dif, dif_len, verbose);
			else
				autocorrect_helper(node->children[i], new_init, pos + 1, max_dif, dif_len + 1, verbose);
			free(new_init);
		}
}

void autocorrect(trie_t *trie)
{
	char *init = (char *)malloc(MAX_CHAR);
	scanf("%s", init);
	int max_dif;
	scanf("%d", &max_dif);
	trie_node_t *node = trie->root;
	int *verbose = (int *)malloc(sizeof(int));
	*verbose = 0;
	for (int j = 0; j < ALPHABET_SIZE; ++j)
		if (node->children[j] != NULL) {
			int kc = init[0] - 'a';
			char *new_init = (char *)malloc(MAX_CHAR);
			strcpy(new_init, init);
			new_init[0] = 'a' + j;
			if (kc == j)
				autocorrect_helper(node->children[j], new_init, 1, max_dif, 0, verbose);
			else
				autocorrect_helper(node->children[j], new_init, 1, max_dif, 1, verbose);
			free(new_init);
		}
	if(*verbose == 0)
		printf("No words found\n");
	free(verbose);
	free(init);
}

void autocomplete_helper_1(trie_node_t *node, char *prefix)
{
	if (node == NULL)
		return;
	if (node->end_of_word == 1){
		printf("%s\n", prefix);
		return;
	}
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		if (node->children[i] != NULL) {
			char *new_prefix = (char *)malloc(MAX_CHAR);
			strcpy(new_prefix, prefix);
			new_prefix[strlen(prefix)] = 'a' + i;
			new_prefix[strlen(prefix) + 1] = '\0';
			autocomplete_helper_1(node->children[i], new_prefix);
			free(new_prefix);
			break;
		}
	return;
}

void autocomplete_helper_2(trie_node_t *node, char *prefix, char *ret, int *ret_len)
{
	if (node == NULL)
		return;
	if (node->end_of_word == 1) {
		int l = strlen(prefix);
		if (l < *ret_len) {
			strcpy(ret, prefix);
			*ret_len = l;
		}
		return;
	}
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		if (node->children[i] != NULL) {
			char *new_prefix = (char *)malloc(MAX_CHAR);
			strcpy(new_prefix, prefix);
			new_prefix[strlen(prefix)] = 'a' + i;
			new_prefix[strlen(prefix) + 1] = '\0';
			autocomplete_helper_2(node->children[i], new_prefix, ret, ret_len);
			free(new_prefix);
		}
	return;
}

void autocomplete_helper_3(trie_node_t *node, char *prefix, char *ret, int *ret_freq)
{
	if (node == NULL)
		return;
	if (node->end_of_word == 1) {
		int l = node->freq;
		if (l > *ret_freq) {
			strcpy(ret, prefix);
			*ret_freq = l;
		}
	}
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		if (node->children[i] != NULL) {
			char *new_prefix = (char *)malloc(MAX_CHAR);
			strcpy(new_prefix, prefix);
			new_prefix[strlen(prefix)] = 'a' + i;
			new_prefix[strlen(prefix) + 1] = '\0';
			autocomplete_helper_3(node->children[i], new_prefix, ret, ret_freq);
			free(new_prefix);
		}
	return;
}

void autocomplete(trie_t *trie)
{
	char *prefix = (char *)malloc(MAX_CHAR);
	scanf("%s", prefix);
	int crit_id;
	scanf("%d", &crit_id);
	trie_node_t *node = trie->root;
	for (int i = 0; i < (int)strlen(prefix); ++i) {
		int kc = prefix[i] - 'a';
		if (node->children[kc] == NULL) {
			printf("No words found\n");
			if (crit_id == 0) {
				printf("No words found\n");
				printf("No words found\n");
			}
			return;
		}
		node = node->children[kc];
	}
	char *word = (char *)malloc(MAX_CHAR);
	strcpy(word, prefix);
	int *freq = (int *)malloc(sizeof(int));
	*freq = MAX_CHAR;
	switch (crit_id) {
	case 0:
		autocomplete_helper_1(node, prefix);
		autocomplete_helper_2(node, prefix, word, freq);
		printf("%s\n", word);
		free(freq);
		free(word);
		freq = (int *)calloc(1, sizeof(int));
		word = (char *)malloc(MAX_CHAR);
		*freq = 0;
		autocomplete_helper_3(node, prefix, word, freq);
		printf("%s\n", word);
		break;
	case 1:
		autocomplete_helper_1(node, prefix);
		break;
	case 2:
		autocomplete_helper_2(node, prefix, word, freq);
		printf("%s\n", word);
		break;
	case 3:
		*freq = 0;
		autocomplete_helper_3(node, prefix, word, freq);
		printf("%s\n", word);
		break;
	}
	free(word);
	free(freq);
	free(prefix);
}