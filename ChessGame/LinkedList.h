#ifndef __LINKEDLIST_HEADER__
#define __LINKEDLIST_HEADER__

typedef struct _Node Node;

typedef int ListType;

struct _Node {
	Node* next;
	Node* prev;
	ListType Value;
};

typedef struct _List {
	Node* head;
	Node* tail;
	Node* cur;
	int count;
}List;

void InitiateList(List* lst);
void DeleteList(List* lst);
void ListPush(List* lst, ListType lt);
void ListSetBegin(List* lst);
void ListSetEnd(List* lst);
int ListNextItem(List* lst);
int ListGetItem(List* lst, ListType* item);
void ListRemoveItem(List* lst);
#endif