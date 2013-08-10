all: flatbit.c
	clang -x c -g flatbit.c -I. -o flatbit
clean:
	rm *.o *.gch flatbit db
