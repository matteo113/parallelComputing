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

	if(myRank == 0){
		for (int i = 1; i < nProc; i++) {
			MPI_Status status;
			int rec = 0;
			MPI_Recv(&rec, sizeof(int), MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			sum += rec;
		}
		std::cout << "Hello, I'm process " << myRank << " and the sum of all the vector is " << sum << std::endl;
	}

	else{
		MPI_Send(&sum, sizeof(int), MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

  MPI_Finalize();
}
