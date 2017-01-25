#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

typedef struct Character {
    int x;
    int y;
    int oldX;
    int oldY;
    char display;
} character;

struct obstacle {
    int x;
    int y;
    char display;
};


void setCursor(int yPos, int xPos);

void generateMap(int xMax, int yMax, double score, double topscore);

void createCharacter(int x, int y, char display, character *myChar);

void generateCharacter(character *myChar);

void setCharacterPosition(character *myChar, int xMax, int yMax, int *firstMove);

void follow(character *myChar, character *follower, int *endGame, int *firstMove);

#endif