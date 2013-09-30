#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct point {
	int x, y;
};
struct point * upper_hull;
struct point * lower_hull;
struct point * points;
static const int points_count = 10;
static const int dimxy = 100;


int cmp_points(const void * lhs, const void * rhs)
{
	return (((struct point *)lhs)->y > ((struct point *)rhs)->y);
}

int ccw(struct point * p1, struct point * p2, struct point * p3)
{
	return (p2->x-p1->x)*(p3->y-p1->y) - (p2->y-p1->y)*(p3->x-p1->x);
}


int main()
{
	FILE * plotter = popen ("gnuplot -p", "w");	
	fprintf(plotter, "set autoscale; plot '-'\n");
	srand(time(NULL));

	points = (point*) calloc(points_count, sizeof(struct point));	
	
	for(int i = 0; i < points_count; i++)
	{
		points[i].x = rand()%dimxy;
		points[i].y = rand()%dimxy;
	}
	
	qsort(points, points_count, sizeof(struct point), cmp_points);
	
	for(int i = 0; i < points_count; i++)
	{
		fprintf(plotter, "%d %d\n", points[i].x, points[i].y);
	}
	fprintf(plotter, "e\n");
	pclose(plotter);
	free(points);
	
}
