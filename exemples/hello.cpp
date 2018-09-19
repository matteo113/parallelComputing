#include <mpi.h>
#include <iostream>

int main(int argc, char **argv) {
  int myRank, nProc;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);

  std::cout << "Hello, I'm process" << myRank << std::endl;
  std::cout << "There is " << nProc << " processes" << std::endl;

  MPI_Finalize();
}
