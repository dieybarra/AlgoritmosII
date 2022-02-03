class Grafo():
 
#Crea el grafo
    def __init__(self, dirigido = False):
 
        self.vertices = {}
        self.dirigido = dirigido
 
#Devuelve la cantidad de vertices que tiene el grafo
 
    def __len__(self):
 
        return len(self.vertices)
 
 
#Devuelve true o false dependiendo de si el vertice esta en el grafo o no
 
    def __contains__(self, vertice):
 
        return vertice in self.vertices
 
#Agrega el vertice al grafo
 
    def agregar_vertice(self, vertice):
        
        self.vertices[vertice] = {}
 
#Agrega al grafo una arista nueva, si ya existe cambia su peso por el nuevo. Se le pasa un origen
#y un destino a la arista, son vertices que ya existen. Si no seespecifica un peso, es 1 por defecto.
 
    def agregar_arista(self, origen, destino, peso):
 
        self.vertices[origen][destino] = peso
        if not self.dirigido:
            self.vertices[destino][origen] = peso
 
 
#Elimina un vertice del grafo
 
    def borrar_vertice(self, vertice):
 
        for v in self.vertices:
            if vertice in self.vertices[v].keys():
                del self.vertices[v][vertice]
        del self.vertices[vertice]
 
 
# Elimina la arista que conecta los vertices de origen y destino
 
    def borrar_arista(self, origen, destino):
        
        del self.vertices[origen][destino]
        if not self.dirigido:
            del self.vertices[destino][origen]
 
 
#Devuelve true si el origen y el destino estan relacionados, si no, devuelve false
 
    def estan_relacionados(self, origen, destino):
 
        return destino in self.vertices[origen]
 
 
#Devuelve una lista con todos los vertices existentes en el grafo
 
    def obtener_vertices(self):
 
        return list(self.vertices)
 
#Devuelve una lista con todos los vertices que son adyacentes al vertice dado#
 
    def obtener_adyacentes(self, vertice):
 
        return self.vertices[vertice].keys()

    def obtener_peso(self, origen, destino):

        return self.vertices[origen][destino]
