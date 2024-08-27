# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -g -O0 -Iinclude

# Directories
SRC_DIR = source
INC_DIR = include
BIN_DIR = bin
O_DIR = obj
TEST_DIR = tests

# Assembler source files
ASSEMBLER_SRC_FILES = main.cpp Assembler.cpp LiteralPool.cpp Parser.cpp ReallocationTable.cpp Symbol.cpp Section.cpp
ASSEMBLER_OBJ_FILES = $(patsubst %.cpp,$(BIN_DIR)/%.o,$(ASSEMBLER_SRC_FILES))

# Linker source files
LINKER_SRC_FILES = linkerMain.cpp Linker.cpp LinkerSymbol.cpp LinkerSection.cpp LinkerReallocation.cpp
LINKER_OBJ_FILES = $(patsubst %.cpp,$(BIN_DIR)/%.o,$(LINKER_SRC_FILES))

# Emulator source files
EMULATOR_SRC_FILES = Emulator.cpp
EMULATOR_OBJ_FILES = $(patsubst %.cpp,$(BIN_DIR)/%.o,$(EMULATOR_SRC_FILES))

# Executable names
ASSEMBLER_TARGET = $(BIN_DIR)/assembler
LINKER_TARGET = $(BIN_DIR)/linker
EMULATOR_TARGET = $(BIN_DIR)/emulator.exe

# Default target
all: $(ASSEMBLER_TARGET) $(LINKER_TARGET) $(EMULATOR_TARGET) | $(O_DIR)

# Rule to link object files to create the assembler executable
$(ASSEMBLER_TARGET): $(ASSEMBLER_OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(ASSEMBLER_OBJ_FILES) -o $@

# Rule to link object files to create the linker executable
$(LINKER_TARGET): $(LINKER_OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(LINKER_OBJ_FILES) -o $@

# Rule to link object files to create the emulator executable
$(EMULATOR_TARGET): $(EMULATOR_OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(EMULATOR_OBJ_FILES) -o $@

# Rule to compile source files into object files for assembler
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile source files into object files for linker
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile source files into object files for emulator
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Create obj directory if it doesn't exist
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

link:
	./bin/linker -hex -place=my_code@0x40000000 -place=math@0xF0000000 -o bin/program.hex \
	$(O_DIR)/handler.o $(O_DIR)/math.o $(O_DIR)/main.o $(O_DIR)/isr_terminal.o $(O_DIR)/isr_timer.o $(O_DIR)/isr_software.o 

emulate:
	./bin/emulator.exe bin/program.hex