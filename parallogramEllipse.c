
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
int image[1000][1000];

void ellipse(int xc,int yc,int rx,int ry)
{
    int x, y, p;
    x=0;
    y=ry;
    p=(ry*ry)-(rx*rx*ry)+((rx*rx)/4);
    while((2*x*ry*ry)<(2*y*rx*rx))
    {
        image[xc+x][yc-y] = 255;
        image[xc-x][yc+y] = 255;
        image[xc+x][yc+y] = 255;
        image[xc-x][yc-y] = 255;

        if(p<0)
        {
            x=x+1;
            p=p+(2*ry*ry*x)+(ry*ry);
        }
        else
        {
            x=x+1;
            y=y-1;
            p=p+(2*ry*ry*x+ry*ry)-(2*rx*rx*y);
        }
    }
    p=((float)x+0.5)*((float)x+0.5)*ry*ry+(y-1)*(y-1)*rx*rx-rx*rx*ry*ry;

    while(y>=0)
    {
        image[xc+x][yc-y] = 255;
        image[xc-x][yc+y] = 255;
        image[xc+x][yc+y] = 255;
        image[xc-x][yc-y] = 255;

        if(p>0)
        {
            y=y-1;
            p=p-(2*rx*rx*y)+(rx*rx);
        }
        else
        {
            y=y-1;
            x=x+1;
            p=p+(2*ry*ry*x)-(2*rx*rx*y)-(rx*rx);
        }
    }

}

void Line (int x1, int y1, int x2, int y2)
{
    int dx, dy, steps,i,x,y;
    float x_inc, y_inc;

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

    for(i = 0; i <= steps ; i++)
    {
        image[x][y] =  255;
        x += x_inc;
        y += y_inc;
    }

}
int main ()
{
    int width = 1000, height = 1000;
    int x1 ,x2,x3,y1,y2,y3,x4,y4;
    int i=0,j=0,A, B,temp,xC, yC,Rx,Ry;
    FILE* img;
    img = fopen("parallelogramEllipse.pgm", "wb");
    fprintf(img, "P2\n");
    fprintf(img, "%d %d\n", width, height);
    fprintf(img, "255\n");

    printf("\nEnter the value of Length and Breadth : ");
    scanf("%d %d", &A, &B);

    printf("Enter the value for xC:\n");
    scanf("%d",&xC);
    printf("Enter the value for yC:\n");
    scanf("%d",&yC);
    printf("Enter the value for the Semi-Major axis (Rx):\n");
    scanf("%d",&Rx);
    printf("Enter the value for Semi-Minor axis (Ry):\n");
    scanf("%d",&Ry);



    for(i;i<1000;i++)
    {
        for(j;j<1000;j++)
            image[i][j] = 0;
    }


    x1 = xC - (A/2);

    y1 = yC - (B/2);
    x2 = xC + (A/2);

    y2 = y1;
    x3 = x2;
    y3 = yC + (B/2);
    x4= x1;
    y4 = y3;

    x1 = x1 + (2) * (y4 - (y1 + B / 2)); //sheer

    x2 = x2 + (2) * (y4 - (y2 + B / 2)); //sheer
    xC = xC + (A/4);

    Line(x1,y1,x2,y2);
    Line(x2,y2,x3,y3);
    Line(x1,y1,x4,y4);
    Line(x3,y3,x4,y4);

    ellipse(xC,yC,Ry,Rx);

    Line(x1,-y1,x2,-y2); //reflection (negation of y)
    Line(x2,-y2,x3,-y3); //reflection (negation of y)
    Line(x1,-y1,x4,-y4); //reflection (negation of y)
    Line(x3,-y3,x4,-y4); //reflection (negation of y)

    ellipse(xC,-yC,Ry,Rx); //reflection (negation of y)


    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            temp = image[j][i];
            fprintf(img, "%d ", temp);
        }
        fprintf(img, "\n");
    }
    fclose(img);
}


