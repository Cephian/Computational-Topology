
obj = build/run.o build/filtered_complex.o build/simplex.o build/witness_complex.o build/homology.o

all: run

run: $(obj)
	g++ $(obj) -O3 -o run

build/run.o: src/run.cpp
	g++ -c src/run.cpp -O3 -o build/run.o

build/homology.o: src/homology.cpp
	g++ -c src/homology.cpp -O3 -o build/homology.o

build/simplex.o: src/complex/simplex.cpp
	g++ -c src/complex/simplex.cpp -O3 -o build/simplex.o

build/filtered_complex.o: src/complex/filtered_complex.cpp
	g++ -c src/complex/filtered_complex.cpp -O3 -o build/filtered_complex.o

build/witness_complex.o: src/construction/witness_complex.cpp
	g++ -c src/construction/witness_complex.cpp -O3 -o build/witness_complex.o

clean:
	rm build/*.o