#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

struct lista;
typedef struct lista lista_t;


typedef struct lista_iter lista_iter_t;


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, se encuentra al principio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, se encuentra al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Borra el primer elemento de la lista. Si la lista tiene elementos,tambien se devuelve su valor, 
// si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del elemento borrado, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Pre: la lista fue creada
// Post: Devuelve el numero de elementos de la lista
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un lista.
// Post: devuelve un nuevo lista que apunta al primer elemento de la lista,
// o a NULL si está vacía.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Pre: el iterador fue creado
// Post: el iterador avanzó una posición y ahora apunta al siguiente elemento de
// la lista, si llegó al final devuelve NULL
bool lista_iter_avanzar(lista_iter_t *iter);

// Pre: El iterador fue creado
// Post: Devuelve el elemento de la lista al que apunta el iterador.
// Si la lista está vacía o el iterador llegó al final, devuelve NULL
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Pre: El iterador fue creado
// Post: Devuelve true si el iterador se encuentra al final de la lista, o false en caso contrario
bool lista_iter_al_final(const lista_iter_t *iter);

// Pre: El iterador fue creado
// Post: Se elimina el iterador
void lista_iter_destruir(lista_iter_t *iter);

// Pre: el iterador fue creado
// Post: se agregó un nuevo elemento a la lista en la posicion en la que se encuentra el iterador,
// devuelve false en caso de error
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Pre: el iterador fue creado.
// Post: Se borró el elemento en la posicion en la que se encuentra el iterador
// y se devolvió su valor, si la lista no estaba vacía. Si estaba al final de la lista 
// o la lista estaba vacía, devuelve NULL.
void *lista_iter_borrar(lista_iter_t *iter);


/* ******************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

// Pre: la lista fue creada
// Post: Recorre los elementos de la lista usando la funcion visitar y opcionalmente
// el valor extra.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación realizada.
void pruebas_lista_estudiante(void);

#endif  // LISTA_H