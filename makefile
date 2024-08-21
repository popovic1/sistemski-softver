# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -g -O0 -Iinclude

# Directories
SRC_DIR = source
BIN_DIR = bin
INC_DIR = include
TEST_DIR = tests
O_DIR = obj

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files (temporarily stored in bin)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRC_FILES))

# Executable name
TARGET = $(BIN_DIR)/assembler

# Default target
all: $(TARGET) | $(O_DIR)

# Rule to link object files to create the executable
$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(OBJ_FILES) -o $@

# Rule to compile source files into object files (in the bin directory)
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Create o directory if it doesn't exist
$(O_DIR):
	mkdir -p $(O_DIR)

# Clean up build files
clean:
	rm -rf $(BIN_DIR) $(O_DIR)

# Phony targets
.PHONY: all clean

assemble:
	./bin/assembler -o $(O_DIR)/main.o $(TEST_DIR)/main.s
	./bin/assembler -o $(O_DIR)/math.o $(TEST_DIR)/math.s
	./bin/assembler -o $(O_DIR)/isr_software.o $(TEST_DIR)/isr_software.s
	./bin/assembler -o $(O_DIR)/isr_timer.o $(TEST_DIR)/isr_timer.s
	./bin/assembler -o $(O_DIR)/isr_terminal.o $(TEST_DIR)/isr_terminal.s
	./bin/assembler -o $(O_DIR)/handler.o $(TEST_DIR)/handler.s