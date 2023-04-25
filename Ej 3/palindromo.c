#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    int rank, size, length, i, result, local_result;
    char word[100], local_word[100];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        printf("Ingrese la palabra: ");
        fflush(stdout);
        scanf("%s", word);
        length = strlen(word);
    }
    
    MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        for (i = 0; i < size; i++) {
            MPI_Send(&word[i * (length / size)], length / size, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    }
    
    MPI_Recv(local_word, length / size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    local_word[length / size] = '\0';
    
    local_result = 1;
    for (i = 0; i < length / (2 * size); i++) {
        if (local_word[i] != local_word[length / size - i - 1]) {
            local_result = 0;
            break;
        }
    }
    
    MPI_Reduce(&local_result, &result, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        if (result) {
            printf("%s true.\n", word);
        } else {
            printf("%s false.\n", word);
        }
    }
    
    MPI_Finalize();
    return 0;
}
