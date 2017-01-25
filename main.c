#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>

#include "util.h"

const double TOP_SCORE = 200;

int main() {
    int endGame = 0;
    int firstMove = 0;
    int gameSpeed = 200;
    int xMax;
    int yMax;
    character myCharacter, follower;

    clock_t time;

    printf("Saisir largeur: ");
    scanf("%d", &yMax);
    printf("Saisir longueur: ");
    scanf("%d", &xMax);

    system("stty -echo");
    system("/bin/stty raw");

    //creation character et follower avec positions random sur la map
    createCharacter((rand() % xMax - 1) + 1, (rand() % yMax - 1) + 1, 'C', &myCharacter);
    createCharacter((rand() % xMax - 1) + 1, (rand() % yMax - 1) + 1, 'F', &follower);

    system("clear");

    //tant qu'on a pas reach la fin du jeu, on continue
    while (endGame == 0) {
        time = clock();
        generateMap(xMax, yMax, (double) time / CLOCKS_PER_SEC, TOP_SCORE);
        generateCharacter(&myCharacter);
        generateCharacter(&follower);
        usleep((__useconds_t) gameSpeed);
        setCharacterPosition(&myCharacter, xMax, yMax, &firstMove);
        follow(&myCharacter, &follower, &endGame, &firstMove);
    }

    if (endGame == 1) {
        system("clear");
        printf("%s\n", "End of game");
        system("stty sane echo");
    }

    return EXIT_SUCCESS;
}