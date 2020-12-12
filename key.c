#define WIDTH 40
#define HEIGHT 30


#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>



typedef struct player{
	char ch;			/* player's character */
	int x;				/* player's cordinates */
}player;


void printMap();
void movePlayer();


/* player */
player p;

int main(){
	int c;
	int key;
	
	initscr();
	noecho();

	printMap();

	/* F1 to Exit */
	while((key = getch()) != KEY_F(1) ){

		movePlayer(key);
	}
	

	endwin();
	return 0;
}

void printMap()
{
	int i;
	p.ch = '+';
	p.x = WIDTH/2;

	curs_set(0);


	keypad(stdscr,TRUE);


	/* player */
	move(HEIGHT-1, p.x);
	addch(p.ch);
	

	/* bottom line */
	move(HEIGHT, 0);
	for (i = 0; i < WIDTH; i++)
		printw("=");
	
	move(HEIGHT + 9,0);
	printw("Input Option:");
	move(HEIGHT + 9,15);
	
	refresh();	


}

void movePlayer(int key){
	
	move(HEIGHT - 1, p.x);
	delch();

	switch(key){

		case KEY_LEFT:
			if(p.x - 1 > 0)
				p.x--;
			break;

		case KEY_RIGHT:
			if(p.x + 1 < WIDTH - 1  )
				p.x++;
			break;
			
	}
	
	

	move(HEIGHT - 1, p.x);
	addch(p.ch);

	refresh();

}
