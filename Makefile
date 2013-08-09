all: flatbit.c
	g++ flatbit.c -I. -o flatbit
clean:
	rm *.o *.gch flatbit db
