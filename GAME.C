#include<conio.h>
#include<stdio.h>
#include<math.h>
#include<graphics.h>
#include<stdlib.h>
#include<dos.h>
#define HEIGHT 480
#define WIDTH 640
#define BOX_SIZE 20
#define PADDING 2*BOX_SIZE

union REGS in,out;
int game_start = 0, processing = 0;
int game_board[100][100];
int test = 0;

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
  gotoxy(10, 9);
  printf("%d",&game_board[x][y] );
  mousehide();
  drawfillsquare(x+1, y+1, BOX_SIZE-2, (game_board[x][y]  ? RED : BLACK));
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

int draw_on_game_board(){
  int i = 0,j = 0,x,y;
  for(x = PADDING; x<=WIDTH-PADDING;x+=BOX_SIZE){

    for(y = PADDING;y<=HEIGHT-PADDING;y+=BOX_SIZE){
      drawfillsquare(x+1, y+1, BOX_SIZE-2,  (game_board[i][j]  ? RED : BLACK));
      j++;
    }
    i++;
  }
  return;
}


/* GAME LOGIC */

int produce(){
  // NEXT MATRIX
  game_board[test++][test] = 1;

  // PUT THE ON THE BOARD
  draw_on_game_board();

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
	 if(cl == 1 && !processing){
		fill_box(x, y);
		// to prevent double click
		delay(120);
	 }else if(cl == 2 && !processing){
		  // start the game
		  processing = 1;
	 }else if(cl == 1){
	    // pause the game
	    processing = 0;
	 }else if(processing){
      mousehide();
	    produce();
      callmouse();
	 }
    }

    //fps
    delay(1000/60);
   }while(!kbhit());
   return 0;
}

/* DEFAULTS */
void initDefaults(){
	int i,j;
	for(i = 0;i<100;i++){
		for(j = 0;j<100;j++){
		  game_board[i][j] = 0;
		}
	}
	return;
}

int main(){

	char c;
	int a,b,gdriver = DETECT,cl, gmode;
	long t;
	clrscr();
	//scanf("%c");
	printf("Press Enter to start");
	initDefaults();
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
