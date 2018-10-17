// ==================================================
// Ring Broadcast Implementation
// ==================================================
// if rank = 0
//   send data to 1 and 2 if they exist
// else
//   receive data
//   send data to rank+2 if it exists
// ==================================================
// Course: Parallel Computing
// Assignment: TP1
// Student: Terry Vogelsang
// ==================================================

#include <mpi.h>
#include <iostream>
#include <vector>

int ring_bcast(std::vector<int> *vector, int myRank, int nProc)
{
  MPI_Status status;

  if(myRank==0)
  {
    if(nProc > 1)
    {
        MPI_Send(vector->data(), vector->size(), MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    if(nProc > 2)
    {
        MPI_Send(vector->data(), vector->size(), MPI_INT, 2, 0, MPI_COMM_WORLD);
    }
  } else
  {
    MPI_Recv(vector->data(), vector->size(), MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    if(myRank+2 < nProc)
    {
        MPI_Send(vector->data(), vector->size(), MPI_INT, myRank+2, 0, MPI_COMM_WORLD);
    }
  }
}

int main(int argc, char **argv)
{
  int myRank, nProc;

 // MPI Setup
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);

  // Vectors initialization : Vector of size 100'000'000. If Rank = 0, vector is filled with 1s
  std::vector<int> vector(100000000, 0);

  if(myRank == 0)
  {
    vector = std::vector<int>(100000000, 1);
  }

  // Wait for all processes to terminate (Be in the same state)
  MPI_Barrier(MPI_COMM_WORLD);

  std::cout << "Before broadcast : Rank(" << myRank << ") 1st Value in vector(" << vector.at(0) <<")"<< std::endl;

  // Get timestamp as start time
  double startTime = MPI_Wtime();

  // Broadcast using Ring Broadcast Algorithm
  ring_bcast(&vector, myRank, nProc);

  // Wait for all processes to terminate (Be in the same state)
  MPI_Barrier(MPI_COMM_WORLD);

  // Get timestamp as end time
  double endTime = MPI_Wtime();

  std::cout << "After broadcast : Rank(" << myRank << ") 1st Value in vector(" << vector.at(0) <<")"<< std::endl;

  if(myRank==0)
  {
     std::cout << "Operation took : " << endTime-startTime << "s" << std::endl;
  }

  MPI_Finalize();
}
