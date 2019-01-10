#include "Array2D.hpp"
#include "julia.cpp"
#include <complex>
#include <iostream>
#include <fstream>
#include <functional>
#include <mpi.h>
#include <thread>

int main(int argc, char** argv){

	int myRank, nProc;

 	MPI_Init(&argc, &argv);
 	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
 	MPI_Comm_size(MPI_COMM_WORLD, &nProc);
 	MPI_Status status;

 	// Wait for all threads to setup
 	MPI_Barrier(MPI_COMM_WORLD);

    std::complex<double> lowerLeft (std::stof(argv[1]), std::stof(argv[2]));
    std::complex<double> upperRight (std::stof(argv[3]), std::stof(argv[4]));
    std::complex<double> c(std::stof(argv[5]), std::stof(argv[6]));
    int imax = std::stoi(argv[7]);
    Array2D<int>domain(std::stoi(argv[8]), std::stoi(argv[9]));
    std::string filename(argv[10]);

	double complexPlanHeight = upperRight.imag() - lowerLeft.imag();

    // Dispatch variables
    int elementsPerProcess[nProc];                                                  // how many elements to send to each process
    int offsets[nProc+1];                                                             // offsets where each segment begins
    int rowsPerProcess = floor(domain.sizeY() / nProc);                                    // This represents the number of rows to send to a not-overloaded process
    int elementsNumPerProcess = rowsPerProcess * domain.sizeX();                           // This represents the number of entries to send to a not-overloaded process
    int reminder = (domain.sizeX() * domain.sizeY()) - (rowsPerProcess * nProc * domain.sizeX());        // We compute the remaining unaffected rows after dispatch among process
    int offset = 0;

    // If there are more process than lines in matrix : End process properly
    if(myRank >= domain.sizeY()-1) {
        MPI_Finalize();
        exit(0);
    }

    // Compute variables
    for (int i = 0; i < nProc; i++ ) {

        elementsPerProcess[i] = elementsNumPerProcess;

        if (reminder > 0) {
            elementsPerProcess[i] += domain.sizeX();
            reminder -= domain.sizeX();
        }

        offsets[i] = offset;
        offset += elementsPerProcess[i];
    }

    // Store the assigned part to compute
    Array2D<int>localDomain(domain.sizeX(), elementsPerProcess[myRank]/domain.sizeX());

    // Compute local complex domain size and C
    double complexDomainLineSize = (complexPlanHeight) / domain.sizeY();

    std::complex<double> localComplexPlanLowerLeft(lowerLeft.real(), lowerLeft.imag() + (offsets[myRank]/domain.sizeX()) * complexDomainLineSize);
    std::complex<double> localComplexPlanUpperRight(upperRight.real(), lowerLeft.imag() + (offsets[myRank]/domain.sizeX() + elementsPerProcess[myRank]/domain.sizeX()) * complexDomainLineSize);

    // Each process compute local domain
    julia(localComplexPlanLowerLeft, localComplexPlanUpperRight, c, imax, std::ref(localDomain));
	if (myRank == 2) {
		writePgm(localDomain, imax, "testt.pgm");
	}

    // Wait for each process to finish
    MPI_Barrier(MPI_COMM_WORLD);

    // Retrieve datas to global domain
    MPI_Gatherv(localDomain.data(), elementsPerProcess[myRank], MPI_INT, domain.data(), elementsPerProcess, offsets, MPI_INT, 0, MPI_COMM_WORLD);

    // Save datas to PGM file
    if(myRank == 0)
        writePgm(domain, imax, filename);

    MPI_Finalize();

    return 0;
}
