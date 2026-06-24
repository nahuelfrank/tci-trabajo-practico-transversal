#include "Automata.h"

void add_transition(TransitionNode** head, State from, Symbol symbol, Tdata to_set) {
	TransitionNode* new_node = (TransitionNode*)malloc(sizeof(TransitionNode));
	new_node->t.from = strdup(from);
	new_node->t.symbol = strdup(symbol);
	new_node->t.to = copy_data(to_set);
	new_node->next = *head;
	*head = new_node;
}

Tdata devuelveTransicion(Automata* A, State q, Symbol a) {
	TransitionNode* curr = A->Delta;
	Tdata result = create_set();
	while (curr != NULL) {
		if (strcmp(curr->t.from, q) == 0 && strcmp(curr->t.symbol, a) == 0) {
			Tdata temp = union_set(result, curr->t.to);
			free_data(&result);
			result = temp;
		}
		curr = curr->next;
	}
	return result;
}

void free_automata(Automata** A) {
	if (A == NULL || *A == NULL) return;
	Automata* aut = *A;
	free_data(&(aut->Q));
	free_data(&(aut->Sigma));
	free_data(&(aut->F));
	if (aut->q0) {
		free(aut->q0);
	}
	TransitionNode* curr = aut->Delta;
	while (curr != NULL) {
		TransitionNode* next = curr->next;
		free(curr->t.from);
		free(curr->t.symbol);
		free_data(&(curr->t.to));
		free(curr);
		curr = next;
	}
	free(aut);
	*A = NULL;
}


Automata* cargaAF() {
	Automata* A = (Automata*)malloc(sizeof(Automata));
	A->Delta = NULL;
	A->deterministic = 0;
	
	printf("\nCargar conjunto Q (ej {q0,q1}): ");
	char* q_str = leeCad();
	A->Q = parse_to_set(q_str);
	free(q_str);
	
	printf("Cargar Sigma (ej {a,b}): ");
	char* s_str = leeCad();
	A->Sigma = parse_to_set(s_str);
	free(s_str);
	
	printf("Cargar transiciones...\n");
	Tdata auxQ = A->Q;
	while (auxQ != NULL && auxQ->data != NULL) {
		Tdata auxS = A->Sigma;
		while (auxS != NULL && auxS->data != NULL) {
			printf("&(%s, %s) = (ej {q1} o vacio {}): ", auxQ->data->string, auxS->data->string);
			char* dest_str = leeCad();
			Tdata dest_set = parse_to_set(dest_str);
			if (dest_set->data != NULL) {
				add_transition(&(A->Delta), auxQ->data->string, auxS->data->string, dest_set);
			}
			free(dest_str);
			free_data(&dest_set);
			auxS = auxS->next;
		}
		auxQ = auxQ->next;
	}
	
	printf("Estado inicial (ej q0): ");
	char* q0_str = leeCad();
	A->q0 = strdup(q0_str);
	free(q0_str);
	
	printf("Cargar estado de aceptacion F (ej {q1}): ");
	char* f_str = leeCad();
	A->F = parse_to_set(f_str);
	free(f_str);
	
	return A;
}

// -------------------------------------------------------------------------
// Helpers para Precarga de Automatas
// -------------------------------------------------------------------------
void _helper_add_trans(Automata* af, char* from, char* sym, char* to_str) {
	Tdata dest = parse_to_set(to_str);
	if (dest && dest->data != NULL) {
		add_transition(&(af->Delta), from, sym, dest);
	}
	free_data(&dest); 
}

Automata* crear_automata_predefinido(int op) {
	Automata* af = (Automata*)malloc(sizeof(Automata));
	af->Delta = NULL;
	af->deterministic = 0;
	
	switch(op) {
	case 2:
		af->Q = parse_to_set("{p0,p1,p2}");
		af->Sigma = parse_to_set("{a,b}");
		af->q0 = strdup("p0");
		af->F = parse_to_set("{p1}");
		_helper_add_trans(af, "p0", "b", "{p2}");
		_helper_add_trans(af, "p1", "a", "{p0}");
		_helper_add_trans(af, "p1", "b", "{p1,p2}");
		_helper_add_trans(af, "p2", "a", "{p0,p1,p2}");
		break;
		case 3:
			af->Q = parse_to_set("{r0,q0,q1,q2,q3,p0,p1}");
			af->Sigma = parse_to_set("{0,1}");
			af->q0 = strdup("r0");
			af->F = parse_to_set("{r0,q3,p0}");
			_helper_add_trans(af, "r0", "0", "{p0}");
			_helper_add_trans(af, "r0", "1", "{q1,p1}");
			_helper_add_trans(af, "q0", "1", "{q1}");
			_helper_add_trans(af, "q1", "0", "{q2}");
			_helper_add_trans(af, "q2", "1", "{q3}");
			_helper_add_trans(af, "q3", "0", "{q3}");
			_helper_add_trans(af, "q3", "1", "{q3}");
			_helper_add_trans(af, "p0", "0", "{p0}");
			_helper_add_trans(af, "p0", "1", "{p1}");
			_helper_add_trans(af, "p1", "0", "{p0}");
			_helper_add_trans(af, "p1", "1", "{p1}");
			break;
			case 4:
				af->Q = parse_to_set("{q0,q1,q2,q3,q4,q5}");
				af->Sigma = parse_to_set("{0,1}");
				af->q0 = strdup("q0");
				af->F = parse_to_set("{q5}");
				_helper_add_trans(af, "q0", "0", "{q0}");
				_helper_add_trans(af, "q0", "1", "{q0,q1,q2}");
				_helper_add_trans(af, "q1", "0", "{q2,q3}");
				_helper_add_trans(af, "q2", "1", "{q4}");
				_helper_add_trans(af, "q3", "0", "{q5}");
				_helper_add_trans(af, "q4", "1", "{q5}");
				_helper_add_trans(af, "q5", "0", "{q5}");
				_helper_add_trans(af, "q5", "1", "{q5}");
				break;
				case 5:
					af->Q = parse_to_set("{q0b,p0,p1,p2,p3}");
					af->Sigma = parse_to_set("{x,y,z}");
					af->q0 = strdup("q0b");
					af->F = parse_to_set("{p0}");
					_helper_add_trans(af, "q0b", "x", "{p3}");
					_helper_add_trans(af, "q0b", "y", "{p3}");
					_helper_add_trans(af, "q0b", "z", "{p2,p3}");
					_helper_add_trans(af, "p1", "x", "{p0}");
					_helper_add_trans(af, "p2", "y", "{p1}");
					_helper_add_trans(af, "p3", "x", "{p3}");
					_helper_add_trans(af, "p3", "y", "{p3}");
					_helper_add_trans(af, "p3", "z", "{p2,p3}");
					break;
						default:
							free(af);
							return NULL;
	}
	return af;
}
// -------------------------------------------------------------------------

int cadenaAceptada(Automata* A, char* cadena) {
	Tdata estados_actuales = create_set();
	Tdata estado_ini = create_str_ast(A->q0);
	insert_set(&estados_actuales, estado_ini);
	free_data(&estado_ini);
	
	int i = 0;
	while (cadena[i] != '\0') {
		char symb_str[2] = {cadena[i], '\0'};
		Tdata nuevos_estados = create_set();
		
		Tdata curr_est = estados_actuales;
		while (curr_est != NULL && curr_est->data != NULL) {
			Tdata alcanzados = devuelveTransicion(A, curr_est->data->string, symb_str);
			Tdata union_temp = union_set(nuevos_estados, alcanzados);
			free_data(&nuevos_estados);
			nuevos_estados = union_temp;
			free_data(&alcanzados);
			curr_est = curr_est->next;
		}
		
		free_data(&estados_actuales);
		estados_actuales = nuevos_estados;
		if (estados_actuales == NULL || estados_actuales->data == NULL) break;
		i++;
	}
	
	Tdata intersec = intersection_set(estados_actuales, A->F);
	int aceptada = (intersec != NULL && intersec->data != NULL);
	
	free_data(&intersec);
	free_data(&estados_actuales);
	return aceptada;
}

// Genera un nombre string para un SET de estados compuesto (ej "{q0,q1}")
char* set_to_str_name(Tdata set) {
	char buffer[512] = "{";
	Tdata aux = set;
	while (aux != NULL && aux->data != NULL) {
		strcat(buffer, aux->data->string);
		if (aux->next != NULL && aux->next->data != NULL) strcat(buffer, ",");
		aux = aux->next;
	}
	strcat(buffer, "}");
	return strdup(buffer);
}

Automata* conversionAFND_AFD(Automata* afnd) {
	Automata* afd = (Automata*)malloc(sizeof(Automata));
	afd->Q = create_set();
	afd->Sigma = copy_data(afnd->Sigma);
	afd->Delta = NULL;
	afd->F = create_set();
	afd->deterministic = 1;
	
	Tdata q0_set = create_set();
	Tdata q0_str_node = create_str_ast(afnd->q0);
	insert_set(&q0_set, q0_str_node);
	free_data(&q0_str_node);
	
	char* q0_name = set_to_str_name(q0_set);
	afd->q0 = strdup(q0_name);
	
	Tdata Qb = create_list();
	append(&Qb, q0_set); // Guardamos como SET compuesto internamente para explorar
	
	Tdata curr_Qb_node = Qb;
	while (curr_Qb_node != NULL && curr_Qb_node->data != NULL) {
		Tdata estado_compuesto_actual = curr_Qb_node->data;
		char* from_name = set_to_str_name(estado_compuesto_actual);
		
		Tdata aux_estado_AFD_str = create_str_ast(from_name);
		insert_set(&(afd->Q), aux_estado_AFD_str); // Registrar estado
		
		// Verificar si es de aceptación
		Tdata inter = intersection_set(estado_compuesto_actual, afnd->F);
		if (inter != NULL && inter->data != NULL) {
			insert_set(&(afd->F), aux_estado_AFD_str);
		}
		free_data(&inter);
		free_data(&aux_estado_AFD_str);
		
		Tdata auxS = afnd->Sigma;
		while (auxS != NULL && auxS->data != NULL) {
			Symbol sym = auxS->data->string;
			Tdata trans_dest = create_set();
			
			// Recorrer los subestados
			Tdata subest = estado_compuesto_actual;
			while (subest != NULL && subest->data != NULL) {
				Tdata alcanzables = devuelveTransicion(afnd, subest->data->string, sym);
				Tdata uni = union_set(trans_dest, alcanzables);
				free_data(&trans_dest);
				trans_dest = uni;
				free_data(&alcanzables);
				subest = subest->next;
			}
			
			if (trans_dest->data != NULL) {
				char* to_name = set_to_str_name(trans_dest);
				Tdata to_set_for_delta = create_set();
				Tdata tmp_dest_str = create_str_ast(to_name);
				insert_set(&to_set_for_delta, tmp_dest_str);
				
				add_transition(&(afd->Delta), from_name, sym, to_set_for_delta);
				
				if (!search(Qb, trans_dest)) {
					append(&Qb, trans_dest);
				}
				
				free_data(&to_set_for_delta);
				free_data(&tmp_dest_str);
				free(to_name);
			}
			free_data(&trans_dest);
			auxS = auxS->next;
		}
		free(from_name);
		curr_Qb_node = curr_Qb_node->next;
	}
	
	free_data(&q0_set);
	free_data(&Qb);
	free(q0_name);
	return afd;
}

Automata* renombrarAFD(Automata* afd) {
	if (!afd) return NULL;
	
	Automata* renom = (Automata*)malloc(sizeof(Automata));
	renom->Q = create_set();
	renom->Sigma = copy_data(afd->Sigma);
	renom->Delta = NULL;
	renom->F = create_set();
	renom->deterministic = 1;
	
	// Obtenemos la cantidad de estados
	int num_states = length(afd->Q);
	
	// Arreglo para mapear el índice (0, 1, 2) con el nombre original ("{q0,q1}")
	char** orig_names = (char**)malloc(sizeof(char*) * num_states);
	
	// 1. Mapear Q original a z0, z1, z2... y popular orig_names
	Tdata auxQ = afd->Q;
	int idx = 0;
	while (auxQ != NULL && auxQ->data != NULL) {
		orig_names[idx] = strdup(auxQ->data->string); // Guardamos copia del nombre original
		
		char z_name[32];
		sprintf(z_name, "z%d", idx);
		
		Tdata z_str = create_str_ast(z_name);
		insert_set(&(renom->Q), z_str); // Agregamos zX a nuevoQ
		free_data(&z_str);
		
		idx++;
		auxQ = auxQ->next;
	}
	
	// 2. Mapear el estado inicial (q0) buscando su índice en orig_names
	int q0_idx = 0;
	for (int i = 0; i < num_states; i++) {
		if (strcmp(orig_names[i], afd->q0) == 0) {
			q0_idx = i;
			break;
		}
	}
	char z0_name[32];
	sprintf(z0_name, "z%d", q0_idx);
	renom->q0 = strdup(z0_name); // Asignamos el nuevo q0
	
	// 3. Mapear los estados finales (nuevoF)
	for (int i = 0; i < num_states; i++) {
		Tdata orig_str_ast = create_str_ast(orig_names[i]);
		// Equivalente a tu IN(returnElem(copyAfd,5), returnElem(Q,i))
		if (belongs(afd->F, orig_str_ast)) { 
			char zf_name[32];
			sprintf(zf_name, "z%d", i);
			Tdata z_str = create_str_ast(zf_name);
			insert_set(&(renom->F), z_str);
			free_data(&z_str);
		}
		free_data(&orig_str_ast);
	}
	
	// 4. Reconstruir Delta (nuevoDelta)
	// En lugar de un doble for, recorremos las transiciones existentes directamente
	TransitionNode* curr = afd->Delta;
	while (curr != NULL) {
		int from_idx = -1;
		// Buscamos a qué índice corresponde el estado de origen
		for (int i = 0; i < num_states; i++) {
			if (strcmp(orig_names[i], curr->t.from) == 0) {
				from_idx = i;
				break;
			}
		}
		
		if (from_idx != -1) {
			char from_z[32];
			sprintf(from_z, "z%d", from_idx);
			
			Tdata to_z_set = create_set();
			Tdata auxTo = curr->t.to;
			
			// Buscamos a qué índice corresponden los estados de destino
			while (auxTo != NULL && auxTo->data != NULL) {
				int to_idx = -1;
				for (int i = 0; i < num_states; i++) {
					if (strcmp(orig_names[i], auxTo->data->string) == 0) {
						to_idx = i;
						break;
					}
				}
				if (to_idx != -1) {
					char to_z[32];
					sprintf(to_z, "z%d", to_idx);
					Tdata to_z_str = create_str_ast(to_z);
					insert_set(&to_z_set, to_z_str); // Agregamos a nuevaTransicion
					free_data(&to_z_str);
				}
				auxTo = auxTo->next;
			}
			
			// Si la transición es válida (no apunta a vacío), la agregamos al nuevo delta
			if (to_z_set->data != NULL) {
				add_transition(&(renom->Delta), from_z, curr->t.symbol, to_z_set);
			}
			free_data(&to_z_set);
		}
		curr = curr->next;
	}
	
	// Liberamos la memoria del arreglo temporal
	for (int i = 0; i < num_states; i++) {
		free(orig_names[i]);
	}
	free(orig_names);
	
	return renom;
}

void mostrarAF(Automata* af) {
	if (!af) return;
	printf("\nQ = ");
	print_data(af->Q);
	printf("\nSigma = ");
	print_data(af->Sigma);
	printf("\nDelta:");
	
	// Recorremos todo Q x Sigma para identificar inclusos los vacíos explícitamente
	Tdata auxQ = af->Q;
	while (auxQ != NULL && auxQ->data != NULL) {
		Tdata auxS = af->Sigma;
		while (auxS != NULL && auxS->data != NULL) {
			char* q_name = auxQ->data->string;
			char* sym_name = auxS->data->string;
			Tdata trans_dest = devuelveTransicion(af, q_name, sym_name);
			
			// Verificación para formato dinámico de llave (ej. Evitar {{p0,p1}})
			if (q_name[0] == '{') {
				printf("\n  &(%s, %s) = ", q_name, sym_name);
			} else {
				printf("\n  &({%s}, %s) = ", q_name, sym_name);
			}
			
			if (trans_dest == NULL || trans_dest->data == NULL) {
				printf("{}"); // Simbolización explícita del estado vacío
			} else {
				print_data(trans_dest);
			}
			
			free_data(&trans_dest);
			auxS = auxS->next;
		}
		auxQ = auxQ->next;
	}
	
	printf("\nq0 = %s", af->q0);
	printf("\nF = ");
	print_data(af->F);
	printf("\n");
}

