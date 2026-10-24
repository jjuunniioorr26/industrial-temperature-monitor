import random
import numpy as np
from sympy import true


#--- Funções ---
    # Opção 1
def inicializar_sensores(linhas,colunas):
    matriz = np.zeros((linhas,colunas))
    for i in range(linhas):
        for j in range(colunas):
            while True: 
                try:
                    matriz[i][j] = int(input(f"\nTemperatura [Esteira {i+1}][Sensor {j+1}]: "))
                    break
                except ValueError:
                    print("Entrada inválida. Por favor, insira um número inteiro.")

    return matriz
    
    
    # Opção 3
def mostrar_esteiras(matriz):
    for i in range(matriz.shape[0]):
        print(f"\nEsteira {i+1}: {matriz[i]}")
    return

 
    # Opção 4
def calcular_medias_esteiras(matriz):
    medias = np.mean(matriz, axis=1)
    for i, media in enumerate(medias):
        print(f"\nEsteira {i+1}: {media:.2f}°C")
    return medias
    
    
    # Opção 5
def verificar_alertas(matriz,limite):
    alertas = []
    for i in range(matriz.shape[0]):
        for j in range(matriz.shape[1]):
            if matriz[i][j] >= limite:
                alertas.append((i, j))
                print(f" \n ALERTA: Esteira {i+1}, Sensor {j+1} -> {matriz[i][j]}°C")
    if not alertas:
        print("\n Nenhum ponto ultrapassou o limite seguro.")
    return alertas
    
    
    # Opção 6
def atualizar_sensores(matriz):
    n, m = matriz.shape
    matriz = np.zeros((n, m))
    for i in range(n):
        for j in range(m):
            while True: 
                try:
                    matriz[i][j] = int(input(f"\nTemperatura [Esteira {i+1}][Sensor {j+1}]: "))
                    break
                except ValueError:
                    print("Entrada inválida. Por favor, insira um número inteiro.")           
    return matriz
    

    # Opção 7
def gerar_matriz_teste():
    n = int(random.randint(2, 10))
    m = int(random.randint(2, 10))
    matriz = np.random.randint(20, 100, size=(n, m))
    print(f"\n Matriz({n},{m})\n")
    print("\nMatriz gerada:\n",matriz)
    return matriz
    
    # Opção 8
def inicializar_sensores_genericos():
    while True:
        try:
            n = int(input("Inserir número de Esteiras: ")) 
            m = int(input("Inserir número de Sensores: "))
            break
        except ValueError:
            print("Entrada inválida. Por favor, insira um número inteiro.")

    return inicializar_sensores(n, m)

    
#--- Matriz Inicial---

matriz = None 

#--- Menu de Opções --- 
while True:
    print("\n--- MENU ---")
    print("\n1 - Inicializar Sensores (3x4)\n2 - Ver Matriz \n3 - Ver Esteiras\n4 - Calcular Médias das Esteiras \n5 - Verificar Alertas\n6 - Inserir novas Temperaturas \n7 - Teste do programa \n8 - Matriz não limitada \n0 - Sair")
    opcao = input("\nEscolha uma opção: ")

    #--- Chamadas de Funções ---
    
    if opcao == "1":
        matriz = inicializar_sensores(3,4)
        
    elif opcao == "2":
        if matriz is None:
            print("\n[ERRO] Matriz não existente. Inicialize os sensores!")
        else:
            print("\n",matriz)
        
    elif opcao == "3":
        if matriz is None:
            print("\n[ERRO] Matriz não existente. Inicialize os sensores!")
        else:
            mostrar_esteiras(matriz)
        
    elif opcao == "4":
        if matriz is None:
            print("\n[ERRO] Matriz não existente. Inicialize os sensores!")
        else:
            calcular_medias_esteiras(matriz)
        
    elif opcao == "5":
        if matriz is None:
            print("\n[ERRO] Matriz não existente. Inicialize os sensores!")
        else:
            verificar_alertas(matriz,75)
        
    elif opcao == "6":
        if matriz is None:
            print("\n[ERRO] Matriz não existente. Inicialize os sensores!")
        else:
            matriz = atualizar_sensores(matriz)
        
    elif opcao == "7":
        matriz = gerar_matriz_teste()
        
    elif opcao == "8":
        matriz = inicializar_sensores_genericos()
    
    elif opcao == "0":
        break
    
    else:
        print("Opção inválida. Tente novamente.")
