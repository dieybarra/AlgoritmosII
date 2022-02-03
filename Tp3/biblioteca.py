from collections import deque
import random

#Devuelvo el camino recorrido en el grafo usando el diccionario de padres
def obtener_camino(padres, origen, fin, grafo):

    camino = []

    while padres[fin]:

        camino.append(fin)
        camino.append(grafo.obtener_peso(fin, padres[fin]))
        fin = padres[fin]

    camino.append(fin)
    camino.reverse()

    return camino

#Devuelve una lista que representa el camino mas corto desde un vertice origen a un vertice fin
#Si no se encontró un camino,devuelve None
def camino_minimo(grafo, origen, fin):
    lista = []
    visitados = set()
    padres = {}
    pila = []
    orden = {}
    padres[origen]=None
    orden[origen]=0
    visitados.add(origen)
    cola = deque()
    cola.append(origen)
    while len(cola) > 0:
        v = cola.popleft()

        if(v == fin):
            return obtener_camino(padres, origen, fin, grafo)

        for w in grafo.obtener_adyacentes(v):
            if w not in visitados:
                padres[w]=v
                orden[w]=orden[v]+1
                visitados.add(w)
                cola.append(w)

    return None

#Devuelve un diccionario en donde las claves son los vertices del grafo y los valores el pagerank 
#calculado para cada vertice
def calcular_pagerank(grafo, n):

    numero_vertices = len(grafo)
    pagerank_aux = 0
    lista_adyacentes = []
    pagerank_anterior = {}
    pagerank_actual = {}

    for v in grafo.obtener_vertices():                 
        pagerank_anterior[v] = (1)/numero_vertices
    for r in range (15):
        for v in grafo.obtener_vertices():
            lista_adyacentes = grafo.obtener_adyacentes(v) 
            for w in lista_adyacentes:
                pagerank_aux += pagerank_anterior[w]/len(grafo.obtener_adyacentes(w))
            pagerank_actual[v]= (0.15 / numero_vertices) + 0.85 * pagerank_aux     
            pagerank_aux = 0
        for v in grafo.obtener_vertices():
            pagerank_anterior[v] = pagerank_actual[v]

    return pagerank_actual

#Devuelve un diccionario en donde las claves son los vertices del grafo y los valores el pagerank 
# personalizado calculado para cada vertice
def calcular_pagerank_personalizado(grafo, numero, recomendaciones, es_usuarios):     

    lista = []                           
    pagerank = {}

    for v in grafo.obtener_vertices():
        pagerank[v] = 0         #inicializo todos los pagerank en 0
    for v in recomendaciones:
        pagerank[v] = 1
        aux = v
        for w in range (0, 200):
            lista = grafo.obtener_adyacentes(aux)
            vertice_random = random.choice(list(lista))
            pagerank[vertice_random] += pagerank[aux]/ len(grafo.obtener_adyacentes(aux))
            aux = vertice_random

    return pagerank


#Devuelve una lista con los vertices que se encuentran a un numero dado de aristas
def obtener_vertices_en_rango(grafo, vertice, rango, diccionario):

    orden = {}
    orden[vertice]=0                                
    cola = deque()                                 
    cola.append(vertice)
    lista = []
    while not len(cola) == 0:
        v = cola.popleft()
        for w in grafo.obtener_adyacentes(v):                
            if w in orden:
                continue
            orden[w]=orden[v]+1
            if orden[w]==rango:
                lista.append(w)
            cola.append(w)
    return lista


#Devuelve el clustering calculado para un dado vertice del grafo
def calcular_clustering_vertice(grafo, vertice):
    kv=0
    nv=0
    lista = grafo.obtener_adyacentes(vertice)
    kv = len(lista)
    visitados = set()
    visitados.add(vertice)
    for w in grafo.obtener_adyacentes(vertice):
        for q in grafo.obtener_adyacentes(w):
            if grafo.estan_relacionados(vertice, q) and q not in visitados:
                nv += 1
        visitados.add(w)
    if kv == 0 or kv == 1: 
        return 0
    return ((2*nv)/(kv*(kv-1)))


#Devuelve el clustering calculado para la red total del grafo
def calcular_clustering_red(grafo):
    clustering_total = 0
    cantidad_de_vertices = 0
    for v in grafo.obtener_vertices():
        clustering_total += calcular_clustering_vertice(grafo, v)
        cantidad_de_vertices += 1
    return (clustering_total/cantidad_de_vertices)