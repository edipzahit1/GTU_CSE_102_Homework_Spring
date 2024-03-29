#include <stdio.h>

/*Needs some test but looks OK for now*/

#define MAX_NEW_COUNT 10
#define HEAD_LINE_SIZE 100
#define PATH_SIZE 100
#define FILE_NAME_LENGTH 20
#define FILE_ERROR_SENTINEL -1

/*these macros are for calculating the key*/
#define f(x) (float) (x * x * x) - (x * x) + 2
#define g(x) (float) (x * x)
#define gof(x) (float) g(f(x))

/*MENU FUNCTION*/
void menu();

/*Function to calculate the length of an array*/
int myStrLen(char *str);

/*Function to copy an array to another*/
void myStrCpy(char *to, char *from);

/*Function to append an array to another*/
void append(char *source, char *append);

/*Function to check if current new is read or not*/
int isRead(int newIndex, FILE *read_history);

/*Prints the current new*/
void printNew(FILE *new);

/*Function to open files*/
int openNewsFiles(FILE *news[], FILE *all_news_id, int newCount);

/*Function to extract the headlines of the news and print them*/
void extr_and_print_headlines(FILE *news[], int newCount);

void readANew(FILE *news[], FILE *read_history, int newCount);    /*Menu option 'a'*/
void listReadNews(FILE *read_history);                            /*Menu option 'b'*/
void printDecryptedInfo(FILE *news[], int newCount);              /*Menu option 'c'*/

void printDecryptedInfo(FILE *news[], int newCount)
{
    int choice;
    float secretNum = 0.0;
    char c;
    printf("Which news do you want to decrypt?: ");
    scanf("%i", &choice);

    if (choice <= 0 || choice > newCount)
    {
        printf("Invalid New Index.\n");
        return;
    }

    /*Going back to beginning of the new*/
    fseek(news[choice - 1], 0, SEEK_SET);
    while (fscanf(news[choice - 1], "%c", &c) != EOF)
    {
        printf("%c", c);    /*print the current character*/
        if (c == '#')   /*if it key value*/
        {
            fscanf(news[choice -1], "%c", &c);  /*get the number*/
            int magicNum = (int) c - '0';       /*convert to integer*/
            secretNum += gof(magicNum);         /*then add it up to sum*/
        }
    }
    printf("\n%i. experiment key = %.2f\n", choice, secretNum);
}

void listReadNews(FILE *read_history)
{
    int index;
    fseek(read_history, 0, SEEK_SET);
    /*Go through entire file and print history*/
    while (fscanf(read_history, "%i", &index) != EOF)
    {
        printf("%i. news is read.\n", index);
    }
}

void printNew(FILE *new)
{
    char c;
    fseek(new, 0, SEEK_SET);
    /*simply print the file*/
    while (fscanf(new, "%c", &c) != EOF)
    {
        printf("%c", c);   
    }
}

int isRead(int newIndex, FILE *read_history)
{
    int isRead = 0;
    int i;
    fseek(read_history, 0, SEEK_SET);
    /*go through the file and check for match*/
    while (fscanf(read_history, "%i", &i) != EOF)
    {
        if (i == newIndex)  isRead = 1;
    }
    return isRead;
}

void readANew(FILE *news[], FILE *read_history, int newCount)
{
    int choice, again;
    printf("Which news do you want to read?:");
    scanf("%i", &choice); //You can add interval conditions here

    if (choice <= 0 || choice > newCount)
    {
        printf("Invalid New Index.\n");
        return;
    }

    /*Check if it is already read*/
    if (isRead(choice, read_history))
    {
        /*If so ask again*/
        printf("This news is read. Do you want to read again? Yes(1) / No (0)");
        scanf("%i", &again);
        if (again)  /*If again print and save*/
        {
            printNew(news[choice - 1]);
            fprintf(read_history, "%i\n", choice);
        }
    }
    else    /*else print and save*/
    {
        printNew(news[choice - 1]);
        fprintf(read_history, "%i\n", choice);
    }
}

int myStrLen(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; ++i);
    return i;
}

int openNewsFiles(FILE *news[], FILE *all_news_id, int newCount)
{
    char path[PATH_SIZE];
    char fileName[FILE_NAME_LENGTH];
    int i;

    for (i = 0; i < newCount; i++)
    {
        /*Here we are first extracting the name of the file from all_news_id file and
        storing the file paths inside "path" array and then opening them*/
        fgets(fileName, sizeof(fileName), all_news_id);
        
        /*If there is a \n character at the end i am removing it to put the path*/
        if (fileName[myStrLen(fileName) - 1] == '\n') 
        {
            fileName[myStrLen(fileName) - 1] = '\0';
        }
        /*First copy the directory path*/
        myStrCpy(path, "CSE102_HW5\\CSE102_HW5 2\\news\\");
        /*Then append the file name*/
        append(path, fileName);
        /*Then append the .txt*/
        append(path, ".txt");
        /*Open the files 1 by 1*/
        news[i] = fopen(path, "r");
        if (news[i] == NULL)
        {
            printf("Error opening %i. file.(Cant find the file)\n", i + 1);
            return FILE_ERROR_SENTINEL;
        }
    }
}

void myStrCpy(char *to, char *from)
{
    int i = 0;
    while (from[i] != '\0')
    {
        to[i] = from[i];
        ++i;
    }
    to[i] = '\0';
}

void append(char *source, char *append)
{
    int i, j;
    for (i = 0; source[i] != '\0'; ++i);
    for (j = 0; append[j] != '\0'; ++i, ++j)
    {
        source[i] = append[j];
    }
    source[i] = '\0';
}

void extr_and_print_headlines(FILE *news[], int newCount)
{
    char headLine[HEAD_LINE_SIZE];  /*Buffer for storing headline*/
    int i;
    for (i = 0; i < newCount; ++i)
    {
        /*Storing the headline in headLine*/
        fgets(headLine, sizeof(headLine)/sizeof(char), news[i]);    
        printf("Title of %i. news: %s", i + 1, headLine);
    }
    printf("\n");
}

void menu()
{
    FILE *news[MAX_NEW_COUNT] = {NULL};  /*For news text*/
    FILE *all_news_id = fopen("CSE102_HW5\\CSE102_HW5 2\\all_news_id.txt", "r"); /*to extract names of files*/
    FILE *read_history = fopen("CSE102_HW5\\CSE102_HW5 2\\readed_news_id.txt", "a+"); /*to keep the read history*/

    if (read_history == NULL || all_news_id == NULL) return;

    /*
    * Here we are counting how many id's are inside all_news_id in order to count the news
    */
    char fileName[FILE_NAME_LENGTH];
    int newCount = 0;
    while (fgets(fileName, sizeof(fileName), all_news_id))
    {
        newCount++;
    }   
    /*Going back to beginning of the file*/
    fseek(all_news_id, 0, SEEK_SET);

    /*Open the files*/
    if (openNewsFiles(news, all_news_id, newCount) == FILE_ERROR_SENTINEL)
        return; /*Error has occured*/

    printf("**********Daily Press**********\n\n");
    printf("Todays new are listed for you :\n\n");
    
    /*Extracting and printing the headlines*/
    extr_and_print_headlines(news, newCount);

    int loopCheck = 1;
    char option;
    do
    {
        printf("What do you want to do?\n");
        printf("a. Read a new.\n");
        printf("b. List the read news.\n");
        printf("c. Get decrypted information from the news.\n");
        scanf(" %c", &option);

        switch (option)
        {
        case 'a':   readANew(news, read_history, newCount);
            break;
        case 'b':   listReadNews(read_history); 
            break;
        case 'c':   printDecryptedInfo(news, newCount);
            break;
        default :   printf("Invalid option.\n");
            break;
        }
        printf("\nDo you want to continue? (Yes (y), No (n))\n");
        scanf(" %c", &option);
        if (option == 'n' || option == 'N') loopCheck = 0;
    } while (loopCheck);
    
    /*Closing the opened files*/
    int i;
    for (i = 0; i < 5; i++)
    {
        fclose(news[i]);
    }
    fclose(all_news_id);
    fclose(read_history);
}

int main()
{
    menu();
}
