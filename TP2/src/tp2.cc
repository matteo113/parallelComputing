#include <mpi.h>
#include <iostream>
#include <vector>
#include <math.h>
#include "Array2D.hpp"
##include <stdlib.h>

void initMatrix(Array2D<double> &matrix, int dimX, int dimY){
	for (int iX=0; iX<dimX; iX++){
		matrix(iX,0) = 0;
		matrix(iX,dimY-1) = 1;
	}

	for (int iY=0; iY<dimY; iY++){
		matrix(0,iY)      = 0;
		matrix(dimX-1,iY) = 1;
	}
}

//Todo : stock in shared memory

//Returns the number of lines atributed to a process given its process number
int myNumbLine(int myRank, int nbProc, int dimY){
	int tmp = (int)floor((double)(dimY-2)/(double)(nbProc-1);

	// The process 0 is the master process so it doesn't gat any lines
	if (myRank == 0) {
		return 0;
	}

	else if (myRank-1<=(dimY-2)%(nbProc-1)){
		return tmp+1;
	}
	else if (myRank < nbProc){
		return tmp;
	}
	else{
		return 0;
	}
}

int main(int argc, char **argv) {
  int myRank, nProc;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);

	const int dimX = atoi(argv[0]);
	const int dimY = atoi(argv[1]);
	const int iteration = atoi(argv[2]);

	//Master node
	if (myRank == 0){

		//Setting up master matrix

		Array2D<double> heat(dimX, dimY, 0);
		Array2D<double> tmp(dimX, dimY, 0);

		initMatrix(&heat, dimX, dimY);
		initMatrix(&tmp, dimX, dimY);

		//Dispatching matrix on every availlable proc

		int count = 1;

		for (int i = 1; i < nProc; i++){
			int nbLine = myNumbLine(i, nProc, dimY);
			MPI_Send(&heat + count * heat.sizeY() , nbLine * heat.sizeY(), MPI_DOUBLE, i, 0, MPI_COMM_WORLD); //sending sub matrix (calculating the offset from the number of lines already sent)
			count += nbLine;

			//Send first ans last line to the right proc :
			//First line
			if (i == 1) {
				MPI_Send(&heat, heat.sizeY(), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			}
			//Last line
			if (i == nProc-1){
				MPI_Send(&heat + (dimY-1) * heat.sizeY(), heat.sizeY(), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			}
		}
	}

	//Slave nodes
	if (myRank > 0) {
		int nbLine = myNumbLine(i, nProc, dimY);
		MPI_Status status;

		//setting up empty submatrix
		Array2D<double> subHeat(dimX, nbLine, 0);
		Array2D<double> subTmp(dimX, nbLine, 0);

		MPI_Recv(*subHeat, nbLine * subHeat.sizeY(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

		//Copying subHeat into subTmp
		for (int iX = 0; iX < dimX; iX++) {
			for (int iY = 0; iY < nbLine; iY++) {
				subTmp(iX, iY) = subHeat(iX, iY);
			}
		}

		// Create empty line for line above and under
		Array2D<double> above(dimX, 1, 0);
		Array2D<double> under(dimX, 1, 0);

		//Receive first and last line
		if (myRank == 1) {
			MPI_Recv(*above, above.sizeY(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
		}
		if (myRank == nProc-1) {
			MPI_Recv(*under, under.sizeY(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
		}

		//iteration step
		for (int i = 0; i < iteration; i++) {

			//sending last lines
			if (myRank != nProc-1) {
				MPI_Send(*subHeat + (nbLine-1), subHeat.sizeY(), MPI_DOUBLE, myRank+1, 0, MPI_COMM_WORLD);
			}

			//Receiving line above
			if (myRank != 1) {
				MPI_Recv(*above, above.sizeY(), MPI_DOUBLE, myRank-1, 0, MPI_COMM_WORLD, &status);
			}

			//Sending first line
			if (myRank != 1) {
				MPI_Send(*subHeat , subHeat.sizeY(), MPI_DOUBLE, myRank-1, 0, MPI_COMM_WORLD);
			}

			//Receiving line under
			if (myRank != nProc-1) {
				MPI_Recv(*under, under.sizeY(), MPI_DOUBLE, myRank+1, 0, MPI_COMM_WORLD, &status);
			}

			// ADD the actual computation step

			// ADD the swap betwen heat and tmp
		}

		// ADD send back to master the results
	}


}
