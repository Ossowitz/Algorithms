#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct User {
    char name[100], str_coins[10];
    int coins, tmp_coins, missile;
    bool winner;
} User;

typedef struct Ship {
    int length, width;
} Ship;

typedef struct Coord {
    int x, y;
} Coord;

typedef struct Node {
    Ship info;
    Coord head, tail;
    int hit;
    struct Node *next;
} Node;

typedef struct Playback {
    char **map;
    struct Playback *next;
} Playback;

// Init
Playback *playback;
Ship ships[30];
User users[2];
int maprow = 10, mapcol = 10;
int numOfTotalShips = 10, largest_ship_area = 5;

int main() {
    int choice;
    do {
        printf("\t1. Play with a friend\n\t"
               "2. Play with bot\n\t"
               "3. Scoreboard\n\t"
               "4. Exit\n"
        );
        scanf("%d", &choice);
        getchar();
        system("cls");
    } while (choice != 4);
    return 0;
}