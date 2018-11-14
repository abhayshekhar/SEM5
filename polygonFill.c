
#include<stdio.h>
#include<math.h>
#include<dos.h>
#include<stdlib.h>

int image[1000][1000];
typedef struct dcPt {
    int x;
    int y;
} ;
typedef struct tEdge {
    int yUpper;
    float xIntersect, dxPerScan;
    struct tEdge *next;
} Edge;

void insertEdge (Edge * list, Edge * edge) {
    Edge * p, *q = list;

    p = q->next;
    while(p!= NULL) {
        if(edge->xIntersect< p->xIntersect)
            p = NULL;
        else {
            q = p;
            p = p ->next;
        }
    }
    edge->next = q->next;
    q->next = edge;
}
int yNext (int k, int cnt, struct dcPt * pts) {
    int j;

    if((k + 1) > (cnt - 1))
        j = 0;
    else
        j = k + 1;
    while (pts[k].y == pts[j].y)
        if ((j + 1) > (cnt - 1))
        j = 0;
    else
        j++;
    return (pts[j].y);
}

void makeEdgeRec (struct dcPt lower, struct dcPt upper, int yComp, Edge * edge, Edge * edges[]) {
    edge->dxPerScan = (float) (upper.x - lower.x) / (upper.y - lower.y);
    edge->xIntersect = lower.x;
    if(upper.y < yComp)
        edge->yUpper = upper.y - 1;
    else
        edge->yUpper = upper.y;
    insertEdge(edges[lower.y],edge);
}

void buildEdgeList (int cnt, struct dcPt * pts, Edge * edges[])
{
    Edge * edge;
    struct dcPt v1, v2;
    int i, yPrev = pts[cnt - 2].y;

    v1.x = pts[cnt - 1].x; v1.y = pts[cnt - 1].y;
    for (i = 0;i < cnt; i++) {
        v2 = pts[i];
        if(v1.y != v2.y) {
            edge = (Edge *) malloc(sizeof(Edge));
            if(v1.y < v2.y)
                makeEdgeRec(v1, v2, yNext (i, cnt, pts),edge,edges);
            else
                makeEdgeRec(v2, v1, yPrev, edge, edges);
        }
        yPrev = v1.y;
        v1 = v2;
    }
}
void buildActiveList (int scan,Edge * active, Edge * edges[])
{
    Edge * p, *q;
    p = edges[scan]->next;
    while(p) {
        q = p ->next;
        insertEdge(active, p);
        p = q;
    }

}
void fillScan (int scan, Edge * active) {
    Edge * p1, *p2;
    int i;

    p1 = active->next;
    while(p1) {
        p2 = p1 -> next;
        for(i = p1 ->xIntersect; i<p2->xIntersect; i++)
            //setPixel ((int) i, scan);
            image[(int) i][scan] = 255;
        p1 = p2 ->next;
    }
}
void deleteAfter(Edge * q) {
    Edge * p = q->next;
    q -> next = p ->next;
    free(p);
}

void updateActiveList (int scan, Edge * active)
{
    Edge * q = active, * p = active -> next;

    while (p)
    if (scan >= p->yUpper) {
        p = p->next;
        deleteAfter(q);
    }
    else{
        p->xIntersect = p->xIntersect + p->dxPerScan;
        q = p;
        p = p->next;
    }
}

void resortActiveList (Edge * active)
{
    Edge * q, * p = active->next;
    active->next = NULL;
    while(p) {
        q = p->next;
        insertEdge (active, p);
        p = q;
    }
}

void scanFill (int cnt, struct dcPt * pts)
{
    Edge * edges[1000], * active;
    int i, scan;
    for(i=0;i<1000; i++){
        edges[i] = (Edge *) malloc (sizeof (Edge));
        edges[i]->next = NULL;
    }
    buildEdgeList(cnt,pts,edges);
    active = (Edge *) malloc (sizeof(Edge));
    active -> next = NULL;
    for(scan = 0;scan < 1000; scan ++) {
        buildActiveList(scan, active, edges);
        if(active->next) {
            fillScan(scan, active);
            updateActiveList(scan,active);
            resortActiveList(active);
        }
    }
}
void drawLine (int x1, int y1, int x2, int y2) {
	int dx, dy, steps,i;
	float x_inc, y_inc, x, y;
	dx = x2 - x1;
	dy =y2 - y1;
	if (abs(dx) >abs(dy))
	steps =abs(dx);
	else
	steps = abs(dy);
	x_inc = dx/(float) steps;
	y_inc =dy/(float) steps;
	x = x1;
	y = y1;
	for(i = 0; i <= steps ;i++){
    image[(int)x][(int)y] = 200;
	x = x + x_inc;
	y = y + y_inc;
       //	delay(50);
	}

}
void main () {
    int height = 1000, width = 1000, temp,i,j,xc,yc,r,x1,y1;
	for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++)
            image[i][j] = 0;
    }
    FILE* pgmimg;
    pgmimg = fopen("polygonFill.pgm", "wb");
    // Writing Magic Number to the File
    fprintf(pgmimg, "P2\n");
    // Writing Width and Height
    fprintf(pgmimg, "%d %d\n", width, height);
    // Writing the maximum gray value
    fprintf(pgmimg, "255\n");
    printf("Enter the co-ordinates of the center:\n");
    scanf("%d %d",&xc,&yc);
    printf("Enter the radius:\n");
    scanf("%d",&r);
    struct dcPt c1[6];
    c1[0].x = xc + r;
    c1[0].y = yc;
    x1 = xc + r;
    y1 = yc;
    int t = 60;
    float tP = t * ((3.14)/180);
    for(i = 1;i < 6;i++){
         c1[i].x = xc + (x1 - xc) * cos(tP) - (y1 - yc) * sin(tP);
         c1[i].y = yc + (x1 - xc) * sin(tP) + (y1 - yc) * cos(tP);
        t+=60;
        tP = t * ((3.14)/180);
    }
    scanFill(6,c1);
    drawLine(c1[0].x,c1[0].y,c1[1].x,c1[1].y);
    drawLine(c1[1].x,c1[1].y,c1[2].x,c1[2].y);
    drawLine(c1[2].x,c1[2].y,c1[3].x,c1[3].y);
    drawLine(c1[3].x,c1[3].y,c1[4].x,c1[4].y);
    drawLine(c1[4].x,c1[4].y,c1[5].x,c1[5].y);
    drawLine(c1[5].x,c1[5].y,c1[0].x,c1[0].y);
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            temp = image[j][i];

            // Writing the gray values in the 2D array to the file
            fprintf(pgmimg, "%d ", temp);
        }
        fprintf(pgmimg, "\n");
    }
    fclose(pgmimg);
}
