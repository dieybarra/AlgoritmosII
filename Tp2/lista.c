#include "lista.h"

#include <stdlib.h>

/* Definición del struct nodo */

typedef struct nodo{
	void* dato;
	struct nodo* proximo;
} nodo_t;

nodo_t* crear_nodo_lista(void* valor){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo)
		return NULL;
	nodo->dato = valor;
	nodo->proximo = NULL;
	return nodo;
}


/* Definición del struct lista.*/
struct lista {
	nodo_t* primero;
	nodo_t* ultimo;
	size_t largo;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(void){

	lista_t* lista = malloc(sizeof(lista_t)); 
	if (lista == NULL) 
        return NULL;
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->largo = 0;
    return lista;
}	

bool lista_esta_vacia(const lista_t *lista){

	//if(lista->primero == NULL && lista->ultimo == NULL){
	if(lista->largo == 0)
		return true;
	return false;
}

bool lista_insertar_primero(lista_t *lista, void *dato){

	nodo_t* nuevo_nodo = crear_nodo_lista(dato);
	if (nuevo_nodo == NULL)
		return false;
	nuevo_nodo->proximo = lista->primero;
	lista->primero = nuevo_nodo;
	if(lista->ultimo == NULL)
		lista->ultimo = nuevo_nodo;
	lista->largo ++;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){

	nodo_t* nuevo_nodo = crear_nodo_lista(dato);
	if (nuevo_nodo == NULL)
		return false;

	if(lista_esta_vacia(lista))
		lista->primero = nuevo_nodo;
	else
		lista->ultimo->proximo = nuevo_nodo;
	
	lista->ultimo = nuevo_nodo;
	lista->largo ++;
	return true;	
}

void *lista_borrar_primero(lista_t *lista){

	if(lista_esta_vacia(lista))
		return NULL;

	nodo_t* auxiliar = lista->primero;
	lista->primero = lista->primero->proximo;
	void* dato = auxiliar->dato;
	free(auxiliar);
	lista->largo --;
	if (lista->largo == 0)
		lista->ultimo = NULL;

	return dato;
}

void *lista_ver_primero(const lista_t *lista){

	if (lista_esta_vacia(lista))
		return NULL;
	else
		return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){

	if (lista_esta_vacia(lista))
		return NULL;
	else
		return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){

	return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){

	while(lista->primero != NULL){
		
		if(destruir_dato != NULL)
			destruir_dato(lista->primero->dato);

		nodo_t* proximo_auxiliar = lista->primero->proximo;
		free(lista->primero);
		lista->primero = proximo_auxiliar;
	}

	free(lista);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

struct lista_iter {
	lista_t* lista;
	nodo_t* anterior;
	nodo_t* actual;
};

lista_iter_t *lista_iter_crear(lista_t *lista){

	lista_iter_t* iter = malloc(sizeof(lista_iter_t)); 
	if (iter == NULL) 
        return NULL;
    iter->lista = lista;
    iter->actual = lista->primero;
    iter->anterior = NULL;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){

	if (iter->actual == NULL)
		return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->proximo;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){

	if(iter->actual == NULL)
		return NULL;
	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){

	if(iter->actual == NULL)
		return true;
	return false;
}

void lista_iter_destruir(lista_iter_t *iter){

	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){

	if (iter->actual == iter->lista->primero){

		if (lista_insertar_primero(iter->lista, dato)){
			iter->actual = iter->lista->primero;
			return true;
		} else{
			return false;
		}	
	} 

	else if(iter->anterior == iter->lista->ultimo){

		if (lista_insertar_ultimo(iter->lista, dato)){
			//iter->anterior = iter->actual;  preguntar si tengo que dejar actual = NULL
			iter->actual = iter->lista->ultimo;
			return true;
		} else{
			return false;
		}
	}

	else{

		nodo_t* nuevo_nodo = crear_nodo_lista(dato);
		if (nuevo_nodo == NULL)
			return false;

		nuevo_nodo->proximo = iter->actual;
		iter->actual = nuevo_nodo;
		iter->anterior->proximo = nuevo_nodo;
		iter->lista->largo ++;
		return true;
	}
}

void *lista_iter_borrar(lista_iter_t *iter){

	if (iter->actual == iter->lista->primero){
		void* dato = lista_borrar_primero(iter->lista);
		iter->actual = iter->lista->primero;
		return dato;

	}

	else if(iter->actual == NULL){
		return NULL;
	}

	else{

		void* dato = iter->actual->dato;
		iter->anterior->proximo = iter->actual->proximo;
		free(iter->actual);
		iter->actual = iter->anterior->proximo;
		if(iter->actual == NULL)
			iter->lista->ultimo = iter->anterior;
		
		iter->lista->largo --;

		return dato;

	}
}

/* *****************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){

	nodo_t* auxiliar = lista->primero;
	bool estado = true;
	while(auxiliar != NULL && estado == true){
		estado = visitar(auxiliar->dato, extra);
		auxiliar = auxiliar->proximo;
	}
}







