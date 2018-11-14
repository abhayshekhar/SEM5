#include<stdio.h>
#include<graphics.h>
#include<math.h>
 void drawline(int,int,int,int);
 int max(int x,int y)
 {if(x>y)
 return x;
 else
 return y;}

 int main()
{int x1,x2,x3,y1,y2,y3;
  int gd=DETECT,gm;
  initgraph(&gd,&gm,"c:\\TC\\BGI");
printf("enter the value of x1 and y1");
scanf("%d %d",&x1,&y1);
printf("\nenter the value of x2,y2");
scanf("%d %d",&x2,&y2);
drawline(x1,y1,x2,y2);
closegraph();
getch();
return 0;
}
void drawline(int x1,int y1,int x2,int y2)
{int dx,dy,steps,x_inc,y_inc,x,y,i;
x=x1;y=y1;
dx=x2-x1;
dy=y2-y1;
steps=max(abs(dx),abs(dy));
x_inc=abs(dx/steps);
y_inc=abs(dy/steps);
for(i=0;i<steps;i++)
{x=x+x_inc;
y=y+y_inc;
putpixel(x,y,GREEN);
}

}











