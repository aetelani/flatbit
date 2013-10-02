#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/param.h>

struct point;
struct edge {
	struct point * e;
	int cost;
};

struct point {
	int x, y, z;
	struct edge edges[8];
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
	fprintf(plotter, "set pm3d map;\n");
//	fprintf(plotter, "splot '-' w p pt 6 ps 0.3 lc rgb variable, '-' w l lc rgb variable;\n"); //, '-' w p lc 0, '-' w lp pt 6 ps 3;\n");
	fprintf(plotter, "splot '-' w labels tc rgb variable, '-' w l lc rgb variable;\n"); //, '-' w p lc 0, '-' w lp pt 6 ps 3;\n");

	for(int i = 0; i < points_count; i++)
	{
		fprintf(plotter, "%d %d %d %c 0xaf0a00\n", points[i].x, points[i].y, points[i].z, (points[i].z==INT_MAX?0xf8:points[i].z));
	}
	fprintf(plotter, "e\n");
    
	fprintf(plotter, "%d %d %d\n", points[0].x, points[0].y, points[0].z);
	for(int i = 0; i < steps; i++)
	{
		fprintf(plotter, "%d %d %d 0xFF00FF\n", path[i].x, path[i].y, path[i].z);
	}
    fprintf(plotter, "e\n");
    pclose(plotter);
}

int iterNeighbors(struct point * arr, int i, int j, int dim) {
	int rowLimit = dim-1, colLimit = dim-1, count = 0;
	for(int x = MAX(0, i-1); x <= MIN(i+1, rowLimit); x++) {
		for(int y = MAX(0, j-1); y <= MIN(j+1, colLimit); y++) {
			if(x != i && y != j) {
				printf("(%d,%d - %d)~%d\n",x, y, x*dim+y, arr[x*dim+y].z);
				arr[x*dim+y].edges[count].e = &arr[x*dim+y];
				arr[x*dim+y].edges[count].cost = rand()%3;
				count++;
			}
		}
	}
	return count;
}

int matrix_points(struct point ** pts, const int count, const int dim)
{
	srand(time(NULL));
	*pts = (struct point *)calloc(count, sizeof(struct point));
	int i,j, edge_count;
	const int corner = 3, side = 5, normal = 8;
	for(i = 0, j = 0, edge_count = 0; i < count;) {
		for(j=0; j < dim; j++)  {
			(*pts)[i+j].x = i/dim;
			(*pts)[i+j].y = j;
			(*pts)[i+j].z = INT_MAX;
			printf("(%d,%d,%d)\n", (*pts)[i+j].x, (*pts)[i+j].y, (*pts)[i+j].z);
		}
		i += j;
	}
	return 0;
}

struct point * points, * beg, * end;
static const int dim = 10; // #x
static const int points_count = 10 * 10; // dim^2
static int edge_count = 10 * 10 * 8; // points_count * neighbor count

int pf(int a, int b)
{

return 0;
}


int main()
{	
	int ret, path_points = 0;
	ret = matrix_points(&points, points_count, dim);
	
	int ncount = iterNeighbors(points, dim-1, 1, dim);
	printf("number of neighbours:%d\n", ncount);
	
	struct point * path = (struct point *)calloc(points_count, sizeof(struct point));
	
	//qsort(points, points_count, sizeof(struct point), cmp_points);
	beg = &points[0];
	end = &points[points_count-1];
	path[path_points++] = *beg;
	path[path_points++] = *end;
	
	pf(1,1);
	plot(points, points_count, path, path_points);
}
