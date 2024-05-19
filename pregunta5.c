#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void multiplicarMatrizPorVector(int N, int matriz[N][N], int vector[N], int resultado_local[N]) {
    for (int i = 0; i < N; i++) {
        resultado_local[i] = 0;
        for (int j = 0; j < N; j++) {
            resultado_local[i] += matriz[i][j] * vector[j];
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size, N;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Ingrese el tamaño N de la matriz y el vector: ");
        scanf("%d", &N);
    }

    // Se transmite el tamaño N a todos los procesos
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int matriz[N][N];
    int vector[N];
    int resultado_local[N], resultado_final[N];

    if (rank == 0) {
        printf("Ingrese los elementos de la matriz:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                scanf("%d", &matriz[i][j]);
            }
        }

        printf("Ingrese los elementos del vector:\n");
        for (int i = 0; i < N; i++) {
            scanf("%d", &vector[i]);
        }
    }

    // Se transmiten la matriz y el vector a todos los procesos
    MPI_Bcast(matriz, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso calcula parte del resultado
    multiplicarMatrizPorVector(N, matriz, vector, resultado_local);

    // Se realiza la reducción de los resultados locales
    MPI_Reduce(resultado_local, resultado_final, N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Resultado de la multiplicación:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", resultado_final[i]);
        }
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}
