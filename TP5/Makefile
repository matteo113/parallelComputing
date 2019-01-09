all: jts jtst jtd

jts: src/julia_thread_simple.cpp src/Array2D.hpp src/julia.cpp
	g++ -o julia_thread_simple src/julia_thread_simple.cpp -pthread -g -std=c++11

jtst: src/julia_thread_static.cpp src/Array2D.hpp src/julia.cpp
	g++ -o julia_thread_static src/julia_thread_static.cpp -pthread -g -std=c++11

jtd: src/julia_thread_dynamic.cpp src/Array2D.hpp src/julia.cpp
	g++ -o julia_thread_dynamic src/julia_thread_dynamic.cpp -pthread -g -std=c++11

test: src/julia.cpp src/Array2D.hpp
	g++ -o julia_test src/julia.cpp -g -std=c++11

clean:
	rm julia_thread_simple julia_thread_dynamic julia_thread_static

cleandat:
	rm *.dat
