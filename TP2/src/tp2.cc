#include <mpi.h>
#include <iostream>
#include <vector>
#include <math.h>
#include "Array2D.hpp"
#include <stdlib.h>
#include <iterator>
#include <fstream>

//Todo : stock in shared memory

//Returns the number of lines atributed to a process given its process number
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

	// Vector initialisation for Scatterv and Gatherv
	std::vector<int> nb_line(nProc);
	std::vector<int> size_vec(nProc);
	std::vector<int> disp(nProc, 0);

	// Computing differents values for each process in order to use Scatterv and Gatherv
	for (int i = 0; i < nProc; i++) {
		// Number of line for each process
		nb_line[i] = myNumbLine(i, nProc, dimY);

		// Size of the submatrix for each process
		size_vec[i] = nb_line[i] * dimX;

		// Taking car of unused process
		if (i >= dimY - 2){
			nProc = i;
			break;
		}

		// Computing displacement for each process
		if (i > 0) {
			disp[i] = disp[i-1] + size_vec[i-1];
		}
		else if (i == 0){
			disp[i] = dimX;
		}
	}

	// resizing Vectors given the new number of process
	nb_line.resize(nProc);
	size_vec.resize(nProc);
	disp.resize(nProc);

	//Setting up master matrix
	Array2D<double> heat(dimX, dimY, 0);

	//setting up empty submatrix
	Array2D<double> subHeat(dimX, nb_line[myRank], 0);
	Array2D<double> subTmp(dimX, nb_line[myRank], 0);

	// Create empty line for line above and under
	std::vector<double> above(dimX, 0);
	std::vector<double> under(dimX, 0);

	// Create empty line first and last line
	std::vector<double> first(dimX, 0);
	std::vector<double> last(dimX, 0);

	if (myRank == 0) {

		//initiate master heat matrix
		for (int iX=0; iX<dimX; iX++) {      // conditions aux bords:
				heat(iX,0) = 0;                 // 0 en haut
				heat(iX,dimY-1) = 1;            // 1 en bas
		}
		for (int iY=0; iY<dimY; iY++) {
				heat(0,iY)      = 0.;           // 0 a gauche
				heat(dimX-1,iY) = 1.;           // 1 a droite
		}
	}

	// Close unused process and things
	if(myRank >= dimY-2){
		if (dimY == 2) {
			if (myRank == 0) {
				save(heat, "chaleur.dat");
			}
		}
		MPI_Finalize();
		exit(0);
	}

	if (myRank == 0){

		// Set first and last line for master matrix, first line of master matrix is automaticaly the line above the first submatrix
		// This is due to the fact that the first and last line of the master matrix ar not copied in any submatrix in order to
		// optinmize code modularity.
		for (int iX = 0; iX < dimX; iX++) {
			above[iX] = heat(iX,0);
			if (nProc == 1) {
				under[iX] = heat(iX, dimY-1);
			}
			else{
				last[iX] = heat(iX, dimY-1);
			}
		}

		//send and last line of the master matric to the last process
		if(nProc != 1){
			MPI_Send(last.data(), last.size(), MPI_DOUBLE, nProc-1, 0, MPI_COMM_WORLD);
		}

	}

	//Receive last line as the line under of the last submatrix
	if (myRank == nProc-1 && nProc != 1) {
		MPI_Recv(under.data(), under.size(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
	}

	//Dispatching  mastermatrix on every availlable proc in their own submatrix
	MPI_Scatterv(heat.data(), size_vec.data(), disp.data(), MPI_DOUBLE, subHeat.data(), size_vec[myRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

		//Copying subHeat into subTmp
		for (int iX = 0; iX < dimX; iX++) {
			for (int iY = 0; iY < nb_line[myRank]; iY++) {
				subTmp(iX, iY) = subHeat(iX, iY);
			}
		}

		//iteration step
		for (int i = 0; i < iteration; i++) {

			for (int iX = 0; iX < dimX; iX++) {

				// Update the first and last line vector withe new values
				first[iX] = subHeat(iX,0);
				last[iX] = subHeat(iX, nb_line[myRank]-1);

			}

			//sending last lines
			if (myRank != nProc-1) {
				MPI_Send(last.data(), last.size(), MPI_DOUBLE, myRank+1, 0, MPI_COMM_WORLD);
			}

			//Receiving line above
			if (myRank != 0) {
				MPI_Recv(above.data(), above.size(), MPI_DOUBLE, myRank-1, 0, MPI_COMM_WORLD, &status);
			}

			//Sending first line
			if (myRank != 0) {
				MPI_Send(first.data() , first.size(), MPI_DOUBLE, myRank-1, 0, MPI_COMM_WORLD);
			}

			//Receiving line under
			if (myRank != nProc-1) {
				MPI_Recv(under.data(), under.size(), MPI_DOUBLE, myRank+1, 0, MPI_COMM_WORLD, &status);
			}

			// The actual computation step
			for (int iX = 1; iX < dimX - 1; iX++){
				for (int iY = 0; iY < nb_line[myRank]; iY++){
					if (nb_line[myRank] == 1){
						subTmp(iX, iY) = 0.25*(subHeat(iX-1,iY) + subHeat(iX+1, iY) + above[iX] + under[iX]);
					}
					else if (iY == 0) {
						subTmp(iX, iY) = 0.25*(subHeat(iX-1,iY) + subHeat(iX+1, iY) + above[iX] + subHeat(iX,iY+1));
					}
					else if (iY == nb_line[myRank] - 1){
						subTmp(iX, iY) = 0.25*(subHeat(iX-1,iY) + subHeat(iX+1, iY) + subHeat(iX,iY-1) + under[iX]);
					}
					else {
						subTmp(iX, iY) = 0.25*(subHeat(iX-1,iY) + subHeat(iX+1, iY) + subHeat(iX,iY-1) + subHeat(iX,iY+1));
					}
				}
			}

			// swap the temporary matrix
			subHeat.unsafeSwap(subTmp);
		}

	// Gather evry submatrix in the mastermatrix on process 0
	MPI_Gatherv(subHeat.data(), size_vec[myRank], MPI_DOUBLE, heat.data(), size_vec.data(), disp.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// Save master matrix in a file
	if (myRank == 0) {
		save(heat, "chaleur.dat");
	}

	// Say goobye
	MPI_Finalize();
}
