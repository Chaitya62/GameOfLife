#include<conio.h>
#include<stdio.h>
#include<math.h>
#include<graphics.h>
#include<stdlib.h>
#include<dos.h>
#define HEIGHT 480
#define WIDTH 640
union REGS in,out;
int game_start = 0;

int callmouse(){
	in.x.ax=1;
	int86(51,&in,&out);
	return 1;
}
void mouseposi(int *xpos,int *ypos,int *click){
	in.x.ax=3;
	int86(51,&in,&out);
	*click=out.x.bx;
	*xpos=out.x.cx;
	*ypos=out.x.dx;
}
int mousehide()
{
  in.x.ax=2;
  int86(51,&in,&out);
  return 1;
}
void setposi(int *xpos,int *ypos)
{
   in.x.ax=4;
   in.x.cx=*xpos;
   in.x.dx=*ypos;
   int86(51,&in,&out);
}

int draw_matrix(int box_size){
  int x_lines,y_lines,i,padding;
  padding = 2*box_size;
  x_lines = WIDTH/box_size -  4;
  y_lines = HEIGHT/box_size - 4;
  for(i = padding; i<WIDTH-padding; i+=box_size){
    line(i, padding, i, HEIGHT-padding);
  }
  for(i = padding; i<HEIGHT-padding; i+=box_size){
    line(padding, i, WIDTH-padding, i);
  }
  return 0;
}

int gameloop(){
  int x,y, cl;
  do
   {
    mouseposi(&x, &y, &cl);
    if(!game_start){
    gotoxy(10,9);
    printf("\n\tClick: %d",cl);
    //cleardevice();
    setcolor((x+y)%10);
    line(x,y,x+100,y+200);
    printf("\n\tPress any key to hide the mouse");
    if(cl == 1){
      game_start = 1;
      cleardevice();
      mousehide();
      draw_matrix(4);
      callmouse();
      
    }

    }else{
      

    }
   }while(!kbhit());
   return 0;
}



int main(){

	char c;
	int a,b,gdriver = DETECT,cl, gmode;
	long t;
  clrscr();
	//scanf("%c");
	printf("Press Enter to start");
	initgraph(&gdriver, &gmode,"C:\\TURBOC3\\BGI");

	 a=100;
	 b=400;
   setposi(&a,&b);
   callmouse();
   if(gameloop()){
      cleardevice();
      gotoxy(10, 9);
      printf("Error occured");
      t = 1000000;
      while(t--);
      exit(1);
   }
	//take_user_input();

	getch();
	mousehide();
	closegraph();

	return 0;
}
