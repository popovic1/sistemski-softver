# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -g -O0 -Iinclude

# Directories
SRC_DIR = source
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include
O_DIR = o

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Executable name
TARGET = $(BIN_DIR)/assembler

# Default target
all: $(TARGET)

# Rule to link object files to create the executable
$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(OBJ_FILES) -o $@

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean

assemble:
	./bin/assembler -o o/main.o tests/main.s
	./bin/assembler -o o/math.o tests/math.s
	./bin/assembler -o o/isr_software.o tests/isr_software.s
	./bin/assembler -o o/isr_timer.o tests/isr_timer.s
	./bin/assembler -o o/isr_terminal.o tests/isr_terminal.s
	./bin/assembler -o o/handler.o tests/handler.s
