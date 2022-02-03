#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "calc_helper.h"
#include "strutil.h"
#include "pila.h"
#include "cola.h"
#include <string.h>


bool comprobar_operador(pila_t* pila, calc_token* tok){

	calc_token* tok_2 = pila_ver_tope(pila);
	if(tok_2 != NULL && tok_2->type == TOK_OPER){

		if((tok->oper.asociatividad == ASSOC_LEFT && tok->oper.precedencia <= tok_2->oper.precedencia) ||
		(tok->oper.asociatividad == ASSOC_RIGHT && tok->oper.precedencia < tok_2->oper.precedencia)){
			return true;
		}
		return false;
	}
	return false;

}
void calcular_infix(char** vector){

	cola_t* cola = cola_crear();
	pila_t* pila = pila_crear();

	int i = 0;
    //bool error = false;
    calc_token* dato = NULL;

    while (vector[i] != NULL){

    	//fprintf(stdout, "%s\n", vector[i]);
    	char* palabra = vector[i];

        //calc_token tok;
        dato = malloc(sizeof(calc_token));
        if (!calc_parse(palabra, dato)){
        	fprintf(stdout, "ERROR");
        }


        //*dato = tok;
        //dato->type = tok.type;

        if (dato->type == TOK_NUM){
        	//dato->value = tok.value;
        	cola_encolar(cola, dato);
        	
        }
        else if (dato->type == TOK_OPER){
        	//dato->oper.op = tok.oper.op;
        	//dato->oper.asociatividad = tok.oper.asociatividad;
        	//dato->oper.precedencia = tok.oper.precedencia;

        	while(comprobar_operador(pila, dato)){
        		calc_token* tok_3 = pila_desapilar(pila);
        		cola_encolar(cola, tok_3);
        	}
        	pila_apilar(pila, dato);

        }	
        else if (dato->type == TOK_LPAREN){
        	pila_apilar(pila, dato);
        }
        else{
        	free(dato);
        	calc_token* tok_2 = pila_ver_tope(pila);
        	while(tok_2->type != TOK_LPAREN){
        		calc_token* tok_3 = pila_desapilar(pila);
        		cola_encolar(cola, tok_3);
        		tok_2 = pila_ver_tope(pila);        		
        	}
        	free(tok_2);
        	pila_desapilar(pila);
        }
        i++;
    }
    while(!pila_esta_vacia(pila)){
    	calc_token* tok_3 = pila_desapilar(pila);
        cola_encolar(cola, tok_3);
    }

    while(!cola_esta_vacia(cola)){
    	calc_token* tok_3 = cola_desencolar(cola);
    	if(tok_3->type == TOK_NUM){
    		fprintf(stdout, " %lli", tok_3->value);
    	}
    	else{
    		if(tok_3->oper.op == OP_ADD)
    		    fprintf(stdout, " +");
    		else if(tok_3->oper.op == OP_SUB)
    			fprintf(stdout, " -");
    		else if(tok_3->oper.op == OP_MUL)
    			fprintf(stdout, " *"); 
    		else if(tok_3->oper.op == OP_DIV)
    			fprintf(stdout, " /");   
    		else
    			fprintf(stdout, " ^"); 		
    	}
    	free(tok_3);
	}
	fprintf(stdout, "\n");
	cola_destruir(cola, NULL);
    pila_destruir(pila);

}

int main(int argc, char const *argv[]){
	
	char* linea = NULL;
	size_t n = 0;

	while((getline(&linea, &n, stdin)) > 0){
		char** vector = infix_split(linea);
		//fprintf(stdout, "%s\n", linea);

		calcular_infix(vector);
		free_strv(vector);
	}
	free(linea);

	return 0;
}