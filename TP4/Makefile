all: tp2

tp2: src/tp2.cc src/Array2D.hpp
	mpiCC -o laplace src/tp2.cc

clean:
	rm laplace

cleandat:
	rm *.dat
