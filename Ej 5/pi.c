#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char** argv) {
    int num_procesos, id_proceso, i, n, local_n, rango_inicial, rango_final;
    double termino, suma_local, suma_global;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proceso);

    n = 1000;
    local_n = n / num_procesos;
    rango_inicial = id_proceso * local_n;
    rango_final = rango_inicial + local_n;

    suma_local = 0.0;
    for (i = rango_inicial; i < rango_final; i++) {
        termino = pow(-1, i) / (2 * i + 1);
        suma_local += termino;
    }

    MPI_Reduce(&suma_local, &suma_global, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (id_proceso == 0) {
        double pi = 4 * suma_global;
        printf("El valor de Pi es aproximadamente: %f\n", pi);
    }

    MPI_Finalize();

    return 0;
}
