#ifndef PACIENTE_H
#define PACIENTE_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct paciente paciente_t;

/* Crea un paciente. Recibe un vector de vectores que contiene el nombre del paciente
 y su numero de inscripcion. Devuelve un puntero al paciente, el cual debe ser destruido con
 paciente_destruir(). */

void* paciente_crear(char** datos, void* extra);

/* Recibe un struct paciente y devuelve su nombre*/

char* paciente_obtener_nombre(paciente_t* paciente);

/* Recibe un struct paciente y devuelve su numero de inscripcion*/

long int paciente_obtener_inscripcion(paciente_t* paciente);


/* Elimina el paciente.*/

void paciente_destruir(void* dato);


#endif  // PACIENTE_H