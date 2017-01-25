#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>

#include <time.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <termios.h>

#include "util.h"

void setCursor(int yPos, int xPos) {
    printf("\033[%d;%dH", yPos + 1, xPos + 1);
}

void generateMap(int xMax, int yMax, double score, double topscore) {
    int x, y;

    setCursor(0, xMax + 10);
    printf("Current score: %f", score);
    if (score <= topscore) {
        setCursor(2, xMax + 10);
        printf("Topscore: %f", topscore);
    } else {
        setCursor(2, xMax + 10);
        printf("Topscore: %f", score);
    }

    for (y = 0; y < yMax; y++) {
        setCursor(y, 0);
        printf("%c", '|');
        setCursor(y, xMax - 1);
        printf("%c", '|');
    }
    for (x = 0; x < xMax; x++) {
        setCursor(1, x);
        printf("%c", '|');
        setCursor(yMax, x);
        printf("%c", '|');
    }
}

void createCharacter(int x, int y, char display, character *myCharacter) {
    myCharacter->y = y;
    myCharacter->x = x;
    myCharacter->oldY = y;
    myCharacter->oldX = x;
    myCharacter->display = display;
}

void generateCharacter(character *myCharacter) {
    setCursor(myCharacter->oldY, myCharacter->oldX);
    printf("%c", ' ');

    setCursor(myCharacter->y, myCharacter->x);
    printf("%c", myCharacter->display);
}

void setCharacterPosition(character *myCharacter, int xMax, int yMax, int *firstMove) {
    int whatIsRead;
    char c;
    struct termios tios;

    if (tcgetattr(fileno(stdin), &tios) < 0) exit(-1);
    tios.c_lflag &= ~(ECHO | ICANON);
    if (tcsetattr(fileno(stdin), TCSAFLUSH, &tios) < 0) exit(-1);

    //reassignation position
    myCharacter->oldY = myCharacter->y;
    myCharacter->oldX = myCharacter->x;

    whatIsRead = read(fileno(stdin), &c, 1);
    if (whatIsRead > 0) {
        switch (c) {
            case 'z':
                if (myCharacter->y > 2) {
                    (*firstMove) = 1;
                    myCharacter->y--;
                }
                break;
            case 's':
                if (myCharacter->y < yMax - 1) {
                    myCharacter->y++;
                }
                break;
            case 'q':
                if (myCharacter->x > 2) {
                    (*firstMove) = 1;
                    myCharacter->x--;
                }
                break;
            case 'd':
                if (myCharacter->x < xMax - 2) {
                    (*firstMove) = 1;
                    myCharacter->x++;
                }
                break;
        }
        fflush(stdout);
    }
}

void follow(character *myCharacter, character *follower, int *endGame, int *firstMove) {
    follower->oldY = follower->y;
    follower->oldX = follower->x;
    if ((*firstMove) == 1) {
        if (myCharacter->x < follower->x)
            follower->x--;
        else if (myCharacter->x > follower->x)
            follower->x++;

        if (myCharacter->y < follower->y)
            follower->y--;
        else if (myCharacter->y > follower->y)
            follower->y++;

        if (myCharacter->x == follower->x && myCharacter->y == follower->y)
            (*endGame) = 1;
    }
}