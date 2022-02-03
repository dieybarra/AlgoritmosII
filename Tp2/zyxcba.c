#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"
#include "csv.h"
#include "lista.h"
#include "abb.h"
#include "hash.h"
#include "cola.h"
#include "heap.h"
#include "paciente.h"
#include "doctor.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"
#define URGENTE "URGENTE"
#define REGULAR "REGULAR"


int comparar_doctores(const char* a, const char* b){

	return strcmp(a, b);
}

long int comparar_pacientes(const void* a, const void* b){

	long int inscripcion_a = paciente_obtener_inscripcion((paciente_t*) a);
	long int inscripcion_b = paciente_obtener_inscripcion((paciente_t*) b);
	return inscripcion_b - inscripcion_a;
}

void pedir_turno(const char** parametros, hash_t* hash_doctores, hash_t* hash_pacientes){

	paciente_t* paciente = hash_obtener(hash_pacientes, parametros[0]);
	if(paciente == NULL){
		fprintf(stdout, ENOENT_PACIENTE, parametros[0]);
		return;
	}
	//fprintf(stderr, "%s\n", parametros[1] );

	//fprintf(stderr, "pertenece? %i", hash_pertenece(hash_doctores, parametros[1]));


	lista_t* lista = hash_obtener(hash_doctores, parametros[1]);
	if(lista == NULL){
		fprintf(stdout, ENOENT_ESPECIALIDAD, parametros[1]);
		return;
	}

	if(strcmp(parametros[2], URGENTE) != 0 && strcmp(parametros[2], REGULAR) != 0){
		fprintf(stdout, ENOENT_URGENCIA, parametros[2]);
		return;
	}
	cola_t* cola_pacientes = lista_ver_primero(lista);
	heap_t* heap_pacientes = lista_ver_ultimo(lista);

	if(strcmp(parametros[2], URGENTE) == 0){
		cola_encolar(cola_pacientes, paciente);
		//fprintf(stderr, "hola");
	}
	else{
		heap_encolar(heap_pacientes, paciente);
	}

	fprintf(stdout, PACIENTE_ENCOLADO, parametros[0]);

	size_t cantidad_pacientes = heap_cantidad(heap_pacientes) + cola_cantidad(cola_pacientes);
	fprintf(stdout, CANT_PACIENTES_ENCOLADOS, cantidad_pacientes, parametros[1]);
}

void atender(const char** parametros, abb_t* abb_doctores, hash_t* hash_doctores, hash_t* hash_pacientes){

	doctor_t* doctor = abb_obtener(abb_doctores, parametros[0]);
	if(doctor == NULL){
		fprintf(stdout, ENOENT_DOCTOR, parametros[0]);
		return;
	}
	lista_t* lista = hash_obtener(hash_doctores, doctor_obtener_especialidad(doctor));

	cola_t* cola_pacientes = lista_ver_primero(lista);
	heap_t* heap_pacientes = lista_ver_ultimo(lista);
	paciente_t* paciente_atendido = NULL;

	if(cola_cantidad(cola_pacientes) == 0 && heap_cantidad(heap_pacientes) == 0){
		fprintf(stdout, SIN_PACIENTES);	
		return;	
	}

	else if(cola_cantidad(cola_pacientes) > 0){
		paciente_atendido = cola_desencolar(cola_pacientes);
	}
	else{
		paciente_atendido = heap_desencolar(heap_pacientes);
	}

	doctor_sumar_atendido(doctor);
	fprintf(stdout, PACIENTE_ATENDIDO, paciente_obtener_nombre(paciente_atendido));

	size_t cantidad_pacientes = heap_cantidad(heap_pacientes) + cola_cantidad(cola_pacientes);
	fprintf(stdout, CANT_PACIENTES_ENCOLADOS, cantidad_pacientes, doctor_obtener_especialidad(doctor));
}

bool imprimir_doctor(const char * clave, void * dato, void * contador){

	size_t* contador_doctores = contador;
	*contador_doctores +=1;
	doctor_t* doctor = (doctor_t*) dato;
	fprintf(stdout, INFORME_DOCTOR, *contador_doctores, clave, doctor_obtener_especialidad(doctor), doctor_obtener_atendidos(doctor));
	return true;	
}

void imprimir_cantidad(iterango_t* iter_rango){
    while(!iterango_al_final(iter_rango)){
        iterango_avanzar(iter_rango);
    }
    fprintf(stdout, DOCTORES_SISTEMA, iterango_cantidad(iter_rango));
    iterango_destruir(iter_rango);
}
 
void informe(const char** parametros, abb_t* abb_doctores, size_t contador){
    doctor_t* doctor;
    size_t contador_doctores = 0;
    if (contador != 2){     
        fprintf(stdout, ENOENT_PARAMS, COMANDO_INFORME);
        return;
    }
 
    iterango_t* iter_rango = NULL;
 
    if(strcmp(parametros[0],"") == 0 && strcmp(parametros[1], "") == 0){
        fprintf(stdout, DOCTORES_SISTEMA, abb_cantidad(abb_doctores));
        abb_in_order(abb_doctores, imprimir_doctor, &contador_doctores);
        return;
    }
    else if(strcmp(parametros[1], "") == 0){
        char* ultimo = NULL;
        iter_rango = iterango_crear(abb_doctores, (char*)parametros[0], ultimo);
        imprimir_cantidad(iter_rango);
        iter_rango = iterango_crear(abb_doctores, (char*)parametros[0], ultimo);        
    }
    else{
        iter_rango = iterango_crear(abb_doctores, (char*)parametros[0], (char*)parametros[1]);
        imprimir_cantidad(iter_rango);
        iter_rango = iterango_crear(abb_doctores, (char*)parametros[0], (char*)parametros[1]);
    }
 	contador_doctores = 1;
    while(!iterango_al_final(iter_rango)){
        doctor = iterango_ver_actual_dato(iter_rango);
        fprintf(stdout, INFORME_DOCTOR, contador_doctores, doctor_obtener_nombre(doctor), doctor_obtener_especialidad(doctor), doctor_obtener_atendidos(doctor));
        iterango_avanzar(iter_rango);
        contador_doctores++;
    }
 
    iterango_destruir(iter_rango);
 
}

void procesar_comando(const char* comando, const char** parametros, abb_t* abb_doctores, hash_t* hash_doctores, hash_t* hash_pacientes, size_t contador){
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		pedir_turno(parametros, hash_doctores, hash_pacientes);

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		atender(parametros, abb_doctores, hash_doctores, hash_pacientes);

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		informe(parametros, abb_doctores, contador);

	} else {
		fprintf(stdout, ENOENT_CMD, comando);
	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(abb_t* abb_doctores, hash_t* hash_doctores, hash_t* hash_pacientes){
	char* linea = NULL;
	size_t c = 0;
	size_t contador = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':', &contador);
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',', &contador);
		procesar_comando(campos[0], (const char**) parametros, abb_doctores, hash_doctores, hash_pacientes, contador);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

bool guardar_doctores_hash(void* dato, void* hash_doctores){
    doctor_t* doctor = dato;

    if (!hash_pertenece((hash_t*)hash_doctores, doctor_obtener_especialidad(doctor))){
    	lista_t* lista = lista_crear();
    	heap_t* heap_pacientes = heap_crear(comparar_pacientes);
    	cola_t* cola_pacientes = cola_crear();
    	lista_insertar_primero(lista, cola_pacientes);
    	lista_insertar_ultimo(lista, heap_pacientes);

     	hash_guardar(hash_doctores, doctor_obtener_especialidad(doctor), lista);
     	//fprintf(stderr, "pertenece? %i", hash_pertenece(hash_doctores, doctor_obtener_especialidad(doctor)));
   	
    }
    return true;
}

bool guardar_doctores_abb(void* dato, void* abb_doctores){
	//fprintf(stdout, "hola" );
   	doctor_t* doctor = dato;
    if (abb_guardar(abb_doctores, doctor_obtener_nombre(doctor), doctor))
    	return true;
    return false;
}

bool guardar_pacientes(void* dato, void* hash_pacientes){
   	paciente_t* paciente = dato;
   	hash_guardar(hash_pacientes, paciente_obtener_nombre(paciente), paciente);
   	return true;
}

void lista_hash_doctores_destruir(void* dato){

	lista_t* lista_doctores = dato;
	cola_destruir(lista_ver_primero(lista_doctores), NULL);
	heap_destruir(lista_ver_ultimo(lista_doctores), NULL);
	lista_destruir(lista_doctores, NULL);
}




int main(int argc, char** argv) {

	if (argc != 3) {
        fprintf(stdin, ENOENT_CANT_PARAMS);
        return 1;
    }	

    lista_t* lista_doctores = csv_crear_estructura(argv[1], doctor_crear, NULL);
    if(lista_doctores == NULL){
 		fprintf(stdout, ENOENT_ARCHIVO, argv[1]);
        return 1;   	
    }

    abb_t* abb_doctores = abb_crear(comparar_doctores, NULL);
    if(!abb_doctores){
    	//fprintf(stdout, "no creo abb");
    	return 1;
    }

    hash_t* hash_doctores = hash_crear(lista_hash_doctores_destruir);
    if(!hash_doctores)
    	return 1;

    lista_iterar(lista_doctores, guardar_doctores_hash, hash_doctores);

    //if(lista_ver_primero(lista_doctores) != NULL){
    //	fprintf(stdout, "hoaaaa" );
    //	fprintf(stdout, "%i\n", abb_guardar(abb_doctores, "Dr Hipócrates", lista_ver_primero(lista_doctores)));
    //}

    lista_iterar(lista_doctores, guardar_doctores_abb, abb_doctores);

	lista_t* lista_pacientes = csv_crear_estructura(argv[2], paciente_crear, NULL);
	if(lista_pacientes == NULL){
 		fprintf(stdout, ENOENT_ARCHIVO, argv[2]);
        return 1;   	
    }
    hash_t* hash_pacientes = hash_crear(NULL);
    if(!hash_pacientes)
    	return 1;

    lista_iterar(lista_pacientes, guardar_pacientes, hash_pacientes);

    procesar_entrada(abb_doctores, hash_doctores, hash_pacientes);

    abb_destruir(abb_doctores);
    hash_destruir(hash_doctores);
    lista_destruir(lista_doctores, doctor_destruir);

    hash_destruir(hash_pacientes);   
    lista_destruir(lista_pacientes, paciente_destruir);

	return 0;
}
