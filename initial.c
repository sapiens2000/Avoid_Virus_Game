

#include <stdio.h>
#include <curses.h>

#define WIDTH  20
#define HEIGHT 30

void printMap();

int main(){



	printMap();









	return 0;
}

void printMap()
{
	initscr();

	int i;

	move(0, HEIGHT + 1);
	for (i = 0; i < WIDTH; i++)
		printf("▤");
}


