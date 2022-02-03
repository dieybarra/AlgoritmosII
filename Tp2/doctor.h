#ifndef DOCTOR_H
#define DOCTOR_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct doctor doctor_t;

/* Crea un doctor. Recibe un vector de vectores que contiene el nombre del doctor
 y su numero especialidad. Devuelve un puntero al doctor, el cual debe ser destruido con
 doctor_destruir(). */

void* doctor_crear(char** datos, void* extra);

/* Recibe un struct doctor y devuelve su nombre*/

char* doctor_obtener_nombre(doctor_t* doctor);

/* Recibe un struct doctor y devuelve su especialidad*/

char* doctor_obtener_especialidad(doctor_t* doctor);

/* Recibe un struct doctor y devuelve la cantidad de atendidos*/

size_t doctor_obtener_atendidos(doctor_t* doctor);

/* Suma un atendido*/

void doctor_sumar_atendido(doctor_t* doctor);

/* Elimina el doctor.*/

void doctor_destruir(void* dato);




#endif  // DOCTOR_H