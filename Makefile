all: flatbit.c
	clang -x c flatbit.c -I. -o flatbit
clean:
	rm *.o *.gch flatbit db
