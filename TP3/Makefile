all: tp3

tp3: src/main.cpp src/Array2D.hpp src/Barrier.hpp
	 g++ -o laplace_thread src/main.cpp -pthread -g

clean:
	rm laplace_thread

cleandat:
	rm *.dat
