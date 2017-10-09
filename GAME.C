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
#define UPPER_X (((WIDTH-PADDING)/BOX_SIZE) - 2)
#define UPPER_Y (((HEIGHT-PADDING)/BOX_SIZE) - 2)
#define LOWER_XY 0

union REGS in,out;
int game_start = 0, processing = 0;
int game_board[100][100];
int neighbours[100][100];
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
	game_board[x][y] = (game_board[x][y] >= 1 ? 0 : 1);

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
	for(x = PADDING; x<WIDTH-PADDING;x+=BOX_SIZE){
		j = 0;
		for(y = PADDING;y<HEIGHT-PADDING;y+=BOX_SIZE){
			drawfillsquare(x+1, y+1, BOX_SIZE-2,  (game_board[i][j] >= 1 ? (game_board[i][j] == 1 ?  RED : YELLOW) : BLACK));
			j++;
		}
		i++;
	}
	return;
}


/* GAME LOGIC */

int get_neighbour(int x, int y) {
	int count = 0;
	int i,j,xi,yi;
	int upper_x = (WIDTH - PADDING)/BOX_SIZE;
	int lower_xy = 0;
	int upper_y = (HEIGHT - PADDING)/ BOX_SIZE;
	upper_x-=2;
	upper_y-=2;
	for(i = -1;i<=1;i++){
		for(j = -1;j<=1;j++){
			xi = (x+j+upper_x)%upper_x;
			yi = (y+i+upper_y)%upper_y;
			if(i == 0 && j == 0) continue;
			if(x+j <=(upper_x) && x+j >= lower_xy && y+i <= upper_y && (y+i) >= lower_xy && game_board[x+j][y+i] >= 1)
				count++;
		}
	}

	return count;
}


int produce(){
	// NEXT MATRIX
	// game_board[test++][test] = 1;

	int x;
	int y;
	int i = 0;
	int j = 0;

	for(i = LOWER_XY;i<=UPPER_X;i++){
		for(j = LOWER_XY;j<=UPPER_Y;j++){
			neighbours[i][j] = 0;
		}
	}

	for(i = LOWER_XY;i<=UPPER_X;i++){
		for(j = LOWER_XY;j<=UPPER_Y;j++){
			neighbours[i][j] = get_neighbour(i , j);
		}
	}
	i = LOWER_XY;
	j = LOWER_XY;

	for(x = PADDING; x<WIDTH-PADDING;x+=BOX_SIZE){
		j = LOWER_XY;
		for(y = PADDING;y<HEIGHT-PADDING;y+=BOX_SIZE){
			int neighbour = neighbours[i][j];

			if(game_board[i][j] >= 1 && neighbour < 2)
			game_board[i][j] = 0;
			else if(game_board[i][j] >= 1 && neighbour > 3)
			game_board[i][j] = 0;
			else if(game_board[i][j] == 0 && neighbour == 3)
			game_board[i][j] = 2;
			else if(game_board[i][j] == 2) game_board[i][j] = 1;

			j++;
		}
		i++;
	}

	// PUT THE ON THE BOARD
	draw_on_game_board();

}


/*  GAME LOOP */

int gameloop(){
	int x,y, cl;
	do {
		mouseposi(&x, &y, &cl);
		if(!game_start) {
			gotoxy(10,9);
			//cleardevice();
			setcolor((x+y)%10);
			line(x,y,x+100,y+200);
			printf("\n\tPress any key to start..");
			if(cl == 1) {
				game_start = 1;
				cleardevice();
				mousehide();
				draw_game_board(BOX_SIZE);
				callmouse();
			}
		} else {
			if(cl == 1 && !processing){
				fill_box(x, y);
				x /= BOX_SIZE;
				x -= 2;
				y /= BOX_SIZE;
				y -= 2;
				//gotoxy(10,9);
				//printf("\n\tClick: %d %d",x, y);
				game_board[x][y] = 1;
				// to prevent double click
				delay(120);
			} else if(cl == 2 && !processing){
					// start the game
					processing = 1;
			} else if(cl == 1){
				// pause the game
				processing = 0;
			} else if(processing){
				mousehide();
				produce();
				callmouse();
			}
		}

		//fps
		delay(1000/35);
	} while(!kbhit());
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

	getch();
	mousehide();
	closegraph();

	return 0;
}
