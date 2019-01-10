all: jts jtst jtd jms jmst

jts: src/julia_thread_simple.cpp src/Array2D.hpp src/julia.cpp
	g++ -o julia_thread_simple src/julia_thread_simple.cpp -pthread -g -std=c++11

jtst: src/julia_thread_static.cpp src/Array2D.hpp src/julia.cpp
	g++ -o julia_thread_static src/julia_thread_static.cpp -pthread -g -std=c++11

jtd: src/julia_thread_dynamic.cpp src/Array2D.hpp src/julia.cpp
	g++ -o julia_thread_dynamic src/julia_thread_dynamic.cpp -pthread -g -std=c++11

jms: src/Array2D.hpp src/julia.cpp src/julia_mpi_simple.cpp
	mpiCC -o julia_mpi_simple src/julia_mpi_simple.cpp

jmst: src/Array2D.hpp src/julia.cpp src/julia_mpi_static.cpp
	mpiCC -o julia_mpi_static src/julia_mpi_static.cpp

test: src/julia.cpp src/Array2D.hpp
	g++ -o julia_test src/julia.cpp -g -std=c++11

clean:
	rm julia_thread_simple julia_thread_dynamic julia_thread_static julia_mpi_simple julia_mpi_static

cleanpgm:
	rm *.pgm
