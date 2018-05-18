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
OBJ1 = $(B_DIR)/main.o
OBJ2 = $(B_DIR)/processor.o
OBJ3 = $(B_DIR)/alu.o
OBJ4 = $(B_DIR)/reorder_buffer.o
OBJ5 = $(B_DIR)/register_file.o
OBJ6 = $(B_DIR)/memory.o
OBJ7 = $(B_DIR)/instructions.o
OBJ8 = $(B_DIR)/alu_reservation_station.o
OBJ9 = $(B_DIR)/branch_unit.o
OBJ10 = $(B_DIR)/branch_unit_reservation_station.o
OBJ11 = $(B_DIR)/decode_issue_unit.o

#Build rule to compile the whole project
Build : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(OBJ8) $(OBJ9) $(OBJ10) $(OBJ11)
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(EXEC) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(OBJ8) $(OBJ9) $(OBJ10) $(OBJ11)

$(OBJ1) : $(S_DIR)/main.cpp
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ1) -c $(S_DIR)/main.cpp

$(OBJ2) : $(S_DIR)/processor.cpp $(S_DIR)/processor.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ2) -c $(S_DIR)/processor.cpp

$(OBJ3) : $(S_DIR)/alu.cpp $(S_DIR)/alu.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ3) -c $(S_DIR)/alu.cpp

$(OBJ4) : $(S_DIR)/reorder_buffer.cpp $(S_DIR)/reorder_buffer.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ4) -c $(S_DIR)/reorder_buffer.cpp

$(OBJ5) : $(S_DIR)/register_file.cpp $(S_DIR)/register_file.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ5) -c $(S_DIR)/register_file.cpp

$(OBJ6) : $(S_DIR)/memory.cpp $(S_DIR)/memory.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ6) -c $(S_DIR)/memory.cpp

$(OBJ7) : $(S_DIR)/instructions.cpp $(S_DIR)/instructions.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ7) -c $(S_DIR)/instructions.cpp

$(OBJ8) : $(S_DIR)/alu_reservation_station.cpp $(S_DIR)/alu_reservation_station.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ8) -c $(S_DIR)/alu_reservation_station.cpp

$(OBJ9) : $(S_DIR)/branch_unit.cpp $(S_DIR)/branch_unit.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ9) -c $(S_DIR)/branch_unit.cpp

$(OBJ10) : $(S_DIR)/branch_unit_reservation_station.cpp $(S_DIR)/branch_unit_reservation_station.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ10) -c $(S_DIR)/branch_unit_reservation_station.cpp

$(OBJ11) : $(S_DIR)/decode_issue_unit.cpp $(S_DIR)/decode_issue_unit.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(OBJ11) -c $(S_DIR)/decode_issue_unit.cpp


#P_DIR specifes the directory where the programs are located
P_DIR = programs
#P_FILE species the program that our processor will run
P_FILE = kernels/vector_addition.mac

run: $(B_DIR)/$(EXEC) $(P_DIR)/$(P_FILE)
	$(B_DIR)/$(EXEC) $(P_DIR)/$(P_FILE)

clean :
	rm -rf $(B_DIR)/*