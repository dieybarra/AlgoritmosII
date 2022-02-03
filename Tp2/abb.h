#ifndef ABB_H
#define ABB_H

#include <stdbool.h> 
#include <stddef.h> 
#include <string.h>

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

bool abb_guardar(abb_t *arbol, const char *clave, void *dato);
void *abb_borrar(abb_t *arbol, const char *clave);

void *abb_obtener(const abb_t *arbol, const char *clave);
bool abb_pertenece(const abb_t *arbol, const char *clave);

size_t abb_cantidad(abb_t *arbol);

void abb_destruir(abb_t *arbol);


/*****************************************************
 * Primitivas del iterador externo del abb
* ****************************************************/

typedef struct abb_iter abb_iter_t;

abb_iter_t *abb_iter_in_crear(const abb_t *arbol);
bool abb_iter_in_avanzar(abb_iter_t *iter);
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);
bool abb_iter_in_al_final(const abb_iter_t *iter);
void abb_iter_in_destruir(abb_iter_t* iter);

/*****************************************************
 * Primitivas del iterador externo del abb con rango
* ****************************************************/
typedef struct abb_iter_rango iterango_t;
 
//Crea un iterador del arbol señalando al primer elemento dado por inicio.*/
iterango_t *iterango_crear(const abb_t *arbol, char* inicio, char* fin);
 
//Avanza posicion del iterador de manera inorder y devuelve true. Si no puede avanzar o el árbol está vacío
//devuelve false
bool iterango_avanzar(iterango_t *iter);
 
//devuelve la clave del nodo que señala el iterador
const char *iterango_ver_actual(const iterango_t *iter);

void *iterango_ver_actual_dato(const iterango_t *iter);
 
//Devuelve true si el iterador llegó al final del arbol, sino, devuelve false
bool iterango_al_final(const iterango_t *iter);
 
//Destruye el iterador
void iterango_destruir(iterango_t* iter);

size_t iterango_cantidad(iterango_t* iter);



/*****************************************************
 * Primitivas del iterador interno del abb
* ****************************************************/


//Visita cada nodo del abb y le aplica la funcion visitar
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


#endif  // ABB_H