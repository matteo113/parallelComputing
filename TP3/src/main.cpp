#include <mpi.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <iterator>
#include <fstream>

#include "Array2D.hpp"
#include "Barrier.hpp"

Array2D<double> heat(1, 1, 0);
Array2D<double> tmp(1, 1, 0);
Barrier barrier;

int myNumbLine(int myRank, int nbProc, int dimY){
	int tmp = (int)floor((double)(dimY-2)/(double)(nbProc));

	 if (myRank<(dimY-2)%(nbProc)){
		return tmp+1;
	}
	else if (myRank < nbProc){
		return tmp;
	}
	else{
		return 0;
	}
}

// Save an Array2D to a .dat file
void save(Array2D<double> &matrix, std::string name) {
  std::ofstream file(name.c_str());
  for (int iY=0; iY<matrix.sizeY(); ++iY) {
     copy(&matrix.data()[iY*matrix.sizeX()], &matrix.data()[iY*matrix.sizeX()]+matrix.sizeX(),
          std::ostream_iterator<double>(file, " "));
     file << "\n";
  }
}

void laplace(int dimX, int dimY, int iteration, int threadNb, int nb_line, int first){
	for (int i = 0; i < iteration; i++) {

		barrier.wait();

		for (int y = first; y < first + nb_line; y++) {
			for (int x = 1; x < dimX -1; x++) {
				tmp(x, y) = 0.25*( heat(iX-1,iY) + heat(iX+1,iY)+ heat(iX,iY-1) + heat(iX,iY+1) );
			}
		}
		barrier.wait();
		if(threadNb == 0){
			heat.unsafeSwap(tmp);
		}
	}
}

int main(int argc, char **argv) {
	int myRank, nProc;

	// MPI Initialisation
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProc);

	MPI_Status status;

	// Argument parsing
	const int dimX = atoi(argv[1]);
	const int dimY = atoi(argv[2]);
	const int iteration = atoi(argv[3]);
	const int nThread = atoi(argv[4]);

	std::vector<int> nb_line(nThread);
	std::vector<int> first(nThread);

	// Computing differents values for each process in order to use Scatterv and Gatherv
	for (int i = 0; i < nThread; i++) {
		// Number of line for each process
		nb_line[i] = myNumbLine(i, nThread, dimY);

		// Taking car of unused thread
		if (i >= dimY - 2){
			nThread = i;
			break;
		}

		// Computing displacement for each thread
		if (i == 0) {
			first[i] = 1;
		} else{
			first[i] = first[i-1] + nb_line[i-1];
		}
	}

	// resizing Vectors given the new number of thread
	nb_line.resize(nThread);
	disp.resize(nThread);

	heat.resize(dimX, dimY);
	tmp.resize(dimX, dimY);

	for (int iX=0; iX<dimX; iX++) {      // conditions aux bords:
			heat(iX,0) = 0;                 // 0 en haut
			tmp(ix,0) = 0;
			heat(iX,dimY-1) = 1;            // 1 en bas
			tmp(ix,dimX-1) = 1;
	}

	for (int iY=0; iY<dimY; iY++) {
			heat(0,iY) = 0.;
			tmp(0,iY) = 0.;           // 0 a gauche
			heat(dimX-1,iY) = 1.;           // 1 a droite
			tmp(dimX-1,iY) = 1.;
	}

	barrier.init(nThread);

	std::vector<std::thread> threads;

	for (int i = 0; i < nThread; i++) {
		threads.push_back(std::thread(laplace, dimX, dimY, iteration, i, nb_line[i], first[i]));
	}

}
