#include<conio.h>
#include<stdio.h>
#include<math.h>
#include<graphics.h>
#include<stdlib.h>
#define HEIGHT 480
#define WIDTH 640


int take_care_of_user_input(int );


int main(){

	char a;
	int gdriver = DETECT, gmode;
	clrscr();
	//scanf("%c");
	printf("Press Enter to start");
	initgraph(&gdriver, &gmode,"C:\\TURBOC3\\BGI");

	line(10, 200, 400, 200);

	take_user_input();

	getch();
	closegraph();

	return 0;
}
