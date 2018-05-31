#==========================================================
# Macros

# CC specifies which compiler we're using
CC = g++

# CFLAGS specifies the additional compilation options we're using
CFLAGS = -Wall

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image

# SOURCE_DIR specifies the source directory of the files to compile
SOURCE_DIR = source

# BUILD_DIR specifies the build directory where the objects and
# the executable file will be compiled to
BUILD_DIR = build

# EXEC specifies the name of the compiled executable file
EXECUTABLE = processor

# MODEL_MODULES contains a list of the "modules" in the model source code
MODEL_MODULES = \
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
model \
register_file \
reorder_buffer \
store_buffer

# VIEW_MODULES contains a list of the "modules" in the view source code
VIEW_MODULES = \
view

# CONTROLLER_MODULES contains a list of the "modules" in the controller source code
CONTROLLER_MODULES = \
controller

# SOURCES contains a list of all of the .cpp source code files
# Add all of the model files to the SOURCES macro
SOURCES = $(addprefix $(SOURCE_DIR)/processor/model/, $(addsuffix .cpp, $(MODEL_MODULES)))
# Add all of the view files to the SOURCES macro
SOURCES += $(addprefix $(SOURCE_DIR)/processor/view/, $(addsuffix .cpp, $(VIEW_MODULES)))
# Add all of the controller files to the SOURCES macro
SOURCES += $(addprefix $(SOURCE_DIR)/processor/controller/, $(addsuffix .cpp, $(CONTROLLER_MODULES)))
# Add the main file to the SOURCES macro
SOURCES += $(SOURCE_DIR)/processor/main.cpp


# OBJECTS contains a list of the object files that need to created
# Add all of the model object files that we need to the OBJECTS macro
OBJECTS = $(addprefix $(BUILD_DIR)/objects/model/, $(addsuffix .o, $(MODEL_MODULES)))
# ADD all of the view object files that we need to the OBJECTS macro
OBJECTS += $(addprefix $(BUILD_DIR)/objects/view/, $(addsuffix .o, $(VIEW_MODULES)))
# Add all of the controller object files that we need to the OJBECTS macro
OBJECTS += $(addprefix $(BUILD_DIR)/objects/controller/, $(addsuffix .o, $(CONTROLLER_MODULES)))
# Add the main object to the OBJECTS macro
OBJECTS += $(BUILD_DIR)/objects/main.o

# PROGRAM_FILE species the program that our processor will run
PROGRAM_FILE = programs/kernels/vector_addition.mac


# ======================================================
# Rules

# Default rule
all : Build

# Build rule to compile the whole project
Build : $(OBJECTS) $(SOURCES)
	$(CC) $(CFLAGS) $(OBJECTS) $(LINKER_FLAGS) -o $(BUILD_DIR)/$(EXECUTABLE)

# $< is the first prerequisite (%.cpp)
# $@ is the target (%.o)

# rule for the main object file
$(BUILD_DIR)/objects/main.o : $(SOURCE_DIR)/processor/main.cpp
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/processor/main.cpp $(LINKER_FLAGS) -o $(BUILD_DIR)/objects/main.o

# pattern rule for the remaining objects files
$(BUILD_DIR)/objects/%.o : $(SOURCE_DIR)/processor/%.cpp $(SOURCE_DIR)/processor/%.h
	$(CC) $(CFLAGS) -c $< $(LINKER_FLAGS) -o $@

# rule for running the processor with the given assembly program
run: $(BUILD_DIR)/$(EXECUTABLE) $(PROGRAM_FILE)
	$(BUILD_DIR)/$(EXECUTABLE) $(PROGRAM_FILE)

# rule for compiling the assembler
assembler : $(SOURCE_DIR)/assembler/assembler.cpp
	$(CC) $(CFLAGS) $(SOURCE_DIR)/assembler/assembler.cpp -o $(BUILD_DIR)/assembler

# rule for deleting all of the object files and the executables in the build directory
# (trying to be specific on the files that are deleted for safety)
clean :
	rm build/objects/*.o build/objects/model/*.o build/objects/view/*.o build/processor build/assembler