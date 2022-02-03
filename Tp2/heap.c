#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

#define CAPACIDAD_INICIAL 3
#define FACTOR_REDIMENSION 2

struct heap{
	void** datos;
	size_t cantidad;
	size_t capacidad;
	cmp_func_t comparar;
};

/* *****************************************************************
 *                    FUNCIONES INTERNAS
 * *****************************************************************/


bool redimensionar(heap_t* heap, size_t nueva_capacidad){

	void** datos_copia = realloc(heap->datos, nueva_capacidad * sizeof(void*));
	if(datos_copia == NULL)
		return false;
	heap->datos = datos_copia;
	heap->capacidad = nueva_capacidad;
	return true;
}

void swap(void** datos, size_t pos_a, size_t pos_b) {
	void* auxiliar = datos[pos_a];
	datos[pos_a] = datos[pos_b];
	datos[pos_b] = auxiliar;
}

void upheap(void** datos, size_t pos_hijo, cmp_func_t comparar){

	if(pos_hijo == 0)
		return;
	size_t pos_padre = (pos_hijo - 1) / 2;;
	if(comparar(datos[pos_padre], datos[pos_hijo]) < 0){
		swap(datos, pos_padre, pos_hijo);
		upheap(datos, pos_padre, comparar);
	}
}

size_t calcular_maximo(void** datos, cmp_func_t comparar, size_t pos_padre, size_t pos_hijo_izquierdo, size_t pos_hijo_derecho, size_t cantidad){

	if(pos_hijo_izquierdo >= cantidad && pos_hijo_derecho >= cantidad)
		return pos_padre;

	size_t pos_maximo;
	if(pos_hijo_derecho < cantidad && comparar(datos[pos_hijo_izquierdo], datos[pos_hijo_derecho]) < 0)
		pos_maximo = pos_hijo_derecho;
	else
		pos_maximo = pos_hijo_izquierdo;

	if(comparar(datos[pos_padre], datos[pos_maximo]) >= 0)
		pos_maximo = pos_padre;

	return pos_maximo;
}

void downheap(void** datos, size_t cantidad, size_t pos_padre, cmp_func_t comparar){

	if(pos_padre >= cantidad)
		return;

	size_t pos_hijo_izquierdo = 2 * pos_padre + 1;
	size_t pos_hijo_derecho = 2 * pos_padre + 2;
	size_t pos_maximo = calcular_maximo(datos, comparar, pos_padre, pos_hijo_izquierdo, pos_hijo_derecho, cantidad);

	if(pos_maximo != pos_padre){
		swap(datos, pos_padre, pos_maximo);
		downheap(datos, cantidad, pos_maximo, comparar);
	}
}

void heapify(void** datos, size_t cantidad, cmp_func_t comparar){

	for (size_t i = cantidad / 2; i != -1; i--){
		downheap(datos, cantidad, i, comparar);
	}
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){

	heapify(elementos, cant, cmp);

	size_t pos_ultimo = cant - 1;
	while(pos_ultimo > 0){
		swap(elementos, pos_ultimo, 0);
		downheap(elementos, pos_ultimo, 0, cmp);
		pos_ultimo--;
	}
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp){

	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap)
		return NULL;

	void** datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));
	if (!datos){
		free(heap);
		return NULL;
	}

	heap->datos = datos;
	heap->cantidad = 0;
	heap->capacidad = CAPACIDAD_INICIAL;
	heap->comparar = cmp;

	return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){

	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap)
		return NULL;

	void** datos = malloc(n * sizeof(void*));
	if (!datos){
		free(heap);
		return NULL;
	}

	for(int i = 0; i < n; i++){
		datos[i] = arreglo[i];
		//printf("%i\n", *((int*)(datos[i])));
	}
	heap->datos = datos;	
	heap->cantidad = n;
	heap->capacidad = n;
	heap->comparar = cmp;
	//printf("%i\n", heap->cantidad);

	heapify(heap->datos, heap->cantidad, cmp);

	return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){

	if(destruir_elemento){
		for (int i = 0; i < heap->cantidad; i++){
			destruir_elemento(heap->datos[i]);
		}
	}	
	free(heap->datos);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap){

	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){

	return heap->cantidad == 0;
}

bool heap_encolar(heap_t *heap, void *elem){

	if(heap->cantidad == heap->capacidad){
		if(!redimensionar(heap, FACTOR_REDIMENSION * heap->capacidad))
			return false;
	}
	heap->datos[heap->cantidad] = elem;
	heap->cantidad++;
	upheap(heap->datos, heap->cantidad - 1, heap->comparar);

	return true;
}

void *heap_ver_max(const heap_t *heap){

	if (heap->cantidad == 0) 
		return NULL;
	return heap->datos[0];
}

void *heap_desencolar(heap_t *heap){

	if(heap->cantidad == 0)
		return NULL;

	if((heap->cantidad) * 4 <= heap->capacidad){
		redimensionar(heap, (heap->capacidad) / FACTOR_REDIMENSION);
	}
	swap(heap->datos, 0, heap->cantidad - 1);
	heap->cantidad--;
	downheap(heap->datos, heap->cantidad, 0, heap->comparar);

	return heap->datos[heap->cantidad];
}






