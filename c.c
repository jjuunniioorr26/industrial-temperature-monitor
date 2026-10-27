#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 1. Estrutura de Mapeamento do Chão de Fábrica (Obrigatória das imagens)
typedef struct {
    float **matriz_sensores; // Matriz dinâmica (ponteiro duplo) M x N
    float *vetor_medias;     // Vetor dinâmico M de médias calculadas
    int qtd_linhas;          // Quantidade de máquinas/setores/galerias (M)
    int qtd_colunas;         // Quantidade de sensores por setor (N)
} PlantaIndustrial;

// 2. Estrutura de Controle, Alarmes e Atuadores (Obrigatória das imagens)
typedef struct {
    int *vetor_atuadores;    // Vetor dinâmico M (1 = Normal/Ligado, 0 = Emergência/Bloqueado)
    int total_alarmes;       // Contador global de falhas detectadas
    float limite_critico;    // Setpoint de segurança limite
} SistemaControle;

/* 
   Assinatura Obrigatória: int alocar_sistema(...)
   Equivale ao np.zeros e preparações de memória do Python.
*/
int alocar_sistema(PlantaIndustrial *p, SistemaControle *c, int l, int c_col) {
    if (p == NULL || c == NULL) return 0;

    p->qtd_linhas = l;
    p->qtd_colunas = c_col;

    // Alocação dinâmica real (Ponteiro para Ponteiro) para a matriz de sensores
    p->matriz_sensores = (float **)malloc(l * sizeof(float *));
    if (p->matriz_sensores == NULL) return 0;

    for (int i = 0; i < l; i++) {
        p->matriz_sensores[i] = (float *)malloc(c_col * sizeof(float));
        if (p->matriz_sensores[i] == NULL) return 0;
    }

    // Alocação dos vetores dinâmicos
    p->vetor_medias = (float *)malloc(l * sizeof(float));
    if (p->vetor_medias == NULL) return 0;

    c->vetor_atuadores = (int *)malloc(l * sizeof(int));
    if (c->vetor_atuadores == NULL) return 0;

    // Inicialização padrão dos dados
    c->total_alarmes = 0;
    c->limite_critico = 75.0f; // Definido pelo Case 1

    for (int i = 0; i < l; i++) {
        c->vetor_atuadores[i] = 1; // Todos iniciam ligados (Normal)
        p->vetor_medias[i] = 0.0f;
        for (int j = 0; j < c_col; j++) {
            p->matriz_sensores[i][j] = 0.0f;
        }
    }

    return 1;
}

/* 
   Assinatura Obrigatória: void inicializar_sensores(...)
   Adaptação da sua Opção 1: Recebe dados pelo teclado e alimenta a matriz estruturada.
*/
void inicializar_sensores(PlantaIndustrial *p) {
    if (p == NULL || p->matriz_sensores == NULL) return;

    for (int i = 0; i < p->qtd_linhas; i++) {
        for (int j = 0; j < p->qtd_colunas; j++) {
            printf("\nTemperatura [Esteira %d][Sensor %d]: ", i + 1, j + 1);
            scanf("%f", &p->matriz_sensores[i][j]);
        }
    }
}

/* 
   Assinatura Obrigatória: void processar_dados(...)
   Adaptação da sua Opção 4: Calcula a média usando a matriz estruturada e salva no vetor_medias.
*/
void processar_dados(PlantaIndustrial *p) {
    if (p == NULL || p->matriz_sensores == NULL || p->vetor_medias == NULL) return;

    for (int i = 0; i < p->qtd_linhas; i++) {
        float soma = 0.0f;
        for (int j = 0; j < p->qtd_colunas; j++) {
            soma += p->matriz_sensores[i][j];
        }
        p->vetor_medias[i] = soma / p->qtd_colunas;
        printf("\nEsteira %d: %.2f°C", i + 1, p->vetor_medias[i]);
    }
    printf("\n");
}

/* 
   Assinatura Obrigatória: void avaliar_seguranca(...)
   Adaptação da sua Opção 5: Varre os dados e altera o vetor de atuadores para 0 se passar do limite.
*/
void avaliar_seguranca(const PlantaIndustrial *p, SistemaControle *c) {
    if (p == NULL || c == NULL || p->matriz_sensores == NULL) return;

    int tem_alerta = 0;
    for (int i = 0; i < p->qtd_linhas; i++) {
        for (int j = 0; j < p->qtd_colunas; j++) {
            if (p->matriz_sensores[i][j] >= c->limite_critico) {
                c->total_alarmes++;
                c->vetor_atuadores[i] = 0; // Desliga atuador (Emergência)
                printf(" \n ALERTA: Esteira %d, Sensor %d -> %.2f°C", i + 1, j + 1, p->matriz_sensores[i][j]);
                tem_alerta = 1;
            }
        }
    }

    if (!tem_alerta) {
        printf("\n Nenhum ponto ultrapassou o limite seguro.");
    }
    printf("\n");
}

/* 
   Assinatura Obrigatória: void emitir_relatorio(...)
   Adaptação das suas Opções 2 e 3 integradas: Imprime a matriz completa, médias e os atuadores.
*/
void emitir_relatorio(const PlantaIndustrial *p, const SistemaControle *c) {
    if (p == NULL || c == NULL || p->matriz_sensores == NULL) return;

    printf("\n--- MATRIZ DE SENSORES ---\n");
    for (int i = 0; i < p->qtd_linhas; i++) {
        printf("Esteira %d: [ ", i + 1);
        for (int j = 0; j < p->qtd_colunas; j++) {
            printf("%.2f ", p->matriz_sensores[i][j]);
        }
        printf("]\n");
    }

    printf("\n--- STATUS DOS ATUADORES E MEDIAS ---\n");
    for (int i = 0; i < p->qtd_linhas; i++) {
        printf("Esteira %d -> Media: %.2f°C | Atuador: %d (%s)\n", 
               i + 1, p->vetor_medias[i], c->vetor_atuadores[i],
               (c->vetor_atuadores[i] == 1) ? "NORMAL" : "BLOQUEADO");
    }
}

/* 
   Assinatura Obrigatória: void liberar_memoria(...)
   Zera e limpa toda a memória da heap alocada no início para prevenir memory leaks.
*/
void liberar_memoria(PlantaIndustrial *p, SistemaControle *c) {
    if (p != NULL) {
        if (p->matriz_sensores != NULL) {
            for (int i = 0; i < p->qtd_linhas; i++) {
                free(p->matriz_sensores[i]);
            }
            free(p->matriz_sensores);
            p->matriz_sensores = NULL;
        }
        if (p->vetor_medias != NULL) {
            free(p->vetor_medias);
            p->vetor_medias = NULL;
        }
    }
    if (c != NULL) {
        if (c->vetor_atuadores != NULL) {
            free(c->vetor_atuadores);
            c->vetor_atuadores = NULL;
        }
    }
}

// Auxiliar para a Opção 6 (Gerar matriz de teste aleatória)
void gerar_matriz_teste(PlantaIndustrial *p, SistemaControle *c) {
    int n = 2 + rand() % 9; // Entre 2 e 10
    int m = 2 + rand() % 9; // Entre 2 e 10

    liberar_memoria(p, c);
    alocar_sistema(p, c, n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            p->matriz_sensores[i][j] = (float)(20 + rand() % 80); // 20 a 100
        }
    }
    printf("\n Matriz(%d,%d) gerada automaticamente por teste.\n", n, m);
}

// Main principal orquestrando as chamadas por referência através de menu
int main(void) {
    PlantaIndustrial planta = {NULL, NULL, 0, 0};
    SistemaControle controle = {NULL, 0, 0.0f};
    int sistema_alocado = 0;
    int opcao;

    srand((unsigned)time(NULL));

    while (1) {
        printf("\n--- MENU ---");
        printf("\n1 - Inicializar Sensores (3x4)\n2 - Ver Matriz e Relatorio \n3 - Calcular Medias das Esteiras\n4 - Verificar Alertas\n5 - Inserir novas Temperaturas (Teste)\n6 - Matriz nao limitada\n0 - Sair\n");
        printf("\nEscolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            break;
        }

        if (opcao == 1) {
            if (sistema_alocado) liberar_memoria(&planta, &controle);
            alocar_sistema(&planta, &controle, 3, 4);
            inicializar_sensores(&planta);
            sistema_alocado = 1;
        } 
        else if (opcao == 2) {
            if (!sistema_alocado) {
                printf("\n[ERRO] Matriz nao existente. Inicialize os sensores!\n");
            } else {
                emitir_relatorio(&planta, &controle);
            }
        } 
        else if (opcao == 3) {
            if (!sistema_alocado) {
                printf("\n[ERRO] Matriz nao existente. Inicialize os sensores!\n");
            } else {
                processar_dados(&planta);
            }
        } 
        else if (opcao == 4) {
            if (!sistema_alocado) {
                printf("\n[ERRO] Matriz nao existente. Inicialize os sensores!\n");
            } else {
                avaliar_seguranca(&planta, &controle);
            }
        } 
        else if (opcao == 5) {
            gerar_matriz_teste(&planta, &controle);
            sistema_alocado = 1;
        } 
        else if (opcao == 6) {
            int n, m;
            printf("Inserir numero de Esteiras: ");
            scanf("%d", &n);
            printf("Inserir numero de Sensores: ");
            scanf("%d", &m);
            
            if (sistema_alocado) liberar_memoria(&planta, &controle);
            alocar_sistema(&planta, &controle, n, m);
            inicializar_sensores(&planta);
            sistema_alocado = 1;
        } 
        else if (opcao == 0) {
            if (sistema_alocado) liberar_memoria(&planta, &controle);
            break;
        } 
        else {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}
