#include "lista.h"
#include "hash.h"
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define CAPACIDAD_INICIAL 13
#define FACTOR_DE_CARGA 3
#define REDIMENSION 3

/* Definición del struct nodo_hash */

typedef struct nodo_hash{
	char* clave;
	void* valor;
} nodo_t;

/* Definición del struct hash */

struct hash{

	size_t cantidad;
	size_t capacidad;
	lista_t** tabla;
	void (*destruir_dato)(void*); 
};

/* Definición del struct hash_iter */

struct hash_iter{

	const hash_t* hash;
	size_t pos_tabla;
	lista_iter_t* lista_iter;
};

/* Funcion de hash utilizada en el TDA */

size_t funcion_hash(const char* clave, size_t capacidad){
	size_t pos_tabla = 1;
	size_t largo = strlen(clave);
	int multiplicador = 1 ;

	for(size_t i=0; i<largo; i++)
	{
		pos_tabla += (unsigned int)((int)clave[i] * multiplicador);
		multiplicador= multiplicador * 10;		
	}
	return pos_tabla % capacidad;
}

/* Aumenta o disminuye el tamaño de la tabla del hash y reposiciona los nodos del hash en la nueva tabla */

bool redimensionar_hash(hash_t* hash, bool aumentar){

	size_t nueva_capacidad;
	if(aumentar)
		nueva_capacidad = hash->capacidad * REDIMENSION;
	else
		nueva_capacidad = hash->capacidad / REDIMENSION;

	lista_t** nueva_tabla = malloc(nueva_capacidad * sizeof(lista_t*));
	if(!nueva_tabla)
		return false;

	for (int i = 0; i < nueva_capacidad; i++){
		nueva_tabla[i] = NULL;
	}
	for (int i = 0; i < hash->capacidad; i++){

		lista_t* lista = hash->tabla[i];
		if(lista != NULL){

			while(!lista_esta_vacia(lista)){

				nodo_t* nodo = lista_borrar_primero(lista);
				size_t nueva_posicion = funcion_hash(nodo->clave, nueva_capacidad);
				if(!nueva_tabla[nueva_posicion])
					nueva_tabla[nueva_posicion] = lista_crear();
				lista_insertar_ultimo(nueva_tabla[nueva_posicion], nodo);
			}
			lista_destruir(lista, NULL);
		}
	}
	free(hash->tabla);
    hash->tabla = nueva_tabla;
    hash->capacidad = nueva_capacidad;
    return true;	
}

/* Dada la clave y la posicion que ocupa en la tabla, devuelve el puntero al nodo si es que se encuentra
en el hash, si no se encuentra, devuelve NULL. Si se le pasa la opcion de borrar, elimina el nodo */

nodo_t* obtener_nodo(const hash_t* hash, const char* clave, size_t posicion, bool borrar){
	
	bool encontrado = false;
	nodo_t* nodo = NULL;
	if(hash->tabla[posicion] != NULL){
		lista_iter_t* iterador = lista_iter_crear(hash->tabla[posicion]);
		while(!lista_iter_al_final(iterador) && !encontrado){
			nodo = lista_iter_ver_actual(iterador);
			if (strcmp(clave, nodo->clave) == 0){
				encontrado = true;
			}
			if(borrar)
				lista_iter_borrar(iterador);
			lista_iter_avanzar(iterador);
		}
		lista_iter_destruir(iterador);
		if(!encontrado)
			return NULL;
	}
	return nodo;
}

/* Reemplaza el dato de una clave que se encuentra en el hash y elimina el dato antiguo si es necesario */

bool reemplazar_dato(hash_t* hash, const char* clave, void* dato, size_t posicion){

	nodo_t* nodo = obtener_nodo(hash, clave, posicion, false);
	if (nodo == NULL)
		return false;

	if(hash->destruir_dato){
		hash->destruir_dato(nodo->valor);
	}
	nodo->valor = dato;
	return true;
}

/* Crea un nuevo nodo_hash y lo devuelve */

nodo_t* nodo_hash_crear(char* clave, void* dato){

	nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo)
    	return NULL;
    nodo->clave = clave;
    nodo->valor = dato;

    return nodo;
}


/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t* hash_crear(hash_destruir_dato_t destruir_dato){

	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash)
		return NULL;
	lista_t** tabla = malloc(CAPACIDAD_INICIAL * sizeof(lista_t*));
	if(!tabla){
		free(hash);
		return NULL;
	}
	for (int i = 0; i < CAPACIDAD_INICIAL; i++){
		tabla[i] = NULL;
	}
	hash->tabla = tabla;
	hash->destruir_dato = destruir_dato;
	hash->capacidad = CAPACIDAD_INICIAL;
	hash->cantidad = 0;
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){

	float factor = (float)(hash->cantidad/hash->capacidad);
	if(factor > FACTOR_DE_CARGA){
		redimensionar_hash(hash, true);

	}
	size_t posicion = funcion_hash(clave, hash->capacidad);

	if(!hash->tabla[posicion]){
		hash->tabla[posicion] = lista_crear();
	}

	char* clave_copia = strdup(clave);

	if(hash_pertenece(hash, clave_copia)){
		reemplazar_dato(hash, clave_copia, dato, posicion);
		free(clave_copia);
		return true;
	}

	nodo_t* nuevo_nodo = nodo_hash_crear(clave_copia, dato);

	if(lista_insertar_ultimo(hash->tabla[posicion], nuevo_nodo)){
		hash->cantidad ++;
		return true;
	}
	return false;
	
}

void *hash_borrar(hash_t *hash, const char *clave){

	float factor = (float)(hash->cantidad/hash->capacidad);
	if((factor < 1 / FACTOR_DE_CARGA) && (hash->capacidad > CAPACIDAD_INICIAL)){
		redimensionar_hash(hash, false);
	}

	if(!hash_pertenece(hash, clave)){
		return NULL;
	}

	size_t posicion = funcion_hash(clave, hash->capacidad);

	hash->cantidad --;
	void* valor = hash_obtener(hash, clave);
	nodo_t* nodo = obtener_nodo(hash, clave, posicion, true);
	free(nodo->clave);
	free(nodo);

	return valor;
}

void *hash_obtener(const hash_t *hash, const char *clave){

	size_t posicion = funcion_hash(clave, hash->capacidad);
	nodo_t* nodo = obtener_nodo(hash, clave, posicion, false);
	if (nodo == NULL)
		return NULL;
	return nodo->valor;
}

bool hash_pertenece(const hash_t *hash, const char *clave){

	size_t posicion = funcion_hash(clave, hash->capacidad);
	if(obtener_nodo(hash, clave, posicion, false) == NULL){
		return false;
	}
	return true;
}

size_t hash_cantidad(const hash_t *hash){

	return hash->cantidad;
}

void hash_destruir(hash_t *hash){

	for (int i = 0; i < hash->capacidad; i++){
		
		lista_t* lista_actual = hash->tabla[i];

		if(lista_actual != NULL){

			while(!lista_esta_vacia(lista_actual)){
				nodo_t* nodo_actual = lista_borrar_primero(lista_actual);
				if(hash->destruir_dato){
					hash->destruir_dato(nodo_actual->valor);
				}
				free(nodo_actual->clave);
				free(nodo_actual);
			}
			lista_destruir(lista_actual, NULL);
		}
	}
	free(hash->tabla);
	free(hash);
}

/* Dado el hash y una posicion de la tabla, devuelve la siguiente posicion en la que hay una lista,
o -1 en caso de que no haya una nueva lista */

int buscar_siguiente_posicion(const hash_t* hash, size_t posicion){

	for (size_t i = posicion + 1; i < hash->capacidad; i++){
		
		if(hash->tabla[i])
			return (int)(i);
	}
	return -1;
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash){

	hash_iter_t* hash_iter = malloc(sizeof(hash_iter_t));
	if(!hash_iter) 
		return NULL;

	for(size_t i = 0; i < hash->capacidad; i++){

		if(hash->tabla[i] != NULL){
			lista_iter_t* lista_iter = lista_iter_crear(hash->tabla[i]);
			if(!lista_iter){
				free(hash_iter);
				return NULL;
			} 
			hash_iter->pos_tabla = i;
			hash_iter->lista_iter = lista_iter;
			hash_iter->hash = hash;
			return hash_iter;
		}
	}
	hash_iter->lista_iter = NULL;
	hash_iter->hash = hash;
	return hash_iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){

	if(hash_iter_al_final(iter))
		return false;

	lista_iter_avanzar(iter->lista_iter);
	if (lista_iter_al_final(iter->lista_iter)){

		int nueva_posicion = buscar_siguiente_posicion(iter->hash, iter->pos_tabla);
		if(nueva_posicion != -1){
			lista_iter_t* nuevo_lista_iter = lista_iter_crear(iter->hash->tabla[nueva_posicion]);
			iter->pos_tabla = (size_t)(nueva_posicion);
			lista_iter_destruir(iter->lista_iter);
			iter->lista_iter = nuevo_lista_iter;
		}
	}
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){

	if (hash_iter_al_final(iter))
		return NULL;

    nodo_t* nodo = lista_iter_ver_actual(iter->lista_iter);
    return nodo->clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){

	if(iter->lista_iter == NULL || lista_iter_al_final(iter->lista_iter))
		return true;
	return false;
}

void hash_iter_destruir(hash_iter_t *iter){

	if (iter->lista_iter) 
	 	lista_iter_destruir(iter->lista_iter);
    free(iter);
}
