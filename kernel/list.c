#include "list.h"
#include <stdio.h> // For NULL
#include <stdlib.h>


list_t* list_new(void* _data) {
	list_t* list = (list_t*)malloc(sizeof(list_t));
	if (list == NULL) return 0;
	list->prev = NULL;
	list->data = _data;
	list->next = NULL;
	return list;
}

list_t* list_add(list_t* _list, void* _data) {
	// Find end of list
	list_t* last = _list;
	while (last->next != NULL) last = last->next;

	list_t* new_last = (list_t*)malloc(sizeof(list_t));
	if (new_last == NULL) return 0;
	new_last->prev = last;
	new_last->data = _data;
	new_last->next = NULL;
	last->next = new_last;
	return new_last;
}

list_t* list_insert(list_t* _list, void* _data) {
	list_t* new_item = (list_t*)malloc(sizeof(list_t));
	if (new_item == NULL) return 0;
	new_item->prev = _list;
	new_item->data = _data;

	if (_list->next == NULL) {
		new_item->next = NULL;
		_list->next = new_item;
	} else {
		list_t* next = _list->next;
		_list->next = new_item;
		new_item->next = next;
		next->prev = new_item;
	}

	return new_item;
}

list_t* list_delete(list_t* _list) {
	list_t* ret;

	if (_list->prev == NULL && _list->next == NULL) {
		ret = NULL;
	} else if (_list->prev == NULL) {
		ret = _list->next;
		_list->next->prev = NULL;
	} else {
		ret = _list->prev;
		if (_list->next != NULL) {
			_list->prev->next = _list->next;
			_list->next->prev = _list->prev;
		} else {
			_list->prev->next = NULL;
		}
	}

	free(_list);

	return ret;
}

list_t* list_get_index(list_t* list, unsigned int i) {
	list_t* tmp = list;
	for (; i>0; i--) {
		if (tmp->next == NULL) return 0;
		tmp = tmp->next;
	}
	return tmp;
}

unsigned int list_size(list_t* list) {
	unsigned int x = 1;

	while (list->next != NULL) {
		list = list->next;
		x++;
	}

	return x;
}
