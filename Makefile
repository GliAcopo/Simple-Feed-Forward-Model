# Makefile

# Compiler and Flags
CC         = gcc
CFLAGS     = -Wall -Wextra -g

# Target Executable
TARGET     = main.a.out

# Source Files
SRC_MAIN   = main.c
SRC_MATRIX = matrix_functions.c
SRC_MODEL  = model_functions.c
SRC_NODE   = node_functions.c

# Header Files
HEADERS    = matrix_functions.h model_functions.h settings.h node_functions.h

# Object Files
OBJ_MATRIX = matrix_functions.o
OBJ_MODEL  = model_functions.o
OBJ_MAIN   = main.o
OBJ_NODE   = node_functions.o

# Default Target
all: $(TARGET)

# Link Object Files to Create Executable
$(TARGET): $(OBJ_MATRIX) $(OBJ_MODEL) $(OBJ_MAIN) $(OBJ_NODE)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ_MATRIX) $(OBJ_MODEL) $(OBJ_MAIN) $(OBJ_NODE) -lm

# Compile matrix_functions.c to matrix_functions.o
matrix_functions.o: $(SRC_MATRIX) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC_MATRIX)

# Compile model_functions.c to model_functions.o
model_functions.o: $(SRC_MODEL) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC_MODEL)
main.o: $(SRC_MAIN) $(HEADERS)

# Compile main.c to main.o
	$(CC) $(CFLAGS) -c $(SRC_MAIN)

# Compile main.c to main.o
node_functions.o: $(SRC_NODE) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC_NODE)

# Clean Build Artifacts
clean:
	rm -f $(OBJ_MATRIX) $(OBJ_MODEL) $(OBJ_MAIN) $(OBJ_NODE) $(TARGET)

# Phony Targets
.PHONY: all clean
