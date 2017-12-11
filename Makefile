PROGRAM=vector_addition

all:
	g++ src/processor.cpp -o bin/processor

run:
	./programs/assembler programs/tests/$(PROGRAM).ass
	./bin/processor ./programs/tests/$(PROGRAM).mac

assembler:
	g++ ./programs/assembler.cpp -o ./programs/assembler

clean:
	rm bin/*
