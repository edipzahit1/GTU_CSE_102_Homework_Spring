#include <stdio.h>

#define MAX_ROW_MAP 100
#define MAX_COL_ROW 100

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
    Coordinate f_coordinate;
} Forest;

typedef struct
{
    Coordinate coordinate;
    int bottle_volume;
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
void display_forest(Forest forest);

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

    char buffer[MAX_COL_ROW];
    int row = 0, col = 0, i;
    while (fgetc(ptr) != '\n');
    while (fgets(buffer, sizeof(buffer), ptr) != NULL)
    {
        i = 0;
        col = 0;
        while (i < forest->width * 2)
        {
            if (buffer[i] == ',')
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
                forest->f_coordinate.X = row;
                forest->f_coordinate.Y = col;
                break;
            case ' ':
                forest->map[row][col] = ROAD;
                break;
            default:
                printf("Invalid character at file.\n");
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
    if (botanist->coordinate.X == forest->f_coordinate.X &&
        botanist->coordinate.Y == forest->f_coordinate.Y)
    {
        printf("I have found it!\n");
        forest->map[botanist->coordinate.X][botanist->coordinate.Y] = ROAD;
        display_forest(*forest);
        return;
    }
    if (botanist->bottle_volume == 0)
    {
        printf("Out of water :(\nGoodbye :(\n");
        return;
    }

    Coordinate oldCoordinate = {botanist->coordinate.X, botanist->coordinate.Y};
    Coordinate newCoordinate;
    do 
    {
        newCoordinate = move(*forest, *botanist);
    } while (newCoordinate.X == oldCoordinate.X && newCoordinate.Y == oldCoordinate.Y);

    if (oldCoordinate.X != newCoordinate.X || oldCoordinate.Y != newCoordinate.Y) 
    {
        forest->map[oldCoordinate.X][oldCoordinate.Y] = ROAD;
        forest->map[newCoordinate.X][newCoordinate.Y] = BOTANIST;
    }
    botanist->coordinate = newCoordinate;
    botanist->bottle_volume--;

    display_forest(*forest);

    return search(forest, botanist);
}

void display_forest(Forest forest)
{
    int i, j;
    for (i = 0; i < forest.width; ++i)
    {
        for (j = 0; j < forest.height; ++j)
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
}

Coordinate move(Forest forest, Botanist botanist)
{
    char direction;
    printf("Move !! (Up (W), Down (S), Right(D), Left(A))\n");
    do 
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

    if (direction >= 'A' && direction <= 'Z') 
        direction = direction + ('a' - 'A');

    Coordinate newCoordinate = {botanist.coordinate.X, botanist.coordinate.Y};

    switch (direction)
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
        newCoordinate.Y < 0 || newCoordinate.Y >= forest.height)
    {
        printf("Stay inside map idiot!!\n");
        return botanist.coordinate;
    }
    if (forest.map[newCoordinate.X][newCoordinate.Y] == TREE)
    {
        printf("You cant walk into trees!!\n");
        return botanist.coordinate;
    }
    return newCoordinate;
}

void saveLastView(Forest forest, Botanist botanist)
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
        return 1;
    
    display_forest(aForest);
    search(&aForest, &aBotanist);
    saveLastView(aForest, aBotanist);
}