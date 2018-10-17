#include <mpi.h>
#include <iostream>
#include <vector>

void bsimple(std::vector<int> *vect, int rank, int nProc){
	MPI_Status stat;

	if (rank == 0){
		for (size_t i = 1; i < nProc; i++) {
			MPI_Send(vect->data(), vect->size(), MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	} else{
		  MPI_Recv(vect->data(), vect->size(), MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
	}
}

int main(int argc, char **argv) {
	int myRank, nProc;

	MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);

	std::vector<int> vect(100000000, 0);

	if(myRank == 0)
	{
		vect = std::vector<int>(100000000, 1);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double start = MPI_Wtime();

	bsimple(&vect, myRank, nProc);

	MPI_Barrier(MPI_COMM_WORLD);
  double end = MPI_Wtime();

	if(myRank==0) std::cout << "temps de l'operation : " << end-start << "[s]" << std::endl;

  MPI_Finalize();
}
