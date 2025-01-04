#ifndef GAME_H
#define GAME_H

typedef struct{
    int posX;
    int posY;
} playerPos;

typedef struct{
    int sosX;
    int sosY;
} snakePos;

extern playerPos player;
extern snakePos snake;
extern int** map;

void gameMap(int height, int width, int** map);
void gameMapDARK(int height, int width, int visibleRange, int** map);
int playerMovement(char direction, int** map, int height, int width);
int snakeMovement(int** map, int height, int width);
void runGame(const char *mapFileName);
void getPlayerPos(int** map, int height, int width);
void getSnakePos(int** map, int height, int width);
int normalRun(int height, int width, int** map);
int darkRun(int height, int width, int** map);
int manhattanDistance(int x1, int y1, int x2, int y2);
int won(void);

#endif