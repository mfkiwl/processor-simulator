PROGRAM=kernels/vector_addition

all:
	g++ src/processor.cpp -o bin/processor

run:
	./programs/assembler programs/$(PROGRAM).ass
	./bin/processor ./programs/$(PROGRAM).mac

assembler:
	g++ ./programs/assembler.cpp -o ./programs/assembler

clean:
	rm bin/*
