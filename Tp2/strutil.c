#include "strutil.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>



char *substr(const char *str, size_t n){

	char* nuevo_str = malloc(sizeof(char)*(n+1));

	if (nuevo_str == NULL)
		return NULL;

	strncpy(nuevo_str, str, n);
	nuevo_str[n] = '\0';
	return nuevo_str;
}

char **split(const char *str, char sep, size_t* contador){

	size_t contador_palabras = 0;

	for (int i = 0; i < strlen(str); ++i){
		if (str[i] == sep)
			contador_palabras++;
	}
	contador_palabras++;

	char** vector = malloc(sizeof(char*)*(contador_palabras + 1));

	int j = 0;
	size_t contador_caracteres = 0;
	int posicion_vector = 0;
	size_t contador_caracteres_copiados = 0;

	while(true){

		if (str[j] != sep && str[j] != '\0')
			contador_caracteres++;

		else{

			vector[posicion_vector] = substr(str + contador_caracteres_copiados, contador_caracteres);
			posicion_vector++;
			contador_caracteres_copiados += (contador_caracteres + 1);
			contador_caracteres = 0;

			if(str[j] == '\0')
				break;
		}
		j++;
	}

	vector[posicion_vector] = NULL;
	*contador = contador_palabras;
	return vector;

}

char *join(char **strv, char sep){

	int i = 0;
	size_t contador_caracteres = 0;

	while(strv[i] != NULL){
		contador_caracteres += strlen(strv[i]);

		if(sep != '\0')
			contador_caracteres++;
		i++;
	}

	if (contador_caracteres == 0 || sep == '\0')
		contador_caracteres++;

	char* nuevo_str = malloc(sizeof(char)*contador_caracteres);

	if(contador_caracteres == 1){
		nuevo_str[0] = '\0';
		return nuevo_str;
	}

	int posicion_vector = 0;
	int posicion_cadena = 0;
	int posicion_nuevo = 0;

	while(strv[posicion_vector] != NULL){

		while(strv[posicion_vector][posicion_cadena] != '\0'){

			nuevo_str[posicion_nuevo] = strv[posicion_vector][posicion_cadena];

			posicion_nuevo++;
			posicion_cadena++;
		}

		if(sep != '\0'){
			nuevo_str[posicion_nuevo] = sep;
			posicion_nuevo++;
		}

		posicion_cadena = 0;
		posicion_vector++;
	}

	if(sep != '\0')
		posicion_nuevo--;

	nuevo_str[posicion_nuevo] = '\0';

	return nuevo_str;

}

void free_strv(char *strv[]){

	for (int i = 0; strv[i] != NULL; i++){
		free(strv[i]);
	}
	free(strv);
}


/*
int main(){

	const char* ejemplo = "hola,como,estas";
	
	char** vector = split(ejemplo,',');

	for (int i = 0; vector[i] != NULL; ++i){

			printf("%s\n", vector[i]);
		
	}
	char* nuevo_str = join(vector,' ');
	printf("%s\n", nuevo_str);


	const char* ejemplo2 = "abc,def";
	
	char** vector2 = split(ejemplo2,',');

	for (int i = 0; vector2[i] != NULL; ++i){

			printf("%s\n", vector2[i]);
		
	}
	char* nuevo_str2 = join(vector2,'\0');
	printf("%s\n", nuevo_str2);

	free_strv(vector);
	free_strv(vector2);
	free(nuevo_str);
	free(nuevo_str2);



	
}*/