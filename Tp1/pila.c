#include "pila.h"

#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void){

	pila_t *pila = malloc(sizeof(pila_t));

    if (pila == NULL) {
        return NULL;
    }
    pila->datos = malloc(3 * sizeof(void*));

    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    pila->cantidad = 0;
    pila->capacidad = 3;
    return pila;
}

void pila_destruir(pila_t *pila){

	free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){

	if (pila->cantidad == 0){
		return true;
	}
	
	return false;
}

bool pila_apilar(pila_t *pila, void *valor){

	if (pila->cantidad == pila->capacidad){

		void** datos_copia = realloc(pila->datos, 2 * pila->capacidad * sizeof(void*));
		if(datos_copia == NULL){
			return false;
		}
		pila->datos = datos_copia;
		pila->capacidad *= 2;
	}

	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

void *pila_ver_tope(const pila_t *pila){
	if(pila->cantidad > 0){
		return pila->datos[pila->cantidad - 1];
	}
	return NULL;
}

void *pila_desapilar(pila_t *pila){

	if (pila->cantidad == 0){
		return NULL;
	}
	if ((pila->cantidad) * 4 <= pila->capacidad){
		pila->datos = realloc(pila->datos, (pila->capacidad) / 2 *sizeof(void*));
		pila->capacidad /= 2;

	}
	pila->cantidad--;
	return pila->datos[pila->cantidad];
	
}


