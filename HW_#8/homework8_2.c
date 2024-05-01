#include <stdio.h>
#include <string.h>

#define MAX_ROW 20
#define MAX_CHAR_AT_ROW 200

char P1[] = "***++++***++++***";
char P2[] = "+*+*+";
char P3[] = "+++++";

void search_p1(char strs[][MAX_CHAR_AT_ROW], int num_rows, int matches[][2], int *num_matches);
void search_p2(char strs[][MAX_CHAR_AT_ROW], int num_rows, int matches[][2], int *num_matches);
void search_p3(char strs[][MAX_CHAR_AT_ROW], int num_rows, int matches[][2], int *num_matches);
int fillStrs(char strs[][MAX_CHAR_AT_ROW], FILE *input2_ptr);
void printResults(int matches[][2], char *search, int num_rows);

int fillStrs(char strs[][MAX_CHAR_AT_ROW], FILE *input2_ptr)
{
    int row_count = 0;

    char line[MAX_CHAR_AT_ROW];
    while (fgets(line, sizeof(line), input2_ptr) != NULL && row_count < MAX_ROW)
    {
        int j = 0;
        while (line[j] != '\n' && line[j] != '\0')
        {
            strs[row_count][j] = line[j];
            ++j;
        }
        strs[row_count][j] = '\0';
        ++row_count;
    }
    return row_count;
}

void search_p1(char strs[][MAX_CHAR_AT_ROW], int num_rows, int matches[][2], int *num_matches)
{
    int i, j;
    int len = strlen(P1);
    *num_matches = 0;

    for (i = 0; i < num_rows; ++i)
        for (j = 0; j < 2; ++j)
            matches[i][j] = -1;
    
    for (i = 0; i < num_rows; i++)
    {
        char *temp = strs[i];
        while (*temp != '\0')
        {
            if (*temp == P1[0]) /*If first char match*/
            {
                char *temp_P1 = P1; /*assign temporray variables for searching*/
                char *temp_ptr = temp;

                /*Countinue searching*/
                while (*(temp_ptr++) == *(temp_P1++) && *temp_ptr != '\0' && *temp_P1 != '\0')
                    ;

                /*If the temporary P1 is at null char it means we found a match*/
                if (*temp_P1 == '\0')
                {
                    matches[*num_matches][0] = i;
                    /*  The difference between found address and start  */
                    /*  index of strs[i] will give us the location      */
                    matches[*num_matches][1] = (int)(temp - strs[i]);
                    (*num_matches)++; /*increment total matches*/
                }
            }
            /*Increment temp for searching next pattern*/
            temp++;
        }
    }
    printResults(matches, P1, *num_matches);
}

void search_p2(char strs[][MAX_CHAR_AT_ROW], int num_rows, int matches[][2], int *num_matches)
{
    int len = strlen(P2);
    if (len > num_rows)
        return;
    
    int i, j;
    *num_matches = 0;
    for (i = 0; i < num_rows; ++i)
        for (j = 0; j < 2; ++j)
            matches[i][j] = -1;
            
    for (i = 0; i < num_rows; ++i) 
    {  
        int row_length = strlen(strs[i]);   /*get the length of current row*/
        for (j = 0; j < row_length; ++j) 
        {
            if (i + len > num_rows) /*If the current row + length of P2 exceeds total row count break*/
                break;

            int isMatch = 1;
            int k;
            for (k = 0; k < len; ++k)   /*Search through the [i + k][j] index*/
            {
                if (strs[i + k][j] != P2[k])    /*If not match*/
                {
                    isMatch = 0;    /*assign ismatch to be false*/
                    break;
                }
            }
            if (isMatch)    /*if ismatch true write the coordinates in matches array*/
            {
                matches[*num_matches][0] = i;
                matches[*num_matches][1] = j;
                (*num_matches)++;
            }
        }
    }
    printResults(matches, P2, *num_matches);
}

void search_p3(char strs[][MAX_CHAR_AT_ROW], int num_rows, int matches[][2], int *num_matches)
{
    int len = strlen(P3);  
    if (len > num_rows)
        return;  
    int i, j;
    for (i = 0; i < num_rows; ++i)
        for (j = 0; j < 2; ++j)
            matches[i][j] = -1;
    *num_matches = 0;  
    /*Here we can check for the top to left diagonal also*/
    for (i = 0; i < num_rows; ++i) 
    {  
        int row_length = strlen(strs[i]);  
        for (j = 0; j < row_length; ++j) 
        {
            if (i + len > num_rows)  
                break;

            int isMatch = 1;
            int k;
            for (k = 0; k < len; ++k)   
            {
                if (strs[i + k][j + k] != P3[k]) 
                {
                    isMatch = 0;  
                    break;
                }
            }
            if (isMatch)  
            {
                matches[*num_matches][0] = i;
                matches[*num_matches][1] = j;  
                (*num_matches)++; 
            }
        }
    }
    printResults(matches, P3, *num_matches);  
}

void printResults(int matches[][2], char *search, int num_matches)
{
    int i, number;
    if (strcmp(search, P1) == 0)
        number = 1;
    else if (strcmp(search, P2) == 0)
        number = 2;
    else
        number = 3;

    for (i = 0; i < num_matches; ++i)
        printf("P%i @ (%i, %i)\n", number, matches[i][0] + 1, matches[i][1] + 1);
}

int main()
{
    FILE *input2_ptr = fopen("input2.txt", "r");
    if (input2_ptr == NULL)
    {
        printf("Error opening file.");
        return 1;
    }
    char strs[MAX_ROW][MAX_CHAR_AT_ROW];
    int matches[MAX_ROW][2];
    int num_mathces;

    int rowCount = fillStrs(strs, input2_ptr);
    search_p1(strs, rowCount, matches, &num_mathces);
    search_p2(strs, rowCount, matches, &num_mathces);
    search_p3(strs, rowCount, matches, &num_mathces);
}