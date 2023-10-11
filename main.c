#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

typedef struct User {
    char name[100], str_coins[10];
    int coins, tmp_coins, missile;
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

char *username();

void init_ship_info();

char **init_map();

void displayMap();

Node *putships_auto(char ***map);

void battleWithBot(char **map1, char **map2, char **map_enemy1, char **map_enemy2, Node *head1, Node *head2);

void scoreboard();

// Init
Playback *playback;
Ship ships[30];
User users[2];
int maprow = 10, mapcol = 10;
int numOfTotalShips = 10, largest_ship_area = 5;

int main() {
    char **map1, **map2, **map_enemy1, **map_enemy2;
    int choice;
    init_ship_info();

    int i;
    Node *head1, *head2;
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
                strcpy(users[0].name, username());
                users[0].coins = search(users[0].name);
                system("cls");
                users[0].tmp_coins = 0;
                users[1].tmp_coins = 0;
                map1 = init_map();
                map2 = init_map();
                map_enemy1 = init_map();
                map_enemy2 = init_map();
                //put the ships on the board
                printf("\t%s\n", users[0].name);
                printf("\tThis is your map:\n");
                displayMap(map1);
                i = 1;
                while (i) {
                    map1 = init_map(); //reset the map
                    head1 = putships_auto(&map1);
                    printf("\tPress 0 to continue\n\tPress 1 to get another map\n");
                    scanf("%d", &i);
                    getchar();
                }
                Sleep(1000);
                system("cls");
                head2 = putships_auto(&map2);
                battleWithBot(map1, map2, map_enemy1, map_enemy2, head1, head2);
                break;
            case 3:
                scoreboard();
                system("pause");
                system("cls");
                break;
        }
    } while (choice != 4);
    return 0;
}

void scoreboard() {
    printf("\tSCOREBOARD\n\n");
    //get data of all the users
    User *all = (User *) malloc(sizeof(User));
    FILE *fp = fopen("users.txt", "r");
    int i, j, num, tmpscore;
    char tmp[50];
    for (i = 0; !feof(fp); i++) {
        all = (User *) realloc(all, (i + 1) * sizeof(User));
        fscanf(fp, "%s %s", &all[i].name, &all[i].str_coins);
        all[i].coins = atoi(all[i].str_coins);
    }
    fclose(fp);
    num = i;
    //print the sorted list
    for (i = 0; i < num; i++)
        printf("\t%s %d\n", all[i].name, all[i].coins);
}

void displayMap(char **map) {
    printf("\t  ");
    for (int i = 0; i < mapcol; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (int i = 0; i < maprow; i++) {
        printf("\t%d ", i);
        for (int j = 0; j < mapcol; j++) {
            if (map[i][j] == 'w') {
                printf("%c ", map[i][j]);
            } else if (map[i][j] == 'e') {
                printf("%c ", map[i][j]);
            } else if (map[i][j] == 'c') {
                printf("%c ", map[i][j]);
            } else if (map[i][j] == 's') {
                printf("%c ", map[i][j]);
            } else if (map[i][j] == 'x') {
                printf("%c ", map[i][j]);
            } else {
                printf("%c ", map[i][j]);
            }
        }
        printf("\n");
    }
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

char *username() {
    int choice;
    char *name = (char *) malloc(100 * sizeof(char));
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
    } else if (choice == 2) {
        FILE *fp = fopen("users.txt", "r");
        char tmpName[100], coins[10];
        printf("\tUsers:\n\n");
        while (!feof(fp)) {
            fscanf(fp, "%s %s", tmpName, coins);
            printf("\t%s %s\n", tmpName, coins);
        }
        printf("\tChoose the user:\n");
        gets(name);
        while (search(name) == -1) {
            printf("\t%s does not exist! Try again:", name);
            gets(name);
        }
        fclose(fp);
    }
    return name;
}

char **init_map() {
    char **map = (char **) malloc(maprow * sizeof(char *));
    for (int i = 0; i < maprow; i++) {
        map[i] = (char *) malloc(mapcol * sizeof(char));
    }
    for (int i = 0; i < maprow; i++) {
        for (int j = 0; j < mapcol; j++) {
            map[i][j] = '.';
        }
    }
    return map;
}

void init_ship_info() {
    ships[0].length = 4;
    ships[0].width = 1;
    for (int i = 1; i <= 2; i++) {
        ships[i].length = 3;
        ships[i].width = 1;
    }
    for (int i = 3; i <= 5; i++) {
        ships[i].length = 2;
        ships[i].width = 1;
    }
    for (int i = 6; i <= 9; i++) {
        ships[i].length = 1;
        ships[i].width = 1;
    }
}

Node *createNode(int length, int width) {
    Node *res = (Node *) malloc(sizeof(Node));
    res->info.length = length;
    res->info.width = width;
    res->next = NULL;
    return res;
}

char **update_map(int x_head, int y_head, int x_tail, int y_tail, char **map) {
    int x, y;
    for (y = y_head; y <= y_tail; y++) {
        for (x = x_head; x <= x_tail; x++) {
            map[y][x] = 's';
        }
    }
    for (y = (y_head - 1 >= 0 ? y_head - 1 : y_head); y <= (y_tail + 1 < maprow ? y_tail + 1 : y_tail); y++) {
        for (x = (x_head - 1 >= 0 ? x_head - 1 : x_head); x <= (x_tail + 1 < mapcol ? x_tail + 1 : x_tail); x++) {
            if (map[y][x] != 's')
                map[y][x] = 'w';
        }
    }
    return map;
}

bool check_map(int x_head, int y_head, int x_tail, int y_tail, char **map) {
    int x, y;
    for (y = y_head; y <= y_tail; y++) {
        for (x = x_head; x <= x_tail; x++) {
            if (map[y][x] == 's')
                return false;
            if (map[y][x] == 'w')
                return false;
        }
    }
    return true;
}

Node *putships_auto(char ***map) {
    srand(time(NULL));
    int botShipDirection = rand() % 2;
    int i, x, y, deltaX, deltaY;
    Node *tmp, *new;
    Node *head = (Node *) malloc(sizeof(Node));
    head = createNode(ships[0].length, ships[0].width);
    if (botShipDirection == 0) { //vertical
        head->head.x = rand() % (mapcol - ships[0].width);
        head->head.y = rand() % (maprow - ships[0].length);
        head->tail.x = head->head.x + ships[0].width - 1;
        head->tail.y = head->head.y + ships[0].length - 1;
    } else { //horizontal
        head->head.x = rand() % (mapcol - ships[0].length);
        head->head.y = rand() % (maprow - ships[0].width);
        head->tail.x = head->head.x + ships[0].length - 1;
        head->tail.y = head->head.y + ships[0].width - 1;
    }
    *map = update_map(head->head.x, head->head.y, head->tail.x, head->tail.y, *map);
    head->hit = 0;
    tmp = head;
    for (int i = 1; i < numOfTotalShips; i++) {
        new = createNode(ships[i].length, ships[i].width);
        botShipDirection = rand() % 2;
        while (1) {
            if (botShipDirection == 0) { //vertical
                new->head.x = rand() % (mapcol - ships[i].width);
                new->head.y = rand() % (maprow - ships[i].length);
                new->tail.x = new->head.x + ships[i].width - 1;
                new->tail.y = new->head.y + ships[i].length - 1;
            } else { //horizontal
                new->head.x = rand() % (mapcol - ships[i].length);
                new->head.y = rand() % (maprow - ships[i].width);
                new->tail.x = new->head.x + ships[i].length - 1;
                new->tail.y = new->head.y + ships[i].width - 1;
            }
            if (check_map(new->head.x, new->head.y, new->tail.x, new->tail.y, *map))
                break;
        }
        *map = update_map(new->head.x, new->head.y, new->tail.x, new->tail.y, *map);
        new->hit = 0;
        tmp->next = new;
        tmp = tmp->next;
    }
    system("cls");
    displayMap(*map);
    return head;
}

char **complete_explosion(char **map, char **map_enemy, Node *curr_ship) {
    int x_head = curr_ship->head.x, y_head = curr_ship->head.y;
    int x_tail = curr_ship->tail.x, y_tail = curr_ship->tail.y;
    int x, y;
    for (y = (y_head - 1 >= 0 ? y_head - 1 : y_head); y <= (y_tail + 1 <= maprow ? y_tail + 1 : y_tail); y++) {
        for (x = (x_head - 1 >= 0 ? x_head - 1 : x_head); x <= (x_tail + 1 <= mapcol ? x_tail + 1 : x_tail); x++) {
            if (map[y][x] == 's' || map[y][x] == 'e')
                map_enemy[y][x] = 'c';
            if (map[y][x] == 'w')
                map_enemy[y][x] = 'w';
        }
    }
    return map_enemy;
}

void savescores() {
    //better way is to realloc "all" everytime u wanna read smth into it:)
    int i, coin;
    //User all[500];
    User *all = (User *) malloc(sizeof(User));
    FILE *fpin = fopen("users.txt", "r");
    for (i = 0; !feof(fpin); i++) {
        all = (User *) realloc(all, (i + 1) * sizeof(User));
        fscanf(fpin, "%s %s", &all[i].name, &all[i].str_coins);
        if (strcmp(all[i].name, users[0].name) == 0)
            sprintf(all[i].str_coins, "%d", users[0].coins);
        else if (strcmp(all[i].name, users[1].name) == 0)
            sprintf(all[i].str_coins, "%d", users[1].coins);
    }
    int num_of_useres = i;
    fclose(fpin);
    FILE *fpout = fopen("users.txt", "w");
    for (i = 0; i < num_of_useres; i++) {
        fprintf(fpout, "%s %s\n", all[i].name, all[i].str_coins);
    }
    fclose(fpout);
}

void battleWithBot(char **map1, char **map2, char **map_enemy1, char **map_enemy2, Node *head1, Node *head2) {
    srand(time(0));
    int choice, difficulty;
    printf("\tChoose the game's difficulty:\n\t(1)easy\n\t(2)hard\n");
    scanf("%d", &difficulty);
    int x, y;
    Node *curr, *prev;
    bool bonus;
    int coins1 = users[0].tmp_coins, coins2 = users[1].tmp_coins;

    while (head1 && head2) {
        bonus = true;
        printf("\t%s\n", users[0].name);
        while (bonus) {
            system("cls");
            printf("\t%s: ", users[0].name);
            printf("%d\n", coins1);
            printf("\tBot: ");
            printf("%d\n", coins2);

            printf("\n\t%s's map\n", users[0].name);
            displayMap(map1);
            printf("\n\tBot's map\n");
            displayMap(map_enemy1); //enemy's map before shooting
            printf("\tYou:\n");
            scanf("%d %d", &x, &y);
            if (map_enemy1[y][x] == '.' && (map2[y][x] == '.' || map2[y][x] == 'w')) {
                map2[y][x] = 'x';
                map_enemy1[y][x] = 'w';
                bonus = false;
            } else if (map2[y][x] == 's') {
                //find the target ship in the linked list
                curr = head2;
                while (x < curr->head.x || x > curr->tail.x || y < curr->head.y || y > curr->tail.y) {
                    prev = curr;
                    curr = curr->next;
                }
                if (curr->hit != curr->info.length * curr->info.width - 1) { //the ship is not completely exploded
                    map2[y][x] = 'e';
                    map_enemy1[y][x] = 'e';
                    coins1++;
                    curr->hit++;
                } else {
                    map_enemy1 = complete_explosion(map2, map_enemy1, curr);
                    map2[y][x] = 'e';
                    printf("\n\t\tCOMPLETE EXPLOSION!\n");
                    //remove the ship from the linked list
                    if (curr == head2)
                        head2 = head2->next;
                    else
                        prev->next = curr->next;
                    int destroyed_ship_area = curr->info.length * curr->info.width;
                    int score = 5 * largest_ship_area / destroyed_ship_area;
                    coins1 += score + 1;
                    if (head2 == NULL)
                        break;
                }
            } else if (map2[y][x] == 'e' || map2[y][x] == 'x' || map_enemy1[y][x] == 'w')
                printf("\tYou have already hit this target! Try again\n");
            Sleep(1500);
            system("cls");
            printf("\t%s: ", users[0].name);
            printf("%d\n", coins1);
            printf("\tBot: ");
            printf("%d\n", coins2);
            printf("\n\t%s's map\n", users[0].name);
            displayMap(map1);
            printf("\n\tBot's map\n");
            displayMap(map_enemy1); //result
            Sleep(1500);
        }
        //bot's turn
        bonus = true;
        if (head2 == NULL)
            break;
        x = rand() % 10;
        y = rand() % 10;
        while (bonus) {
            while (map1[y][x] == 'e' || map1[y][x] == 'x' ||
                   map_enemy2[y][x] == 'w') { //the target has already been hit
                bonus = true; //the bot can try again so it stays in the loop
                x = rand() % mapcol;
                y = rand() % maprow;
            }
            if (map_enemy2[y][x] == '.' && (map1[y][x] == '.' || map1[y][x] == 'w')) { //the target is the ocean
                map1[y][x] = 'x';
                map_enemy2[y][x] = 'w';
                bonus = false;
            } else if (map1[y][x] == 's') { //the target is a part of a ship
                //find the target ship in the linked list
                curr = head1;
                while (x < curr->head.x || x > curr->tail.x || y < curr->head.y || y > curr->tail.y) {
                    prev = curr;
                    curr = curr->next;
                }
                if (curr->hit != curr->info.length * curr->info.width - 1) { //the ship is not completely exploded
                    map1[y][x] = 'e';
                    map_enemy2[y][x] = 'e';
                    coins2++;
                    curr->hit++;
                    //generate another x,y coordinates as a bonus
                    if (difficulty == 1) {
                        x = rand() % mapcol;
                        y = rand() % maprow;
                    } else if (difficulty == 2) {
                        bool found = false;
                        while (1) {
                            for (int i = 1;; i++) {
                                if (x + i >= mapcol)
                                    break;
                                if (map1[y][x + i] == 's') {
                                    x += i;
                                    found = true;
                                    break;
                                }
                                if (map1[y][x + i] == 'w')
                                    break;
                            }
                            if (found) break;
                            for (int i = 1;; i++) {
                                if (x - i < 0)
                                    break;
                                if (map1[y][x - i] == 's') {
                                    x -= i;
                                    found = true;
                                    break;
                                }
                                if (map1[y][x - i] == 'w')
                                    break;
                            }
                            if (found) break;
                            for (int i = 1;; i++) {
                                if (y + i > maprow)
                                    break;
                                if (map1[y + i][x] == 's') {
                                    y += i;
                                    found = true;
                                    break;
                                }
                                if (map1[y + i][x] == 'w')
                                    break;
                            }
                            if (found) break;
                            for (int i = 1;; i++) {
                                if (y - i < 0)
                                    break;
                                if (map1[y - i][x] == 's') {
                                    y -= i;
                                    found = true;
                                    break;
                                }
                                if (map1[y - i][x] == 'w')
                                    break;
                            }
                            if (found) break;
                        }
                    }
                } else {
                    map_enemy2 = complete_explosion(map1, map_enemy2, curr);
                    map1 = complete_explosion(map1, map1, curr);
                    printf("\n\t\tCOMPLETE EXPLOSION!\n");
                    //unlink the ship from the linked list
                    if (curr == head1)
                        head1 = head1->next;
                    else
                        prev->next = curr->next;
                    int destroyed_ship_area = curr->info.length * curr->info.width;
                    int score = 5 * largest_ship_area / destroyed_ship_area;
                    coins2 += score + 1;
                    if (head1 == NULL)
                        break;
                    //next target as a bonus
                    x = rand() % mapcol;
                    y = rand() % maprow;
                }
            }

            Sleep(1500);
            printf("\t%s: ", users[0].name);
            printf("%d\n", coins1);
            printf("\tBot: ");
            printf("%d\n", coins2);
            printf("\n\t%s's map\n", users[0].name);
            displayMap(map1);
            printf("\n\tBot's map\n");
            displayMap(map_enemy1); //result
            Sleep(1500);
        }
    }
    //end of the game
    if (head1 == NULL && head2 != NULL) {
        printf("\tBot won!\n");
        users[0].coins += coins1 / 2;
    } else if (head2 == NULL && head1 != NULL) {
        printf("\t%s won!\n", users[0].name);
        users[0].coins += coins1;
    } else {
        printf("\tIt's a tie\n");
        users[0].coins += coins1;
    }
    savescores();
    printf("\tYou will be forwarded back to the menu...\n");
    Sleep(5000);
    system("cls");
}