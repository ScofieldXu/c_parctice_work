#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

/*run in windows only*/
#include <windows.h>

#define MAZE_WIDTH 60
#define MAZE_HEIGHT 20
#define NOTMOVE 0;
#define LEFT -1;
#define RIGHT 1;
#define UP 2;
#define DOWN -2;

static char maze[100][100];

int x, y, p, q;

void clearMaze()
{
	for (int i = 0; i < MAZE_HEIGHT; i++)
	{
		for (int j = 0; j < MAZE_WIDTH; j++)
		{
			maze[i][j] = '#';
		}
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
			p = 0; q = _q; break;
		case 1:
			p = _p; q = MAZE_WIDTH - 1; break;
		case 2:
			p = MAZE_HEIGHT - 1; q = _q; break;
		case 3:
			p = _p; q = 0; break;
	}
	maze[p][q] = ' ';
}

void initFirstPosition()
{
	while (abs(x - p) < MAZE_HEIGHT / 2 || abs(y - q) < MAZE_WIDTH / 2)
	{
		x = (rand() % (MAZE_HEIGHT - 2)) + 1;
		y = (rand() % (MAZE_WIDTH - 2)) + 1;
	}	
	maze[x][y] = 'O';
}

int buildAmazingMaze()
{
	clearMaze();
	initMazeExit();
	initFirstPosition();

	int last_direct[3];
	last_direct[0] = NOTMOVE;
	last_direct[1] = NOTMOVE;
	last_direct[2] = NOTMOVE;
	int curr_direct = NOTMOVE;
	int _x, _y, step_count;
	int positive_left_right = NOTMOVE;
	int positive_up_down = NOTMOVE;
	int ret = -1;

	_x = x;
	_y = y;
	step_count = 0;

	if (x - p > 0)
		positive_up_down = 2;
	else
		positive_up_down = 3;

	if (y - q > 0)
		positive_left_right = 0;
	else
		positive_left_right = 1;

	while (1)
	{
		/*build successful*/
		if ((_x == p && _y + 1 == q) 
			|| (_x == p && _y - 1 == q) 
			|| (_x + 1 == p && _y == q) 
			|| (_x - 1 == p && _y == q))
		{
			ret = 0;
			break;
		}

		/*build fail*/
		if (step_count > (MAZE_HEIGHT - 1)*(MAZE_WIDTH - 1) / 4)
		{
			break;
		}

		int __x, __y;
		int rand_direct = rand() % 6;

		/*more chanse direct to exit*/
		if (rand_direct == 4)
			rand_direct = positive_up_down;
		else if (rand_direct == 5)
			rand_direct = positive_left_right;

		switch (rand_direct)
		{
			/*left one step*/
		case 0:
			curr_direct = LEFT; __x = _x; __y = _y - 1; break;
			/*right one step*/
		case 1:
			curr_direct = RIGHT; __x = _x; __y = _y + 1; break;
			/*up one step*/
		case 2:
			curr_direct = UP; __x = _x - 1; __y = _y; break;
			/*down one step*/
		case 3:
			curr_direct = DOWN; __x = _x + 1; __y = _y; break;
		}

		if (last_direct[2] + curr_direct == 0)	//don't turn around
			continue;
		else if (last_direct[0] + last_direct[1] + last_direct[2] + curr_direct == 0)	//don't turn around
			continue;
		else if (__x == 0 || __x == MAZE_HEIGHT - 1)	//up and down wall
			continue;
		else if (__y == 0 || __y == MAZE_WIDTH - 1)	//left and right wall
			continue;
			
		last_direct[0] = last_direct[1];
		last_direct[1] = last_direct[2];
		last_direct[2] = curr_direct;
		_x = __x;
		_y = __y;
		
		maze[_x][_y] = ' ';
	}

	spaceMaze();

	return ret;
}

int main()
{	
	srand((unsigned int)time(NULL));
	char ch;

	ch = 'y';

	while (ch == 'y')
	{
		while (buildAmazingMaze() < 0);
		system("cls");

		for (int i = 0; i <= MAZE_HEIGHT; i++)
			puts(maze[i]);

		while (x != p || y != q)
		{
			ch = _getch();
			switch (ch)
			{
			case 'w':
				if (maze[x - 1][y] != '#')
				{
					maze[x--][y] = ' ';
					maze[x][y] = 'O';
				}
				break;
			case 's':
				if (maze[x + 1][y] != '#')
				{
					maze[x++][y] = ' ';
					maze[x][y] = 'O';
				}
				break;
			case 'a':
				if (maze[x][y - 1] != '#')
				{
					maze[x][y--] = ' ';
					maze[x][y] = 'O';
				}
				break;
			case 'd':
				if (maze[x][y + 1] != '#')
				{
					maze[x][y++] = ' ';
					maze[x][y] = 'O';
				}
				break;
			default:
				break;
			}

			system("cls");
			for (int i = 0; i <= MAZE_HEIGHT; i++)
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
	
	printf("See you!\n");
	Sleep(3000);
	return EXIT_SUCCESS;
}