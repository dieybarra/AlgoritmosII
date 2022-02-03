#include "cola.h"

#include <stdlib.h>

/* Definición del struct nodo */

typedef struct nodo{
	void* dato;
	struct nodo* proximo;
} nodo_t;

nodo_t* crear_nodo(void* valor){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo)
		return NULL;
	nodo->dato = valor;
	nodo->proximo = NULL;
	return nodo;
}


/* Definición del struct cola.*/
struct cola {
	nodo_t* primero;
	nodo_t* ultimo;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t *cola_crear(void){

	cola_t* cola = malloc(sizeof(cola_t)); 
	if (cola == NULL) 
        return NULL;
    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){

	while(cola->primero != NULL){
		
		if(destruir_dato != NULL)
			destruir_dato(cola->primero->dato);

		nodo_t* proximo_auxiliar = cola->primero->proximo;
		free(cola->primero);
		cola->primero = proximo_auxiliar;
	}

	free(cola);
}

bool cola_esta_vacia(const cola_t *cola){

	if(cola->primero == NULL && cola->ultimo == NULL){
		return true;
	}
	return false;
}

bool cola_encolar(cola_t *cola, void *valor){

	if (cola_esta_vacia(cola)){

		cola->ultimo = crear_nodo(valor);
		cola->primero = cola->ultimo;
	} else{

		cola->ultimo->proximo = crear_nodo(valor);
		cola->ultimo = cola->ultimo->proximo;
	}

	if (cola->ultimo == NULL)
		return false;
	else
		return true;
}

void *cola_ver_primero(const cola_t *cola){

	if (cola_esta_vacia(cola))
		return NULL;
	else
		return cola->primero->dato;
}

void *cola_desencolar(cola_t *cola){

	if (cola_esta_vacia(cola))
		return NULL;
	else{

		void* dato_auxiliar = cola->primero->dato;
		nodo_t* proximo_auxiliar = cola->primero->proximo;
		free(cola->primero);

		cola->primero = proximo_auxiliar;
		if(cola->primero == NULL)
			cola->ultimo = NULL;
		return dato_auxiliar;
	}
}
