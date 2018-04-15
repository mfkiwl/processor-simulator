#S_DIR specifies the source directory of the files to compile
S_DIR = source
#B_DIR specifies the build directory where the executable will be located
B_DIR = build

#OBJS specifies which files to compile as part of the project
S_FILES = processor.cpp
#OBJ_NAME specifies the name of our executable
EXEC = processor

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -Wall

#LINKER_FLAGS specifies the libraries we're liking against
LINKER_FLAGS = -lSDL2


all : $(S_DIR)/$(S_FILES)
	$(CC) $(S_DIR)/$(S_FILES) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(EXEC)

clean:
	rm -rf $(B_DIR)/*

#P_DIR specifes the directory where the programs are located
P_DIR = programs
#P_FILE species the program that our processor will run
P_FILE = kernels/vector_addition.mac

run: $(B_DIR)/$(EXEC) $(P_DIR)/$(P_FILE)
	$(B_DIR)/$(EXEC) $(P_DIR)/$(P_FILE)

