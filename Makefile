#S_DIR specifies the source directory of the files to compile
S_DIR = source/model
#B_DIR specifies the build directory where the executable will be located
B_DIR = build

#OBJ_NAME specifies the name of our executable
EXEC = processor

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
CFLAGSS = -Wall

#LINKER_FLAGS specifies the libraries we're liking against
LINKER_FLAGS = -lSDL2 -lSDL2_image

#OBJs specify the objects to created for each module
OBJ1 = $(B_DIR)/processor.o
OBJ2 = $(B_DIR)/alu.o
OBJ3 = $(B_DIR)/reorder_buffer.o
OBJ4 = $(B_DIR)/register_file.o

#Build rule to compile the whole project
Build : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4)
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(EXEC) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4)

$(OBJ1) : $(S_DIR)/processor.cpp $(S_DIR)/processor.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ1) -c $(S_DIR)/processor.cpp

$(OBJ2) : $(S_DIR)/alu.cpp $(S_DIR)/alu.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ2) -c $(S_DIR)/alu.cpp

$(OBJ3) : $(S_DIR)/reorder_buffer.cpp $(S_DIR)/reorder_buffer.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ3) -c $(S_DIR)/reorder_buffer.cpp

$(OBJ4) : $(S_DIR)/register_file.cpp $(S_DIR)/register_file.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ4) -c $(S_DIR)/register_file.cpp


#P_DIR specifes the directory where the programs are located
P_DIR = programs
#P_FILE species the program that our processor will run
P_FILE = kernels/vector_addition.mac

run: $(B_DIR)/$(EXEC) $(P_DIR)/$(P_FILE)
	$(B_DIR)/$(EXEC) $(P_DIR)/$(P_FILE)

clean :
	rm -rf $(B_DIR)/*