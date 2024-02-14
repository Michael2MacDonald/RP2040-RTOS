#ifndef __LIST_H
#define __LIST_H

typedef struct List list_t;

typedef struct List {
	list_t* prev;
	void* data;
	list_t* next;
} list_t;


// Creates new list and returns pointer to it
list_t* list_new(void* data);

// Append new item to the end of the list
list_t* list_add(list_t* list, void* data);

// Insert new item after given list item
list_t* list_insert(list_t* list, void* data);

// Delete given list item, returns list item before deleted item if exists, otherwise returns following list item. Returns nullptr if no more list items exist.
// Warning: free data before deleting item
list_t* list_delete(list_t* list);

list_t* list_get_index(list_t* list, unsigned int i);

unsigned int list_size(list_t* list);

#endif /** End: __LIST_H */