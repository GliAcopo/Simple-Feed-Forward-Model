all:
	gcc -o dynamic_matrix.a.out dynamic_matrix.c
	gcc -o model_functions.a.out model_functions.c
	gcc -o Feed_Forward_simple_model.a.out Feed_Forward_simple_model.c

clean:
	rm dynamic_matrix.a.out
	rm model_functions.a.out
	rm Feed_Forward_simple_model.a.out

matrix:
	gcc -o dynamic_matrix.a.out dynamic_matrix.c

model_functions:
	gcc -o model_functions.a.out model_functions.c

Feed_Forward_simple_model:
	gcc -o Feed_Forward_simple_model.a.out Feed_Forward_simple_model.c

