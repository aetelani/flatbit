#include <stdio.h>  
#include <stdlib.h>
#include <time.h>

struct point {
	int x, y;
    int hull;
};
struct point * upper_hull;
struct point * lower_hull;
struct point * points;
static const int points_count = 50;
static const int dimxy = 100;


int cmp_points(const void * lhs, const void * rhs)
{
    if (((struct point *)lhs)->x == ((struct point *)rhs)->x)
        return ((struct point *)lhs)->y == ((struct point *)rhs)->y;
    else
        return (((struct point *)lhs)->x > ((struct point *)rhs)->x);
}

int ccw(struct point p1, struct point p2, struct point p3)
{
	int ret = (p2.x-p1.x)*(p3.y-p1.y) - (p2.y-p1.y)*(p3.x-p1.x);
	printf("(%d,%d):ccw:%d\n", (p2.x-p1.x)*(p3.y-p1.y), (p2.y-p1.y)*(p3.x-p1.x), 0<ret);
	return ret > 0;
}

int main()
{
	srand(time(NULL));

	points = (point*) calloc(points_count, sizeof(struct point));
	struct point * hull = (point*) calloc(points_count, sizeof(struct point));

	for(int i = 0; i < points_count; i++)
	{
		points[i].x = rand()%dimxy;
		points[i].y = rand()%dimxy;
	}

    qsort(points, points_count, sizeof(struct point), cmp_points);

    int hind = 0;
	for(int i=0; i < points_count; i++)
	{
		while (hind >= 2 && !ccw(hull[hind-2], hull[hind-1], points[i])) {hind--;}
        hull[hind++] = points[i];
	}

     for(int i = points_count-2; i >= 0; --i)
     {
        while (hind >= 2 && !ccw(hull[hind-2], hull[hind-1], points[i])) {hind--;}
        hull[hind++] = points[i];
    } 

    printf("hull:");
	for (int i=0; i < hind; i++)
    {
        printf("(%d,%d),", hull[i].x, hull[i].y);
	}
    printf("\n");


    FILE * plotter = popen ("gnuplot -p ", "w");	
	fprintf(plotter, "set autoscale; plot '-' w p pt 1 ps 2, '-' w p lc 0, '-' w lp pt 6 ps 3;\n");
	fprintf(plotter, "%d %d\ne\n", points[0].x, points[0].y);
	for(int i = 0; i < points_count; i++)
	{
		fprintf(plotter, "%d %d\n", points[i].x, points[i].y);
	}
	fprintf(plotter, "e\n");
    
	fprintf(plotter, "%d %d\n", points[0].x, points[0].y);
	for(int i = 0; i < hind; i++)
	{
		fprintf(plotter, "%d %d\n", hull[i].x, hull[i].y);
	}
    fprintf(plotter, "e\n");
    
	pclose(plotter);
	free(points);
	
}
