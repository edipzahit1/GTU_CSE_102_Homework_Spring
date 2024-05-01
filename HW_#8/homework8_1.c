#include <stdio.h>
#include <string.h>

#define MAX_STRING_LENGTH 50
#define MAX_COL 5
#define MAX_PEOPLE 50
#define MAX_LINE MAX_STRING_LENGTH * MAX_COL
#define AGE_COL 2
#define FIRST_BRANCH_COL 3
#define SECOND_BRANCH_COL 4

/* Function prototypes */
int fillTable(char table[][MAX_COL][MAX_STRING_LENGTH], FILE *inp1_ptr);
int convert_int(char *ages_as_char_arr);
void remove_newLines(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount);
void menu(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount);
void fillPerson(char person[][MAX_STRING_LENGTH], char *line);
void printTable(char table[][MAX_COL][MAX_STRING_LENGTH], int totalPeople);
void sort_people_by_age(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount);
void sort_people_by_branch(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount);
void filter_people_by_branch(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount);
void filter_people_by_profession(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount);
void sort_ages(int arr[][MAX_PEOPLE], int start, int end);
void sort_branches(char branches[][MAX_STRING_LENGTH], int *indexes, int peopleCount);
void swap_int(int *a, int *b);
void swap_str(char *str1, char *str2);

/* Converts a string to an integer, ignoring leading whitespaces */
int convert_int(char *ages_as_char_arr)
{
    int result = 0;
    while (*ages_as_char_arr == ' ')    /* Skip the whitespaces at the beginning */
        ages_as_char_arr++;
    
    while (*ages_as_char_arr != '\0')   
    {
        result = result * 10 + (*ages_as_char_arr - '0');   /* Convert character to integer */
        ages_as_char_arr++;
    }
    return result;
}

/* Swaps two integers */
void swap_int(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Swaps two strings */
void swap_str(char *str1, char *str2)
{
    char temp[MAX_STRING_LENGTH];
    strcpy(temp, str1);
    strcpy(str1, str2);
    strcpy(str2, temp);
}

/* Sorts an array of ages with corresponding indexes */
void sort_ages(int arr[][MAX_PEOPLE], int start, int end)
{
    int i, j;
    for (i = start; i <= end; ++i)
    {
        for (j = start; j <= end; j++)
        {
            if (arr[0][i] < arr[0][j])
            {
                swap_int(&arr[0][i], &arr[0][j]);
                swap_int(&arr[1][i], &arr[1][j]);
            }
        }     
    }
}

/* Sorts an array of strings (branches) with corresponding indexes */
void sort_branches(char branches[][MAX_STRING_LENGTH], int *indexes, int peopleCount)
{
    int i, j;
    for (i = 0; i < peopleCount; i++)
    {
        for (j = i + 1; j < peopleCount; ++j)
        {
            if (strcmp(branches[i], branches[j]) > 0)
            {
                swap_str(branches[i], branches[j]);
                swap_int(&indexes[i], &indexes[j]);
            }
        }
    }
}

/* Sorts people by age, displaying sorted information */
void sort_people_by_age(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount)
{
    int ages[2][MAX_PEOPLE];  /* Array to hold ages and corresponding indexes */
    int i, j;
    for (i = 0; i < peopleCount; ++i)   /* Save age and index */
    {
        ages[0][i] = convert_int(table[i][AGE_COL]);
        ages[1][i] = i;
    }

    sort_ages(ages, 0, peopleCount - 1);    /* Sort ages and indexes */

    printf("Sorted Individuals by Age:\n");
    printf("=============================\n");
    printf("NAME            SURNAME        AGE            BRANCH1         BRANCH2\n");
    printf("----            -------        ---            -------         -------\n");
    
    for (i = 0; i < peopleCount; ++i)
    {
        int index = ages[1][i]; /* Retrieve the index of the individual */
        for (j = 0; j < MAX_COL; ++j)
        {
            printf("%-20s", table[index][j]); /* Print details of the individual */
        }
        printf("\n");
    }
}

/* Sorts people by branch and then by age within each branch */
void sort_people_by_branch(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount)
{
    char branches[MAX_PEOPLE][MAX_STRING_LENGTH]; /* Array to hold branch names */
    int corr_indexes[MAX_PEOPLE]; /* Array to hold corresponding indexes */
    int i, j;
    for (i = 0; i < peopleCount; ++i)
    {
        strcpy(branches[i], table[i][FIRST_BRANCH_COL]);
        corr_indexes[i] = i;
    }
    sort_branches(branches, corr_indexes, peopleCount);

    int ages[2][MAX_PEOPLE]; /* Array to hold ages and indexes */
    for (i = 0; i < peopleCount; i++)
    {
        ages[0][i] = convert_int(table[corr_indexes[i]][AGE_COL]);
        ages[1][i] = corr_indexes[i];
    }

    /* Sort ages within the same branch */
    for (i = 0; i < peopleCount; ++i)
    {
        int index = ages[1][i]; /* Retrieve the index of the individual */
        int start = i;
        while (i < peopleCount - 1 && strcmp(branches[i], branches[i + 1]) == 0)
        {
            ++i;
        }
        int end = i;
        sort_ages(ages, start, end);        
    }    

    printf("Sorted Individuals by Branch:\n");
    printf("=============================\n");
    printf("NAME            SURNAME        AGE            BRANCH1         BRANCH2\n");
    printf("----            -------        ---            -------         -------\n");

    for (j = 0; j < peopleCount; ++j)
    {
        int index = ages[1][j];
        for (int k = 0; k < MAX_COL; ++k)
        {
            printf("%-20s", table[index][k]);
        }
        printf("\n");
    }
}

/* Filters people by branch */
void filter_people_by_branch(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount)
{
    char branch[MAX_STRING_LENGTH];
    printf("Enter the branch: ");
    scanf("%s", branch);

    int matches[MAX_PEOPLE]; /* Array to store indices of matching records */
    int i, j;
    for (i = 0; i < peopleCount; ++i)
        matches[i] = -1;

    for (i = 0; i < peopleCount; ++i)
    {
        for (j = FIRST_BRANCH_COL; j <= SECOND_BRANCH_COL; j++)
        {
            if (strstr(table[i][j], branch) != NULL)    /* Check if branch matches */
            {
                matches[i] = i;
            }
        }
    }
    int matchFound = 0;
    for (int i = 0; i < peopleCount; ++i) 
    {
        if (matches[i] != -1) 
        {
            matchFound = 1;
            break;
        }
    }
    if (!matchFound) 
    {
        printf("Not found for branch: \"%s\"\n", branch);
        return;
    }
    printf("Entries found for \"%s\"\n", branch);
    printf("NAME            SURNAME             AGE             BRANCH1         BRANCH2\n");
    printf("----            -------             ---             -------         -------\n");
    for (i = 0; i < peopleCount; ++i)
    {
        if (matches[i] != -1)
        {
            for (j = 0; j < MAX_COL; ++j)
                printf("%-20s", table[i][j]); 
            printf("\n");
        }
    }  
}

/* Parses a line from a file and fills a person's data */
void fillPerson(char person[][MAX_STRING_LENGTH], char *line)
{
    int semicolonNumber = 0;
    int i = 0, j = 0;
    while (line[i] != '\0')
    {
        if (line[i] == ',')
        {
            person[semicolonNumber][j] = '\0';  /* Null terminate the current information */
            semicolonNumber++;  /* Increment counter */
            j = 0;  /* Reset the counter for other information */
        }
        else
        {
            person[semicolonNumber][j] = line[i];   /* Copy contents */
            j++;
        }
        ++i;
    }
    person[semicolonNumber][j] = '\0';  /* Null terminate the last person */

    j = 0;
    while (semicolonNumber < MAX_COL - 1)   /* If there are missing information, fill them with space */
    {
        person[semicolonNumber][j] = ' ';
        semicolonNumber++;
    }
}

/* Reads data from a file and fills the table with person data */
int fillTable(char table[][MAX_COL][MAX_STRING_LENGTH], FILE *inp1_ptr)
{
    char line[MAX_LINE];
    int personCount = 0;
    while (fgets(line, sizeof(line)/sizeof(char), inp1_ptr))
    {
        if (personCount >= MAX_PEOPLE) 
        {
            printf("Maximum number of people exceeded.\n");
            return personCount;
        }
        fillPerson(table[personCount], line);
        personCount++;
    }
    return personCount;
}

/* Prints the table of people */
void printTable(char table[][MAX_COL][MAX_STRING_LENGTH], int totalPeople)
{
    int i, j;
    printf("NAME            SURNAME        AGE            BRANCH1         BRANCH2\n\n");
    printf("----            -------        ---            -------         -------\n\n");
    for (i = 0; i < totalPeople; ++i)
    {
        for (j = 0; j < MAX_COL; ++j)
            printf("%-20s", table[i][j]);    
        
        printf("\n");
    }   
}   

/* Filters and displays computer scientists who are not mathematicians */
void filter_people_by_profession(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount)
{
    int i, j;
    for (i = 0; i < peopleCount; ++i)
    {
        /* Find computer scientist who are not mathematicians */
        if (strstr(table[i][FIRST_BRANCH_COL], "COMPUTER SCIENCE") != NULL &&
            strstr(table[i][SECOND_BRANCH_COL], "MATHEMATICS") == NULL)
        {
            int j;
            for (j = 0; j < MAX_COL; j++)
                printf("%-20s", table[i][j]);
            
            printf("\n");
        }
    }
}

/* Displays a menu and processes user input */
void menu(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount)
{
    printf("********Menu*********\n");
    int choice, loopCheck = 1;
    do
    {
        printf("1) Sort and display all individuals by age\n");
        printf("2) Sort and individuals in the branch by age\n");
        printf("3) Show individuals by branch\n");
        printf("4) Show computer scientist who are not mathematicians\n");
        printf("5) Exit\n");
        scanf("%i", &choice);

        switch (choice)
        {
        case 1:
            sort_people_by_age(table, peopleCount);
            break;
        case 2:
            sort_people_by_branch(table, peopleCount);
            break;
        case 3:
            filter_people_by_branch(table, peopleCount);
            fflush(stdin);  /* Cleaning the stdin because \n is there */
            break;
        case 4:
            filter_people_by_profession(table, peopleCount);
            break;
        case 5:
            loopCheck = 0;
            break;
        default:
            printf("Invalid option enter again dumbass.\n");
            break;
        }
    } while (loopCheck);
    printf("Exiting...\n");
}

/* Removes new lines from table data */
void remove_newLines(char table[][MAX_COL][MAX_STRING_LENGTH], int peopleCount) 
{
    int i, j;
    for (i = 0; i < peopleCount; ++i)
    {
        for (j = FIRST_BRANCH_COL; j < MAX_COL; j++)
        {
            int lastChar = strlen(table[i][j]) - 1;
            if (lastChar >= 0 && table[i][j][lastChar] == '\n')
            {
                table[i][j][lastChar] = '\0';
            }
        }
    }
}

/* Main function, entry point of the program */
int main()
{
    FILE *input1_ptr = fopen("input1.txt", "r");
    FILE *input2_ptr = fopen("input2.txt", "r");
    if (input1_ptr == NULL || input2_ptr == NULL)
    {
        printf("Error opening files.\n");
        return 1;
    }

    char table[MAX_PEOPLE][MAX_COL][MAX_STRING_LENGTH];

    int totalPeople = fillTable(table, input1_ptr);
    remove_newLines(table, totalPeople);
    printTable(table, totalPeople);

    fclose(input1_ptr);
    fclose(input2_ptr);

    menu(table, totalPeople);

    return 0;
}
