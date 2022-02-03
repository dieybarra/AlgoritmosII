#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "calc_helper.h"
#include "strutil.h"



calc_num suma(calc_num a, calc_num b){

	return a + b;
}

calc_num resta(calc_num a, calc_num b){

    return a - b;
}

calc_num producto(calc_num a, calc_num b){

    return a * b;
}

calc_num division(calc_num a, calc_num b){

    return a / b;
}

calc_num potencia(calc_num a, calc_num b){

	return pow(a, b);
}

bool raiz_cuadrada(calc_num a, calc_num* resultado){

	if(a >= 0){
		*resultado = sqrt(a);
		return true;
	}
	return false;
  
}

calc_num logaritmo(calc_num a,calc_num b){

	float log_a = log(a);
	float log_b = log(b);
    return (log_a / log_b);
}

calc_num operador_ternario(calc_num a, calc_num b, calc_num c){

    return a ? b : c;
}

bool comprobar_dos_op(pilanum_t* pila, calc_num* numero_2, calc_num* numero_1){

	return (desapilar_num(pila, numero_2) && desapilar_num(pila, numero_1));

}

bool comprobar_tres_op(pilanum_t* pila, calc_num* numero_3, calc_num* numero_2, calc_num* numero_1){

	return (desapilar_num(pila, numero_3) && desapilar_num(pila, numero_2) && desapilar_num(pila, numero_1));

}

bool operacion_dos_op(calc_num numero_1, calc_num numero_2, calc_num* resultado, enum oper_type op){

	if (op == OP_ADD){
		*resultado = suma(numero_1, numero_2);
		return true;
	} 
	else if (op == OP_SUB){
		*resultado = resta(numero_1, numero_2);
		return true;
	}
	else if (op == OP_MUL){
		*resultado = producto(numero_1, numero_2);
		return true;
	}
	else if (op == OP_DIV && numero_2 != 0){
		*resultado = division(numero_1, numero_2);
		return true;
	}
	else if (op == OP_POW && numero_2 >= 0){
		*resultado = potencia(numero_1, numero_2);
		return true;
	}
	else if (op == OP_LOG && numero_2 >= 2){
		*resultado = logaritmo(numero_1, numero_2);
		return true;
	}
	return false;
}



void calcular_dc(char** vector){

	pila_t* pila = pilanum_crear();
	if (!pila){
        fprintf(stdout,"%s","ERROR\n");
        return;
    }

	calc_num resultado = 0;
    int i = 0;
    bool error = false;

    while ((vector[i] != NULL) && (!error)){

        char* palabra = vector[i];

        struct calc_token tok;

        if (!calc_parse(palabra, &tok)){
        	error = true;
        }
		else if (tok.type == TOK_NUM) {

        	apilar_num(pila, tok.value);
        	i++;;
        }
        else if (tok.type == TOK_OPER){

        	if(tok.oper.num_operandos == 2){

        		calc_num numero_2, numero_1;

        		if (!(comprobar_dos_op(pila, &numero_2, &numero_1) && operacion_dos_op(numero_1, numero_2, &resultado, tok.oper.op))){

        			error= true;
        		}
        	}
        	else if(tok.oper.num_operandos == 1){

        		calc_num numero;
        		if (!(desapilar_num(pila, &numero) && raiz_cuadrada(numero, &resultado))){

        			error = true;
        		}
        	}
        	else{

        		calc_num numero_1, numero_2, numero_3;
        		if (comprobar_tres_op(pila, &numero_3, &numero_2, &numero_1)){
        			resultado = operador_ternario(numero_1, numero_2, numero_3);
        		}
        		else{
        			error = true;
        		}
        	}
        	apilar_num(pila, resultado);
        	i++;

        }
    }

    if (!error){

    	if(desapilar_num(pila, &resultado)){

    		if (desapilar_num(pila, &resultado)){
    			error = true;
    		}	
    	}
    	else{
    		error = true;
    	}
    }

    pilanum_destruir(pila);

    if (error){
      	fprintf(stdout, "ERROR\n");
    }
    else{
       	fprintf(stdout, "%lli\n", resultado);
    }


}

int main(int argc, char const *argv[]){
	
	char* linea = NULL;
	size_t n = 0;

	while((getline(&linea, &n, stdin)) > 0){

		char** vector = dc_split(linea);
		calcular_dc(vector);
		free_strv(vector);
	}
	free(linea);
	//fclose(archivo);

	return 0;
}