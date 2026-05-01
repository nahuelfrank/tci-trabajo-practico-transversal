#include <stdio.h>
#include "Tdata.h"




/* ==================== FUNCIONES AUXILIARES ==================== */

// Crear un nodo STR a partir de un literal (ej: "hola")
Tdata crear_str(const char* texto) {
	Tdata nodo = create_str_ast();
	nodo->string = str_new(texto);
	if (nodo->string == NULL) printf("ERROR: str_new devolvió NULL para '%s'\n", texto);
	else printf("DEBUG: creado string '%s'\n", nodo->string);
	return nodo;
}

// Imprimir un Tdata (STR, LIST o SET) de forma legible
void mostrar(Tdata nodo) {
	if (nodo == NULL) {
		printf("NULL");
		return;
	}
	switch (nodo->nodeType) {
	case STR:
		printf("\"%s\"", nodo->string ? nodo->string : "");
		break;
	case SET:
		printf("{ ");
		{
			Tdata aux = nodo;
			while (aux) {
				mostrar(aux->data);
				if (aux->next) printf(", ");
				aux = aux->next;
			}
		}
		printf(" }");
		break;
		case LIST:
			printf("[ ");
			{
				Tdata aux = nodo;
				while (aux) {
					mostrar(aux->data);
					if (aux->next) printf(", ");
					aux = aux->next;
				}
			}
			printf(" ]");
			break;
	}
}

// Función para imprimir con título
void imprimir(const char* titulo, Tdata nodo) {
	printf("%s", titulo);
	mostrar(nodo);
	printf("\n");
}

/* ==================== PRUEBAS ==================== */

int main() {
	
	printf("\n=== PRUEBA DE INSERCION DIRECTA ===\n");
	Tdata nodo_prueba = create_str_ast();
	nodo_prueba->string = str_new("hola");
	imprimir("Nodo STR creado manualmente: ", nodo_prueba);
	
	Tdata conjunto_prueba = create_set();
	insert_set(&conjunto_prueba, nodo_prueba);
	imprimir("Conjunto despues de insertar: ", conjunto_prueba);
	
	free_tdata(nodo_prueba);
	free_tdata(conjunto_prueba);
	
	// -------------------------------------------------------------
	// 1. Conjuntos simples y duplicados
	// -------------------------------------------------------------
	printf("--- 1. Conjunto simple y duplicados ---\n");
	Tdata A = create_set();
	insert_set(&A, crear_str("uno"));
	insert_set(&A, crear_str("dos"));
	insert_set(&A, crear_str("tres"));
	insert_set(&A, crear_str("dos"));  // duplicado -> no se agrega
	imprimir("Conjunto A =", A);
	
	// -------------------------------------------------------------
	// 2. Lista simple con duplicados
	// -------------------------------------------------------------
	printf("\n--- 2. Lista simple con duplicados ---\n");
	Tdata L = create_list();
	append(&L, crear_str("uno"));
	append(&L, crear_str("dos"));
	append(&L, crear_str("tres"));
	append(&L, crear_str("uno"));
	append(&L, crear_str("tres"));
	append(&L, crear_str("dos"));
	imprimir("Lista L =", L);
	
	// -------------------------------------------------------------
	// 3. Unión, intersección, diferencia de conjuntos
	// -------------------------------------------------------------
	printf("\n--- 3. Operaciones entre conjuntos ---\n");
	Tdata B = create_set();
	insert_set(&B, crear_str("dos"));
	insert_set(&B, crear_str("tres"));
	insert_set(&B, crear_str("cuatro"));
	imprimir("Conjunto B =", B);
	
	Tdata unionAB = union_set(A, B);
	imprimir("A ? B =", unionAB);
	
	Tdata interAB = intersection_set(A, B);
	imprimir("A n B =", interAB);
	
	Tdata diffAB = difference_set(A, B);
	imprimir("A - B =", diffAB);
	
	printf("¿A ? B? %s\n", subset(A, B) ? "Sí" : "No");
	printf("¿A = B? %s\n", equals_set(A, B) ? "Sí" : "No");
	
	// -------------------------------------------------------------
	// 4. Producto cartesiano
	// -------------------------------------------------------------
	printf("\n--- 4. Producto cartesiano (conjuntos chicos) ---\n");
	Tdata C = create_set();
	insert_set(&C, crear_str("x"));
	insert_set(&C, crear_str("y"));
	Tdata D = create_set();
	insert_set(&D, crear_str("1"));
	insert_set(&D, crear_str("2"));
	Tdata prod = cartesian_product(C, D);
	imprimir("C = {x, y}\nD = {1, 2}\nC x D =", prod);
	free_tdata(prod);
	free_tdata(C);
	free_tdata(D);
	
	// -------------------------------------------------------------
	// 5. Conversión STR ? LIST ? STR
	// -------------------------------------------------------------
	printf("\n--- 5. Conversion de string a lista de caracteres y vuelta ---\n");
	Tdata original = crear_str("HolaMundo");
	imprimir("String original:", original);
	
	// Convertir a lista de caracteres
	Tdata listaChar = create_list();
	char* p = original->string;
	while (*p) {
		char letra[2] = {*p, '\0'};
		append(&listaChar, crear_str(letra));
		p++;
	}
	imprimir("Lista de caracteres:", listaChar);
	
	// Reconstruir concatenando
	Tdata reconstruido = crear_str("");
	Tdata aux = listaChar;
	while (aux) {
		Tdata nuevo = concat_str(reconstruido, aux->data);
		free_tdata(reconstruido);
		reconstruido = nuevo;
		aux = aux->next;
	}
	imprimir("Reconstruido (debe ser igual):", reconstruido);
	
	free_tdata(original);
	free_tdata(listaChar);
	free_tdata(reconstruido);
	
	// -------------------------------------------------------------
	// 6. Conjunto vacío
	// -------------------------------------------------------------
	printf("\n--- 6. Conjunto vacio ---\n");
	Tdata vacio = create_set();
	imprimir("Conjunto vacio =", vacio);
	printf("¿Pertenece 'algo' al vacio? %s\n", belongs(vacio, crear_str("algo")) ? "Si" : "No");
	free_tdata(vacio);
	
	// -------------------------------------------------------------
	// 7. Listas anidadas y heterogéneas
	// -------------------------------------------------------------
	printf("\n--- 7. Listas y conjuntos anidados (heterogeneos) ---\n");
	// Construir {cuatro, [tres, uno, {dos, tres}], {seis, dos}}
	Tdata conjuntoAnidado = create_set();
	insert_set(&conjuntoAnidado, crear_str("cuatro"));
	
	Tdata listaInterna = create_list();
	append(&listaInterna, crear_str("tres"));
	append(&listaInterna, crear_str("uno"));
	Tdata subConj = create_set();
	insert_set(&subConj, crear_str("dos"));
	insert_set(&subConj, crear_str("tres"));
	append(&listaInterna, subConj);
	free_tdata(subConj);  // ya se clonó
	insert_set(&conjuntoAnidado, listaInterna);
	free_tdata(listaInterna);
	
	Tdata otroConj = create_set();
	insert_set(&otroConj, crear_str("seis"));
	insert_set(&otroConj, crear_str("dos"));
	insert_set(&conjuntoAnidado, otroConj);
	free_tdata(otroConj);
	imprimir("Conjunto anidado =", conjuntoAnidado);
	
	// Construir [uno, uno, {dos, uno}, [tres, dos, uno]]
	Tdata listaHeterogenea = create_list();
	append(&listaHeterogenea, crear_str("uno"));
	append(&listaHeterogenea, crear_str("uno"));
	Tdata setInterno = create_set();
	insert_set(&setInterno, crear_str("dos"));
	insert_set(&setInterno, crear_str("uno"));
	append(&listaHeterogenea, setInterno);
	free_tdata(setInterno);
	Tdata listaInterna2 = create_list();
	append(&listaInterna2, crear_str("tres"));
	append(&listaInterna2, crear_str("dos"));
	append(&listaInterna2, crear_str("uno"));
	append(&listaHeterogenea, listaInterna2);
	free_tdata(listaInterna2);
	imprimir("Lista heterogenea =", listaHeterogenea);
	
	free_tdata(conjuntoAnidado);
	free_tdata(listaHeterogenea);
	
	// -------------------------------------------------------------
	// 8. Copias vs referencias (deep copy)
	// -------------------------------------------------------------
	printf("\n--- 8. Prueba de copia profunda (deep copy) ---\n");
	Tdata orig = crear_str("original");
	Tdata copia = deep_copy(orig);
	// Modificamos el string original
	str_free(orig->string);
	orig->string = str_new("modificado");
	imprimir("Original después de modificar:", orig);
	imprimir("Copia (debe seguir siendo 'original'):", copia);
	free_tdata(orig);
	free_tdata(copia);
	
	// -------------------------------------------------------------
	// 9. Búsqueda en listas (search)
	// -------------------------------------------------------------
	printf("\n--- 9. Busqueda en lista ---\n");
	Tdata listaBuscar = create_list();
	append(&listaBuscar, crear_str("manzana"));
	append(&listaBuscar, crear_str("pera"));
	append(&listaBuscar, crear_str("uva"));
	Tdata buscar = crear_str("pera");
	printf("¿'pera' esta en la lista? %s\n", search(listaBuscar, buscar) ? "Si" : "No");
	free_tdata(buscar);
	free_tdata(listaBuscar);
	
	// -------------------------------------------------------------
	// Limpieza final
	// -------------------------------------------------------------
	printf("\n--- Liberando memoria restante ---\n");
	free_tdata(A);
	free_tdata(B);
	free_tdata(unionAB);
	free_tdata(interAB);
	free_tdata(diffAB);
	free_tdata(L);
	
	printf("\n===== PRUEBAS COMPLETADAS EXITOSAMENTE =====\n");
	return 0;
}
