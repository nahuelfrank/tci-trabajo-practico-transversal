#ifndef TDATA_H
#define TDATA_H
#include "String.h" 
#include <stdbool.h>
	
#define STR 1
#define SET 2
#define LIST 3

struct dataType{
	int nodeType;   /* STR, SET, LIST */
	union{
		str string;
		struct{
			struct dataType* data;
			struct dataType* next;
		};
	};
};

typedef struct dataType* Tdata;

/* Funciones de creacion */
Tdata create_str_ast();
Tdata create_list();
Tdata create_set();

/* Operaciones generales */
int equals(Tdata a, Tdata b); // compara dos elementos tData y verifica si son iguales

/* Deep copy de cualquier nodo Tdata (STR, LIST o SET) */
Tdata deep_copy(Tdata src);

/* Operaciones sobre LIST */
void append(Tdata* list, Tdata elem);
int length(Tdata list);
Tdata copy_list(Tdata list);
Tdata concat(Tdata l1, Tdata l2);
Tdata search(Tdata list, Tdata elem);
short search_2(Tdata list, Tdata elem);
int search_3(Tdata list, Tdata elem);

/* Operaciones sobre SET */
void insert_set(Tdata* set, Tdata elem);   // sin duplicados
int belongs(Tdata set, Tdata elem);        
void remove_set(Tdata* set, Tdata elem);
Tdata union_set(Tdata A, Tdata B);
Tdata intersection_set(Tdata A, Tdata B);
Tdata difference_set(Tdata A, Tdata B);
int subset(Tdata A, Tdata B);
int equals_set(Tdata A, Tdata B);
Tdata cartesian_product(Tdata A, Tdata B);

/* Operaciones sobre STR (adicionales) */
Tdata concat_str(Tdata s1, Tdata s2);      // concatenaci�n de dos strings

#endif
