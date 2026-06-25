#include "Automata.h"

int menu() {
	int op;
	printf("\n--- Menu Principal ---\n");
	printf("(1) Cargar AF\n");
	printf("(2) Mostrar AF\n");
	printf("(3) Analizar Cadena\n");
	printf("(4) Convertir de AFND a AFD\n");
	printf("(0) Salir del programa\n");
	printf("Su eleccion: ");
	scanf("%d", &op);
	getchar();
	return op;
}

int main() {
	Automata* af = NULL;
	Automata* afd = NULL;
	Automata* afdRenom = NULL;
	int op, subOp;
	char* cad;
	
	do {
		op = menu();
		switch (op) {
		case 1:
			// Si se vuelve a cargar, se libera todo lo anterior de la memoria heap
			if (af) free_automata(&af);
			if (afd) free_automata(&afd);
			if (afdRenom) free_automata(&afdRenom);
			
			printf("\nSeleccione Automata:\n");
			printf(" 1 - Nuevo Automata (Carga Manual)\n");
			printf(" 2 - Acepta cadenas empiecen con ba\n");
			printf(" 3 - Acepta cadenas que no terminen con 11 o cadenas que empiecen con 101\n");
			printf(" 4 - Acepta cadenas que contengan el 1011, 100, 111\n");
			printf(" 5 - Acepta cadenas que terminan en zyx\n");
			printf(" Opcion: ");
			scanf("%d", &subOp);
			getchar();
			
			if (subOp == 1) {
				af = cargaAF();
				printf("\nAutomata manual cargado con exito.\n");
			} else if (subOp >= 2 && subOp <= 5) {
				af = crear_automata_predefinido(subOp);
				printf("\nAutomata predefinido cargado con exito.\n");
			} else {
				printf("\nOpcion invalida.\n");
			}
			break;
			
		case 2:
			if (!af) {
				printf("\nError, no hay AF cargado.\n");
			} else if (!afd) {
				printf("\n--- Vista de Automata Original ---\n");
				mostrarAF(af);
			} else {
				printf("\nSeleccione el Automata a mostrar:\n");
				printf("1 - AFND Original\n");
				printf("2 - AFD Convertido\n");
				//printf("3 - AFD Renombrado\n");
				printf("Opcion: ");
				scanf("%d", &subOp);
				getchar();
				
				switch (subOp) {
				case 1:
					printf("\n--- AFND Original ---\n");
					mostrarAF(af);
					break;
				case 2:
					printf("\n--- AFD Convertido ---\n");
					mostrarAF(afd);
					break;
				//case 3:
				//	printf("\n--- AFD Renombrado ---\n");
				//	mostrarAF(afdRenom);
				//	break;
				default:
					printf("Opcion invalida.\n");
				}
			}
			break;
			
		case 3:
			if (!af) {
				printf("\nError, no hay AF cargado.\n");
			} else {
				Automata* autTrabajo = af;
				if (afd) {
					printf("\nSeleccione con que automata desea trabajar:\n");
					printf("1 - AFND originalmente ingresado\n");
					printf("2 - AFD convertido\n");
					printf("Opcion: ");
					scanf("%d", &subOp);
					getchar();
					if (subOp == 2) {
						autTrabajo = afd;
					}
				}
				
				printf("Ingrese la cadena a Analizar: ");
				cad = leeCad();
				if (cadenaAceptada(autTrabajo, cad)) {
					printf("\nLa cadena es ACEPTADA.\n");
				} else {
					printf("\nLa cadena NO es aceptada.\n");
				}
				free(cad);
			}
			break;
			
		case 4:
			if (af) {
				if (afd) free_automata(&afd);
				if (afdRenom) free_automata(&afdRenom);
				
				afd = conversionAFND_AFD(af);
				afdRenom = renombrarAFD(afd);
				printf("\nConversion realizada.\n");
			} else {
				printf("Cargue un AF primero.\n");
			}
			break;
			
		case 0:
			break;
		default:
			printf("Opcion incorrecta.\n");
		}
	} while (op != 0);
	
	if (af) free_automata(&af);
	if (afd) free_automata(&afd);
	if (afdRenom) free_automata(&afdRenom);
	
	return 0;
}
