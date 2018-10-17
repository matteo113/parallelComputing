all: simple ring hypercube mpi

mpi: mpi_bcast.cpp
	mpiCC -o mpi mpi_bcast.cpp

simple: simple_bcast.cpp
	mpiCC -o simple simple_bcast.cpp

ring: ring_bcast.cpp
	mpiCC -o ring ring_bcast.cpp

hypercube: hypercube_bcast.cc
	mpiCC -o hypercube hypercube_bcast.cc

clean:
	rm simple ring hypercube mpi
