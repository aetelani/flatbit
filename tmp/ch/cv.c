#include <stdio.h>  
#include <stdlib.h>
#include <time.h>

struct point {
	int x, y;
};
struct point * upper_hull;
struct point * lower_hull;
struct point * points;
static const int points_count = 500;
static const int dimxy = 1000;

int plot(struct point * points, int points_count, struct point * hull, int hind);

int cmp_points(const void * lhs, const void * rhs)
{
    return ((struct point *)lhs)->x == ((struct point *)rhs)->x ?
			((struct point *)lhs)->y > ((struct point *)rhs)->y :
			((struct point *)lhs)->x > ((struct point *)rhs)->x ;
}

int ccw(struct point p1, struct point p2, struct point p3)
{
	int ret = (p2.x-p1.x)*(p3.y-p1.y) - (p2.y-p1.y)*(p3.x-p1.x);
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

     for(int i = points_count-2, lim = hind+1; i >= 0; --i)
     {
        while (hind >= lim && !ccw(hull[hind-2], hull[hind-1], points[i])) {hind--;}
        hull[hind++] = points[i];
    } 

    /*printf("hull:");
	for (int i=0; i < hind; i++) { printf("(%d,%d),", hull[i].x, hull[i].y); }
    printf("\n");*/
    
	int ret = plot(points, points_count, hull, hind);
	
	free(points);
	
}

int plot(struct point * points, int points_count, struct point * hull, int hind)
{
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
}
