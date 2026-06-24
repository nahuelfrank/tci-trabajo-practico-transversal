#ifndef AUTOMATA_H
#define AUTOMATA_H

#include "Tdata.h"

typedef str State;
typedef str Symbol;

typedef struct {
	State from;
	Symbol symbol;
	Tdata to; // SET de estados destino (almacena nodos tipo STR)
} Transition;

typedef struct transitionNode {
	Transition t;
	struct transitionNode* next;
} TransitionNode;

typedef struct {
	Tdata Q;       // SET de STR
	Tdata Sigma;   // SET de STR
	TransitionNode* Delta;
	State q0;      // STR
	Tdata F;       // SET de STR
	int deterministic;
} Automata;

Automata* cargaAF();
int cadenaAceptada(Automata* A, char* cadena);
Automata* conversionAFND_AFD(Automata* afnd);
Automata* renombrarAFD(Automata* afd);
void mostrarAF(Automata* af);
Tdata devuelveTransicion(Automata* A, State q, Symbol a);
void free_automata(Automata** A); // Función para liberar memoria dinámicamente

Automata* crear_automata_predefinido(int op); // Generador de autómatas de prueba

#endif
