#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

short command_handler(char *command);

void insert_helper(trie_t *trie);

void load_helper(trie_t *trie);

void remove_helper(trie_t *trie);

void autocorrect_helper(trie_node_t *node, char *init,
						int pos, int max_dif, int dif_len, int *verbose);

void autocorrect(trie_t *trie);

void autocomplete_helper_1(trie_node_t *node, char *prefix);

void autocomplete_helper_2(trie_node_t *node, char *prefix,
						   char *word, int *len);

void autocomplete_helper_3(trie_node_t *node, char *prefix,
						   char *ret, int *ret_freq);

void autocomplete(trie_t *trie);