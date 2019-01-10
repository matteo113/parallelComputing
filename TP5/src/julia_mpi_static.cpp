#include "Array2D.hpp"
#include "julia.cpp"
#include <complex>
#include <iostream>
#include <fstream>
#include <functional>
#include <mpi.h>
#include <thread>

std::vector<int> getLinesMapping(int nProc, int domainY) {

    std::vector<int> mapping = std::vector<int>(domainY);

    for(int i = 0; i < nProc; i++) {
        int tmp = i % nProc;

        for(int j = 0; j < domainY; j++) {

            if(j % nProc == tmp) {
                mapping[j] = i;
            }
        }
    }
    return mapping;
}

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
    int offsets[nProc];
    int rowsPerProcess = floor(domain.sizeY() / nProc);
    int elementsNumPerProcess = rowsPerProcess * domain.sizeX();
    int reminder = (domain.sizeX() * domain.sizeY()) - (rowsPerProcess * nProc * domain.sizeX());
    int offset = 0;


    if(myRank >= domain.sizeY()-1) {
        MPI_Finalize();
        exit(0);
    }

    std::vector<int> mapping = getLinesMapping(nProc, domain.sizeY());

    int lineCounter = 0;

    for(int j = 0; j < domain.sizeY(); j++) {

        if(mapping[j] == myRank) {
            lineCounter++;
        }
    }

    Array2D<int> localLines = Array2D<int>(domain.sizeX(), lineCounter);

    int computeCounter = 0;

    for(int j = 0; j < domain.sizeY(); j++) {

        if(mapping[j] == myRank) {

            Array2D<int> lineToSend = Array2D<int>(domain.sizeX(), 1);
            double complexDomainLineSize = (complexPlanHeight) / domain.sizeY();

            std::complex<double> localComplexPlanLowerLeft(lowerLeft.real(), lowerLeft.imag() + j * complexDomainLineSize);
            std::complex<double> localComplexPlanUpperRight(upperRight.real(), lowerLeft.imag() + j * complexDomainLineSize);

            julia(localComplexPlanLowerLeft, localComplexPlanUpperRight, c, imax, std::ref(lineToSend));

            for(int x = 0; x < domain.sizeX(); x++) {
                localLines(x, computeCounter) = lineToSend(x, 0);
            }

            computeCounter++;

            if(myRank != 0) {
                MPI_Send(lineToSend.data(), domain.sizeX(), MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

    }

    if(myRank == 0) {

        Array2D<int> receivedLine = Array2D<int>(domain.sizeX(), 1);

        int count = 0;

        for(int j = 0; j < domain.sizeY(); j++) {

            if(mapping[j] != 0) {

                MPI_Recv(receivedLine.data(), domain.sizeX(), MPI_INT, mapping[j], 0, MPI_COMM_WORLD, &status);
            } else {

                for(int p = 0; p < domain.sizeX(); p++) {
                    receivedLine(p,0) = localLines(p,count);
                }

                count++;
            }

            for(int x = 0; x < domain.sizeX(); x++) {
                domain(x,j) = receivedLine(x,0);
            }
        }

    }

	MPI_Barrier(MPI_COMM_WORLD);
	double end = MPI_Wtime();

    if(myRank == 0) {
        writePgm(domain, imax, filename);
		std::cout<<"Total execution time  : "<< (end-start)*1000 << "ms" << std::endl;
    }

    MPI_Finalize();

    return 0;

}
