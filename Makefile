# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Targets
all: dynamic_matrix.a.out model_functions.a.out Feed_Forward_simple_model.a.out

# Compile dynamic_matrix.c into an executable
dynamic_matrix.a.out: dynamic_matrix.c matrix.h
	$(CC) $(CFLAGS) -o dynamic_matrix.a.out dynamic_matrix.c

# Compile model_functions.c and link with dynamic_matrix.c
model_functions.a.out: model_functions.c matrix.h
	$(CC) $(CFLAGS) -o model_functions.a.out model_functions.c dynamic_matrix.c

# Compile Feed_Forward_simple_model.c and link with model_functions.c and dynamic_matrix.c
Feed_Forward_simple_model.a.out: Feed_Forward_simple_model.c matrix.h model_functions.h
	$(CC) $(CFLAGS) -o Feed_Forward_simple_model.a.out Feed_Forward_simple_model.c model_functions.c dynamic_matrix.c

# Clean build artifacts
clean:
	rm -f *.a.out

# Individual Targets (Optional)
matrix:
	$(CC) $(CFLAGS) -o dynamic_matrix.a.out dynamic_matrix.c

model_functions:
	$(CC) $(CFLAGS) -o model_functions.a.out model_functions.c dynamic_matrix.c

Feed_Forward_simple_model:
	$(CC) $(CFLAGS) -o Feed_Forward_simple_model.a.out Feed_Forward_simple_model.c model_functions.c dynamic_matrix.c

