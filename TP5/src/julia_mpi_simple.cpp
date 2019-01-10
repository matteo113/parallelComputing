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

 	MPI_Barrier(MPI_COMM_WORLD);

    std::complex<double> lowerLeft (std::stof(argv[1]), std::stof(argv[2]));
    std::complex<double> upperRight (std::stof(argv[3]), std::stof(argv[4]));
    std::complex<double> c(std::stof(argv[5]), std::stof(argv[6]));
    int imax = std::stoi(argv[7]);
    Array2D<int>domain(std::stoi(argv[8]), std::stoi(argv[9]));
    std::string filename(argv[10]);

	MPI_Barrier(MPI_COMM_WORLD);
	double start = MPI_Wtime();

	double complexPlanHeight = upperRight.imag() - lowerLeft.imag();


    int elementsPerProcess[nProc];
    int offsets[nProc+1];
    int rowsPerProcess = floor(domain.sizeY() / nProc);
    int elementsNumPerProcess = rowsPerProcess * domain.sizeX();
    int reminder = (domain.sizeX() * domain.sizeY()) - (rowsPerProcess * nProc * domain.sizeX());
    int offset = 0;

    if(myRank >= domain.sizeY()-1) {
        MPI_Finalize();
        exit(0);
    }

    for (int i = 0; i < nProc; i++ ) {

        elementsPerProcess[i] = elementsNumPerProcess;

        if (reminder > 0) {
            elementsPerProcess[i] += domain.sizeX();
            reminder -= domain.sizeX();
        }

        offsets[i] = offset;
        offset += elementsPerProcess[i];
    }

    Array2D<int>localDomain(domain.sizeX(), elementsPerProcess[myRank]/domain.sizeX());

    double complexDomainLineSize = (complexPlanHeight) / domain.sizeY();

    std::complex<double> localComplexPlanLowerLeft(lowerLeft.real(), lowerLeft.imag() + (offsets[myRank]/domain.sizeX()) * complexDomainLineSize);
    std::complex<double> localComplexPlanUpperRight(upperRight.real(), lowerLeft.imag() + (offsets[myRank]/domain.sizeX() + elementsPerProcess[myRank]/domain.sizeX()) * complexDomainLineSize);

    julia(localComplexPlanLowerLeft, localComplexPlanUpperRight, c, imax, std::ref(localDomain));
	if (myRank == 2) {
		writePgm(localDomain, imax, "testt.pgm");
	}

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gatherv(localDomain.data(), elementsPerProcess[myRank], MPI_INT, domain.data(), elementsPerProcess, offsets, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	double end = MPI_Wtime();

    if(myRank == 0){
        writePgm(domain, imax, filename);
		std::cout<<"Total execution time  : "<< (end-start)*1000 << "ms" << std::endl;
	}

    MPI_Finalize();

    return 0;
}
