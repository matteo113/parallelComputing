#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  int myRank, nProc;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);

  std::vector<int> v(10, 0);

  if(myRank == 0){
    for(int i=0; i<v.size(); i++) v[i] = i;
    std::cout << "I'm process 0 and I'm sending my vector" << std::endl;
    MPI_Send(v.data(), v.size(), MPI_INT, 1, 0, MPI_COMM_WORLD);
  }
  else if(myRank == 1){
    MPI_Status status;
    std::cout << "I'm process 1, last element of my vector before receive is " << v.back() << std::endl;
    MPI_Recv(v.data(), v.size(), MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    std::cout << "I'm process 1, last element of my vector after receive is " << v.back() << std::endl;
  }
  else{
    std::cout << "I'm process " << myRank << " I have nothing to do" << std::endl;
  }

  MPI_Finalize();
}
