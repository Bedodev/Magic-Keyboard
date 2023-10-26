#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "aux.h"

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define MAX_CHAR 256

int main(void)
{
	trie_t *trie = trie_create(sizeof(int), ALPHABET_SIZE, ALPHABET);

	while (1) {
		char *command = (char *)malloc(MAX_CHAR);
		scanf("%s", command);
		switch (command_handler(command)) {
		case 1:
			insert_helper(trie);
			break;
		case 2:
			load_helper(trie);
			break;
		case 3:
			remove_helper(trie);
			break;
		case 4:
			autocorrect(trie);
			break;
		case 5:
			autocomplete(trie);
			break;
		case 0:
			trie_free(&trie);
			free(command);
			return 0;
			break;
		default:
			if (strcmp(command, "\n") != 0)
				printf("Invalid command!\n");
			break;
		}
		free(command);
	}
	return 0;
}
