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
    int offsets[nProc];                                                             // offsets where each segment begins
    int rowsPerProcess = floor(domain.sizeY() / nProc);                                    // This represents the number of rows to send to a not-overloaded process
    int elementsNumPerProcess = rowsPerProcess * domain.sizeX();                           // This represents the number of entries to send to a not-overloaded process
    int reminder = (domain.sizeX() * domain.sizeY()) - (rowsPerProcess * nProc * domain.sizeX());        // We compute the remaining unaffected rows after dispatch among process
    int offset = 0;


    // If there are more process than lines in matrix : End process properly
    if(myRank >= domain.sizeY()-1) {
        MPI_Finalize();
        exit(0);
    }

    std::vector<int> mapping = getLinesMapping(nProc, domain.sizeY());                         // Each process knows the nature of god

    int lineCounter = 0;

    // Retrieve line number per Rank
    for(int j = 0; j < domain.sizeY(); j++) {

        if(mapping[j] == myRank) {
            lineCounter++;
        }
    }

    // Create local lines storage
    Array2D<int> localLines = Array2D<int>(domain.sizeX(), lineCounter);

    // Knows which line it is
    int computeCounter = 0;

    // Each process will compute its attributed lines
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

            // If line wasn't computed by process 0
            if(mapping[j] != 0) {

                // Retrieve line computed by another rank
                MPI_Recv(receivedLine.data(), domain.sizeX(), MPI_INT, mapping[j], 0, MPI_COMM_WORLD, &status);
            } else {

                // Process 0 calculated the line, retrieves value for local memory
                for(int p = 0; p < domain.sizeX(); p++) {
                    receivedLine(p,0) = localLines(p,count);
                }

                count++;
            }

            // Group results in domain
            for(int x = 0; x < domain.sizeX(); x++) {
                domain(x,j) = receivedLine(x,0);
            }
        }

    }

    // // Wait for each process to finish
    MPI_Barrier(MPI_COMM_WORLD);

    if(myRank == 0) {
        writePgm(domain, imax, filename);
    }

    MPI_Finalize();

    return 0;

}
