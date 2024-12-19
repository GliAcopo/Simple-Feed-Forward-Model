# Makefile

# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target Executable
TARGET = Feed_Forward_simple_model.a.out

# Source Files
SRC_MATRIX = matrix_functions.c
SRC_MODEL = model_functions.c
SRC_FEED = Feed_Forward_simple_model.c
SRC_NODES = node_functions.c

# Header Files
HEADERS = matrix.h model_functions.h settings.h node_functions.h

# Object Files
OBJ_MATRIX = matrix_functions.o
OBJ_MODEL = model_functions.o
OBJ_FEED = Feed_Forward_simple_model.o
OBJ_NODES = node_functions.o

# Default Target
all: $(TARGET)

# Link Object Files to Create Executable
$(TARGET): $(OBJ_MATRIX) $(OBJ_MODEL) $(OBJ_FEED) $(OBJ_NODES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ_MATRIX) $(OBJ_MODEL) $(OBJ_FEED) $(OBJ_NODES)

# Compile matrix_functions.c to matrix_functions.o
matrix_functions.o: $(SRC_MATRIX) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC_MATRIX)

# Compile model_functions.c to model_functions.o
model_functions.o: $(SRC_MODEL) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC_MODEL)

# Compile Feed_Forward_simple_model.c to Feed_Forward_simple_model.o
Feed_Forward_simple_model.o: $(SRC_FEED) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC_FEED)

# Compile Feed_Forward_simple_model.c to Feed_Forward_simple_model.o
node_functions.o: $(SRC_NODES) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC_NODES)

# Clean Build Artifacts
clean:
	rm -f $(OBJ_MATRIX) $(OBJ_MODEL) $(OBJ_FEED) $(OBJ_NODES) $(TARGET)

# Phony Targets
.PHONY: all clean
