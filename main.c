#include <stdio.h>
#include <stdlib.h>



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