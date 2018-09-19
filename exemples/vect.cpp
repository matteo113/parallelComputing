#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  int myRank, nProc;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);

  std::vector<int> v(10, myRank);
  int sum = 0;
  for(auto i: v) sum += i;

  std::cout << "Hello, I'm process" << myRank << " and the sum of my vector is " << sum << std::endl;

  MPI_Finalize();
}
