

#include <stdio.h>
#include <ncurses.h>

#define WIDTH  40
#define HEIGHT 30

void printMap();

int main(){
	int c;
	echo();

	printMap();
	
	/* User can input option */
	scanw("%c",&c);


	endwin();
	return 0;
}
/* display bottom and player */
void printMap()
{
	initscr();

	int i;
	/* player */
	move(HEIGHT-1, WIDTH/2);
		printw("+");

	/* bottom */
	move(HEIGHT, 0);
	for (i = 0; i < WIDTH; i++)
		printw("=");
	
	move(HEIGHT + 9,0);
	printw("Input Option:");
	move(HEIGHT + 9,15);

	refresh();	


}


