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

# MODULES contains a list of the "modules" in the source code
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
processor \
register_file \
reorder_buffer \
store_buffer

# SOURCES contains a list of all of the .cpp source code files
# Add all of the model files to the SOURCES macro
SOURCES = $(addprefix $(SOURCE_DIR)/model/, $(addsuffix .cpp, $(MODEL_MODULES)))
# Add all of the view files to the SOURCES macro
SOURCES += $(SOURCE_DIR)/view/view.cpp
# Add the main file to the SOURCES macro
SOURCES += $(SOURCE_DIR)/main.cpp


# OBJECTS contains a list of the object files that need to created
# Add all of the model object files that we need to the OBJECTS macro
OBJECTS = $(addprefix $(BUILD_DIR)/model/, $(addsuffix .o, $(MODEL_MODULES)))
# ADD all of the view object ilfes that we need to the OBJECTS macro
OBJECTS += $(BUILD_DIR)/view/view.o
# Add the main object to the OBJECTS macro
OBJECTS += $(BUILD_DIR)/main.o

# PROGRAM_FILE species the program that our processor will run
PROGRAM_FILE = programs/kernels/vector_addition.mac


# ======================================================
# Rules

all : Build

# Build rule to compile the whole project
Build : $(OBJECTS) $(SOURCES)
	$(CC) $(CFLAGS) $(OBJECTS) $(LINKER_FLAGS) -o $(BUILD_DIR)/$(EXECUTABLE)

# rule for the main object file
$(BUILD_DIR)/main.o : $(SOURCE_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/main.cpp $(LINKER_FLAGS) -o $(BUILD_DIR)/main.o

# using pattern rules for the remaining objects files
# $< is the first prerequisite
# $@ is the target
$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.cpp $(SOURCE_DIR)/%.h
	$(CC) $(CFLAGS) -c $< $(LINKER_FLAGS) -o $@


# run the processor with the given assembly program
run: $(BUILD_DIR)/$(EXECUTABLE) $(PROGRAM_FILE)
	$(BUILD_DIR)/$(EXECUTABLE) $(PROGRAM_FILE)


# delete the executable and all of the object files
# (specific on the files that we are deleting for safety)
clean :
	rm build/*.o build/model/*.o build/view/*.o build/processor