CFLAGS=-g -Wall
export CFALGS
default:  compile

compile:
	g++  src/main.cpp -o  bin/main

run:
	./bin/main 
