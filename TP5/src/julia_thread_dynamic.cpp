#include "Array2D.hpp"
#include "julia.cpp"
#include <complex>
#include <iostream>
#include <fstream>
#include <functional>
#include <thread>

int main(int argc, char** argv){

    // initialisation des parametres
    std::complex<double> lowerLeft (std::stof(argv[1]), std::stof(argv[2]));
    std::complex<double> upperRight (std::stof(argv[3]), std::stof(argv[4]));
    std::complex<double> c(std::stof(argv[5]), std::stof(argv[6]));
    int imax = std::stoi(argv[7]);
    Array2D<int>domain(std::stoi(argv[8]), std::stoi(argv[9]));
    std::string filename(argv[10]);
	int nThread = atoi(argv[11]);
	int l = atoi(argv[12]);

	std::vector<std::thread> threads;

	for (int i = 0; i < nThread; i++) {
		threads.push_back(std::thread(juliaThreadDynamic, lowerLeft, upperRight, c, imax, std::ref(domain), l));
	}

	for(auto& t : threads) t.join();

    // ecriture du resultat dans un fichier
    writePgm(domain, imax, filename);

    return 0;
}
