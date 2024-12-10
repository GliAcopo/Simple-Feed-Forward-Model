# Makefile

# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target Executable
TARGET = Feed_Forward_simple_model.a.out

# Source Files
SRC_DYNAMIC = dynamic_matrix.c
SRC_MODEL = model_functions.c
SRC_FEED = Feed_Forward_simple_model.c

# Header Files
HEADERS = matrix.h model_functions.h

# Object Files
OBJ_DYNAMIC = dynamic_matrix.o
OBJ_MODEL = model_functions.o
OBJ_FEED = Feed_Forward_simple_model.o

# Default Target
all: $(TARGET)

# Link Object Files to Create Executable
$(TARGET): $(OBJ_DYNAMIC) $(OBJ_MODEL) $(OBJ_FEED)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ_DYNAMIC) $(OBJ_MODEL) $(OBJ_FEED)

# Compile dynamic_matrix.c to dynamic_matrix.o
dynamic_matrix.o: $(SRC_DYNAMIC) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC_DYNAMIC)

# Compile model_functions.c to model_functions.o
model_functions.o: $(SRC_MODEL) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC_MODEL)

# Compile Feed_Forward_simple_model.c to Feed_Forward_simple_model.o
Feed_Forward_simple_model.o: $(SRC_FEED) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC_FEED)

# Clean Build Artifacts
clean:
	rm -f $(OBJ_DYNAMIC) $(OBJ_MODEL) $(OBJ_FEED) $(TARGET)

# Phony Targets
.PHONY: all clean
