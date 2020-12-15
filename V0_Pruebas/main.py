vecinos = [-1, 1, 2, 3, 4]
n_procesos = 3

cnt_v = 0
vecinos_por_visitar = list()
i = 0
while i < len(vecinos):
    if vecinos[i] != -1:
        vecinos_por_visitar.append(vecinos[i])
        cnt_v += 1
    i += 1
    if cnt_v == n_procesos - 1 or i == len(vecinos):
        if len(vecinos_por_visitar) < n_procesos - 1:
            vecinos_por_visitar = vecinos_por_visitar + [-1] * (n_procesos - 1 - len(vecinos_por_visitar)) 
        print("procesando: ", vecinos_por_visitar)
        vecinos_por_visitar = list()
        cnt_v = 0

