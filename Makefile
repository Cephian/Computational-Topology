
obj = build/tests.o build/filtered_complex.o build/simplex.o build/witness_complex.o build/homology.o

all: $(obj)
	g++ $(obj) -o run

build/tests.o:
	g++ -c src/tests.cpp -o build/tests.o

build/homology.o: src/homology.cpp
	g++ -c src/homology.cpp -o build/homology.o

build/simplex.o: src/complex/simplex.cpp
	g++ -c src/complex/simplex.cpp -o build/simplex.o

build/filtered_complex.o: src/complex/filtered_complex.cpp
	g++ -c src/complex/filtered_complex.cpp -o build/filtered_complex.o

build/witness_complex.o: src/construction/witness_complex.cpp
	g++ -c src/construction/witness_complex.cpp -o build/witness_complex.o

clean:
	rm build/*.o