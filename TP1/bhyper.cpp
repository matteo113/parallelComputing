#include <mpi.h>
#include <iostream>
#include <vector>
#include <math.h>

void bhyper(std::vector<int> *vect, int rank, int nProc){
	MPI_Status stat;

	double dim = log2(nProc);

	for (int step = 0; step < dim ; step++) {
		if(rank >>  step == 0) {
			int neibors = rank xor 1<<step;
			MPI_Send(vect->data(), vect->size(), MPI_INT, neibors, 0, MPI_COMM_WORLD);
		}
		if(rank >> step == 1) {
			MPI_Recv(vect->data(), vect->size(), MPI_INT, MPI_ANY_SOURCE, 0,MPI_COMM_WORLD, &stat);
		}
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

	std::cout << "Before broadcast : Rank(" << myRank << ") 1st Value in vector(" << vect.at(0) <<")"<< std::endl;

	double start = MPI_Wtime();

	bhyper(&vect, myRank, nProc);

	MPI_Barrier(MPI_COMM_WORLD);
  double end = MPI_Wtime();

	if(myRank==0) std::cout << "temps de l'operation : " << end-start << "[s]" << std::endl;

	std::cout << "After broadcast : Rank(" << myRank << ") 1st Value in vector(" << vect.at(0) <<")"<< std::endl;


  MPI_Finalize();
}
