#include<conio.h>
#include<stdio.h>
#include<math.h>
#include<graphics.h>
#include<stdlib.h>
#include<dos.h>
#define HEIGHT 480
#define WIDTH 640
#define BOX_SIZE 10
#define PADDING 2*BOX_SIZE

union REGS in,out;
int game_start = 0;
short game_board[1000][1000];

/* MOUSE FUNCTIONS */

int callmouse(){
	in.x.ax=1;
	int86(51,&in,&out);
	return 1;
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

void mouseposi(int *xpos,int *ypos,int *click){
  in.x.ax=3;
  int86(51,&in,&out);
  *click=out.x.bx;
  *xpos=out.x.cx;
  *ypos=out.x.dx;
}



/* DRAW AND FILL FUNCTIONS */

int drawfillsquare(int x, int y, int size,int color){
    int i;
    setcolor(color);
    for(i = y;i<y+size;i++){
	line(x, i, x+size, i);
    }
    return 0;
}

int fill_box(int mousex, int mousey){
  int x = (mousex/BOX_SIZE)*BOX_SIZE;
  int y = (mousey/BOX_SIZE)*BOX_SIZE;

  // if outside boundary
  if(x >= WIDTH-PADDING || x < PADDING || y >= HEIGHT-PADDING || y < PADDING ) {
      return -1;
  }
  game_board[x][y] = !game_board[x][y];
  mousehide();
  //setfillstyle(SOLID_FILL, RED);
  drawfillsquare(x+1, y+1, BOX_SIZE-2, RED);
  //rectangle(x, y, x + BOX_SIZE, y + BOX_SIZE);
  //floodfill(x+BOX_SIZE/2, y + BOX_SIZE/2, RED);
  callmouse();
  return 0;
}


int draw_game_board(int box_size){
  int x_lines,y_lines,i;
  x_lines = WIDTH/box_size -  4;
  y_lines = HEIGHT/box_size - 4;
  setcolor(WHITE);
  for(i = PADDING; i<=WIDTH-PADDING; i+=box_size){
    line(i, PADDING, i, HEIGHT-PADDING);
  }
  for(i = PADDING; i<=HEIGHT-PADDING; i+=box_size){
    line(PADDING, i, WIDTH-PADDING, i);
  }
  return 0;
}



/*  GAME LOOP */

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
      draw_game_board(BOX_SIZE);
      callmouse();

    }

    }else{
       if(cl == 1)  fill_box(x, y);
    }

    delay(60);
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
