all: simple ring hypercube mpi

mpi: bmpi.cpp
	mpiCC -o mpi bmpi.cpp

simple: bsimple.cpp
	mpiCC -o simple bsimple.cpp

ring: bring.cpp
	mpiCC -o ring bring.cpp

hypercube: bhyper.cpp
	mpiCC -o hypercube bhyper.cpp

clean:
	rm simple ring hypercube mpi
