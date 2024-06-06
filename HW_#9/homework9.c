#include <stdio.h>

#define MAX_ROW_MAP 100
#define MAX_COL_ROW 100
#define MAX_FLOWER 100

typedef struct
{
    int X;
    int Y;
} Coordinate;

typedef struct
{
    int map[MAX_ROW_MAP][MAX_COL_ROW];
    int width;
    int height;
    int remaining_flower;                        /*I have add this field additionaly*/
    Coordinate f_coordinates[MAX_FLOWER];
} Forest;

typedef struct
{
    Coordinate coordinate;
    int bottle_volume;
    int flowers_collected;                  /*I have add this field additionaly*/
} Botanist;

typedef enum
{
    BOTANIST,
    TREE,
    FLOWER,
    ROAD
} map_element;

int init_game(Forest *forest, Botanist *botanist);
void search(Forest *forest, Botanist *botanist);
Coordinate move(Forest forest, Botanist botanist);
void display_forest(Forest forest, Botanist botanist);
int isFlower(Coordinate f_coordinates[], Botanist botanist);

int init_game(Forest *forest, Botanist *botanist)
{
    FILE *ptr = fopen("init.txt", "r");
    if (ptr == NULL)
    {
        printf("File could not found.\n");
        return 0;
    }
    
    fscanf(ptr, "%i,%i", &forest->height, &forest->width);
    fscanf(ptr, "%i,%i,%i", &botanist->coordinate.X, &botanist->coordinate.Y, &botanist->bottle_volume);

    forest->remaining_flower = 0;
    botanist->flowers_collected = 0;
    int j;
    for (j = 0; j < MAX_FLOWER; j++)
        forest->f_coordinates[j].X = -1;    /*indicate empty locations*/

    char buffer[MAX_COL_ROW];
    int row = 0, col = 0, i;
    while (fgetc(ptr) != '\n');     /*skip the line*/
    while (fgets(buffer, sizeof(buffer), ptr) != NULL)  /*get the map here*/
    {
        i = 0;
        col = 0;
        while (i < forest->width * 2)   /*go through row*/
        {
            if (buffer[i] == ',' || buffer[i] == '\n')   /*if comma continue*/
            {
                i++;
                continue;
            }
            switch (buffer[i])  
            {
            case 'T':
                forest->map[row][col] = TREE;
                break;
            case 'B':
                forest->map[row][col] = BOTANIST;
                break;
            case 'X':
                forest->map[row][col] = FLOWER;
                forest->f_coordinates[forest->remaining_flower].X = row; 
                forest->f_coordinates[forest->remaining_flower].Y = col;
                forest->remaining_flower++;
                break;
            case ' ':
                forest->map[row][col] = ROAD;
                break;
            default:
                printf("Invalid character at file. %c\n", buffer[i]);
                fclose(ptr);
                return 0;
            }
            col++;
            i++;
        }
        row++;
    }
    fclose(ptr);
    return 1;
}

void search(Forest *forest, Botanist *botanist)
{   
    if (isFlower(forest->f_coordinates, *botanist)) /*Check if current position is flower (We can simply check if == 'X')*/
    {
        if (botanist->bottle_volume > 0) 
        {  
            printf("I have found a flower!\n");
            botanist->flowers_collected++;
            botanist->bottle_volume--; 
            forest->remaining_flower--;
            for (int i = 0; i < MAX_FLOWER; i++) 
            {
                if (forest->f_coordinates[i].X == botanist->coordinate.X && forest->f_coordinates[i].Y == botanist->coordinate.Y) 
                {
                    forest->f_coordinates[i].X = -1;
                    break;
                }
            }
        } 
        else 
        {
            printf("No more space in the bottle to collect flowers.\n");
        }
    }
    display_forest(*forest, *botanist); /*print forest*/

    if (forest->remaining_flower == 0)  /*Check the end of game*/
    {
        printf("Game is finished. All flowers found.\n");
        return;
    }

    Coordinate oldCoordinate = botanist->coordinate;
    Coordinate newCoordinate = move(*forest, *botanist);

    if (newCoordinate.X != oldCoordinate.X || newCoordinate.Y != oldCoordinate.Y) /*if old and new coordinate are not same move botanist*/
    {
        forest->map[oldCoordinate.X][oldCoordinate.Y] = ROAD;  
        forest->map[newCoordinate.X][newCoordinate.Y] = BOTANIST;  
        botanist->coordinate = newCoordinate; 

        search(forest, botanist);
    }
    else    /*if it is same call again with same parameters to move again*/
    {
        printf("Invalid move try again.\n");
        search(forest, botanist); 
    }
}

int isFlower(Coordinate f_coordinates[], Botanist botanist) /*Returns if current pos is flower*/
{
    int i = 0;
    int isFlower = 0;
    for (i = 0; i < MAX_FLOWER; i++)
        if (f_coordinates[i].X == botanist.coordinate.X && f_coordinates[i].Y == botanist.coordinate.Y)
            isFlower = 1;
    return isFlower;
}

void display_forest(Forest forest, Botanist botanist)   /*print the forest and botainst info*/
{
    int i, j;

    for (i = 0; i < forest.height; ++i)
    {
        for (j = 0; j < forest.width; ++j)
        {
            switch (forest.map[i][j])
            {
            case TREE:
                printf("T ");
                break;
            case BOTANIST:
                printf("B ");
                break;
            case ROAD:
                printf("  ");
                break;
            case FLOWER:
                printf("X ");
                break;
            }
        }
        printf("\n");
    }
    printf("Botanist's Current Position: (%d, %d)\n", botanist.coordinate.X, botanist.coordinate.Y);
    printf("Number of Flowers Collected: %d\n", botanist.flowers_collected);
    printf("Remaining Bottle Volume: %d\n", botanist.bottle_volume);
}

Coordinate move(Forest forest, Botanist botanist)
{
    char direction;
    printf("Move !! (Up (W), Down (S), Right(D), Left(A))\n");
    do /*Get input until you get valid move*/
    {
        scanf(" %c", &direction);
        if (direction != 'W' && direction != 'w' &&
            direction != 'A' && direction != 'a' &&
            direction != 'S' && direction != 's' &&
            direction != 'D' && direction != 'd') 
        {
            printf("Bad entry!!! Enter again.\n");
        } 
        else 
        {
            break;
        }
    } while (1);

    if (direction >= 'A' && direction <= 'Z') /*convert to lower case if it is upper*/
        direction = direction + ('a' - 'A');

    Coordinate newCoordinate = {botanist.coordinate.X, botanist.coordinate.Y};  /*new instance of coordinate*/

    switch (direction)  /*update coordinate based on direction*/
    {
    case 'w':
        newCoordinate.X--;
        break;
    case 'a':
        newCoordinate.Y--;
        break;
    case 's':
        newCoordinate.X++;
        break;
    case 'd':
        newCoordinate.Y++;
        break;
    }
    
    if (newCoordinate.X < 0 || newCoordinate.X >= forest.width ||
        newCoordinate.Y < 0 || newCoordinate.Y >= forest.height)    /*handle outside of the forest*/
    {
        printf("Stay inside map!!\n");
        return botanist.coordinate; /*return the same position*/
    }
    if (forest.map[newCoordinate.X][newCoordinate.Y] == TREE)   /*handle the trees*/
    {
        printf("You cant walk into trees!!\n");
        return botanist.coordinate; /*return the same position*/
    }
    return newCoordinate; /*return new position*/
}

void saveLastView(Forest forest, Botanist botanist) /*save the last view of the map and info*/
{
    FILE *last_ptr = fopen("last.txt", "w");

    if (last_ptr == NULL)
    {
        printf("Unable to open \"last.txt\"\n");
        return;
    }
    int i, j;
    for (i = 0; i < forest.height; ++i)
    {
        for (j = 0; j < forest.width; ++j)
        {
            switch (forest.map[i][j])
            {
            case TREE:
                fprintf(last_ptr, "T ");
                break;
            case BOTANIST:
                fprintf(last_ptr, "B ");
                break;
            case ROAD:
                fprintf(last_ptr, "  ");
                break;
            case FLOWER:
                fprintf(last_ptr, "X ");
                break;
            }
        }
        fprintf(last_ptr, "\n");
    }
    fprintf(last_ptr, "Last position of botanist: (%i, %i)\n", botanist.coordinate.X, botanist.coordinate.Y);
    fprintf(last_ptr, "Remaining water volume: %i\n", botanist.bottle_volume);

    fclose(last_ptr);
}

int main()
{
    Forest aForest;
    Botanist aBotanist;

    if (!init_game(&aForest, &aBotanist))
        return 1;   /*if there is a file error or invalid character at file*/

    search(&aForest, &aBotanist);       /*gameplay*/
    saveLastView(aForest, aBotanist);   /*save last*/
}
