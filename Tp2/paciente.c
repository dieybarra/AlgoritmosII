#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include "paciente.h"
#include "lista.h"
#include "heap.h"
#include "cola.h"


struct paciente{
	char* nombre;
	long int inscripcion;
};

void* paciente_crear(char** datos, void* extra){

	size_t* contador = extra;
	if(*contador != 2){
		return NULL;
	}

	paciente_t* paciente = malloc(sizeof(paciente_t));
	if(!paciente)
		return NULL;
	paciente->nombre = strdup(datos[0]);
	paciente->inscripcion =	strtol(datos[1], NULL, 10);

	return paciente;
}

char* paciente_obtener_nombre(paciente_t* paciente){
	return paciente->nombre;
}

long int paciente_obtener_inscripcion(paciente_t* paciente){
	return paciente->inscripcion;
}

void paciente_destruir(void* dato){

	paciente_t* paciente = (paciente_t*) dato;
	free(paciente->nombre);
	free(paciente);
}
