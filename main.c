#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define WIDTH	30
#define HEIGHT	20
#define MAX	50

typedef struct _virus {
	char c[2];
	int v_x, v_y;
} virus_t;
virus_t virus_set[MAX];

int rear = 0;			// number of current virus
int p_x = WIDTH / 2;		// player x position
int p_y = HEIGHT - 1 ;		// player y position
int exit_flag = 0;		// check for exit

void drawMap();
void drawPlayer(int);
void drawVirus(int);
void virusMaking(int);

int main() {
	int ch;	// for user input
	int cnt;	// for adjust speed for drawVirus() and virusMaking()
				
	srand(time(NULL));
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	while (1) {
		ch = getch();
		
		if (ch == KEY_F(1) || exit_flag == 1)
			break;
		
		erase();
		drawMap();
		drawPlayer(ch);
		virusMaking(cnt);
		drawVirus(cnt);
		refresh();
		cnt = (cnt + 1) % 5000;
	}
	
	curs_set(1);
	endwin();
	return 0;
}

void drawPlayer(int ch) {
	int p_speed = 1;

	if (ch == KEY_LEFT || ch == KEY_SLEFT) {
		if (ch == KEY_SLEFT)
			p_speed = 3;
		
		p_x -= p_speed;
		
		if (p_x < 1) {
			p_x = 1;
		}
	}
	
	if (ch == KEY_RIGHT || ch == KEY_SRIGHT) {
		if(ch == KEY_SRIGHT)
			p_speed = 3;
	
		p_x += p_speed;
		
		if (p_x > WIDTH - 1) {
			p_x = WIDTH - 1;
		}
	}
	
	move(p_y, p_x);
	printw("0");
}

void drawMap() {
	int i;
	
	for (i = 0; i <= WIDTH; i++) {
		move(0, i);
		printw("=");		
		move(HEIGHT, i);
		printw("=");
	}
	
	for (i = 1; i < HEIGHT; i++) {
		move(i, 0);
		printw("|");
		move(i, WIDTH);
		printw("|");
	}
}

void drawVirus(int cnt) {
	int i;
	
	if (rear == 0) return;
	
	for (i = 1; i <= rear; i++) {
		mvaddstr(virus_set[i].v_y, virus_set[i].v_x, virus_set[i].c);	// draw virus
		
		if (cnt == 0) {
			virus_set[i].v_y++;	// increase virus y value
			
			// game-over condition
			if (p_x == virus_set[i].v_x && p_y == virus_set[i].v_y) {
				exit_flag = 1;
			}
			
			// virus reset condition
			if (virus_set[i].v_y > HEIGHT - 1) {
				virus_set[i].v_x = (rand() % (WIDTH - 1)) + 1;
				virus_set[i].v_y = 1;
			}
		}
	}
	
	if(exit_flag) {
		move(10, 10);
		printw("Game Over");
		nodelay(stdscr, FALSE);
	}
}

void virusMaking(int cnt) {
	if( cnt == 2500 && rear < 30) {
		rear++;
		strcpy(virus_set[rear].c, "#");
		virus_set[rear].v_x = (rand() % (WIDTH - 1)) + 1;
		virus_set[rear].v_y = 1;
	}	
}
