

#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include "doctor.h"
#include "lista.h"
#include "heap.h"
#include "cola.h"


struct doctor{
	char* nombre;
	char* especialidad;
	size_t atendidos;
};

void* doctor_crear(char** datos, void* extra){
	size_t* contador = extra;
	if(*contador != 2){
		return NULL;
	}
	doctor_t* doctor = malloc(sizeof(doctor_t));
	if(!doctor)
		return NULL;
	doctor->nombre = strdup(datos[0]);
	doctor->especialidad = strdup(datos[1]);
	doctor->atendidos = 0;

	return doctor;
}

char* doctor_obtener_nombre(doctor_t* doctor){
	return doctor->nombre;
}

char* doctor_obtener_especialidad(doctor_t* doctor){
	return doctor->especialidad;
}

size_t doctor_obtener_atendidos(doctor_t* doctor){
	return doctor->atendidos;
}

void doctor_sumar_atendido(doctor_t* doctor){
	doctor->atendidos ++;
}


void doctor_destruir(void* dato){

	doctor_t* doctor = (doctor_t*) dato; 
	free(doctor->nombre);
	free(doctor->especialidad);
	free(doctor);
}
