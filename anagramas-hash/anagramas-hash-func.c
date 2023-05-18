#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "anagramas-hash.h"
#include "wrappers.h"

struct list *list_create(void)
// crea una lista vacía, retorna puntero a la misma
{
	struct list *new_list = (struct list*) malloc_or_exit(sizeof(struct list));
	new_list -> first = NULL;
	new_list -> last = NULL;
	new_list -> qty = 0;
	return new_list;
}

struct list *list_insert_last_word(struct list *l, char *word)
// inserta una palabra al final de la lista l, retorna el puntero a la lista
// si la lista es NULL, crea una nueva
{
	if(l == NULL){
		l = list_create();
	}
	struct listnode *newnode = (struct listnode*) malloc_or_exit(sizeof(struct listnode));
	newnode -> word = strdup_or_exit(word);
	newnode -> next = NULL;

	if (l -> first == NULL){ //last = first = newnode por ser el unico que hay
		l -> first = newnode;
		l -> last = newnode;
		l -> qty++;
	}else{
		l -> last -> next = newnode; //apuntar el ultimo nodo al nuevo que se esta insertando
		l -> last = newnode;
		l -> qty++;
	}
	return l;
}

struct hashnode *hash_insert_word(struct hashnode *node, char *key, char *word)
// busca key en el hash (y si no encuentra inserta key al inicio en lista simple)
// luego agrega palabra al final de la lista de anagramas asociadas a la key
// retorna siempre el primero de la lista: node si key estaba o el nuevo nodo si se insertó
{
	struct hashnode *newnode;
	for(newnode = node; newnode != NULL; newnode = newnode -> next){
		if (strcmp((newnode -> key),key) == 0){
			break;
		}
	}

	if(newnode == NULL){
		newnode = (struct hashnode*) malloc_or_exit(sizeof(struct hashnode));
		newnode -> wlist = NULL;
		newnode -> key = strdup_or_exit(key);
		newnode -> next = node;
		node = newnode;
	}

	newnode -> wlist = list_insert_last_word(newnode -> wlist, word);
	return node;
}


char *sort_word(char *word)
// ordena EN EL LUGAR la palabra 'word' según código ASCII en forma ascendente
{
	int i,j;
	int length = strlen(word);
	char temp;
	for(i=0; i<length-1; i++){
		for(j=i+1; j<length; j++){
			if (word[i] > word[j]){
			        temp = word[i];
        			word[i] = word[j];
        			word[j] = temp;
			}
		}
	}
	return word;
}

void print_anagrams(struct hashnode *hn)
// escribe (printf) la lista de anagramas para ese hashnode
{
	while(hn != NULL){
		if ((hn -> key) != NULL){
			if((hn -> wlist -> qty) >1){
				printf("%s : ", (hn -> key));
				struct listnode *temp = hn -> wlist -> first;
				while(temp != NULL){
					printf("%s , ", temp -> word);
					temp = temp -> next;
				}
				printf("\n");
			}
		}
		hn = hn -> next;
	}
}
