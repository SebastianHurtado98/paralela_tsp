# paralela_tsp
TSP MPI OMP

# "Experimento"
El objetivo de esto es aprender a hacer un "BFS" usando stack y procesos sin pensar en procesos complejos como la reducción de matrices. Es decir, abstraernos.
Crearlo en un archivo aparte: stack.cpp

## pasos
- Leer la matriz
- Poner 1 en un stack. Este será nuestro primer nodo visitado.
- Coger los vecinos de 1, es decir, los que no tienen distancia -1.
- Mandar n vecinos a n procesos (1 vecino por proceso, solo manda el numero). Desde el proceso, imprimir algo como "Desde rank soy el nodo #". Además, enviar desde este proceso i al proceso 0 la distancia. Para esto, tambien debes haber enviado la matriz a cada proceso y el nodo "padre" para saber la distancia entre padre y vecino. 
- La cantidad de vecinos debe ser superior a la cantidad de procesos, por lo tanto, en una siguiente iteración, enviaremos los nodos restantes a los procesos. 
- Compara las distancias que te han mandado todos los vecinos (podría ser secuencialmente en algun array).
- El proceso 0 escoge la menor distancia. Pones el nodo con menor distancia en el stack y repites el procedimiento, obviando nodos ya visitados.
- El procedimiento termina cuando el stack tenga n nodos visitados.

## interrogantes
Estos son los problemas más grandes que tenemos que resolver para esta implementación:
- ¿Cómo podemos recibir la distancia del proceso n al proceso 0, y luego comparar todas las distancias?
    - Gather, Allgather, Gatherv
- ¿Cómo podemos enviar primero n nodos a n procesos y luego el resto X nodos a X procesos?
    - Scatter, Scatterv
- ¿Cómo podemos iterar este proceso hasta que el stack tenga todos los nodos?
    - While (?)