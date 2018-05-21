#==========================================================
# Macros

# CC specifies which compiler we're using
CC = g++

# CFLAGS specifies the additional compilation options we're using
CFLAGS = -Wall

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS =

# SOURCE_DIR specifies the source directory of the files to compile
SOURCE_DIR = source/model

# BUILD_DIR specifies the build directory where the objects and
# the executable file will be compiled to
BUILD_DIR = build

# EXEC specifies the name of the compiled executable file
EXECUTABLE = processor

# MODULES contains a list of the "modules" in the source code
MODULES = \
main \
alu \
alu_reservation_station \
branch_unit \
branch_unit_reservation_station \
decode_issue_unit \
fetch_unit \
instructions \
load_buffer \
load_store_unit \
load_store_unit_reservation_station \
memory \
processor \
register_file \
reorder_buffer \
store_buffer

# SOURCES contains a list of all of the .cpp source code files
SOURCES = $(addprefix $(SOURCE_DIR)/, $(addsuffix .cpp, $(MODULES)))

# OBJECTS contains a list of the object files that need to created
OBJECTS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(MODULES)))

# PROGRAM_FILE species the program that our processor will run
PROGRAM_FILE = programs/kernels/vector_addition.mac


# ======================================================
# Rules

all : Build

# Build rule to compile the whole project
Build : $(OBJECTS) $(SOURCES)
	$(CC) $(CFLAGS) $(LINKER_FLAGS) $(OBJECTS) -o $(BUILD_DIR)/$(EXECUTABLE)

# rule for the main object file
$(BUILD_DIR)/main.o : $(SOURCE_DIR)/main.cpp
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -c $(SOURCE_DIR)/main.cpp -o $(BUILD_DIR)/main.o

# using pattern rules for the remaining objects files
# $< is the first prerequisite
# $@ is the target
$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.cpp $(SOURCE_DIR)/%.h
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -c $< -o $@


# run the processor with the given assembly program
run: $(BUILD_DIR)/$(EXECUTABLE) $(PROGRAM_FILE)
	$(BUILD_DIR)/$(EXECUTABLE) $(PROGRAM_FILE)


# delete the executable and all of the object files
# (specific on the files that we are deleting for safety)
clean :
	rm build/*.o build/processor