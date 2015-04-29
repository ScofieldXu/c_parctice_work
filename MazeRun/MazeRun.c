#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

/*run in windows only*/
#include <windows.h>

#include "type.h"

/*the maze map*/
static char **maze;

/*location of exit*/
Position exitPos = { 0, 0 };

/*location of person*/
Position personPos = { 0, 0 };

/*command buf*/
char ch;

static void markByCoordinate(int const *x, int const *y, char ch)
{
	maze[*x][*y] = ch;
}

static void markByPosition(Position const *pos, char ch)
{
	markByCoordinate(&pos->x, &pos->y, ch);
}

void freeMaze()
{
	if (maze != NULL)
		free(maze);
}

void clearMaze()
{
	freeMaze();
	
	maze = malloc(MAZE_HEIGHT * sizeof(char*));

	for (int i = 0; i < MAZE_HEIGHT; i++)
	{
		char *line;
		line = malloc(MAZE_WIDTH * sizeof(char));
		memset(line, '#', MAZE_WIDTH);
		maze[i] = line;
	}
}

void spaceMaze()
{
	for (int i = 1; i < MAZE_HEIGHT - 1; i++)
	{
		for (int j = 1; j < MAZE_WIDTH - 1; j++)
		{
			if (maze[i][j] == '#')
			{
				int c = rand() % 3;
				if (c == 0) maze[i][j] = ' ';
			}
			else if (maze[i][j] == ' ')
			{
				if ((maze[i][j - 1] == ' ' || (j - 1 == 0))
					&& (maze[i][j + 1] == ' ' || (j + 1 == MAZE_WIDTH - 1))
					&& (maze[i - 1][j] == ' ' || (i - 1 == 0))
					&& (maze[i + 1][j] == ' ' || (i + 1 == MAZE_HEIGHT - 1))
					&& (maze[i - 1][j - 1] == ' ' || (i - 1 == 0) || (j - 1 == 0))
					&& (maze[i - 1][j + 1] == ' ' || (i - 1 == 0) || (j + 1 == MAZE_WIDTH - 1))
					&& (maze[i + 1][j - 1] == ' ' || (i + 1 == MAZE_HEIGHT - 1) || (j - 1 == 0))
					&& (maze[i + 1][j + 1] == ' ' || (i + 1 == MAZE_HEIGHT - 1) || (j + 1 == MAZE_WIDTH - 1)))
				{
					maze[i][j] = '#';
				}
			}
		}
	}
}

void initMazeExit()
{
	int s = rand() % 4;
	int _p = (rand() % (MAZE_HEIGHT - 2)) + 1;
	int _q = (rand() % (MAZE_WIDTH - 2)) + 1;

	switch (s)
	{
		case 0:
			exitPos.x = 0; exitPos.y = _q; break;
		case 1:
			exitPos.x = _p; exitPos.y = MAZE_WIDTH - 1; break;
		case 2:
			exitPos.x = MAZE_HEIGHT - 1; exitPos.y = _q; break;
		case 3:
			exitPos.x = _p; exitPos.y = 0; break;
	}

	markByPosition(&exitPos, ' ');
}

void initFirstPosition()
{
	do{
		personPos.x = (rand() % (MAZE_HEIGHT - 2)) + 1;
		personPos.y = (rand() % (MAZE_WIDTH - 2)) + 1;
	} while (abs(personPos.x - exitPos.x) < (MAZE_HEIGHT / 2 - 1) || abs(personPos.y - exitPos.y) < (MAZE_WIDTH / 2 - 1));

	markByPosition(&personPos, 'O');
}

int buildAmazingMaze()
{
	clearMaze();
	initMazeExit();
	initFirstPosition();

	int step_count = 0;
	int positive_left_right = NOTMOVE;
	int positive_up_down = NOTMOVE;
	Position runWayPos = { personPos.x, personPos.y };

	if (personPos.x - exitPos.x > 0)
		positive_up_down = 2;
	else
		positive_up_down = 3;

	if (personPos.y - exitPos.y > 0)
		positive_left_right = 0;
	else
		positive_left_right = 1;

	while ((runWayPos.x != exitPos.x || runWayPos.y + 1 != exitPos.y)
		&& (runWayPos.x != exitPos.x || runWayPos.y - 1 != exitPos.y)
		&& (runWayPos.x + 1 != exitPos.x || runWayPos.y != exitPos.y)
		&& (runWayPos.x - 1 != exitPos.x || runWayPos.y != exitPos.y))
	{
		step_count++;

		/*too many steps, build fail*/
		if (step_count > (MAZE_HEIGHT - 1)*(MAZE_WIDTH - 1) / 4)
			return -1;

		DirectPosition runWay = {
			{0, 0}, 0, {0, 0, 0}
		};

		/*more chanse direct to exit*/
		int rand_direct = rand() % 6;
		if (rand_direct == 4)
			rand_direct = positive_up_down;
		else if (rand_direct == 5)
			rand_direct = positive_left_right;

		switch (rand_direct){
			/*left one step*/
			case 0:
				runWay.curr_direct = LEFT; runWay.pos.x = runWayPos.x; runWay.pos.y = runWayPos.y - 1; break;
			/*right one step*/
			case 1:
				runWay.curr_direct = RIGHT; runWay.pos.x = runWayPos.x; runWay.pos.y = runWayPos.y + 1; break;
			/*up one step*/
			case 2:
				runWay.curr_direct = UP; runWay.pos.x = runWayPos.x - 1; runWay.pos.y = runWayPos.y; break;
			/*down one step*/
			case 3:
				runWay.curr_direct = DOWN; runWay.pos.x = runWayPos.x + 1; runWay.pos.y = runWayPos.y; break;
		}

		if (runWay.last_directs[2] + runWay.curr_direct == 0	//don't turn around
		 || runWay.last_directs[0] + runWay.last_directs[1] + runWay.last_directs[2] + runWay.curr_direct == 0	//don't turn around
		 || runWay.pos.x == 0 || runWay.pos.x == MAZE_HEIGHT - 1	//up and down wall
		 || runWay.pos.y == 0 || runWay.pos.y == MAZE_WIDTH - 1)	//left and right wall
			continue;
			
		runWay.last_directs[0] = runWay.last_directs[1];
		runWay.last_directs[1] = runWay.last_directs[2];
		runWay.last_directs[2] = runWay.curr_direct;
		
		runWayPos.x = runWay.pos.x;
		runWayPos.y = runWay.pos.y;

		markByPosition(&runWayPos, ' ');
	}

	spaceMaze();

	return 0;
}

int main()
{	
	srand((unsigned int)time(NULL));
	
	ch = 'y';

	while (ch == 'y')
	{
		while (buildAmazingMaze() < 0);
		system("cls");

		for (int i = 0; i < MAZE_HEIGHT; i++)
			puts(maze[i]);

		while (personPos.x != exitPos.x || personPos.y != exitPos.y)
		{
			ch = _getch();
			switch (ch)
			{
			case 'w':
				if (maze[personPos.x - 1][personPos.y] != '#')
				{
					maze[personPos.x--][personPos.y] = ' ';
					maze[personPos.x][personPos.y] = 'O';
				}
				break;
			case 's':
				if (maze[personPos.x + 1][personPos.y] != '#')
				{
					maze[personPos.x++][personPos.y] = ' ';
					maze[personPos.x][personPos.y] = 'O';
				}
				break;
			case 'a':
				if (maze[personPos.x][personPos.y - 1] != '#')
				{
					maze[personPos.x][personPos.y--] = ' ';
					maze[personPos.x][personPos.y] = 'O';
				}
				break;
			case 'd':
				if (maze[personPos.x][personPos.y + 1] != '#')
				{
					maze[personPos.x][personPos.y++] = ' ';
					maze[personPos.x][personPos.y] = 'O';
				}
				break;
			default:
				break;
			}

			system("cls");
			for (int i = 0; i < MAZE_HEIGHT; i++)
				puts(maze[i]);
		}

		system("cls");
		printf("You win!\n");

		int ret;
		do {
			printf("\nTry anther maze?(y/n): ");
			ret = scanf("%s", &ch);
			while (getchar() != '\n'); /* Clear the input buffer */
		} while (ret != 1);
	}

	freeMaze();
	printf("See you!\n");
	Sleep(3000);
	return EXIT_SUCCESS;
}