CFLAGS = -gcc -lpthread

sudoku.x: sudoku.c
	$(CFLAGS) sudoku.c -o sudoku.x

