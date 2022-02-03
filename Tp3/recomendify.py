#!/usr/bin/python3
import csv
from sys import argv
from sys import stdin
from collections import deque
import random
from grafo import Grafo
import biblioteca

GUION = " - "


def crear_grafo_canciones(diccionario):
    grafo = Grafo()

    for playlist in diccionario.values():

        for i in range(len(playlist) - 1):

            for j in range(i+1, len(playlist)):

                if playlist[i] not in grafo:
                    grafo.agregar_vertice(playlist[i])
                if playlist[j] not in grafo:
                    grafo.agregar_vertice(playlist[j])

                if not grafo.estan_relacionados(playlist[i], playlist[j]):
                    grafo.agregar_arista(playlist[i], playlist[j], playlist)

    return grafo

def procesar_archivo(ruta):
  
    grafo = Grafo()
    diccionario = {}
    with open(ruta, 'r') as archivo:
        archivo_tsv = csv.reader(archivo, delimiter="\t")
        linea = next(archivo_tsv, None)
        linea = next(archivo_tsv, None)        

        while linea:

            if len(linea) != 7:
                linea= next(archivo_tsv, None)
                continue

            nombre_cancion = linea[2] + GUION + linea[3]
            playlist = linea[5]

            if linea[1] not in grafo:
                grafo.agregar_vertice(linea[1])

            if nombre_cancion not in grafo:    
                grafo.agregar_vertice(nombre_cancion)

            grafo.agregar_arista(linea[1], nombre_cancion, playlist)

            if playlist not in diccionario:
                diccionario[playlist] = []
            
            diccionario[playlist].append(nombre_cancion)    

            linea = next(archivo_tsv, None)

        return grafo, diccionario


def comando_pagerank(grafo, n):

    dic_pagerank_canciones = {}
    dic_pagerank = biblioteca.calcular_pagerank(grafo, n)

    for v in grafo.obtener_vertices():
        clave_actual = dic_pagerank[v]
        if GUION in v:
            while clave_actual in dic_pagerank_canciones:
                clave_actual *= 1.00001
            dic_pagerank_canciones[clave_actual] = v

    return dic_pagerank_canciones


def comando_pagerank_personalizado(grafo, numero, canciones, es_usuarios):        

    dic_pagerank = {}
    pagerank = biblioteca.calcular_pagerank_personalizado(grafo, numero, canciones, es_usuarios)

    for v in pagerank:

        if not es_usuarios:

            if GUION in v and v not in canciones:
                dic_pagerank[pagerank[v]] = v
        else:
            if GUION not in v:
                dic_pagerank[pagerank[v]] = v

    return dic_pagerank


def ciclo_backtracking(grafo, largo, cancion, actual, lista, visitados):
    
    visitados.append(actual)
    lista.append(actual)

    for v in grafo.obtener_adyacentes(actual):
        if v == cancion and largo == len(lista):
            lista.append(cancion)
            return True

    for w in grafo.obtener_adyacentes(actual):
        if (w not in visitados  and (len(lista) <= largo)):
            if (ciclo_backtracking(grafo, largo, cancion, w, lista , visitados)):
                return True
    visitados.remove(actual)
    lista.remove(actual)
    return False   

def comando_ciclo(grafo, largo, cancion, diccionario):

    lista = []
    visitados = []
    hay_ciclo = ciclo_backtracking(grafo, largo, cancion, cancion, lista, visitados)
    if( not hay_ciclo):
        return None
    else:
        return(lista)


def comando_clustering(grafo, cancion, diccionario):

    if cancion == "":
        return biblioteca.calcular_clustering_red(grafo)
    else:
        return biblioteca.calcular_clustering_vertice(grafo, cancion)

def imprimir_camino(camino):

    if camino:

        formato = [" --> aparece en playlist --> ", " --> de --> ", " --> tiene una playlist --> ", " --> donde aparece --> "]
        formato_final = formato * (int(len(camino)/4))

        for i in range(len(camino)):
            if (i < len(camino) -1):
                print(camino[i], end = "")
            else:
                print(camino[i])

            if(i < len(camino) -1):
                print(formato_final [i], end = "")

    else: 
        print("No se encontro recorrido")

def imprimir_pagerank(lista_ordenada):

    for i in range(len(lista_ordenada)):
        if i == len(lista_ordenada) - 1:
            print(lista_ordenada[i][1])
            continue
        print(lista_ordenada[i][1], end="; ")

def imprimir_ciclo(lista):

    for i in range(len(lista)):
        print(lista[i], end = "")
        if i == len(lista) - 1:
            print()
            continue
        print(" --> ", end ="")


def procesar_comando(grafo, comando, parametros, grafo_canciones, diccionario, dic_pagerank):

    if comando == "camino":

        parametros_separados = parametros.split(" >>>> ")

        if len(parametros_separados) != 2:
            print("Error en los parametros")
            return

        origen = parametros_separados[0]
        destino = parametros_separados[1]

        if origen not in grafo or destino not in grafo:
            print("Tanto el origen como el destino deben ser canciones")
            return

        elif GUION not in origen or GUION not in destino:
            print("Tanto el origen como el destino deben ser canciones")
            return                    

        camino = biblioteca.camino_minimo(grafo, origen, destino)

        imprimir_camino(camino)


    elif comando == "mas_importantes":

        parametro = int(parametros)

        if not dic_pagerank:
            dic_pagerank = comando_pagerank(grafo, parametro)

        lista_ordenada = sorted(dic_pagerank.items(), reverse = True)[0:parametro]

        imprimir_pagerank(lista_ordenada)
        
        return dic_pagerank

    elif comando == "recomendacion":

        parametros_separados = parametros.split(maxsplit = 1)
        es_usuarios = parametros_separados[0] == "usuarios"
        parametros_separados = parametros_separados[1].split(maxsplit = 1)
        numero = int(parametros_separados[0])
        canciones = parametros_separados[1].split(" >>>> ")

        dic_pagerank_pers = comando_pagerank_personalizado(grafo, numero, canciones, es_usuarios)

        lista_ordenada = sorted(dic_pagerank_pers.items(), reverse = True)[0:numero]

        imprimir_pagerank(lista_ordenada)

    elif comando == "ciclo":

        grafo_canciones = crear_grafo_canciones(diccionario)

        parametros_separados = parametros.split(maxsplit = 1)
        largo = int(parametros_separados[0])
        cancion = parametros_separados[1]
        lista = comando_ciclo(grafo_canciones, largo, cancion, diccionario)

        if lista:
            imprimir_ciclo(lista)
        else:
            print("No se encontro recorrido")


    elif comando == "rango":

        grafo_canciones = crear_grafo_canciones(diccionario)

        parametros_separados = parametros.split(maxsplit = 1)
        rango = int(parametros_separados[0])
        cancion = parametros_separados[1]
        lista = biblioteca.obtener_vertices_en_rango(grafo_canciones, cancion, rango, diccionario)

        print(len(lista))

    elif comando == "clustering":

        grafo_canciones = crear_grafo_canciones(diccionario)

        clustering = comando_clustering(grafo_canciones, parametros, diccionario)
        print(round(clustering, 3))

def main():

    if len(argv) != 2:
        print("Error. Cantidad de comandos erroneo")
        return

    grafo_usuarios, diccionario_playlists = procesar_archivo(argv[1])
    grafo_canciones = {}
    dic_pagerank = {}

    for linea in stdin:

        linea = linea.rstrip("\n")
        linea_separada = linea.split(maxsplit = 1)
        comando = linea_separada[0]

        if len(linea_separada) > 1:
            parametros = linea_separada[1]
        else:
            parametros = ""

        dic_pagerank = procesar_comando(grafo_usuarios, comando, parametros, grafo_canciones, diccionario_playlists, dic_pagerank)

if __name__ == "__main__":
    main()