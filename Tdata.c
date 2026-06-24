#include "Tdata.h"

// Utilidad para leer cadena
char* leeCad() {
	void* aux;
	char c;
	int t = 0;
	char* cadena = (char*)malloc(sizeof(char));
	if (cadena == NULL) return cadena;
	c = getchar();
	if (c != EOF) {
		if (c != '\n') { cadena[t] = c; t++; }
		c = getchar();
		while (c != EOF && c != '\n') {
			t++;
			aux = (char*)realloc(cadena, sizeof(char) * t);
			if (aux != NULL) { cadena = aux; cadena[t - 1] = c; }
			else break;
			c = getchar();
		}
		aux = (char*)realloc(cadena, sizeof(char) * (t + 1));
		if (aux != NULL) { cadena = aux; cadena[t] = '\0'; }
		else cadena[t - 1] = '\0';
	} else cadena[0] = '\0';
	return cadena;
}

// 1. Creación
Tdata create_str_ast(str s) {
	Tdata n = (Tdata)malloc(sizeof(struct dataType));
	n->nodeType = STR;
	n->string = strdup(s);
	return n;
}

Tdata create_list() {
	Tdata n = (Tdata)malloc(sizeof(struct dataType));
	n->nodeType = LIST;
	n->data = NULL;
	n->next = NULL;
	return n;
}

Tdata create_set() {
	Tdata n = (Tdata)malloc(sizeof(struct dataType));
	n->nodeType = SET;
	n->data = NULL;
	n->next = NULL;
	return n;
}

// Utilidades Generales
Tdata copy_data(Tdata d) {
	if (d == NULL) return NULL;
	Tdata copy = (Tdata)malloc(sizeof(struct dataType));
	copy->nodeType = d->nodeType;
	if (d->nodeType == STR) {
		copy->string = strdup(d->string);
	} else {
		copy->data = copy_data(d->data);
		copy->next = copy_data(d->next);
	}
	return copy;
}

void free_data(Tdata* d) {
	if (*d != NULL) {
		if ((*d)->nodeType == STR) {
			free((*d)->string);
		} else {
			free_data(&((*d)->data));
			free_data(&((*d)->next));
		}
		free(*d);
		*d = NULL;
	}
}

int equals_data(Tdata a, Tdata b) {
	if (a == NULL && b == NULL) return 1;
	if (a == NULL || b == NULL) return 0;
	if (a->nodeType != b->nodeType) return 0;
	
	if (a->nodeType == STR) {
		return strcmp(a->string, b->string) == 0;
	}
	
	if (a->nodeType == LIST || a->nodeType == SET) {
		return equals_set(a, b); // Igualdad profunda sin importar orden para sets
	}
	return 0;
}

void print_data(Tdata d) {
	if (d == NULL) return;
	if (d->nodeType == STR) {
		printf("%s", d->string);
	} else {
		if (d->nodeType == SET) printf("{");
		else printf("[");
		
		Tdata aux = d;
		while (aux != NULL && aux->data != NULL) {
			print_data(aux->data);
			if (aux->next != NULL && aux->next->data != NULL) printf(",");
			aux = aux->next;
		}
		
		if (d->nodeType == SET) printf("}");
		else printf("]");
	}
}

// 3. LIST
void append(Tdata* list, Tdata elem) {
	if (*list == NULL) *list = create_list();
	if ((*list)->data == NULL) {
		(*list)->data = copy_data(elem);
	} else {
		Tdata aux = *list;
		while (aux->next != NULL) aux = aux->next;
		aux->next = create_list();
		aux->next->data = copy_data(elem);
	}
}

int length(Tdata list) {
	int count = 0;
	Tdata aux = list;
	while (aux != NULL && aux->data != NULL) {
		count++;
		aux = aux->next;
	}
	return count;
}

Tdata copy_list(Tdata list) {
	return copy_data(list);
}

Tdata concat(Tdata l1, Tdata l2) {
	Tdata result = copy_list(l1);
	Tdata aux2 = l2;
	while (aux2 != NULL && aux2->data != NULL) {
		append(&result, aux2->data);
		aux2 = aux2->next;
	}
	return result;
}

int search(Tdata list, Tdata elem) {
	Tdata aux = list;
	while (aux != NULL && aux->data != NULL) {
		if (equals_data(aux->data, elem)) return 1;
		aux = aux->next;
	}
	return 0;
}

// 4. SET
int belongs(Tdata set, Tdata elem) {
	Tdata aux = set;
	while (aux != NULL && aux->data != NULL) {
		if (equals_data(aux->data, elem)) return 1;
		aux = aux->next;
	}
	return 0;
}

void insert_set(Tdata* set, Tdata elem) {
	if (*set == NULL) *set = create_set();
	if (!belongs(*set, elem)) {
		if ((*set)->data == NULL) {
			(*set)->data = copy_data(elem);
		} else {
			Tdata aux = *set;
			while (aux->next != NULL) aux = aux->next;
			aux->next = create_set();
			aux->next->data = copy_data(elem);
		}
	}
}

void remove_set(Tdata* set, Tdata elem) {
	if (*set == NULL || (*set)->data == NULL) return;
	Tdata aux = *set;
	Tdata prev = NULL;
	
	while (aux != NULL && aux->data != NULL) {
		if (equals_data(aux->data, elem)) {
			if (prev == NULL) {
				if (aux->next != NULL) {
					*set = aux->next;
					aux->next = NULL;
					free_data(&aux);
				} else {
					free_data(&(aux->data));
					aux->data = NULL; // Queda vacío
				}
			} else {
				prev->next = aux->next;
				aux->next = NULL;
				free_data(&aux);
			}
			return;
		}
		prev = aux;
		aux = aux->next;
	}
}

Tdata union_set(Tdata A, Tdata B) {
	Tdata result = create_set();
	Tdata auxA = A;
	while (auxA != NULL && auxA->data != NULL) {
		insert_set(&result, auxA->data);
		auxA = auxA->next;
	}
	Tdata auxB = B;
	while (auxB != NULL && auxB->data != NULL) {
		insert_set(&result, auxB->data);
		auxB = auxB->next;
	}
	return result;
}

Tdata intersection_set(Tdata A, Tdata B) {
	Tdata result = create_set();
	Tdata auxA = A;
	while (auxA != NULL && auxA->data != NULL) {
		if (belongs(B, auxA->data)) {
			insert_set(&result, auxA->data);
		}
		auxA = auxA->next;
	}
	return result;
}

Tdata difference_set(Tdata A, Tdata B) {
	Tdata result = create_set();
	Tdata auxA = A;
	while (auxA != NULL && auxA->data != NULL) {
		if (!belongs(B, auxA->data)) {
			insert_set(&result, auxA->data);
		}
		auxA = auxA->next;
	}
	return result;
}

int subset(Tdata A, Tdata B) {
	Tdata auxA = A;
	while (auxA != NULL && auxA->data != NULL) {
		if (!belongs(B, auxA->data)) return 0;
		auxA = auxA->next;
	}
	return 1;
}

int equals_set(Tdata A, Tdata B) {
	return subset(A, B) && subset(B, A);
}

// Helper interactivo
Tdata parse_to_set(char* s) {
	Tdata set = create_set();
	char* token;
	char buffer[256];
	int j = 0;
	
	// Remover llaves
	for(int i=0; s[i]!='\0'; i++){
		if(s[i] != '{' && s[i] != '}' && s[i] != ' '){
			buffer[j++] = s[i];
		}
	}
	buffer[j] = '\0';
	
	token = strtok(buffer, ",");
	while (token != NULL) {
		Tdata elem = create_str_ast(token);
		insert_set(&set, elem);
		free_data(&elem);
		token = strtok(NULL, ",");
	}
	return set;
}
