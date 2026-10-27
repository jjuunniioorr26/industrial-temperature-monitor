// criando em c
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

//Reserva memoria com malloc para a matriz, retorna 1 para sucesso ou 0 para falha de memoria
int alocar_sistema(PlantaIndustrial *p, SistemaControle *c, int M, int N) {
    if (p == NULL || c == NULL) return 0;

    p->qtd_linhas = M;
    p->qtd_colunas = N;

    // Alocação dinâmica real (Ponteiro para Ponteiro) para a matriz de sensores
    p->matriz_sensores = (float **)malloc(M * sizeof(float *));
    if (p->matriz_sensores == NULL) {
        return 0;
    }

    for (int i = 0; i < M; i++) {
        p->matriz_sensores[i] = (float *)malloc(N * sizeof(float));
        if (p->matriz_sensores[i] == NULL) {
            return 0;
        }
    }

    // Alocação dos vetores dinâmicos
    p->vetor_medias = (float *)malloc(M * sizeof(float));
    if (p->vetor_medias == NULL) {
        return 0;
    }

    c->vetor_atuadores = (int *)malloc(M * sizeof(int));
    if (c->vetor_atuadores == NULL) {
        return 0;
    }

    // Inicialização padrão dos dados
    c->total_alarmes = 0;
    c->limite_critico = 75.0f; 

    for (int i = 0; i < M; i++) {
        c->vetor_atuadores[i] = 1; // Todos iniciam ligados (Normal)
        p->vetor_medias[i] = 0.0f;
        for (int j = 0; j < N; j++) {
            p->matriz_sensores[i][j] = 0.0f;
        }
    }

    return 1;
}

//ALimenta a matriz dinamica de sensores com dados simulados dentro da faixa ideal
void inicializar_sensores(PlantaIndustrial *p) {
    if (p == NULL || p->matriz_sensores == NULL) {
        return;
    }

    for (int i = 0; i < p->qtd_linhas; i++) {
        for (int j = 0; j < p->qtd_colunas; j++) {
            printf("\nTemperatura [Esteira %d][Sensor %d]: ", i + 1, j + 1);
            scanf("%f", &p->matriz_sensores[i][j]);
        }
    }
}

//Percorre a matriz dinamica por ponteiros, calcula a media de cada setor e grava o resultado
void processar_dados(PlantaIndustrial *p) {
    if (p == NULL || p->matriz_sensores == NULL || p->vetor_medias == NULL) {
        return;
    }

    for (int i = 0; i < p->qtd_linhas; i++) {
        float soma = 0.0f;
        for (int j = 0; j < p->qtd_colunas; j++) {
            soma += p->matriz_sensores[i][j];
        }
        p->vetor_medias[i] = soma / (p->qtd_colunas);
        printf("\nEsteira %d: %.2f°C", i + 1, p->vetor_medias[i]);
    }
    printf("\n");
}

//Varre os dados por referencia. Caso algum valor da matriz supere o limite critico, incrementa total alarmes
//e altera o vetor_atuadores so setor para 0
void avaliar_seguranca(const PlantaIndustrial *p, SistemaControle *c) {
    if (p == NULL || c == NULL || p->matriz_sensores == NULL) {
        return;
    }

    int tem_alerta = 0;
    for (int i = 0; i < p->qtd_linhas; i++) {
        for (int j = 0; j < p->qtd_colunas; j++) {
            if (p->matriz_sensores[i][j] >= c->limite_critico) {
                c->total_alarmes++;
                c->vetor_atuadores[i] = 0; // Desliga atuador
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

//Exibe o mapa completo de sensores, o vetor de médias e o estado dos atuadores formatado na tela
void emitir_relatorio(const PlantaIndustrial *p, const SistemaControle *c) {
    if (p == NULL || c == NULL || p->matriz_sensores == NULL) {
        return;
    }

    printf("\n--- MATRIZ DE SENSORES ---\n");
    for (int i = 0; i < p->qtd_linhas; i++) {
        printf("Esteira %d:[ ", i + 1);
        for (int j = 0; j < p->qtd_colunas; j++) {
            printf("%.2f ", p->matriz_sensores[i][j]);
        }
        printf("]\n");
    }

    printf("\n--- STATUS DOS ATUADORES E MEDIAS ---\n");
    for (int i = 0; i < p->qtd_linhas; i++) {
        printf("Esteira %d -> Media: %.2f°C | Atuador: %d (",
           i + 1, p->vetor_medias[i], c->vetor_atuadores[i]);

        if (c->vetor_atuadores[i] == 1) {
            printf("NORMAL");
        } else {
            printf("BLOQUEADO");
        }

        printf(")\n");
    }
}

//Desaloca cada linha d amatriz dinamica e todos os vetores ultilizando free, prevenindo esgotamento de memoria
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

// Gera uma matriz aleatoria para teste
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
    int estado = 0;   // 0 = menu
    int opcao;

    srand((unsigned)time(NULL));

    while (estado != 9) {   // 9 = sair
        switch (estado) {

            case 0:  // MENU
                printf("\n--- MENU ---");
                printf("\n1 - Inicializar Sensores (3x4)\n2 - Ver Matriz e Relatorio \n3 - Calcular Medias das Esteiras\n4 - Verificar Alertas\n5 - Inserir novas Temperaturas (Teste)\n6 - Matriz nao limitada\n0 - Sair\n");
                printf("\nEscolha uma opcao: ");

                if (scanf("%d", &opcao) != 1) {
                    printf("Entrada invalida.\n");
                    estado = 9;
                    break;
                }

                if (opcao == 0) estado = 9;
                else if (opcao >= 1 && opcao <= 6) estado = opcao;
                else printf("Opcao invalida. Tente novamente.\n");

                // estados 2, 3 e 4 exigem matriz alocada
                if (estado >= 2 && estado <= 4 && !sistema_alocado) {
                    printf("\n[ERRO] Matriz nao existente. Inicialize os sensores!\n");
                    estado = 0;
                }
                break;

            case 1:  // INICIALIZAR
                if (sistema_alocado) liberar_memoria(&planta, &controle);
                alocar_sistema(&planta, &controle, 3, 4);
                inicializar_sensores(&planta);
                sistema_alocado = 1;
                estado = 0;
                break;

            case 2:  // RELATORIO
                emitir_relatorio(&planta, &controle);
                estado = 0;
                break;

            case 3:  // MEDIAS
                processar_dados(&planta);
                estado = 0;
                break;

            case 4:  // ALERTAS
                avaliar_seguranca(&planta, &controle);
                estado = 0;
                break;

            case 5:  // TESTE
                gerar_matriz_teste(&planta, &controle);
                sistema_alocado = 1;
                estado = 0;
                break;

            case 6: {  // MATRIZ LIVRE
                int n, m;
                printf("Inserir numero de Esteiras: ");
                scanf("%d", &n);
                printf("Inserir numero de Sensores: ");
                scanf("%d", &m);

                if (n > 0 && m > 0) {
                    if (sistema_alocado) liberar_memoria(&planta, &controle);
                    alocar_sistema(&planta, &controle, n, m);
                    inicializar_sensores(&planta);
                    sistema_alocado = 1;
                } else {
                    printf("Valores invalidos.\n");
                }
                estado = 0;
                break;
            }
        }
    }

    if (sistema_alocado) liberar_memoria(&planta, &controle);
    return 0;
}
