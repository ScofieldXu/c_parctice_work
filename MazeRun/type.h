
#define MAZE_WIDTH 60
#define MAZE_HEIGHT 20
#define NOTMOVE 0;
#define LEFT -1;
#define RIGHT 1;
#define UP 2;
#define DOWN -2;

/*position struct*/
typedef struct Position{
	int x;
	int y;
} Position;

/*direct position struct*/
typedef struct DirectPosition{
	Position pos;
	int curr_direct;
	int last_directs[3];
} DirectPosition;