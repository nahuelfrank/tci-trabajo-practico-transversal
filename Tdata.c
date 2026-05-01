#include "Tdata.h"
#include <stdlib.h>
#include <string.h>

/* ==================== FUNCIONES DE CREACION ==================== */

Tdata create_str_ast(){
	Tdata n = (Tdata)malloc(sizeof(struct dataType));
	n->nodeType = STR;
	n->string = NULL;
	return n;
}

Tdata create_set(){
	Tdata n = (Tdata)malloc(sizeof(struct dataType));
	n->nodeType = SET;
	n->data = NULL;
	n->next = NULL;
	return n;
}

Tdata create_list(){
	Tdata n = (Tdata)malloc(sizeof(struct dataType));
	n->nodeType = LIST;
	n->data = NULL;
	n->next = NULL;
	return n;
}
	
/* ==================== DEEP COPY ==================== */
	
Tdata deep_copy(Tdata src) {
	if (src == NULL) return NULL;
	
	Tdata copy = (Tdata)malloc(sizeof(struct dataType));
	copy->nodeType = src->nodeType;
	
	switch (src->nodeType) {
		case STR:
			copy->string = (src->string != NULL) ? str_copy(src->string) : NULL;
			copy->data   = NULL;   /* mantener union limpia */
			copy->next   = NULL;
			break;
			
		case LIST:
		case SET:
			/* El nodo cabecera/contenedor no tiene dato propio;
			data apunta al primer elemento real, next al siguiente nodo */
			copy->data = deep_copy(src->data);
			copy->next = deep_copy(src->next);
			break;
		}
	
	return copy;
}

/* ==================== FUNCIONES GENERALES (equals, free_tdata) ==================== */

int equals(Tdata a, Tdata b) {
	if (a == NULL && b == NULL) return 1;
	if (a == NULL || b == NULL) return 0;
	if (a->nodeType != b->nodeType) return 0;
	
	switch (a->nodeType) {
	case STR:
		if (a->string == NULL && b->string == NULL) return 1;
		if (a->string == NULL || b->string == NULL) return 0;
		return strcmp(a->string, b->string) == 0;
		
	case LIST:
	case SET: {
		Tdata curA = a;
		Tdata curB = b;
		while (curA != NULL && curB != NULL) {
			if (!equals(curA->data, curB->data)) return 0;
			curA = curA->next;
			curB = curB->next;
		}
		return (curA == NULL && curB == NULL);
	}
	default:
		return 0;
	}
}

void free_tdata(Tdata nodo) {
	if (nodo == NULL) return;
	switch (nodo->nodeType) {
	case STR:
		if (nodo->string != NULL) free(nodo->string);
		break;
	case LIST:
	case SET:
		free_tdata(nodo->data);
		free_tdata(nodo->next);
		break;
	}
	free(nodo);
}



/* ==================== OPERACIONES SOBRE LIST ==================== */

void append(Tdata* list, Tdata elem) {
	if (list == NULL || elem == NULL) return;
	
	/* Crear el nuevo nodo contenedor con deep copy del elemento */
	Tdata new_node = (Tdata)malloc(sizeof(struct dataType));
	new_node->nodeType = LIST;
	new_node->data     = deep_copy(elem);   /* hacemos deep copy del elemento */
	new_node->next     = NULL;
	
	/* Caso: lista vacia (cabecera sin datos) */
	if ((*list)->data == NULL) {
		(*list)->data = new_node->data;
		(*list)->next = NULL;
		free(new_node);        
		return;
	}
	
	/* Recorrer hasta el ultimo nodo */
	Tdata current = *list;
	while (current->next != NULL) {
		current = current->next;
	}
	
	/* Enlazar el nuevo nodo al final */
	current->next = new_node;
}

/* Me devuelve el tamano de una lista */
int length(Tdata list) {
	// Validaciones: si es NULL o no es una lista
	if (list == NULL || list->nodeType != LIST) return 0;
	
	// Lista vacia (cabecera sin datos)
	if (list->data == NULL) return 0;
	
	int count = 0;
	Tdata current = list;
	
	while (current != NULL && current->data != NULL) {
		count++;
		current = current->next;
	}
	
	return count;
}

/* Realiza un deep copy de una lista especificamente */
Tdata copy_list(Tdata list) {
	if (list == NULL || list->nodeType != LIST) return NULL;
	return deep_copy(list);
}

/* Concatena dos cadenas */
Tdata concat(Tdata l1, Tdata l2) {
	// Validaciones
	if (l1 == NULL || l1->nodeType != LIST) return deep_copy(l2);
	if (l2 == NULL || l2->nodeType != LIST) return deep_copy(l1);
	
	// Deep copy de ambas listas para no modificar los originales
	Tdata result = deep_copy(l1);
	
	// Si l2 esta vacia, retornamos solo la copia de l1
	if (l2->data == NULL) return result;
	
	// Navegar hasta el ultimo nodo de result
	Tdata current = result;
	while (current->next != NULL) {
		current = current->next;
	}
	
	// Concatenar deep copy de l2 al final
	// Si result estaba vacia, copiar data de l2 directamente en la cabecera
	if (current->data == NULL) {
		Tdata l2_copy = deep_copy(l2);
		current->data = l2_copy->data;
		current->next = l2_copy->next;
		free(l2_copy);  // liberamos solo el contenedor, no su contenido
	} else {
		current->next = deep_copy(l2);
	}
	
	return result;
}	

/* Busca elem en list comparando con equals(). */
// Propuesta 1: devuelve el Tdata o NULL si no lo encuentra
Tdata search(Tdata list, Tdata elem) {
	/* Validaciones */
	if (list == NULL || list->nodeType != LIST) return NULL;
	if (elem == NULL) return NULL;
	
	/* Lista vacía */
	if (list->data == NULL) return NULL;
	
	Tdata current = list;
	while (current != NULL && current->data != NULL) {
		if (equals(current->data, elem)) {
			return current;   /* retorna el nodo contenedor */
		}
		current = current->next;
	}
	
	return NULL;   /* no encontrado */
}

// Propuesta 2: devuelve 1 si lo encuentra y 0 si no
short search_2(Tdata list, Tdata elem){
	//Validaciones
	if(list == NULL || list->nodeType != LIST) return 0;
	if(elem == NULL) return 0;
	//Lista vacia
	if(list->data == NULL) return 0;
	Tdata current = list;
	while(current != NULL && current->data != NULL){
		if(equals(current->data, elem)){
			return 1; //encontrado
		}
		current = current->next;
	}
	return 0; //no encontrado
}
	
//Propuesta 3: devuelve la posicion en la que se encuentra
int search_3(Tdata list, Tdata elem){
	int contador = 0;
	//Validaciones
	if(list== NULL || list->nodeType != LIST) return -1;
	if(elem == NULL) return -1;
	//Lista vacia
	if(list->data == NULL) return -1;
	Tdata current = list;
	while(current != NULL && current->data != NULL){
		if(equals(current->data, elem)){
			return contador; //posicion en la que la encontro
		}
		current = current->next;
		contador++;
	}
	return -1; //no encontrado
}
	


/* ==================== OPERACIONES SOBRE STR ==================== */

Tdata concat_str(Tdata s1, Tdata s2) {
	if (s1 == NULL || s1->nodeType != STR) return NULL;
	if (s2 == NULL || s2->nodeType != STR) return NULL;
	
	const char *str1 = (s1->string != NULL) ? s1->string : "";
	const char *str2 = (s2->string != NULL) ? s2->string : "";
	size_t len = strlen(str1) + strlen(str2) + 1;
	char *nuevo = (char *)malloc(len);
	if (nuevo == NULL) return NULL;
	strcpy(nuevo, str1);
	strcat(nuevo, str2);
	
	Tdata result = create_str_ast();
	result->string = nuevo;
	return result;
}

/* ==================== OPERACIONES SOBRE SET ==================== */

int belongs(Tdata conjunto, Tdata elem) {
	Tdata nodo = conjunto;
	while (nodo != NULL) {
		if (equals(nodo->data, elem))
			return 1;
		nodo = nodo->next;
	}
	return 0;
}

void insert_set(Tdata* conjunto, Tdata elem) {
	if (conjunto == NULL) return;
	if (belongs(*conjunto, elem)) return;
	
	Tdata nuevoNodo = create_set();
	nuevoNodo->data = deep_copy(elem);
	nuevoNodo->next = *conjunto;
	*conjunto = nuevoNodo;
}

void remove_set(Tdata* conjunto, Tdata elem) {
	if (conjunto == NULL || *conjunto == NULL) return;
	
	Tdata anterior = NULL;
	Tdata nodo = *conjunto;
	
	while (nodo != NULL) {
		if (equals(nodo->data, elem)) {
			if (anterior == NULL)
				*conjunto = nodo->next;
			else
				anterior->next = nodo->next;
			
			free_tdata(nodo->data);
			free(nodo);
			return;
		}
		anterior = nodo;
		nodo = nodo->next;
	}
}

Tdata union_set(Tdata A, Tdata B) {
	Tdata resultado = create_set();
	Tdata nodo;
	
	for (nodo = A; nodo != NULL; nodo = nodo->next)
		insert_set(&resultado, nodo->data);
	for (nodo = B; nodo != NULL; nodo = nodo->next)
		insert_set(&resultado, nodo->data);
	
	return resultado;
}

Tdata intersection_set(Tdata A, Tdata B) {
	Tdata resultado = create_set();
	Tdata nodo;
	
	for (nodo = A; nodo != NULL; nodo = nodo->next)
		if (belongs(B, nodo->data))
			insert_set(&resultado, nodo->data);
	
	return resultado;
}

Tdata difference_set(Tdata A, Tdata B) {
	Tdata resultado = create_set();
	Tdata nodo;
	
	for (nodo = A; nodo != NULL; nodo = nodo->next)
		if (!belongs(B, nodo->data))
			insert_set(&resultado, nodo->data);
	
	return resultado;
}

int subset(Tdata A, Tdata B) {
	Tdata nodo;
	for (nodo = A; nodo != NULL; nodo = nodo->next)
		if (!belongs(B, nodo->data))
			return 0;
	return 1;
}

int equals_set(Tdata A, Tdata B) {
	return subset(A, B) && subset(B, A);
}

Tdata cartesian_product(Tdata A, Tdata B) {
	Tdata resultado = create_set();
	Tdata nodoA, nodoB;
	
	for (nodoA = A; nodoA != NULL; nodoA = nodoA->next) {
		for (nodoB = B; nodoB != NULL; nodoB = nodoB->next) {
			// Crear una lista de dos elementos [nodoA->data, nodoB->data]
			Tdata par = create_list();
			append(&par, nodoA->data);
			append(&par, nodoB->data);
			// Insertar el par en el conjunto resultado (sin duplicados)
			insert_set(&resultado, par);
			// Liberar el temporal (insert_set ya hizo deep copy)
			free_tdata(par);
		}
	}
	return resultado;
}




