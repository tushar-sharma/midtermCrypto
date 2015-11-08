CFLAGS=-g -Wall
export CFALGS
default:  compile
h=h

compile:
	g++  -I $(h) src/main.cpp -o  bin/main
	g++  -I $(h) tests/squareMultiply.cpp -o  bin/squareMultiply
	g++  -I $(h) tests/moduloInverse.cpp -o  bin/moduloInverse
	g++  -I $(h) tests/gcd.cpp -o  bin/gcd


run:
	./bin/main 

mod:
	./bin/squareMultiply

inv:
	./bin/moduloInverse
gcd:
	./bin/gcd
