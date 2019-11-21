#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

void InitiateList(List* lst) {
	lst->head = (Node*)malloc(sizeof(Node));
	lst->tail = (Node*)malloc(sizeof(Node));

	lst->head->next = lst->tail;
	lst->head->prev = NULL;
	lst->head->Value = 0;

	lst->tail->next = NULL;
	lst->tail->prev = lst->head;
	lst->tail->Value = 0;

	lst->cur = lst->head;
	lst->count = 0;
}

void DeleteList(List* lst) {
	Node* ndTmp = lst->head;
	Node* ndNextTmp = lst->head->next;

	do {
		free(ndTmp);
		ndTmp = ndNextTmp;
		ndNextTmp = ndNextTmp->next;
	} while (ndNextTmp != NULL);

	free(ndTmp);
}

void ListPush(List* lst, ListType lt) {
	Node* ndTmp = (Node*)malloc(sizeof(Node));
	ndTmp->Value = lt;

	ndTmp->prev = lst->tail->prev;
	ndTmp->next = lst->tail;
	lst->tail->prev->next = ndTmp;
	lst->tail->prev = ndTmp;
}

void ListSetBegin(List* lst) {
	lst->cur = lst->head;
}
void ListSetEnd(List* lst) {
	lst->cur = lst->tail;
}

int ListNextItem(List* lst) {
	if (lst->cur->next == lst->tail)
		return 0;

	lst->cur = lst->cur->next;
	return 1;
}

int ListGetItem(List* lst, ListType * item) {
	if (lst->cur->next == NULL || lst->cur->prev == NULL)
		return -1;

	*item = lst->cur->Value;
	return 0;
}

void ListRemoveItem(List* lst) {
	Node* ndTmp = lst->cur->prev;

	if (lst->cur == lst->head || lst->cur == lst->tail)
		return;

	lst->cur->prev->next = lst->cur->next;
	lst->cur->next->prev = lst->cur->prev;
	
	free(lst->cur);
	lst->cur = ndTmp;
}