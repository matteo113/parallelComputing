// ==================================================
// MPI Broadcast Implementation
// ==================================================
// Course: Parallel Computing
// Assignment: TP1
// Student: Terry Vogelsang
// ==================================================

#include <mpi.h>
#include <iostream>
#include <vector>
 
int main(int argc, char **argv) {
  
  int myRank, nProc;
 
 // MPI Setup
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);
 
  // Vectors initialization : Vector of size 100'000'000. If Rank = 0, vector is filled with 1s
  std::vector<int> vector(100000000, 0);
  if(myRank == 0) vector = std::vector<int>(100000000, 1);
 
  // Wait for all processes to terminate (Be in the same state) 
  MPI_Barrier(MPI_COMM_WORLD);

  // Get timestamp as start time
  double startTime = MPI_Wtime();

  // Broadcast using built-in MPI_Bcast method
  MPI_Bcast( vector.data(), vector.size(), MPI_INT, 0, MPI_COMM_WORLD );
 
  // Wait for all processes to terminate (Be in the same state) 
  MPI_Barrier(MPI_COMM_WORLD);

  // Get timestamp as end time
  double endTime = MPI_Wtime();

  if(myRank==0) std::cout << "Operation took : " << endTime-startTime << "s" << std::endl;
 
  MPI_Finalize();
}