#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct point {
	int x, y, z;
};

int cmp_points(const void * lhs, const void * rhs)
{
    return ((struct point *)lhs)->x == ((struct point *)rhs)->x ?
			((struct point *)lhs)->y > ((struct point *)rhs)->y :
			((struct point *)lhs)->x > ((struct point *)rhs)->x ;
}

int plot(struct point * points, int points_count, struct point * path, int steps)
{
	FILE * plotter = popen ("gnuplot -p ", "w");
	fprintf(plotter, "set autoscale;\n");
	fprintf(plotter, "plot '-' w p pt 6 ps 1, '-' w l lc 0;\n"); //, '-' w p lc 0, '-' w lp pt 6 ps 3;\n");

	//fprintf(plotter, "%d %d\ne\n", points[0].x, points[0].y);
	for(int i = 0; i < points_count; i++)
	{
		fprintf(plotter, "%d %d\n", points[i].x, points[i].y);
	}
	fprintf(plotter, "e\n");
    
	fprintf(plotter, "%d %d\n", points[0].x, points[0].y);
	for(int i = 0; i < steps; i++)
	{
		fprintf(plotter, "%d %d\n", path[i].x, path[i].y);
	}
    fprintf(plotter, "e\n");
    pclose(plotter);
}

int matrix_points(struct point ** pts, const int count, const int dim)
{
	srand(time(NULL));
	*pts = (struct point *)calloc(count, sizeof(struct point));
	int i,j;
	for(i = 0, j = 0; i < count;)
	{
		for(j=0;j<dim;j++) 
		{
			(*pts)[i+j].x = i/dim;
			(*pts)[i+j].y = j;
			(*pts)[i+j].z = rand()%3;
			//printf("(%d,%d,%d)\n", (*pts)[j].x, (*pts)[j].y, (*pts)[j].z);
		}
		i += j;
	}
	return 0;
}

struct point * points, * beg, * end;
static const int dim = 10;
static const int points_count = 10 * 10;

int pf(int a, int b)
{

return 0;
}


int main()
{	
	int ret, path_points = 0;
	ret = matrix_points(&points, points_count, dim);
	struct point * path = (struct point *)calloc(points_count, sizeof(struct point));
	
	//qsort(points, points_count, sizeof(struct point), cmp_points);
	beg = &points[0];
	end = &points[points_count-1];
	path[path_points++] = *beg;
	path[path_points++] = *end;
	
	pf(1,1);
	plot(points, points_count, path, path_points);
}
