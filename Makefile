all:
	gcc -o dynamic_matrix.a.out dynamic_matrix.c
	gcc -o Feed_Forward_simple_model.a.out Feed_Forward_simple_model.c

clear:
	rm dynamic_matrix.a.out
	rm Feed_Forward_simple_model.a.out

matrix:
	gcc -o dynamic_matrix.a.out dynamic_matrix.c

