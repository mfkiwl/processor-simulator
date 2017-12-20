PROGRAM=tests/memory

all:
	g++ src/processor.cpp -o bin/processor

run:
	./programs/assembler programs/$(PROGRAM).asm
	./bin/processor ./programs/$(PROGRAM).mac

assembler:
	g++ ./programs/assembler.cpp -o ./programs/assembler

clean:
	rm bin/*
