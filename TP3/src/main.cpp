#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <iterator>
#include <thread>
#include <fstream>

#include "Array2D.hpp"
#include "Barrier.hpp"

Array2D<double> heat(1, 1, 0);
Array2D<double> tmp(1, 1, 0);
Barrier barrier;

// Returns the number of lines assigned to each process or thread
// The lines are divised equally
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

// performe the actual computation step for each thread. For that matter we need to give the thread number and the lines the thread is responsible for
void laplace(int dimX, int dimY, int iteration, int threadNb, int nb_line, int first){
	//iteration step
	for (int i = 0; i < iteration; i++) {

		//We wait until each thread reached this point (to be sure the matrix have been correctly swapped)
		barrier.wait();

		//Computation step
		for (int y = first; y < first + nb_line; y++) {
			for (int x = 1; x < dimX -1; x++) {
				tmp(x, y) = 0.25*( heat(x-1,y) + heat(x+1,y)+ heat(x,y-1) + heat(x,y+1) );
			}
		}

		//We wait until each thread reached this point (To be sure each thread finisshed the computation step before swapping matrix)
		barrier.wait();

		//thread 0 swap matrix
		if(threadNb == 0){
			heat.unsafeSwap(tmp);
		}
	}
}


int main(int argc, char **argv) {

	// Argument parsing
	const int dimX = atoi(argv[1]);
	const int dimY = atoi(argv[2]);
	const int iteration = atoi(argv[3]);
	int nThread = atoi(argv[4]);

	//Those vector helps storring which thread takes care of which lines so we do not need to caal the attribution function each time
	std::vector<int> nb_line(nThread);
	std::vector<int> first(nThread);

	// Computing differents values for each process in order fill the two vectors
	for (int i = 0; i < nThread; i++) {
		// Number of line for each thread
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
	first.resize(nThread);

	//resizing the two matrix since the attributes have been initialised with an arbitrary value
	heat.resize(dimX, dimY);
	tmp.resize(dimX, dimY);

	for (int iX=0; iX<dimX; iX++) {      // Borders initial conditions
			heat(iX,0) = 0;                 // 0 up
			tmp(iX,0) = 0;
			heat(iX,dimY-1) = 1;            // 1 down
			tmp(iX,dimX-1) = 1;
	}

	for (int iY=0; iY<dimY; iY++) {
			heat(0,iY) = 0.;
			tmp(0,iY) = 0.;           			// 0 left
			heat(dimX-1,iY) = 1.;           // 1 right
			tmp(dimX-1,iY) = 1.;
	}

	//initialisation of the barrier object
	barrier.init(nThread);

	// making a vector to hol the threads
	std::vector<std::thread> threads;

	//filling the vector and launching the threads with the correct values
	for (int i = 0; i < nThread; i++) {
		threads.push_back(std::thread(laplace, dimX, dimY, iteration, i, nb_line[i], first[i]));
	}

	//wainting until each thread is finished
	for(auto& t : threads) t.join();

	//saving the final matrix
	save(heat, "chaleur.dat");

}
