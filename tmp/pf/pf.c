#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/param.h>
#include <math.h>
#include <assert.h>


#define UNDEF INT_MIN
#define INF INT_MAX
#define VISITED 0xc3b8
#define NOT_VISITED 'o'
#define HOME 'H'
#define TARGET 'X'
#define NODE_EDGE_COUNT 8
struct point;
struct edge {
	struct point * e;
	int cost;
};

struct point {
	int x, y, z, flag;
	struct edge edges[NODE_EDGE_COUNT];
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
	fprintf(plotter, "plot '-' w labels tc rgb variable, '-' w l lc rgb variable;\n"); //, '-' w p lc 0, '-' w lp pt 6 ps 3;\n");

	for(int i = 0; i < points_count; i++)
	{
		fprintf(plotter, "%d %d %c 0xaf0000\n", points[i].x, points[i].y, points[i].flag);
	}
	fprintf(plotter, "e\n");
    
	fprintf(plotter, "%d %d %d\n", points[0].x, points[0].y, points[0].z);
	for(int i = 0; i < steps; i++)
	{
		fprintf(plotter, "%d %d %d 0xFF00FF\n", path[i].x, path[i].y, path[i].z);
	}
    fprintf(plotter, "e\n");
    pclose(plotter);
    return 0;
}

int setNodeNeighbors(struct point ** arr, int i, int j, int dim) {
	int rowLimit = dim-1, colLimit = dim-1, count = 0;
	for(int x = MAX(0, i-1); x <= MIN(i+1, rowLimit); x++) {
		for(int y = MAX(0, j-1); y <= MIN(j+1, colLimit); y++) {
			if(x != i || y != j) {
				(*arr)[i*dim+j].edges[count].e = &((*arr)[x*dim+y]);
				(*arr)[i*dim+j].edges[count].cost = rand()%3;
				count++;
			}
		}
	}
	return count;
}

int matrix_points(struct point ** pts, const int count, const int dim)
{
	srand(time(NULL));
	(*pts) = (struct point *)realloc(*pts, count * sizeof(struct point));
	int i,j, cnt=0;
	const int corner = 3, side = 5, normal = 8;
	for(i = 0, j = 0; i < count;) {
		for(j=0; j < dim; j++)  {
			(*pts)[i+j].x = i/dim;
			(*pts)[i+j].y = j;
			(*pts)[i+j].z = INF;
			(*pts)[i+j].flag = NOT_VISITED;
			cnt++;
//			printf("(%d,%d,%d)\n", (*pts)[i+j].x, (*pts)[i+j].y, (*pts)[i+j].z);
		}
		i += j;
	}
	return cnt;
}

struct point * points, * beg, * end;
static const int dim = 10; // #x
static const int points_count = 10 * 10; // dim^2
static int edge_count = 10 * 10 * NODE_EDGE_COUNT; // points_count * neighbor count

int setupNeighbors(struct point * ps, int cnt)
{
	int frac, intp;
	for(int i=0; i < cnt; i++) {
		//frac = modf(i/dim, &intp);
		intp = (int) i/dim;
		frac = i - intp*dim;
		setNodeNeighbors(&ps, intp, frac, dim);
	}
}

int cheapestNotVisitedNeighbor(struct point * ps)
{
	assert(!!ps);
	int cheapest = INF, cheapind = UNDEF;	
	for(int i=0; !!ps->edges[i].e && i < NODE_EDGE_COUNT; i++)
	{
		if (ps->edges[i].e->flag == NOT_VISITED && ps->edges[i].cost < cheapest) {
			cheapest = ps->edges[i].cost;
			cheapind = i;
		}
	}
	assert(cheapind!=INF);
	return cheapind;
}

int pf(struct point * ps, int cnt)
{
    int startIndex;
    for (startIndex=0; ps[startIndex].flag != HOME; startIndex++);
    setupNeighbors(ps,cnt);
    printf("cheapest not visited neighbor ind:%d\n", cheapestNotVisitedNeighbor(ps));
	
return 0;
}


int main()
{	
	int ret, path_points = 0;
	ret = matrix_points(&points, points_count, dim);
	
	int ncount = setNodeNeighbors(&points, 0, 0, dim);
	printf("number of neighbours:%d\n", ncount);
	
	struct point * path = (struct point *)calloc(points_count, sizeof(struct point));
	
	//qsort(points, points_count, sizeof(struct point), cmp_points);
	beg = &points[0];
	end = &points[points_count-1];
	path[path_points++] = *beg;
	path[path_points++] = *end;
	beg->flag = HOME;
	end->flag = TARGET;
	
	pf(points, points_count);
	plot(points, points_count, path, path_points);
}
