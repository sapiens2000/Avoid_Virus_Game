#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
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
void drawVirus();
void *virusMaking(void*);
void addq(virus_t);
void deleteq();
void queueFull();
void copy(virus_t*, virus_t*, virus_t*);

int main() {
	pthread_t t1;
	int ch;
				
	srand(time(NULL));
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	MALLOC(queue, capacity * sizeof(*queue));
	pthread_create(&t1, NULL, virusMaking, NULL);
	
	while (1) {
		ch = getch();
		
		if (ch == KEY_F(1)) {
			exit_flag = 1;
			break;
		}
		
		erase();
		
		drawMap();
		drawPlayer(ch);
		refresh();
	}
	
	pthread_join(t1, NULL);
	
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

void drawVirus() {
	int i;
	
	if (rear != front) {
		for (i = front + 1; i <= rear; i++) {
			move(queue[i].v_y, queue[i].v_x);
			addstr(" ");
			queue[i].v_y++;
			move(queue[i].v_y, queue[i].v_x);
			addstr(queue[i].c);
			refresh();
		}
	}
}

void *virusMaking(void *none) {
	virus_t new;
	
	while (1) {
		if (exit_flag == 1) {
			free(queue);
			curs_set(1);
			endwin();
			exit(1);
		}

		strcpy(new.c, "#");
		new.v_x = (rand() % (WIDTH - 1)) + 1;
		new.v_y = 1;
		addq(new);
		drawVirus();
		sleep(rand() % 3);
	}
}


void addq(virus_t item) {
	rear = (rear + 1) % capacity;
	if (front == rear);
		queueFull();
	queue[rear] = item;
}

void deleteq() {
	if( front == rear) {
		fprintf(stderr, "deleteq error");
		exit(EXIT_FAILURE);
	}
		
	front = (front + 1) % capacity;
}

void queueFull() {
	int start;
	virus_t* newQueue;

	MALLOC(newQueue, 2 * capacity * sizeof(*queue));
	
	start = (front + 1) % capacity;
	rear--;
	
	if (start < 2)
		copy(queue + start, queue + start + capacity - 1, newQueue);
	else {
		copy(queue + start, queue + capacity, newQueue);
		copy(queue, queue + rear + 1, newQueue + capacity - start);
	}

	front = 2 * capacity - 1;
	rear = capacity - 1;
	capacity *= 2;
	
	free(queue);
	
	queue = newQueue;
}

void copy(virus_t* start, virus_t* end, virus_t* newQueue) {
	int i;
	
	
	for (i = 0; i < end - start; i++)
		newQueue[i] = start[i];
}
