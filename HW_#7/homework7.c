#include <stdio.h>

#define SIDE_LENGTH 7
#define H_LARGE_ROW 1   /*Large cup row for human*/
#define H_LARGE_COL 6   /*Large cup col for human*/
#define C_LARGE_ROW 0   /*Large cup row for comp*/
#define C_LARGE_COL 0   /*Large cup col for comp*/

typedef enum
{
    human,
    computer
} player_t;

void printBoard(const int board[][SIDE_LENGTH]);
void initializeGame(int board[][SIDE_LENGTH], int n);
int gamePlay(int board[][SIDE_LENGTH]);
int gethumanMove(const int board[][SIDE_LENGTH]);
int getComputerMove(const int board[][SIDE_LENGTH]);
int move(int board[][SIDE_LENGTH], int cup_index, player_t current);
int isEnd(const int board[][SIDE_LENGTH], player_t current_user);

int isEnd(const int board[][SIDE_LENGTH], player_t current_user)
{
    int row = 0, startCup = 1, endCup = SIDE_LENGTH;    /*Default current user is computer*/
    /*setting the start and end of loop conditions based of current user*/
    if (current_user == human)  
    {
        row = 1;
        startCup = 0;
        endCup = SIDE_LENGTH - 1;
    }
    int i, isEnd = 1;
    for (i = startCup; i < endCup; ++i) /*Checking if current users side is all empty*/
    {
        if (board[row][i] != 0)
            isEnd = 0;
    }
    return isEnd;   /*if it is empty returns 1 otherwise 0*/
}

int move(int board[][SIDE_LENGTH], int cup_index, player_t current)
{
    int row, willPlayMore = 0;
    if (current == human)   /*Set the row for current user*/
        row = 1;
    else
        row = 0;

    int totalStone = board[row][cup_index];     /*Get the total amount of stone at played index*/
    board[row][cup_index] = 0;                  /*Set the played index to 0*/

    /*
    If you want to start putting stones from where you take them
    uncomment this condition 
    */
    /*if (totalStone != 1)    /*If the cup does not contain only 1 stone
    {
        /*increment or decrement the cup index in order to put first stone to entered index
        if (current == human)   
            cup_index--;
        else    
            cup_index++;
    }*/

    while (totalStone > 0)
    {
        if (row) /*If it is at second row*/
        {
            if (cup_index != SIDE_LENGTH - 1) /*If it is not end of the row*/
                cup_index++;                  /*Increment cup_index*/
            else
                row = 0; /*else set the row to first row*/
        }
        else /*If it is at first row*/
        {
            if (cup_index != 0) /*If it is not start*/
                cup_index--;    /*decrement index*/
            else
                row = 1; /*else set row to second row*/
        }
        /*After getting to large cups switch rows (In order to avoid putting opponents large cup)*/
        if ((cup_index == H_LARGE_COL && current == computer))
            row = 0;
        if ((cup_index == C_LARGE_COL && current == human))
            row = 1;

        /*Put the stone to the current cup and decrement total stone count*/
        board[row][cup_index]++;
        totalStone--;

        if (!totalStone)
        {
            /*Assign willPlayMore if last stone put in large cups*/
            if ((cup_index == H_LARGE_COL && row) || (cup_index == C_LARGE_COL && !row))
                willPlayMore = 1;
            /*
            That sentence where it says 
            "Distribution restarts from the cup where the last stone lands if the cup isn’t empty." 
            simply remove this condition if you want to play a normal mancala
            */
            else if (board[row][cup_index] != 1)
            {  
                totalStone = board[row][cup_index];
                board[row][cup_index] = 0;
            }
        }
    }
    printf("\n");
    return willPlayMore; /*Return the if player will play one more*/
}

int getComputerMove(const int board[][SIDE_LENGTH]) /*Finds the maximum elements index of computers side*/
{
    printf("Computer playing...\n");
    printf("Computer played!\n");
    int max_index = 1;
    int max_stones = board[0][1];
    int i;
    for (i = 2; i < SIDE_LENGTH; ++i)   /*Find the maximum stone*/
    {
        if (board[0][i] > max_stones)
        {
            max_stones = board[0][i];
            max_index = i;
        }
    }
    return max_index;   /*return the cup index of maximum stones*/
}

int gethumanMove(const int board[][SIDE_LENGTH])
{
    int index, isValid = 0;
    printf("Enter cup index: ");
    do
    {
        scanf("%i", &index);
        if (board[1][index - 1] == 0 || index > H_LARGE_COL || index <= 0)
            printf("Please enter a valid index. (You cant play 0 cups)\n");
        else
            isValid = 1;
    } while (!isValid);
    return index - 1; /*For array indexing*/
}

int gamePlay(int board[][SIDE_LENGTH])
{
    player_t current_turn = human, winner;  /*assign first turn to human*/
    int isFinish = 0;
    int cup_index, score;

    while (!isFinish)   /*If neither side is empty*/
    {
        if (current_turn == human)
            cup_index = gethumanMove(board);
        else
            cup_index = getComputerMove(board);

        int willPlayMore = move(board, cup_index, current_turn);    /*return 1 if current player will move more*/

        printBoard(board);
        isFinish = isEnd(board, current_turn);  /*Check current side is empty*/

        if (!isFinish)  /*if its not end*/
        {
            if (!willPlayMore)                                              /*if the player will not play again*/
                current_turn = (current_turn == human) ? computer : human;  /*switch the current player*/ 
            else                                                            /*else print another move with current player*/
                printf("Another move!!\n");  
        }
    }
    winner = (board[0][C_LARGE_COL] > board[1][H_LARGE_COL]) ? computer : human;    /*assign the winner*/
    return winner;
}

void initializeGame(int board[][SIDE_LENGTH], int n)
{
    int i, j;
    board[C_LARGE_ROW][C_LARGE_COL] = 0;
    for (i = 1; i < SIDE_LENGTH; ++i)               /*  [comp_large_cup] [] [] [] [] [] []  */
        board[0][i] = n;                            /*  [] [] [] [] [] [] [huma_large_cup]  */
    for (j = 0; j < SIDE_LENGTH - 1; ++j)
        board[1][j] = n;
    board[H_LARGE_ROW][H_LARGE_COL] = 0;
}

void printBoard(const int board[][SIDE_LENGTH])
{
    int i;
    printf("+----+---+---+---+---+---+---+----+\n");
    printf("|    ");
    for (i = 1; i < SIDE_LENGTH; ++i)
        printf("|%2i ", board[0][i]);
    printf("|    |");
    printf("\n|%3i |", board[C_LARGE_ROW][C_LARGE_COL]);
    printf("---+---+---+---+---+---");
    printf("|%3i |\n", board[H_LARGE_ROW][H_LARGE_COL]);
    printf("|    ");
    for (i = 0; i < SIDE_LENGTH - 1; ++i)
        printf("|%2i ", board[1][i]);
    printf("|    |\n");
    printf("+----+---+---+---+---+---+---+----+\n");
}

int main()
{
    int board[2][SIDE_LENGTH];

    initializeGame(board, 4);
    printBoard(board);

    player_t winner = gamePlay(board);

    if (winner == human)
        printf("You Won!! Your score is: %i", board[1][H_LARGE_COL]);
    else
        printf("Computer Won!! Computers score is: %i", board[0][C_LARGE_COL]);
}