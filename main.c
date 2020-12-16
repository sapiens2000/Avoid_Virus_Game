#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define WIDTH  30
#define HEIGHT 20
#define MALLOC(p, s) \
	if(!((p) = malloc(s))) {\
		fprintf(stderr, "Insufficient memory"); \
		exit(EXIT_FAILURE); \
	}

typedef struct _virus {
	char c[2];
	int v_x, v_y;
} virus_t;
virus_t* queue;

int capacity = 16;
int front = 0;
int rear = 0;
int p_x = WIDTH / 2;
int p_y = HEIGHT - 1 ;
int exit_flag = 0;

void drawMap();
void drawPlayer(int);
void drawVirus(int);
void virusMaking(int);
void addq(virus_t);
void deleteq();
void queueFull();
void copy(virus_t*, virus_t*, virus_t*);

int main() {
	int ch, cnt;
				
	srand(time(NULL));
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	MALLOC(queue, capacity * sizeof(*queue));

	while (1) {
		ch = getch();
		
		if (ch == KEY_F(1)) {
			exit_flag = 1;
			break;
		}
		
		erase();
		drawMap();
		drawPlayer(ch);
		virusMaking(cnt);
		drawVirus(cnt);
		refresh();
		cnt = (cnt + 1) % 5000;
	}
	
	curs_set(1);
	free(queue);
	endwin();
	return 0;
}

void drawPlayer(int ch) {	
	if (ch == KEY_LEFT) {
		if (p_x > 1) {
			p_x--;
		}
	}
	
	if (ch == KEY_RIGHT) {
		if (p_x < WIDTH - 1) {
			p_x++;
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
	
	if (rear != front) {
		for (i = front + 1; i <= rear; i++) {
			mvaddstr(queue[i].v_y, queue[i].v_x, queue[i].c);
			if (cnt == 0) {
				queue[i].v_y++;
				if(queue[i].v_y > HEIGHT - 1) {
					queue[i].v_x = (rand() % (WIDTH - 1)) + 1;
					queue[i].v_y = 1;
				}
			}
		}
	}
}

void virusMaking(int cnt) {
	virus_t new;
	
	if( cnt == 2500 && (abs(front - rear) < 15)) {
		strcpy(new.c, "#");
		new.v_x = (rand() % (WIDTH - 1)) + 1;
		new.v_y = 1;
		addq(new);
	}	
}


void addq(virus_t item) {
	rear = (rear + 1) % capacity;		
	queue[rear] = item;
}
