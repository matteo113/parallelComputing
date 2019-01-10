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

    auto start = std::chrono::steady_clock::now();

	std::vector<int> linePerProcess(nThread);                                                  // how many elements to send to each process
	std::vector<int> offsets(nThread, 0);                                                             // offsets where each segment begins
	int baseLineperProcess = floor(domain.sizeY() / nThread);
	for (int i = 0; i < nThread; i++) {
		linePerProcess[i] = baseLineperProcess;
		if (i < domain.sizeY() % nThread) {
			linePerProcess[i] += 1;
		}
		if ( i != 0){
			offsets[i] = offsets[i-1] + linePerProcess[i-1];
		}
	}
	std::vector<std::thread> threads;

	for (int i = 0; i < nThread; i++) {
		threads.push_back(std::thread(juliaThreadSimple, lowerLeft, upperRight, c, imax, std::ref(domain), offsets[i], linePerProcess[i]));
	}

	for(auto& t : threads) t.join();
    auto end = std::chrono::steady_clock::now();

    writePgm(domain, imax, filename);

    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout<<"Total execution time  : "<< delta.count() << "ms" << std::endl;
    return 0;
}
