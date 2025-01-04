#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "newSleep.h"
#include "terminal.h"
#include "random.h"
#include "LinkedList.h"

playerPos player;
snakePos snake;
LinkedList* undo;

void runGame(const char *mapFileName) {
    int height, width, i, j;
    int** map;

    /*Declare the Linked List*/
    undo = (LinkedList*)malloc(sizeof(LinkedList));
    undo->head = NULL;
    undo->tail = NULL;

    /*Open the file*/
    FILE* ptr;
    ptr = fopen(mapFileName, "r");

    if(ptr == NULL) {
        printf("File does not exist.");
    }

    /*get the height and width from the first two digits in the map.txt*/
    fscanf(ptr, "%d %d", &height, &width);
    map = malloc(height * sizeof(int*));

    /*Put the map into a 2D array*/
    for(i = 0; i < height; i++) {
        map[i] = malloc(width * sizeof(int));
        for(j = 0; j < width; j++) {
            fscanf(ptr, "%d", &map[i][j]);
        }
    }

    fclose(ptr);

    /*Find the players and snakes player*/
    getPlayerPos(map, height, width);
    getSnakePos(map, height, width);

    int result;
    result = 0;

    /*Checks the user has given the DARK keyword*/
    #ifdef DARKMODE
        result = darkRun(height, width, map); /*DARK mode*/
    #else
        result = normalRun(height, width, map);/*Normal mode*/
    #endif

    /*Result 1 is given when the user gets to the teasure*/
    if(result == 1) {
        printf("YOU WON!!!");
    }

    /*Result 2 is given when the user gets eaten by the snake*/
    else if(result == 2) {
        printf("YOU LOSE!!!");
    }

    /*Free all alocated memory.*/
    
    freeMallocs(undo); /*A method in LinkedList.c to free mallocs*/

    free(undo);
    
    for(i = 0; i < height; i++) {
        free(map[i]);
    }
    free(map);
}


/*Compiles if the user has given the DARK keyword*/
#ifdef DARKMODE
/*Get the Manhattan Distance*/
int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

/* DARK mode */
int darkRun(int height, int width, int** map) {

    int visibleRange = 3;
    int result = 0, snakeResult = 0;
    char ch;
    positions positions; /*struct storing the player and snake positions in the linked list*/
    int i, j, lanternX = 0, lanternY = 0, foundLantern = 0;

    /*Find the position of the lantern*/
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 20; j++) {
            if(map[i][j] == 2) {
                lanternX = j; 
                lanternY = i;
            }
        }
    }
    
    /*inserts the the current player position and snake position to the linked list*/
    insertNode(undo, player.posX, player.posY, snake.sosX, snake.sosY);

    /*Main loop*/

    /*clears the terminal*/
    system("clear"); 

    /*runs the loop until the player gets to the treasure or gets eaten by the snake*/
    while(result != 1 && snakeResult == 0) {

        system("tput cup 0 0");
        gameMapDARK(height, width, visibleRange, map); /*prints out the DARK mode map*/

        /*takes in the user input and stores it in 'ch'*/
        disableBuffer();
        scanf(" %c", &ch);
        enableBuffer();


        if (ch == 'w'||ch == 'a'||ch == 's'||ch == 'd') {
            snakeResult = snakeMovement(map, height, width); /*random snake movement*/
            result = playerMovement(ch, map, height, width); /*takes in the value of ch and moves the player accordingly*/   
        }
        
        /*UNDO*/
        else if(ch == 'u') {

            positions = getPrev(undo); /*get the previous positions and stores them in the positions struct*/

            if (undo->tail->Prev != NULL) {
                getPrev(undo); /*undo*/

                map[player.posY][player.posX] = 0; /*delets the players current postion*/
                map[positions.posY][positions.posX] = 3; /*put the player in the previous position*/
                
                map[snake.sosY][snake.sosX] = 0; /*delets the snakes current postion*/
                map[positions.sosY][positions.sosX] = 4; /*put the snake in the previous position*/

                /*assigns the previous positions to current positions*/
                player.posX = positions.posX;
                player.posY = positions.posY;

                snake.sosX = positions.sosX;
                snake.sosY = positions.sosY;
            }

            /*if the player has picked up the lantern the undo will return the lantern and rest the visibilty*/
            if(foundLantern == 1) {
                map[lanternY][lanternX] = 2;
                visibleRange = 3;
                foundLantern = 0;
            }
            if(lanternX == player.posX && lanternY == player.posY) {
                foundLantern = 1;
            }
        }

        /*if the player picks up the lantern, the visibility increases to 6*/
        if(result == 2) {
            visibleRange = 6;
        } 

        /*if the player is within 1 block of the snake the snake will eat the player*/
        if(player.posY <= snake.sosY+1 && player.posY >= snake.sosY-1 && player.posX <= snake.sosX+1 && player.posX >= snake.sosX-1) {
            snakeResult = 2;
            result = 2;
            map[player.posY][player.posX] = 4;
            map[snake.sosY][snake.sosX] = 0;
            system("clear");
        }

        /*inserts the the current player position and snake position to the linked list*/
        insertNode(undo, player.posX, player.posY, snake.sosX, snake.sosY);

        system("tput cup 0 0");
        gameMapDARK(height, width, visibleRange, map); /*prints out the DARK mode map*/

        newSleep(0.01);
    }

    return result;
}


#else
int normalRun(int height, int width, int** map) {
    int result = 0, snakeResult = 0;
    char ch;
    positions positions; /*struct storing the player and snake positions in the linked list*/
    int i, j, lanternX = 0, lanternY = 0, foundLantern = 0;

    /*find the lanterns position*/
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 20; j++) {
            if(map[i][j] == 2) {
                lanternX = j; 
                lanternY = i;
            }
        }
    }

    /*inserts the the current player position and snake position to the linked list*/
    insertNode(undo, player.posX, player.posY, snake.sosX, snake.sosY);

    /*Main Loop*/

    system("clear"); /*clears the terminal*/

    /*runs the loop until the player gets to the treasure or gets eaten by the snake*/
    while(result != 1 && snakeResult == 0) {

        system("tput cup 0 0");
        gameMap(height, width, map);/*prints out the map*/

        /*takes in the user input and stores it in 'ch'*/
        disableBuffer();
        scanf(" %c", &ch);
        enableBuffer();

        if(ch == 'w'||ch == 'a'||ch == 's'||ch == 'd') {
            snakeResult = snakeMovement(map, height, width);
            result = playerMovement(ch, map, height, width);   
        }

        /*UNDO*/
        else if(ch == 'u') {
            positions = getPrev(undo); /*get the previous positions and stores them in the positions struct*/

            if(undo->tail->Prev != NULL) {
                getPrev(undo);
                map[player.posY][player.posX] = 0; /*delets the players current postion*/
                map[positions.posY][positions.posX] = 3; /*put the player in the previous position*/

                map[snake.sosY][snake.sosX] = 0; /*delets the snakes current postion*/
                map[positions.sosY][positions.sosX] = 4; /*put the snake in the previous position*/

                /*assigns the previous positions to current positions*/
                player.posX = positions.posX;
                player.posY = positions.posY;

                snake.sosX = positions.sosX;
                snake.sosY = positions.sosY;
            }

            /*if the player has picked up the lantern the undo will return the lantern and rest the visibilty*/
            if(foundLantern == 1) {
                map[lanternY][lanternX] = 2;
                foundLantern = 0;
            }
            if(lanternX == player.posX && lanternY == player.posY) {
                foundLantern = 1;
            }
        }
        
        /*if the player is within 1 block of the snake the snake will eat the player*/
        if(player.posY <= snake.sosY+1 && player.posY >= snake.sosY-1 && player.posX <= snake.sosX+1 && player.posX >= snake.sosX-1) {
            snakeResult = 2;
            result = 2;
            map[player.posY][player.posX] = 4;
            map[snake.sosY][snake.sosX] = 0;
            system("clear");
        }

        /*inserts the the current player position and snake position to the linked list*/
        insertNode(undo, player.posX, player.posY, snake.sosX, snake.sosY);

        system("tput cup 0 0");
        gameMap(height, width, map); /*prints out the map*/

        newSleep(0.01);
    }

    return result;
}
#endif

/*Find the players position.*/
void getPlayerPos(int** map, int height, int width) {
    int i, j;
    player.posX = 0; 
    player.posY = 0;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if(map[i][j] == 3) {
                player.posX = j;
                player.posY = i;
            }
        }
    }
}

/*Find the snakes position.*/
void getSnakePos(int** map, int height, int width) {
    int i, j;
    snake.sosX = 0; 
    snake.sosY = 0;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if(map[i][j] == 4) {
                snake.sosX = j; 
                snake.sosY = i;
            }
        }
    }
}

/*Compiles if the user has given the DARK keyword*/
#ifdef DARKMODE
void gameMapDARK(int height, int width, int visibleRange, int** map) {
    int i, j, k;

    /*Draw out the border*/
    for(k = 0; k < width+2; k++) {
        printf("*");
    }
    printf("\n");

    /*Prints out the map*/
    for (i = 0; i < height; i++) {
        printf("*");
        for (j = 0; j < width; j++) {

            /*if the current part of the map is within the players visible range, it prints that position*/
            if (manhattanDistance(i, j, player.posY, player.posX) <= visibleRange) {
                switch(map[i][j]) {
                    case 0: printf("."); break; /*Empty Spaces*/
                    case 1: printf("O"); break; /*Wall*/
                    case 2: printf("@"); break; /*Lantern*/
                    case 3: printf("P"); break; /*Player*/
                    case 4: printf("~"); break; /*Snake*/
                    case 5: printf("$"); break; /*Treasure*/
                }
            }

            /*if its not visible by the player, it prints an empty space*/
            else {
                printf(" ");
            }
        }
        printf("*");
        printf("\n");
    }

    /*Draw out the border*/
    for(k = 0; k < width + 2; k++) {
        printf("*");
    }

    /*prints the controlls*/
    printf("\n\nPress w to move up.\n");
    printf("Press s to move down.\n");
    printf("Press a to move left.\n");
    printf("Press d to move right.\n");
    printf("Press u to undo.\n\n");
}

#else
void gameMap(int height, int width, int** map) {
    int i, j, k;

    /*Draw out the border*/
    for(k = 0; k < width+2; k++) {
        printf("*");
    }
    printf("\n");

    /*prints out the map*/
    for (i = 0; i < height; i++) {
        printf("*");
        for (j = 0; j < width; j++) {
            switch (map[i][j]) {
                case 0: printf(" "); break; /*Empty Space*/
                case 1: printf("O"); break; /*Wall*/
                case 2: printf("@"); break; /*Lantern*/
                case 3: printf("P"); break; /*Player*/
                case 4: printf("~"); break; /*Snake*/
                case 5: printf("$"); break; /*Treasure*/
                default: break;
            }
        }
        printf("*");
        printf("\n");
    }

    /*Draw out the border*/
    for(k = 0; k < width + 2; k++) {
        printf("*");
    } 

    /*prints the controlls*/
    printf("\n\nPress w to move up.\n");
    printf("Press s to move down.\n");
    printf("Press a to move left.\n");
    printf("Press d to move right.\n");
    printf("Press u to undo.\n\n");
}
#endif

/*Controlles the player*/
int playerMovement(char direction, int** map, int height, int width) {
    int result = 0;

    switch (direction) {
        case 'w': 
            /*makes sure the player doesnt go out of border and doesnt go through walls*/
            if(player.posY > 0 && map[player.posY-1][player.posX] != 1) { 

                /*If the player gets to the treasure assign result to 1*/
                if(map[player.posY-1][player.posX] == 5) {
                    result = 1; /*Won*/
                    map[player.posY][player.posX] = 0;
                    player.posY--; 
                    map[player.posY][player.posX] = 3;
                }
                
                else {
                    /*if the player picks up the lantern assign result to 2*/
                    if(map[player.posY-1][player.posX] == 2) {
                        result = 2;
                    }

                    /*moves the player*/
                    map[player.posY][player.posX] = 0;
                    player.posY--; 
                    map[player.posY][player.posX] = 3;
                }
            } 
            break;

        case 's': 
            /*makes sure the player doesnt go out of border and doesnt go through walls*/
            if(player.posY < height-1 && map[player.posY+1][player.posX] != 1) {

                /*If the player gets to the treasure assign result to 1*/
                if(map[player.posY+1][player.posX] == 5) {
                    result = 1; /*Won*/
                    map[player.posY][player.posX] = 0;
                    player.posY++; 
                    map[player.posY][player.posX] = 3;
                }
                
                else {
                    /*if the player picks up the lantern assign result to 2*/
                    if(map[player.posY+1][player.posX] == 2) {
                        result = 2; /*Lantern*/
                    }

                    /*moves the player*/
                    map[player.posY][player.posX] = 0;
                    player.posY++; 
                    map[player.posY][player.posX] = 3;
                } 
            } 
            break;

        case 'a': 
            /*makes sure the player doesnt go out of border and doesnt go through walls*/
            if(player.posX > 0 && map[player.posY][player.posX-1] != 1) {

                /*If the player gets to the treasure assign result to 1*/
                if(map[player.posY][player.posX-1] == 5) {
                    result = 1; /*Won*/
                    map[player.posY][player.posX] = 0;
                    player.posX--;
                    map[player.posY][player.posX] = 3;
                } 
                
                else {
                    /*if the player picks up the lantern assign result to 2*/
                    if(map[player.posY][player.posX-1] == 2) {
                        result = 2; /*Lantern*/
                    }

                    /*moves the player*/
                    map[player.posY][player.posX] = 0;
                    player.posX--;
                    map[player.posY][player.posX] = 3;
                }
            } 
            break;

        case 'd': 
            /*makes sure the player doesnt go out of border and doesnt go through walls*/
            if(player.posX < width-1 && map[player.posY][player.posX+1] != 1) { 

                /*If the player gets to the treasure assign result to 1*/
                if(map[player.posY][player.posX+1] == 5) {
                    result = 1; /*Won*/
                    map[player.posY][player.posX] = 0;
                    player.posX++;
                    map[player.posY][player.posX] = 3;
                }
                
                else {
                    /*if the player picks up the lantern assign result to 2*/
                    if(map[player.posY][player.posX+1] == 2) {
                        result = 2; /*Lantern*/
                    }

                    /*moves the player*/
                    map[player.posY][player.posX] = 0;
                    player.posX++;
                    map[player.posY][player.posX] = 3;
                }
            } 
            break;
        default:
            break;
    }
    return result;
}

/*Controlles the player*/
int snakeMovement(int** map, int height, int width) {
    int min = 1, max = 4, nextMove, snakeResult = 0;

    initRandom();

    nextMove = randomUCP(min, max); /*generates a number between 1 and 4*/

    switch (nextMove) {
        case 1: 
            /*makes sure the snake doesnt go out of border and doesnt go through walls*/
            if(snake.sosY > 0 && (map[snake.sosY-1][snake.sosX] != 1 && map[snake.sosY-1][snake.sosX] != 5)) { 
                map[snake.sosY][snake.sosX] = 0;
                snake.sosY--; 
                map[snake.sosY][snake.sosX] = 4;
            } 
            break;

        case 2: 
            /*makes sure the snake doesnt go out of border and doesnt go through walls*/
            if(snake.sosY < height-1  && (map[snake.sosY+1][snake.sosX] != 1 && map[snake.sosY+1][snake.sosX] != 5)) {
                map[snake.sosY][snake.sosX] = 0;
                snake.sosY++; 
                map[snake.sosY][snake.sosX] = 4;
            } 
            break;

        case 3: 
            /*makes sure the player doesnt go out of border and doesnt go through walls*/
            if(snake.sosX > 0 && (map[snake.sosY][snake.sosX-1] != 1 && map[snake.sosY][snake.sosX-1] != 5)) {
                map[snake.sosY][snake.sosX] = 0;
                snake.sosX--;
                map[snake.sosY][snake.sosX] = 4;
            } 
            break;

        case 4: 
            /*makes sure the player doesnt go out of border and doesnt go through walls*/
            if(snake.sosX < width-1 && (map[snake.sosY][snake.sosX+1] != 1 && map[snake.sosY][snake.sosX+1] != 5)) { 
                map[snake.sosY][snake.sosX] = 0;
                snake.sosX++;
                map[snake.sosY][snake.sosX] = 4;
            } 
            break;
        default:
            break;
        }

    return snakeResult;
}