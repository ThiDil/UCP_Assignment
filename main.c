#include <stdio.h>
#include <string.h>
#include "game.h"

int main(int argc, char *argv[]) {
    /*Check if the number of arguments is exactly 2 (program name + map file)*/
    if (argc < 2) {
        printf("Usage: %s <map_file> [DARK]\n", argv[0]);
    }

    runGame(argv[1]); /*Calls the runGame function and passes the first argument to it*/
    
    return 0;
}