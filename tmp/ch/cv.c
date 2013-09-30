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
	return (((struct point *)lhs)->x > ((struct point *)rhs)->x);
}

int ccw(struct point p1, struct point p2, struct point p3)
{
	int ret = (p2.x-p1.x)*(p3.y-p1.y) - (p2.y-p1.y)*(p3.x-p1.x);
	printf("(%d,%d):ccw:%d\n", (p2.x-p1.x)*(p3.y-p1.y), (p2.y-p1.y)*(p3.x-p1.x), 0<ret);
	return ret;
}

struct hull_point {
	struct point * p;
	struct hull_point * next;
	int ccw;
};

int main()
{
	FILE * plotter = popen ("gnuplot -p", "w");	
	fprintf(plotter, "set autoscale; plot '-', '-'\n");
	srand(time(NULL));

	points = (point*) calloc(points_count, sizeof(struct point));
	struct hull_point * hull_first, * hull = (hull_point*) calloc(1, sizeof(hull_point));
	hull_first = hull;
	
	for(int i = 0; i < points_count; i++)
	{
		points[i].x = rand()%dimxy*10;
		points[i].y = rand()%dimxy*10;
	}
	
	qsort(points, points_count, sizeof(struct point), cmp_points);
	
	for(int i = 1, j=0; i < points_count; i++)
	{
		//printf("(%d,%d)-(%d,%d),(%d,%d):ccw:%d\n", points[i-1].x, points[i-1].y, points[i].x, points[i].y, points[i+1].x, points[i+1].y, !!ccw(points[i-1], points[i], points[i+1]));
		j=i;
		while (j+1 < points_count && ccw(points[i-1], points[j], points[j+1]) && j++)
		{
			hull->p = &(points[j+1]);
			hull->ccw = 1;
			hull->next = (hull_point*) calloc(1, sizeof(hull_point));
			hull = hull->next;
		}
	}
	struct hull_point * u;
	u = hull_first;
	while (u && u->p) {
		printf("hull:%d,%d\n", u->p->x, u->p->y);
		u = u->next;
	}
	
	fprintf(plotter, "%d %d w p 1 -1\ne\n", points[0].x, points[0].y);
	for(int i = 0; i < points_count; i++)
	{
		fprintf(plotter, "%d %d\n", points[i].x, points[i].y);
	}
	fprintf(plotter, "e\n");
	pclose(plotter);
	free(points);
	
}
