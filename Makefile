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
OBJ1 = main
OBJ2 = alu
OBJ3 = alu_reservation_station
OBJ4 = branch_unit
OBJ5 = branch_unit_reservation_station
OBJ6 = decode_issue_unit
OBJ7 = fetch_unit
OBJ8 = instructions
OBJ9 = load_buffer
OBJ10 = load_store_unit
OBJ11 = load_store_unit_reservation_station
OBJ12 = memory
OBJ13 = processor
OBJ14 = register_file
OBJ15 = reorder_buffer
OBJ16 = store_buffer

#variable containing all of the object files we need
OBJS = $(B_DIR)/$(OBJ1).o $(B_DIR)/$(OBJ2).o $(B_DIR)/$(OBJ3).o $(B_DIR)/$(OBJ4).o $(B_DIR)/$(OBJ5).o \
$(B_DIR)/$(OBJ6).o $(B_DIR)/$(OBJ7).o $(B_DIR)/$(OBJ8).o $(B_DIR)/$(OBJ9).o $(B_DIR)/$(OBJ10).o \
$(B_DIR)/$(OBJ11).o $(B_DIR)/$(OBJ12).o $(B_DIR)/$(OBJ13).o $(B_DIR)/$(OBJ14).o $(B_DIR)/$(OBJ15).o \
$(B_DIR)/$(OBJ16).o


#Build rule to compile the whole project
Build : $(OBJS)
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(EXEC) $(OBJS)

#Rules to compile all of the objects
$(B_DIR)/$(OBJ1).o : $(S_DIR)/$(OBJ1).cpp
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ1).o -c $(S_DIR)/$(OBJ1).cpp

$(B_DIR)/$(OBJ2).o : $(S_DIR)/$(OBJ2).cpp $(S_DIR)/$(OBJ2).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ2).o -c $(S_DIR)/$(OBJ2).cpp

$(B_DIR)/$(OBJ3).o : $(S_DIR)/$(OBJ3).cpp $(S_DIR)/$(OBJ3).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ3).o -c $(S_DIR)/$(OBJ3).cpp

$(B_DIR)/$(OBJ4).o : $(S_DIR)/$(OBJ4).cpp $(S_DIR)/$(OBJ4).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ4).o -c $(S_DIR)/$(OBJ4).cpp

$(B_DIR)/$(OBJ5).o : $(S_DIR)/$(OBJ5).cpp $(S_DIR)/$(OBJ5).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ5).o -c $(S_DIR)/$(OBJ5).cpp

$(B_DIR)/$(OBJ6).o : $(S_DIR)/$(OBJ6).cpp $(S_DIR)/$(OBJ6).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ6).o -c $(S_DIR)/$(OBJ6).cpp

$(B_DIR)/$(OBJ7).o : $(S_DIR)/$(OBJ7).cpp $(S_DIR)/$(OBJ7).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ7).o -c $(S_DIR)/$(OBJ7).cpp

$(B_DIR)/$(OBJ8).o : $(S_DIR)/$(OBJ8).cpp $(S_DIR)/$(OBJ8).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ8).o -c $(S_DIR)/$(OBJ8).cpp

$(B_DIR)/$(OBJ9).o : $(S_DIR)/$(OBJ9).cpp $(S_DIR)/$(OBJ9).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ9).o -c $(S_DIR)/$(OBJ9).cpp

$(B_DIR)/$(OBJ10).o : $(S_DIR)/$(OBJ10).cpp $(S_DIR)/$(OBJ10).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ10).o -c $(S_DIR)/$(OBJ10).cpp

$(B_DIR)/$(OBJ11).o : $(S_DIR)/$(OBJ11).cpp $(S_DIR)/$(OBJ11).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ11).o -c $(S_DIR)/$(OBJ11).cpp

$(B_DIR)/$(OBJ12).o : $(S_DIR)/$(OBJ12).cpp $(S_DIR)/$(OBJ12).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ12).o -c $(S_DIR)/$(OBJ12).cpp

$(B_DIR)/$(OBJ13).o : $(S_DIR)/$(OBJ13).cpp $(S_DIR)/$(OBJ13).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ13).o -c $(S_DIR)/$(OBJ13).cpp

$(B_DIR)/$(OBJ14).o : $(S_DIR)/$(OBJ14).cpp $(S_DIR)/$(OBJ14).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ14).o -c $(S_DIR)/$(OBJ14).cpp

$(B_DIR)/$(OBJ15).o : $(S_DIR)/$(OBJ15).cpp $(S_DIR)/$(OBJ15).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ15).o -c $(S_DIR)/$(OBJ15).cpp

$(B_DIR)/$(OBJ16).o : $(S_DIR)/$(OBJ16).cpp $(S_DIR)/$(OBJ16).h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(B_DIR)/$(OBJ16).o -c $(S_DIR)/$(OBJ16).cpp



#P_DIR specifes the directory where the programs are located
P_DIR = programs
#P_FILE species the program that our processor will run
P_FILE = kernels/vector_addition.mac

run: $(B_DIR)/$(EXEC) $(P_DIR)/$(P_FILE)
	$(B_DIR)/$(EXEC) $(P_DIR)/$(P_FILE)

clean :
	rm -rf $(B_DIR)/*