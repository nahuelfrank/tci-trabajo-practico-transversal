#ifndef TDATA_H
#define TDATA_H

#include "String.h"

#define STR 1
#define SET 2
#define LIST 3

struct dataType {
	int nodeType;   /* STR, SET, LIST */
	union {
		str string;
		struct {
			struct dataType* data;
			struct dataType* next;
		};
	};
};

typedef struct dataType* Tdata;

// 1. Funciones de creación
Tdata create_str_ast(str s);
Tdata create_list();
Tdata create_set();

// Funciones generales y de utilidad
Tdata copy_data(Tdata d);
void free_data(Tdata* d);
void print_data(Tdata d);
int equals_data(Tdata a, Tdata b);
Tdata parse_to_set(char* s); // Helper para parsear "{q0,q1}" a SET

// 3. Operaciones sobre LIST
void append(Tdata* list, Tdata elem);
int length(Tdata list);
Tdata copy_list(Tdata list);
Tdata concat(Tdata l1, Tdata l2);
int search(Tdata list, Tdata elem);

// 4. Operaciones sobre SET
void insert_set(Tdata* set, Tdata elem);
int belongs(Tdata set, Tdata elem);
void remove_set(Tdata* set, Tdata elem);
Tdata union_set(Tdata A, Tdata B);
Tdata intersection_set(Tdata A, Tdata B);
Tdata difference_set(Tdata A, Tdata B);
int subset(Tdata A, Tdata B);
int equals_set(Tdata A, Tdata B);

#endif
