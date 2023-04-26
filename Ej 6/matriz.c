#include <mpi.h>
#include <omp.h>
#include <stdio.h>

#define N 100

int main(int argc, char *argv[]) {
  int rank, size;
  int A[N][N], B[N][N], C[N][N];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Inicializar matrices A y B
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[i][j] = i + j;
      B[i][j] = i - j;
    }
  }

  // Dividir la matriz en bloques para que cada proceso MPI maneje una sección de la matriz
  int rows_per_process = N / size;
  int start_row = rank * rows_per_process;
  int end_row = start_row + rows_per_process;

  // Realizar la multiplicación de la matriz en paralelo
  #pragma omp parallel for collapse(2)
   for (int i = start_row; i < end_row; i++) {
    for (int j = 0; j < N; j++) {
      int sum = 0;
      for (int k = 0; k < N; k++) {
        sum += A[i][k] * B[k][j];
      }
      C[i][j] = sum;
    }
  }

  // Combinar los resultados parciales de cada proceso MPI
  if (rank == 0) {
    for (int i = 1; i < size; i++) {
      MPI_Recv(&C[i * rows_per_process][0], rows_per_process * N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  } else {
    MPI_Send(&C[start_row][0], rows_per_process * N, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  // Imprimir la matriz resultado
  if (rank == 0) {
    printf("Matriz resultado:\n");
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        printf("%d ", C[i][j]);
      }
      printf("\n");
        }
  }

  MPI_Finalize();

  return 0;
}
