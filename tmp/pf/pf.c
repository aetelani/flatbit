#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/param.h>
#include <math.h>
#include <assert.h>


#define UNDEF INT_MIN
#define INF INT_MAX
#define VISITED 'X'
#define NOT_VISITED 'o'
#define HOME 'H'
#define TARGET '@'
#define NODE_EDGE_COUNT 8

static FILE * dbg;

struct point;
struct edge {
	struct point * e;
	int cost;
    int flag;
};

struct point {
	int x, y, z, flag;
	struct edge edges[NODE_EDGE_COUNT];
	struct point * parent;
};

int cmp_points(const void * lhs, const void * rhs)
{
    return ((struct point *)lhs)->x == ((struct point *)rhs)->x ?
			((struct point *)lhs)->y > ((struct point *)rhs)->y :
			((struct point *)lhs)->x > ((struct point *)rhs)->x ;
}

int plot(struct point * points, int points_count, struct point * path)
{
	FILE * plotter = popen ("gnuplot -p ", "w");
	fprintf(plotter, "set autoscale;\n");
	fprintf(plotter, "set pm3d map;\n");
//	fprintf(plotter, "splot '-' w p pt 6 ps 0.3 lc rgb variable, '-' w l lc rgb variable;\n"); //, '-' w p lc 0, '-' w lp pt 6 ps 3;\n");
	//fprintf(plotter, "plot '-' w labels tc rgb variable, '-' w l lc rgb variable;\n"); //, '-' w p lc 0, '-' w lp pt 6 ps 3;\n");
    fprintf(plotter, "plot '-' w labels tc rgb variable, '-' w vector;\n"); //, '-' w p lc 0, '-' w lp pt 6 ps 3;\n");

	for(int i = 0; i < points_count; i++)
	{
		char *color;
		switch(points[i].flag) {
			case VISITED: color = "0xfa0000"; break;
			default: color = "0x000000"; break;
		}
		fprintf(plotter, "%d %d %c %s\n", points[i].x, points[i].y, points[i].flag, color);
	}
	fprintf(plotter, "e\n");

    dbg = fopen("debug", "w");
	//fprintf(plotter, "%d %d %d\n", points[0].x, points[0].y, points[0].z);
    for (struct point * p = path, * pp = path->parent; !!pp; p = p->parent, pp = p->parent)
	//for(int i = 0; i < steps; i++)
	{
		fprintf(plotter, "%d %d %d %d\n",pp->x, pp->y, p->x-pp->x, p->y-pp->y);
        fprintf(dbg, "%d %d %d %d\n", pp->x, pp->y, p->x-pp->x, p->y-pp->y);
        
	}
    fclose(dbg);
    fprintf(plotter, "e\n");
    pclose(plotter);
    return 0;
}

int setNodeNeighbors(struct point ** arr, int i, int j, int dim) {
    int skip = 0;
	int rowLimit = dim-1, colLimit = dim-1, count = 0;
	for(int x = MAX(0, i-1); x <= MIN(i+1, rowLimit); x++) {
		for(int y = MAX(0, j-1); y <= MIN(j+1, colLimit); y++) {
			if(x != i || y != j) {
                for(int k = 0; k < NODE_EDGE_COUNT; k++) { // single direction connections
                    for(int l = 0; l < NODE_EDGE_COUNT; l++) {
                        if((*arr)[x*dim+y].edges[k].e == &(*arr)[i*dim+j]) {
                            skip = 1;
                        }
                    }
                }
                if (skip) { skip = 0; continue; }
                else {
                    (*arr)[i*dim+j].edges[count].e = &((*arr)[x*dim+y]);
                    (*arr)[i*dim+j].edges[count].cost = rand()%3;
                    (*arr)[i*dim+j].edges[count].flag = 0;
                    //fprintf(dbg,"%d,%d->%d.%d:%d. flag:%d\n", i, j,  (*arr)[i*dim+j].edges[count].e->x, (*arr)[i*dim+j].edges[count].e->y, (*arr)[i*dim+j].edges[count].cost, (*arr)[i*dim+j].edges[count].flag);
                    count++;
                }
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

int setupNeighbors(struct point * ps, int cnt, int dim)
{
    for(int i=0; i < cnt; i++) {
        for (int j=0; j < NODE_EDGE_COUNT; j++)
            ps[i].edges[j].flag = 1;
    }
	int frac, intp;
	for(int i=0; i < cnt; i++) {
		//frac = modf(i/dim, &intp);
		intp = (int) i/dim;
		frac = i - intp*dim;
		setNodeNeighbors(&ps, intp, frac, dim);
	}
}

struct point * cheapestNotVisitedNeighbor(struct point * ps)
{
	assert(!!ps);
	int cheapest = INF;
	struct point * cheapptr = NULL;
	for(int i=0; !!ps->edges[i].e && i < NODE_EDGE_COUNT; i++)
	{
		if ((ps->edges[i].e->flag == NOT_VISITED || ps->edges[i].e->flag == TARGET) && ps->edges[i].e->z > (ps->edges[i].cost + ps->z)) {
			ps->edges[i].e->z = (ps->edges[i].cost + ps->z);						
		}
		
		if ((ps->edges[i].e->flag == NOT_VISITED || ps->edges[i].e->flag == TARGET) && ps->edges[i].e->z < cheapest) {
			cheapest = ps->edges[i].e->z;
			cheapptr = ps->edges[i].e;
			cheapptr->parent = ps;			
		}
//		if(ps->edges[i].e->flag == TARGET) printf("Target on sight\n");
		//printf("errrr: ind:%d, %d == NOT_VISITED, cost %d\n", i, ps->edges[i].e->flag == NOT_VISITED, cheapest);		
	}
	//if (cheapptr) printf("cheapest:%d, %d : %d\n", cheapptr->x, cheapptr->y, cheapptr->z);
	return cheapptr;
}
int getInd(struct point * ps, struct point * p, int cnt)
{ 
	int i;
    for (i=0; &ps[i] != p; i++) {assert(i<=cnt);}
    return i;
}
struct point * rollbackToParent(struct point * p)
{
	struct point * ret = NULL;	
	ret = p->parent;
	p->parent = NULL;
	assert(ret);
	return ret;
}

int pf(struct point * ps, int cnt, int dim)
{
    int startIndex;
    for (startIndex=0; ps[startIndex].flag != HOME; startIndex++);
    setupNeighbors(ps,cnt, dim);
    
    struct point * pptr1, * parent, * pptr0 = &ps[startIndex];
    
    int safeSwitch = 10;
    for (;;)
    {
		pptr1 = cheapestNotVisitedNeighbor(pptr0);

        /*int cheapest = INT_MAX, cind = -1;
        for (int i = 0; i < cnt; i++)
        {
            if (ps[i].z < cheapest && ps[i].flag != HOME) { cheapest = ps[i].z; cind = i; }
        }
        pptr1 = &ps[cind];
        pptr1->parent = pptr0;*/
        
		pptr0->flag = VISITED;
		
		if (pptr1 != NULL) {			
			if(pptr1->flag == TARGET) {
				printf("TARGET FOUND\n");
				break;
			}
			//printf("iter, %d, %d, ", pptr0->x, pptr0->y); printf("-> %d, %d\n", pptr1->x, pptr1->y);		
			parent = pptr0;
			pptr0 = pptr1;
		} else {
			//rollback to parent node if no solution			
			//parent->flag = NOT_VISITED;
			//pptr0 = parent;
			
			//pptr0 = rollbackToParent(pptr0);
			printf("rollback to %d, %d\n", pptr0->x, pptr0->y);
			assert(pptr0->parent);
			
			/*int i;
			for (int i=0; i < NODE_EDGE_COUNT && pptr0->edges[i].e && pptr0->edges[i].e->flag == VISITED; i++); 
			if (i == NODE_EDGE_COUNT) { printf("all visited in node!\n" ); assert(0); }*/
		}
	}
    //int ind = getInd(ps,pptr0, INT_MAX);
    //printf("cheapest not visited neighbor ind:%d\n", ind);
return 0;
}


int main()
{
    static struct point * points, * beg, * end;
    const int dim = 10; // #x
    const int points_count = 10 * 10; // dim^2
    const int edge_count = 10 * 10 * NODE_EDGE_COUNT; // points_count * neighbor count
    
	int ret, path_points = 0;
	ret = matrix_points(&points, points_count, dim);
	
	int ncount = setNodeNeighbors(&points, 0, 0, dim);
	
	//qsort(points, points_count, sizeof(struct point), cmp_points);
	beg = &points[0];
	end = &points[points_count-1];
	beg->flag = HOME;
	beg->parent = 0;
	beg->z = 0;
	end->flag = TARGET;

    struct point * path = end;
	
	pf(points, points_count, dim);
	plot(points, points_count, path);
}
