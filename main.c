#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

int search(char name[100]);
char* username();

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
        switch (choice) {
            case 2:

        }
    } while (choice != 4);
    return 0;
}

int search(char name[100]) {
    char tmpName[100], coins[10];
    FILE *file = fopen("users.txt", "r");
    while (!feof(file)) {
        fscanf(file, "%s %s", tmpName, coins);
        if (strcmp(name, tmpName) == 0) {
            fclose(file);
            int score = atoi(coins);
            return score;
        }
    }
    fclose(file);
    return -1;
}

char* username() {
    int choice;
    char* name = (char*) malloc(100 * sizeof(char));
    printf("\t1. New user\n\t"
           "2. Choose user\n"
    );
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        printf("\tEnter a username: ");
        gets(name);
        while (search(name) != -1) {
            printf("\t%s already exists.\n Try again: ", name);
            gets(name);
        }
        FILE *fp = fopen("users.txt", "a");
        fprintf(fp, "\n%s 0", name);
        fclose(fp);
    } 
    return name;
}