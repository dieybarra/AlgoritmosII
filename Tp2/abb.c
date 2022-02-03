#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
 
typedef struct abb_nodo {
    struct abb_nodo *izq;
    struct abb_nodo *der;
    char *clave;
    void *dato;
}abb_nodo_t;
 
struct abb {
    abb_nodo_t *raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    size_t cant;
};

struct abb_iter_rango{
    pila_t* pila;
    size_t cantidad;           
    char* inicio;
    char* fin;
};
 
struct abb_iter{
    pila_t* pila;
};
 
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(!abb){
        return NULL;
    }
    abb->cant=0;
    abb->destruir_dato = destruir_dato;
    abb->cmp = cmp;
 
    abb->raiz = NULL;
 
    return abb;
}
 
abb_nodo_t* crear_nodo(void* dato, char* clave){
    abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
    if (!nodo){
        return NULL;
    }
    nodo->dato=dato;
    nodo->clave = clave;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}
 
abb_nodo_t* abb_guardar_wrapper(abb_nodo_t* raiz, char* clave, void* dato, abb_comparar_clave_t cmp, abb_destruir_dato_t destruir, int* posicion){
    int comparacion = cmp(clave, raiz->clave);
    if(!raiz->der && comparacion>0){       
        *posicion = 1;
        return raiz;           
    }
    else if(!raiz->izq && comparacion<0){
        *posicion = -1;
        return raiz;        
    }
    else if(!raiz->izq && !raiz->der){
        if(comparacion<0){
            *posicion = -1;
            return raiz;            
        }
        else if(comparacion>0){		
            *posicion = 1;
            return raiz;            
        }
    }
    if(comparacion == 0){
    	
        if(destruir){
            destruir(raiz->dato);           
        }
        raiz->dato = dato;
        *posicion = 0;
        free(clave);
        return raiz;
    }
    else if(comparacion<0){
        return abb_guardar_wrapper(raiz->izq, clave, dato, cmp, destruir,posicion);
    }
    else{
        return abb_guardar_wrapper(raiz->der, clave, dato, cmp,  destruir, posicion);
    }
}
 
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    int posicion=0;
    abb_nodo_t* nodo;
    char* clave_copia = strdup(clave);
    if(!arbol->raiz){
        nodo = crear_nodo(dato, clave_copia);
        if(!nodo){
            return false;
        }
        arbol->raiz = nodo;
        arbol->cant++;
        return true;
    }
    nodo = abb_guardar_wrapper(arbol->raiz, clave_copia, dato, arbol->cmp, arbol->destruir_dato, &posicion);   
    if(posicion == -1){
        nodo->izq = crear_nodo(dato,clave_copia);
        if(!nodo){
            return false;
        }
        arbol->cant++;
        return true;
 
    }
    else if(posicion == 1){
        nodo->der = crear_nodo(dato,clave_copia);
        if(!nodo){
            return false;
        }
        arbol->cant++;
        return true;
 
    }
    else{
        return true;
    }
 
}
 
size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}
 
abb_nodo_t* buscar_nodo(abb_nodo_t* raiz, char* clave, abb_comparar_clave_t cmp, abb_nodo_t** padre){

    int comparacion;
    if(!raiz){
        *padre = NULL;
        return NULL;
    }
    comparacion = cmp(clave, raiz->clave);
    int comparar_hijo;

    if(comparacion == 0){
        *padre = NULL;
        return raiz;
    }
    
    if((!raiz->der && comparacion>0) || (!raiz->izq && comparacion<0)){
        *padre = NULL;
        return NULL;
    }
 
    else if(comparacion<0){
        comparar_hijo=cmp(clave, raiz->izq->clave);
        if(comparar_hijo==0){
            *padre = raiz;
            return raiz->izq;
        }
        return buscar_nodo(raiz->izq, clave, cmp, padre);           
    }
    else{
        comparar_hijo=cmp(clave, raiz->der->clave);
        if(comparar_hijo==0){
            *padre = raiz;
            return raiz->der;
        }
        return buscar_nodo(raiz->der, clave, cmp, padre);
    }
}
 
 
 
bool abb_pertenece(const abb_t *arbol, const char *clave){
    abb_nodo_t* nodo_aux;
    abb_nodo_t* padre;
    nodo_aux = buscar_nodo(arbol->raiz, (char*)clave, arbol->cmp, &padre);
    if(nodo_aux != NULL){
        return true;
    }
    return false;
}
 
void *abb_obtener(const abb_t *arbol, const char *clave){
    abb_nodo_t* nodo_aux;
    abb_nodo_t* padre;
    nodo_aux = buscar_nodo(arbol->raiz, (char*)clave, arbol->cmp, &padre);
    if(nodo_aux){
        return nodo_aux->dato;
    }
    return NULL;
}
 
 
int borrar_hijo(char* clave_padre, char* clave_nodo, abb_comparar_clave_t cmp){    
    return cmp(clave_padre, clave_nodo);
}
 
abb_nodo_t* buscar_reemplazante(abb_nodo_t* nodo){
    if(!nodo->der){
        return nodo;
    }
    return buscar_reemplazante(nodo->der);
}

void dos_hijos(abb_t* arbol, abb_nodo_t* nodo_aux){
	abb_nodo_t* reemplazante;
	reemplazante = buscar_reemplazante(nodo_aux->izq);
	char* clave_reemplazo = strdup(reemplazante->clave);
	void* dato_reemplazo = abb_borrar(arbol, reemplazante->clave);
	nodo_aux->dato = dato_reemplazo;
	free(nodo_aux->clave);          
	nodo_aux->clave = clave_reemplazo;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    abb_nodo_t* nodo_aux;
    abb_nodo_t* padre;
    void* dato;
    nodo_aux = buscar_nodo(arbol->raiz, (char*)clave, arbol->cmp, &padre);
    if(!nodo_aux){
        return NULL;
    }
    dato = nodo_aux->dato;
    if(!padre){
    	if(!nodo_aux->der && !nodo_aux->izq){
	        free(nodo_aux->clave);
	        free(nodo_aux);
	        arbol->cant--;
	        arbol->raiz = NULL;
	        return dato;
    	}
    	else if(nodo_aux->der && nodo_aux->izq){
	    	dos_hijos(arbol, nodo_aux);
	        return dato;
    	}
    	else{
      		abb_nodo_t* nuevo_hijo = (nodo_aux->der) ? nodo_aux->der : nodo_aux->izq;
    		free(nodo_aux->clave);
	        free(nodo_aux);
	        arbol->raiz = nuevo_hijo;
	        arbol->cant--;
    		return dato;
    	}

    }
    if(nodo_aux->der && nodo_aux->izq){     
        dos_hijos(arbol, nodo_aux);
        return dato;
    }
 
    abb_nodo_t* nuevo_hijo = (nodo_aux->der) ? nodo_aux->der : nodo_aux->izq;

    if (padre->izq == nodo_aux){
        padre->izq = nuevo_hijo;
    }else if (padre->der == nodo_aux){
        padre->der = nuevo_hijo;
    }else{
        arbol->raiz = nuevo_hijo;
    }
 
    free(nodo_aux->clave);
    free(nodo_aux);
    arbol->cant--;
    return dato;
 
 
 
}
 
void abb_destruir_nodos(abb_nodo_t* raiz, abb_destruir_dato_t destruir){
    if(!raiz){
        return;
    }
    abb_destruir_nodos(raiz->izq, destruir);
    abb_destruir_nodos(raiz->der, destruir);

    if(destruir){
        destruir(raiz->dato);
    }
    
    free(raiz->clave);
    free(raiz);
    
 
}
 
void abb_destruir(abb_t *arbol){
    abb_destruir_nodos(arbol->raiz, arbol->destruir_dato);
    free(arbol);
}
 
void iterar_inorder(abb_nodo_t* raiz, bool visitar(const char *, void *, void *), void *extra, bool* seguir_iterando){
	if(!raiz || !(*seguir_iterando)){
		return;
	}
	if(*seguir_iterando){			
    	iterar_inorder(raiz->izq, visitar, extra, seguir_iterando);
	}
	if(*seguir_iterando){
    	*seguir_iterando = visitar(raiz->clave,raiz->dato, extra);
	}
	if(*seguir_iterando){
    	iterar_inorder(raiz->der, visitar, extra, seguir_iterando);
	}
}
 
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	bool seguir_iterando=true;
    if(!visitar){
        return;
    }
    iterar_inorder(arbol->raiz, visitar, extra, &seguir_iterando);
}
 
void apilar_izquierda(pila_t* pila, abb_nodo_t* raiz){
    if(!raiz){
        return;
    }
    pila_apilar(pila,raiz);
    apilar_izquierda(pila, raiz->izq);
}
 
abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter || !arbol){
        return NULL;
    }
    iter->pila = pila_crear();
    apilar_izquierda(iter->pila, arbol->raiz);
    return iter;
}
 
 
const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	abb_nodo_t* actual;
	if(pila_ver_tope(iter->pila)){
    	actual =  pila_ver_tope(iter->pila);
    	return actual->clave;
	}
	return NULL;
    
}
 
bool abb_iter_in_avanzar(abb_iter_t *iter){
    abb_nodo_t* nodo;
    if(!pila_esta_vacia(iter->pila)){
        nodo = pila_desapilar(iter->pila);
        if(nodo->der){
            apilar_izquierda(iter->pila, nodo->der);        
        }
        return true;
    }
    return false;
}
 
bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}
 
void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free (iter);
}

/*********************************************
* Primitivas del iterador externo con rango
**********************************************/

void apilar_izquierda_rango(pila_t* pila, abb_nodo_t* raiz, char* inicio, char* fin, abb_comparar_clave_t cmp, size_t* cantidad){
    if(!raiz){
        return;
    }
 
    int comparar_inicio;
    int comparar_fin;
 
    if(inicio){
        comparar_inicio = cmp(raiz->clave, inicio);
    }
    else{
        comparar_inicio = 1;
    }
    if(fin){
        comparar_fin = cmp(raiz->clave, fin);
    }
    else{
        comparar_fin = -1;
    }
    if(comparar_inicio >= 0 && comparar_fin <= 0){
        pila_apilar(pila,raiz);
        (*cantidad)++;
        raiz = raiz->izq;
    }
    if(comparar_inicio < 0)
        raiz = raiz->der;
    if(comparar_fin >0)
        raiz = raiz->izq;
    apilar_izquierda_rango(pila, raiz, inicio, fin, cmp, cantidad);
}
 
iterango_t *iterango_crear(const abb_t *arbol, char* inicio, char* fin){
    iterango_t* iter = malloc(sizeof(iterango_t));
    if(!iter || !arbol){
        return NULL;
    }
    iter->pila = pila_crear();
    iter->cantidad=0;
    if(inicio){
        iter->inicio = inicio;
    }
    else{
        iter->inicio = NULL;
    }
    if(fin){
        iter->fin = fin;
    }
    else{
        iter->fin = NULL;
    }
    apilar_izquierda_rango(iter->pila, arbol->raiz, inicio, fin, arbol->cmp, &(iter->cantidad));
    return iter;
}
 
 
const char *iterango_ver_actual(const iterango_t *iter){
    abb_nodo_t* actual;
    if(pila_ver_tope(iter->pila)){
        actual =  pila_ver_tope(iter->pila);
        return actual->clave;
    }
    return NULL;
}

void *iterango_ver_actual_dato(const iterango_t *iter){
    abb_nodo_t* actual;
    if(pila_ver_tope(iter->pila)){
        actual = pila_ver_tope(iter->pila);
        return actual->dato;
    }
    return NULL;
}
 
bool iterango_avanzar(iterango_t *iter){
    abb_nodo_t* nodo;
    if(!pila_esta_vacia(iter->pila)){
        nodo = pila_desapilar(iter->pila);
        if(nodo->der){
            apilar_izquierda_rango(iter->pila, nodo->der, iter->inicio, iter->fin, strcmp, &(iter->cantidad));        
        }
        return true;
    }
    return false;
}
 
bool iterango_al_final(const iterango_t *iter){
    return pila_esta_vacia(iter->pila);
}
 
void iterango_destruir(iterango_t* iter){
    pila_destruir(iter->pila);
    free (iter);
}
 
size_t iterango_cantidad(iterango_t* iter){
    return iter->cantidad;
}
 


