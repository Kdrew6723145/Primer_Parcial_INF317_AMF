#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 20

int main(int argc, char** argv) {

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long int f[N];
    f[0] = 0, f[1] = 1;
    int i;

    for(i = 2; i < N; i++) {
        f[i] = f[i-1] + f[i-2];
    }

    if(rank == 0) {
        printf("Los primeros %d números de la serie de Fibonacci son:\n", N);
        for(i = 0; i < N; i++) {
            printf("%lld ", f[i]);
        }
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}
