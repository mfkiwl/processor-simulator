#S_DIR specifies the source directory of the files to compile
S_DIR = source
#B_DIR specifies the build directory where the executable will be located
B_DIR = build

#OBJS specifies which files to compile as part of the project
S_FILES = $(S_DIR)/model/processor.cpp $(S_DIR)/model/alu.cpp $(S_DIR)/model/reorder_buffer.cpp
#OBJ_NAME specifies the name of our executable
EXEC = processor

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -Wall

#LINKER_FLAGS specifies the libraries we're liking against
LINKER_FLAGS = -lSDL2 -lSDL2_image


all : Build

Build: $(S_FILES)
	$(CC) $(S_FILES) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(EXEC)

clean:
	rm -rf $(B_DIR)/*

#P_DIR specifes the directory where the programs are located
P_DIR = programs
#P_FILE species the program that our processor will run
P_FILE = kernels/vector_addition.mac

run: $(B_DIR)/$(EXEC) $(P_DIR)/$(P_FILE)
	$(B_DIR)/$(EXEC) $(P_DIR)/$(P_FILE)

