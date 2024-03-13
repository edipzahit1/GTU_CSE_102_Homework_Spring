#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BOARD_SIZE 16

#define FIRST_WALL_TOP 7
#define FIRST_WALL_BOTTOM 11
#define FIRST_WALL_LEFT 6
#define FIRST_WALL_RIGHT 10

#define SECOND_WALL_TOP 5
#define SECOND_WALL_BOTTOM 13
#define SECOND_WALL_LEFT 4
#define SECOND_WALL_RIGHT 12

int initializeGame(char board[]);
void printBoard(char board[]);
int movePlayer(char direction, char board[]);
int isWall(int i_value, int line);
int placeOnes(int total_1_placed, char board[]);
int placeTwos(int total_2_placed, char board[], int how_many_2);

/* Returns 2 for the second wall, 1 for first wall, and 0 for no wall.*/
int isWall(int i_value, int line) {
    if ((line == SECOND_WALL_TOP || line == SECOND_WALL_BOTTOM) && 
        (BOARD_SIZE * line % i_value >= SECOND_WALL_LEFT) && 
        (BOARD_SIZE * line % i_value <= SECOND_WALL_RIGHT)) {
        return 2;
    } else if ((line > SECOND_WALL_TOP && line < SECOND_WALL_BOTTOM) && 
               ((BOARD_SIZE * line % i_value == SECOND_WALL_LEFT) ||
               (BOARD_SIZE * line % i_value == SECOND_WALL_RIGHT))) {
        return 2;
    } else if ((line == FIRST_WALL_TOP || line == FIRST_WALL_BOTTOM) && 
               (BOARD_SIZE * line % i_value >= FIRST_WALL_LEFT) && 
               (BOARD_SIZE * line % i_value <= FIRST_WALL_RIGHT)) {
        return 1;
    } else if ((line > FIRST_WALL_TOP && line < FIRST_WALL_BOTTOM) && 
              ((BOARD_SIZE * line % i_value == FIRST_WALL_LEFT) ||
               (BOARD_SIZE * line % i_value == FIRST_WALL_RIGHT))) {
        return 1;
    } else {            
        return 0;
    }
}

int initializeGame(char board[]) {
    int i, line = 0;
    int courses_placed_1 = 0;

    /*This loop initializes the '.' s and '#' s*/
    for (i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        if (i % BOARD_SIZE == 0) {
            line++;
        }
        if (isWall(i, line)) {
            board[i] = '#';
        } else if (line == 9 && (BOARD_SIZE * line % i) == 8) {
            board[i] = 'P';
        } else {
            board[i] = '.';
        }
    }

    /*These two loops place the 1 s and 2 s randomly within the walls*/
    int total_1_placed = 0, total_2_placed = 0;
    while (total_1_placed < 4) {
        total_1_placed = placeOnes(total_1_placed, board);
    }

    /*Put at least three 2 s*/
    int how_many_2 = rand() % 5 + 3;
    while (total_2_placed < how_many_2) {
        total_2_placed = placeTwos(total_2_placed, board, how_many_2);
    }

    board[BOARD_SIZE * BOARD_SIZE - 1] = 'X';

    return how_many_2 + 4;
}

/*Function to place 2 s.*/
int placeTwos(int total_2_placed, char board[], int how_many_2) {
    int j;
    int startPoint = BOARD_SIZE * SECOND_WALL_TOP + SECOND_WALL_LEFT;
    int endPoint = BOARD_SIZE * SECOND_WALL_BOTTOM - SECOND_WALL_RIGHT;

    for (j = startPoint; j < endPoint; ++j) {
        int row = j / BOARD_SIZE;
        int col = j % BOARD_SIZE;

        /*Checking boundries for between second and first wall.
        Might seem complicated but works.*/
        if (row >= SECOND_WALL_TOP && row <= SECOND_WALL_BOTTOM &&
            col >= SECOND_WALL_LEFT && col <= SECOND_WALL_RIGHT &&
            !(row >= FIRST_WALL_TOP && row <= FIRST_WALL_BOTTOM && col >= FIRST_WALL_LEFT && col <= FIRST_WALL_RIGHT) &&
            board[j] != 'P' && board[j] != '#' && board[j] != '1') {

            /*You can adjust the possibilty to get a more homogenous 2 displacement.*/
            int willBePlaced = rand() % 2;
            if (willBePlaced && total_2_placed < how_many_2) {
                board[j] = '2';
                total_2_placed++;
            }
        }
    }
    return total_2_placed;
}

/*Function to place 1 s*/
int placeOnes(int total_1_placed, char board[]) {
    int j;
    int startPoint = BOARD_SIZE * FIRST_WALL_TOP + FIRST_WALL_LEFT;
    int endPoint = BOARD_SIZE * FIRST_WALL_BOTTOM - FIRST_WALL_RIGHT;

    for (j = startPoint; j < endPoint ; ++j) {
        int row = j / BOARD_SIZE;
        int col = j % BOARD_SIZE;
        
        /*Conditions to place 1 s inside first wall*/
        if (row >= FIRST_WALL_TOP && row <= FIRST_WALL_BOTTOM && 
            col >= FIRST_WALL_LEFT && col <= FIRST_WALL_RIGHT &&
            board[j] != 'P' && board[j] != '#') {
            /*Here willBePlaced variable determines whether 
            that particular cell will contain 1 or not*/
            int willBePlaced = rand() % 2;
            if (willBePlaced && total_1_placed < 4 && board[j] != '1') {
                board[j] = '1';
                total_1_placed++;
            }
        }
    }
    return total_1_placed;
}

int movePlayer(char direction, char board[]) {

    if (direction >= 'a' && direction <= 'z') {
        direction -= 32; /*Converting upper case*/
    }
    int currentPos;
    int newPosition = -1;
    int newRow, newCol;
    int ECTS_point = 0;

    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        if (board[i] == 'P') {
            currentPos = i;
            break;
        }
    }

    newCol = currentPos % BOARD_SIZE;
    newRow = currentPos / BOARD_SIZE;

    switch (direction)
    {
    case 'W':
        newRow--;
        break;
    case 'A':
        newCol--;
        break;
    case 'S':
        newRow++;
        break;
    case 'D':
        newCol++;
        break;
    default:
        newCol = -1;
        break;
    }

    if (newRow >= 0 && newCol < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
        newPosition = newRow * BOARD_SIZE + newCol;
        if (board[newPosition] != '#') {
            if (board[newPosition] == '1' || board[newPosition] == '2') {
                ECTS_point = 8;
            }
            board[currentPos] = '.';
            board[newPosition] = 'P';
        } else if (board[newPosition] == 'X') {
            ECTS_point = -1;
            printf("Congruluuations.\n");
        } else {
            printf("You dont have enough ECTS!!.\n");
        }
    }

    return ECTS_point;
}

void printBoard(char board[]) {
    int i;
    for (i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        if (i % BOARD_SIZE == 0 && i != 0) {
            printf("\n");
        }
        printf("%c ", board[i]);
    }
    printf("\n");   
}

int main() {
    srand(time(NULL));

    char board[BOARD_SIZE * BOARD_SIZE];

    int ECTS_destination = initializeGame(board);
    printBoard(board);
    
    char movement;
    int totalMovement = 0;
    int totalECTS = 0;
    int gameStatus;

    printf("Use W (Up), A (Left), S (Down), D (Right) to move.\n");

    while (totalECTS < 32) {
        printf("Enter your move: ");
        scanf(" %c", &movement);
        printf("\n");

        totalECTS += movePlayer(movement, board);
        printBoard(board);

        printf("Total ECTS: %i\n", totalECTS);
    }

    int i, line;
    for (i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
        if (i % BOARD_SIZE == 0) {
            line++;
        }
        if (isWall(i, line) == 1) {
            board[i] = '.';
        } 
    }
    printBoard(board);

    while (totalECTS < (ECTS_destination + 4) * 8) {
        printf("Enter your move: ");
        scanf(" %c", &movement);
        printf("\n");

        totalECTS += movePlayer(movement, board);
        printBoard(board);

        printf("Total ECTS: %i\n", totalECTS);
    }

    for (i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
        if (i % BOARD_SIZE == 0) {
            line++;
        }
        if (isWall(i, line) == 2) {
            board[i] = '.';
        } 
    }
    int status = 0;
    while (status != -1) {
        printf("Enter your move: ");
        scanf(" %c", &movement);
        printf("\n");

        status = movePlayer(movement, board);
        printBoard(board);

        printf("Total ECTS: %i\n", totalECTS);
    }

    return 0;
}
