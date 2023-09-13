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
char* username();
void init_ship_info();
char** init_map();
void displayMap();
Node* putships_auto(char*** map);

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
                users[0].tmp_coins = 0; users[1].tmp_coins = 0;
                map1 = init_map();
                map2 = init_map();
                map_enemy1 = init_map();
                map_enemy2 = init_map();
                //put the ships on the board
                printf("\t%s\n", users[0].name);
                printf("\tThis is your map:\n");
                displayMap(map1);
                i = 1;
                while (i)
                {
                    map1 = init_map(); //reset the map
                    head1 = putships_auto(&map1);
                    printf("\tPress 0 to continue\n\tPress 1 to get another map\n");
                    scanf("%d", &i);
                    getchar();
                }
                Sleep(1000);
                system("cls");
                head2 = putships_auto(&map2);
                //battleWithBot(map1, map2, map_enemy1, map_enemy2, head1, head2);
                break;
        }
    } while (choice != 4);
    return 0;
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
            }else if (map[i][j] == 'e') {
                printf("%c ", map[i][j]);
            } else if (map[i][j] == 'c') {
                printf( "%c " , map[i][j]);
            } else if (map[i][j] == 's') {
                printf( "%c " , map[i][j]);
            } else if (map[i][j] == 'x') {
                printf( "%c " , map[i][j]);
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

char** init_map() {
    char **map = (char**)malloc(maprow * sizeof(char*));
    for (int i = 0; i < maprow; i++) {
        map[i] = (char*)malloc(mapcol * sizeof(char));
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

Node* createNode(int length, int width) {
    Node *res = (Node*)malloc(sizeof(Node));
    res->info.length = length;
    res->info.width = width;
    res->next = NULL;
    return res;
}

char **update_map(int x_head, int y_head, int x_tail, int y_tail, char **map)
{
    int x, y;
    for (y = y_head; y <= y_tail; y++)
    {
        for (x = x_head; x <= x_tail; x++)
        {
            map[y][x] = 's';
        }
    }
    for (y = (y_head - 1 >= 0 ? y_head - 1 : y_head); y <= (y_tail + 1 < maprow ? y_tail + 1 : y_tail); y++)
    {
        for (x = (x_head - 1 >= 0 ? x_head - 1 : x_head); x <= (x_tail + 1 < mapcol ? x_tail + 1 : x_tail); x++)
        {
            if (map[y][x] != 's')
                map[y][x] = 'w';
        }
    }
    return map;
}

bool check_map(int x_head, int y_head, int x_tail, int y_tail, char **map)
{
    int x, y;
    for (y = y_head; y <= y_tail; y++)
    {
        for (x = x_head; x <= x_tail; x++)
        {
            if (map[y][x] == 's')
                return false;
            if (map[y][x] == 'w')
                return false;
        }
    }
    return true;
}

Node *putships_auto(char ***map)
{
    srand(time(NULL));
    int botShipDirection = rand() % 2;
    int i, x, y, deltaX, deltaY;
    Node *tmp, *new;
    Node *head = (Node *)malloc(sizeof(Node));
    head = createNode(ships[0].length, ships[0].width);
    if (botShipDirection == 0)
    { //vertical
        head->head.x = rand() % (mapcol - ships[0].width);
        head->head.y = rand() % (maprow - ships[0].length);
        head->tail.x = head->head.x + ships[0].width - 1;
        head->tail.y = head->head.y + ships[0].length - 1;
    }
    else
    { //horizontal
        head->head.x = rand() % (mapcol - ships[0].length);
        head->head.y = rand() % (maprow - ships[0].width);
        head->tail.x = head->head.x + ships[0].length - 1;
        head->tail.y = head->head.y + ships[0].width - 1;
    }
    *map = update_map(head->head.x, head->head.y, head->tail.x, head->tail.y, *map);
    head->hit = 0;
    tmp = head;
    for (int i = 1; i < numOfTotalShips; i++)
    {
        new = createNode(ships[i].length, ships[i].width);
        botShipDirection = rand() % 2;
        while (1)
        {
            if (botShipDirection == 0)
            { //vertical
                new->head.x = rand() % (mapcol - ships[i].width);
                new->head.y = rand() % (maprow - ships[i].length);
                new->tail.x = new->head.x + ships[i].width - 1;
                new->tail.y = new->head.y + ships[i].length - 1;
            }
            else
            { //horizontal
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
