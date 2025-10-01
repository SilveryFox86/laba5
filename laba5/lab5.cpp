#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "");

    int N; // размерность матрицы
    double p; // вероятность появления ребра
    double p_loop; // вероятность появления петли

    printf("Введите размерность матрицы (N): ");
    scanf("%d", &N);

    printf("Введите вероятность появления ребра (0,0 - 1,0): ");
    scanf("%lf", &p);

    printf("Введите вероятность появления петли (0,0 - 1,0): ");
    scanf("%lf", &p_loop);

    // Проверка корректности ввода
    if (N <= 0) {
        printf("Ошибка: размерность должна быть положительным числом\n");
        return 1;
    }

    if (p < 0.0 || p > 1.0) {
        printf("Ошибка: вероятность должна быть в диапазоне от 0,0 до 1,0\n");
        return 1;
    }

    if (p_loop < 0.0 || p_loop > 1.0) {
        printf("Ошибка: вероятность петли должна быть в диапазоне от 0,0 до 1,0\n");
        return 1;
    }

    // Выделение памяти для матрицы смежности
    int** adj_matrix = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        adj_matrix[i] = (int*)malloc(N * sizeof(int));
    }

    srand(time(NULL));

    printf("\n=== ЗАДАНИЕ 1 ===\n\n");

    // 1. Генерация матрицы смежности
    printf("1. Матрица смежности:\n");

    // Сначала заполняем всю матрицу нулями
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            adj_matrix[i][j] = 0;
        }
    }

    // Генерируем петли (диагональные элементы)
    for (int i = 0; i < N; i++) {
        double random_value = (double)rand() / RAND_MAX;
        if (random_value < p_loop) {
            adj_matrix[i][i] = 1; // петля
        }
    }

    // Генерируем ребра для недиагональных элементов
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                double random_value = (double)rand() / RAND_MAX;
                if (random_value < p) {
                    adj_matrix[i][j] = 1;
                    adj_matrix[j][i] = 1; 
                }
            }
        }
    }

   
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", adj_matrix[i][j]);
        }
        printf("\n");
    }

    
    int edges = 0;
    int loops = 0;

    printf("\n2. Степени вершин:\n");
    for (int i = 0; i < N; i++) {
        int degree = 0;
        for (int j = 0; j < N; j++) {
            if (adj_matrix[i][j] == 1) {
                if (i == j) {
                    degree += 2; 
                }
                else {
                    degree += 1; 
                }
            }
        }
        printf("   Вершина %d: степень %d", i, degree);

 
        if (adj_matrix[i][i] == 1) {
            printf(" (есть петля)");
        }
        printf("\n");
    }

  
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (adj_matrix[i][j] == 1) {
                edges++;
            }
        }
    }

    // Подсчет петель
    for (int i = 0; i < N; i++) {
        if (adj_matrix[i][i] == 1) {
            loops++;
        }
    }

    printf("\n   Размер графа: %d ребер (из них %d петель)\n", edges + loops, loops);


    int* isolated = (int*)malloc(N * sizeof(int));
    int* pendant = (int*)malloc(N * sizeof(int));
    int* dominant = (int*)malloc(N * sizeof(int));
    int isolated_count = 0, pendant_count = 0, dominant_count = 0;

    for (int i = 0; i < N; i++) {
        int degree = 0;
        int degree_without_loops = 0;

        for (int j = 0; j < N; j++) {
            if (adj_matrix[i][j] == 1) {
                if (i == j) {
                    degree += 2; 
                }
                else {
                    degree += 1; 
                    degree_without_loops += 1; 
                }
            }
        }


        if (degree_without_loops == 0) {
            isolated[isolated_count] = i;
            isolated_count++;
        }
        else if (degree_without_loops == 1) {
            pendant[pendant_count] = i;
            pendant_count++;
        }

       
        if (degree >= N) { 
            dominant[dominant_count] = i;
            dominant_count++;
        }
    }

    printf("\n3. Особые вершины:\n");
    printf("   Изолированные: [");
    for (int i = 0; i < isolated_count; i++) {
        printf("%d", isolated[i]);
        if (i < isolated_count - 1) printf(", ");
    }
    printf("]\n");

    printf("   Концевые: [");
    for (int i = 0; i < pendant_count; i++) {
        printf("%d", pendant[i]);
        if (i < pendant_count - 1) printf(", ");
    }
    printf("]\n");

    printf("   Доминирующие: [");
    for (int i = 0; i < dominant_count; i++) {
        printf("%d", dominant[i]);
        if (i < dominant_count - 1) printf(", ");
    }
    printf("]\n");

    printf("\n=== ЗАДАНИЕ 2 ===\n\n");

   
    int max_edges = N * (N - 1) / 2 + N; 
    int edge_count = 0;
    int loop_count = 0;

    // Находим все ребра и петли
    int** edges_list = (int**)malloc(max_edges * sizeof(int*));
    for (int i = 0; i < max_edges; i++) {
        edges_list[i] = (int*)malloc(2 * sizeof(int));
    }

    // Сначала добавляем петли
    for (int i = 0; i < N; i++) {
        if (adj_matrix[i][i] == 1) {
            edges_list[edge_count][0] = i;
            edges_list[edge_count][1] = i; // петля
            edge_count++;
            loop_count++;
        }
    }

    // Затем добавляем обычные ребра
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (adj_matrix[i][j] == 1) {
                edges_list[edge_count][0] = i;
                edges_list[edge_count][1] = j;
                edge_count++;
            }
        }
    }

    // Создаем матрицу инцидентности
    int** inc_matrix = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        inc_matrix[i] = (int*)malloc((edge_count > 0 ? edge_count : 1) * sizeof(int));
    }

    // Заполняем матрицу инцидентности нулями
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < edge_count; j++) {
            inc_matrix[i][j] = 0;
        }
    }

    // Заполняем единицами (для петель ставим 2 в одной ячейке)
    for (int j = 0; j < edge_count; j++) {
        int v1 = edges_list[j][0];
        int v2 = edges_list[j][1];

        if (v1 == v2) {
            // Петля - ставим 2 в соответствующей вершине (ПЕТЛЯ ДАЕТ +2)
            inc_matrix[v1][j] = 2;
        }
        else {
            // Обычное ребро
            inc_matrix[v1][j] = 1;
            inc_matrix[v2][j] = 1;
        }
    }

    printf("1. Матрица инцидентности:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < edge_count; j++) {
            printf("%d ", inc_matrix[i][j]);
        }
        printf("\n");
    }

    if (edge_count > 0) {
        printf("Ребра: [");
        for (int i = 0; i < edge_count; i++) {
            if (edges_list[i][0] == edges_list[i][1]) {
                printf("(%d) петля", edges_list[i][0]); // петля
            }
            else {
                printf("(%d,%d)", edges_list[i][0], edges_list[i][1]); // обычное ребро
            }
            if (i < edge_count - 1) printf(", ");
        }
        printf("]\n");
    }
    else {
        printf("Ребра: []\n");
    }

    // 2. Размер графа
    printf("\n2. Размер графа: %d ребер (из них %d петель)\n", edge_count, loop_count);

 
    int* isolated_inc = (int*)malloc(N * sizeof(int));
    int* pendant_inc = (int*)malloc(N * sizeof(int));
    int* dominant_inc = (int*)malloc(N * sizeof(int));
    int isolated_count_inc = 0, pendant_count_inc = 0, dominant_count_inc = 0;

    for (int i = 0; i < N; i++) {
        int degree = 0;
        int degree_without_loops = 0;

        for (int j = 0; j < edge_count; j++) {
            if (inc_matrix[i][j] == 2) {
                degree += 2; 
            }
            else if (inc_matrix[i][j] == 1) {
                degree += 1; 
                degree_without_loops += 1; 
            }
        }

        
        if (degree_without_loops == 0) {
            isolated_inc[isolated_count_inc] = i;
            isolated_count_inc++;
        }
        else if (degree_without_loops == 1) {
            pendant_inc[pendant_count_inc] = i;
            pendant_count_inc++;
        }

       
        if (degree >= N) {
            dominant_inc[dominant_count_inc] = i;
            dominant_count_inc++;
        }
    }

    printf("\n3. Особые вершины:\n");
    printf("   Изолированные: [");
    for (int i = 0; i < isolated_count_inc; i++) {
        printf("%d", isolated_inc[i]);
        if (i < isolated_count_inc - 1) printf(", ");
    }
    printf("]\n");

    printf("   Концевые: [");
    for (int i = 0; i < pendant_count_inc; i++) {
        printf("%d", pendant_inc[i]);
        if (i < pendant_count_inc - 1) printf(", ");
    }
    printf("]\n");

    printf("   Доминирующие: [");
    for (int i = 0; i < dominant_count_inc; i++) {
        printf("%d", dominant_inc[i]);
        if (i < dominant_count_inc - 1) printf(", ");
    }
    printf("]\n");

    for (int i = 0; i < N; i++) {
        free(adj_matrix[i]);
        free(inc_matrix[i]);
    }
    free(adj_matrix);
    free(inc_matrix);

    for (int i = 0; i < max_edges; i++) {
        free(edges_list[i]);
    }
    free(edges_list);

    free(isolated);
    free(pendant);
    free(dominant);
    free(isolated_inc);
    free(pendant_inc);
    free(dominant_inc);

    return 0;
}