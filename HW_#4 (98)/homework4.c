#include <stdio.h>

#define ERROR_CHAR '#'
#define ERROR_ID -1
#define FILE_OP_ERROR -2

float FinalAverage_102 = 0;
float MidtermAverage_102 = 0;
float FinalAverage_108 = 0;
float MidtermAverage_108 = 0;

/*Utility functions*/
char first_initial(FILE *id_fp, int id_toSearch);
char last_initial(FILE *id_fp, int id_toSearch);
int get_id_fi(FILE *id_fp, char first_initial);
int get_id_li(FILE *id_fp, char last_initial);
float average_grade(FILE *info_fp, int id);

/*Function to calculate class averages before displaying menu*/
void calc_whole_average(FILE *info_fp);

/*Returns the letter based on the semicolon destination input
Return ERROR_CHAR if there is no person with that id*/
char get_letter_with_sc(FILE *id_fp, int id_toSearch, int destination_semi);

/*Returns the id based on the semicolon destination input*/
int get_id_with_sc(FILE *id_fp, char letter_toSearch, int destination_semi);

/*Function to get the searched ID line. Returns 1 if found*/
int get_the_id_line(FILE *fp, int id_toSearch);

/*Function to display menu*/
void displayMenu(FILE *info_fp, FILE *id_fp);

/*Function to calculate GPA*/
float calculate_GPA(int midterm, int final, int course_id);

/*Functions for the menu*/
void numberOfClasses(FILE *info_fp);
int findAllWithSameRole(FILE *id_fp); /*Creates a temprorary file and removes it*/
void printCourseID(FILE *info_fp);
void printAllPassFail(FILE *info_fp);
int printAllPassFail_W_inital(FILE *id_fp, FILE *info_fp); /*Creates a temprorary file and removes it*/
void printStudentGrade(FILE *info_fp);
void printWholeClass(FILE *info_fp);

int get_id_with_sc(FILE *id_fp, char letter_toSearch, int destination_semi)
{
    int id = ERROR_ID; /*Initial value for return value*/
    char c;
    int semicolonCount = 0;
    do
    {
        fscanf(id_fp, "%i", &id);

        while (fscanf(id_fp, "%c", &c) != EOF && c != '\n' && semicolonCount < destination_semi)
        {
            if (c == ';') semicolonCount++;
        }
        /*If character does not found then get to the
        new line reset the counter and search next line*/
        if (c != letter_toSearch)
        {
            while (fscanf(id_fp, "%c", &c) != EOF && c != '\n');
            semicolonCount = 0;
        }
        else /*Found it*/
        {
            break;
        }
    } while (semicolonCount != destination_semi);

    return id;
}

int get_the_id_line(FILE *fp, int id_toSearch)
{
    int id, isFound = 0;
    char c;
    while (fscanf(fp, "%i", &id) != EOF)
    {
        if (id == id_toSearch) 
        {
            isFound = 1;
            break;
        }
        /*Get to the new line*/
        while (fscanf(fp, "%c", &c) != EOF && c != '\n');
    }
    return isFound;
}

char get_letter_with_sc(FILE *id_fp, int id_toSearch, int destination_semi)
{
    char c;
    int isFound = get_the_id_line(id_fp, id_toSearch);
    int semicolonCount = 0;
    if (isFound) 
    {
        while (semicolonCount < destination_semi)
        {
            fscanf(id_fp, "%c", &c);
            if (c == ';') semicolonCount++;
        }
        fscanf(id_fp, "%c", &c);
    }
    else 
    {
        c = ERROR_CHAR;
    }
    return c;
}

/*If you use this functions other than display menu loop it may cause 
error since file pointers are not set beginning during program anywhere else*/
char first_initial(FILE *id_fp, int id_toSearch) 
{
    return get_letter_with_sc(id_fp, id_toSearch, 1);
}

char last_initial(FILE *id_fp, int id_toSearch)
{
    return get_letter_with_sc(id_fp, id_toSearch, 2);
}

int get_id_fi(FILE *id_fp, char first_initial) 
{
    return get_id_with_sc(id_fp, first_initial, 1);
}

int get_id_li(FILE *id_fp, char last_initial) 
{
    return get_id_with_sc(id_fp, last_initial, 2);
}
/*----------------------------------------------------------*/

float average_grade(FILE *info_fp, int id_toSearch)
{
    char c;
    int isFound = get_the_id_line(info_fp, id_toSearch);

    int midterm1 = -1, midterm2 = -1, final = -1, course_id = -1;
    int midterm_average;

    fscanf(info_fp, "; %i ; %i ; %i ; %i", &midterm1, &midterm2, &final, &course_id);

    if (final == -1)
    {
        printf("No student with ID: %i\n", id_toSearch);
        return ERROR_ID;
    }
    if (course_id == -1) /*Indicating 108*/
    {
        course_id = final;
        final = midterm2;
        midterm_average = midterm1;
    }
    else /*Indicating 102*/
    {
        midterm_average = (midterm1 + midterm2) / 2;
    }

    /*Here i could not understand the difference between letter grade and number grade
    so since return type of the function is int i decided to switch the letter grade to number grade*/
    return calculate_GPA(midterm_average, final, course_id);
}

float calculate_GPA(int midterm, int final, int course_id)
{
    int finalAv = FinalAverage_108; /*initial values for averages*/
    int midAv = MidtermAverage_108;
    if (course_id == 102)
    {
        finalAv = FinalAverage_102;
        midAv = MidtermAverage_102;
    }
    float totalGPA, midtermGpa, finalGpa;

    if      (final < 40 && final < finalAv) finalGpa = 1.0;
    else if (final > 40 && final < finalAv) finalGpa = 2.5; 
    else if (final > 40 && final > finalAv) finalGpa = 3.0;
    else if (final < 90 && final > finalAv) finalGpa = 3.5;
    else                                    finalGpa = 4.0; 

    if      (midterm < 40 && midterm < midAv) midtermGpa = 1.0;
    else if (midterm > 40 && midterm < midAv) midtermGpa = 2.5; 
    else if (midterm > 40 && midterm > midAv) midtermGpa = 3.0;
    else if (midterm < 90 && midterm > midAv) midtermGpa = 3.5;
    else                                      midtermGpa = 4.0; 

    totalGPA = (midtermGpa + finalGpa) / 2;

    /*if      (totalGPA < 1.0) return 'F';
    else if (totalGPA < 2.5) return 'D';
    else if (totalGPA < 3.0) return 'C';
    else if (totalGPA < 3.5) return 'B';
    else                     return 'A';*/

    return totalGPA;
}

void numberOfClasses(FILE *info_fp) 
{
    int id_toSearch;
    printf("Enter instructor ID: ");
    scanf(" %i", &id_toSearch);

    int isFound = get_the_id_line(info_fp, id_toSearch);
    int totalCourse = -1;
    char c;
    
    /*Get the semicolon*/
    fscanf(info_fp, "%c", &c);

    fscanf(info_fp, "%i", &totalCourse);

    if (totalCourse != -1)
    {
        printf("Number of courses with the ID %i: %i\n", id_toSearch, totalCourse);
    }
    else 
    {
        printf("Instructor could not found.\n");
    }
}

int findAllWithSameRole(FILE *id_fp) 
{
    FILE *printed_ids_fp = fopen("printed_ids.txt", "a+");

    if (printed_ids_fp == NULL) return ERROR_ID;
    
    char initialLetter, c;
    int id, semiColonCount = 0;
    int department, printedID;
    printf("Enter the initial letter of the role: ");
    scanf(" %c", &initialLetter);

    while (fscanf(id_fp, "%i", &id) != EOF)
    {
        while (semiColonCount < 3)
        {
            fscanf(id_fp, "%c", &c);
            if (c == ';') semiColonCount++;
        }
        semiColonCount = 0;

        /*Getting the first letter of role*/
        fscanf(id_fp, "%c", &c);
        if (c == initialLetter)
        {
            /*Scan the temporary file and check if the current 
            id is in there, if so update the flag isPrinted*/
            int isPrinted = 0;
            fseek(printed_ids_fp, 0, SEEK_SET);
            while (fscanf(printed_ids_fp, "%i", &printedID) != EOF)
            {
                if (id == printedID)
                {
                    isPrinted = 1;
                    break;
                }
            }
            if (!isPrinted)
            {    
                /*Get to the department*/
                while (fscanf(id_fp, "%c", &c) != EOF && c != ';');
                fscanf(id_fp, "%i", &department);
                printf("ID: %i, Department: %i\n", id, department);

                /*Print the encountered ID to the temporatry file*/
                fprintf(printed_ids_fp, "%i\n", id);
            }
        }
        while (fscanf(id_fp, "%c", &c) != EOF && c != '\n');
    }
    /*Close the temporary file*/
    fclose(printed_ids_fp);

    /*Delete the temporary file*/
    if (remove("printed_ids.txt") == 0)
    {
        return 1; /*Indicating success*/
    }
    else 
    {
        return FILE_OP_ERROR; /*Indicating error*/
    }
}

void printCourseID(FILE *info_fp)
{
    int id_toSearch;
    printf("Enter user ID: ");
    scanf(" %i", &id_toSearch);

    char c;
    int isFound = get_the_id_line(info_fp, id_toSearch);
    
    int course_id = -1, midterm1, midterm2, final = -1;
    if (isFound)
    {
        int status_fscanf = fscanf(info_fp, " ; %i ; %i ; %i ; %i", &midterm1, &midterm2, &final, &course_id);
        if (status_fscanf == 4) /*Meaning we are in 102*/
        {
            printf("Student with ID: %i, Course ID: %i.\n", id_toSearch, course_id);
        } 
        else if (status_fscanf == 3) /*Meaning we are in 108*/
        {
            printf("Student with ID: %i, Course ID: %i.\n", id_toSearch, final); /*Final is course_id here*/
        }
        else 
        {
            printf("The person with ID: %i, is not a student.\n", id_toSearch);
        }
    }
    else
    {
        printf("Invalid ID.\n");
    }
}

void printAllPassFail(FILE *info_fp)
{
    int id, midterm1, midterm2, final, course_id, midtermAv;
    char c;
    float GPA;
    int status_fscanf;
    while ((status_fscanf = fscanf(info_fp, "%i ; %i ; %i ; %i ; %i", &id, &midterm1, &midterm2, &final, &course_id)) != EOF)
    {
        if (status_fscanf == 3) /*Indicating instructor*/
        {
            /*Get the end of line and check for another student*/
            while (fscanf(info_fp, "%c", &c) != EOF && c != '\n');
            continue;
        }
        else if (status_fscanf == 4) /*Indicating 108*/
        {
            course_id = final;
            final = midterm2;
            midtermAv = midterm1;
        }
        else 
        {
            midtermAv = (midterm1 + midterm2) / 2;
        }
        GPA = calculate_GPA(midtermAv, final, course_id);
        printf("Student with ID: %i, GPA: %.2f, Course ID: %i.\n", id, GPA, course_id);
        while (fscanf(info_fp, "%c", &c) != EOF && c != '\n');
    }
}

int printAllPassFail_W_inital(FILE *id_fp, FILE *info_fp) 
{
    FILE *printed_ids_fp = printed_ids_fp = fopen("printed_ids.txt", "a+");

    if (printed_ids_fp == NULL) return FILE_OP_ERROR;

    char initialLetter;
    printf("Enter the initial letter of the student: ");
    scanf(" %c", &initialLetter);
    
    int id, current_id;
    int midterm1, midterm2, final, course_id;
    float GPA;
    char c;

    /*Read each student ID from the info file*/ 
    while (fscanf(info_fp, "%i", &id) != EOF) 
    {
        int already_printed = 0;
        fseek(printed_ids_fp, 0, SEEK_SET); /*Reset file position to the beginning*/ 
        /*Check if the person already printed*/
        while (fscanf(printed_ids_fp, "%i", &current_id) != EOF) 
        {
            if (current_id == id) 
            {
                already_printed = 1;
                break;
            }
        }
        if (!already_printed)
        {
            /*Reset the file position of the ID file to start from the beginning*/ 
            fseek(id_fp, 0, SEEK_SET);
            /*Variable to store the first initial read from the ID file*/ 
            char id_initial;
            /*Search the entire ID file to find the initial letter for the current ID*/
            while (fscanf(id_fp, "%i ; %c", &current_id, &id_initial) != EOF) 
            {
                /* Check if the current ID matches the ID from the info file*/
                if (current_id == id) 
                {
                    /*Check if the first initial matches the user input*/ 
                    if (id_initial == initialLetter) 
                    {
                        int status_fscanf = fscanf(info_fp, " ; %i ; %i ; %i ; %i", &midterm1, &midterm2, &final, &course_id);
                        if (status_fscanf == 3) /*Indicating 108 student*/
                        {
                            GPA = calculate_GPA(midterm1, midterm2, final); /*Since midterm2 is final and final is course_id for 108*/
                            printf("Student with ID: %i, GPA: %.2f, Course ID: %i\n", id, GPA, final);
                        }
                        else if (status_fscanf == 4) /*Indicating 102 student*/ 
                        { 
                            GPA = calculate_GPA((midterm1 + midterm2) / 2, final, course_id);
                            printf("Student with ID: %i, GPA: %.2f, Course ID: %i\n", id, GPA, course_id);
                        }
                        /*Write the printed id in the printed file*/
                        fprintf(printed_ids_fp, "%i\n", id);
                        /*Break the inner loop since we printed the student*/
                        break;
                    }
                }
                /*For each iteration get to the en of line and take another id id_inital pair*/
                while (fscanf(id_fp, "%c", &c) != EOF && c != '\n');
            }
            /*Get to the end of line for info file*/
        }
        while (fscanf(info_fp, "%c", &c) != EOF && c != '\n');
    }

    /*Close the temporary file*/
    fclose(printed_ids_fp);

    /*Delete the temporary file*/
    if (remove("printed_ids.txt") == 0)
    {
        return 1; /*Indicating success*/
    }
    else 
    {
        return FILE_OP_ERROR; /*Indicating error*/
    }
}

void printStudentGrade(FILE *info_fp)
{
    int id, current_id, isFound = 0;
    char c;
    float GPA;
    printf("Enter ID: ");
    scanf(" %i", &id);

    int midterm1, midterm2, final, course_id;
    int status_fscanf;
    while (fscanf(info_fp, "%i", &current_id) != EOF)
    {
        if (id == current_id)
        {
            status_fscanf = fscanf(info_fp, " ; %i ; %i ; %i ; %i", &midterm1, &midterm2, &final, &course_id);            
            if (status_fscanf == 3) /*Indicating 108 student*/
            {
                GPA = calculate_GPA(midterm1, midterm2, final); /*Since midterm2 is final and final is course_id for 108*/
                printf("Student with ID: %i, GPA: %.2f, Course ID: %i\n", id, GPA, final);
            }
            else if (status_fscanf == 4) /*Indicating 102 student*/ 
            { 
                GPA = calculate_GPA((midterm1 + midterm2) / 2, final, course_id);
                printf("Student with ID: %i, GPA: %.2f, Course ID: %i\n", id, GPA, course_id);
                isFound = 1;
            }
            else /*Indicating instructor*/
            {
                printf("The ID: %i, not belongs to a student.\n", id);
            }
            break;
        }
        while (fscanf(info_fp, "%c", &c) != EOF && c != '\n');
    }
    if (!isFound)
    {
        printf("Invalid ID.\n");
    }
}

void printWholeClass(FILE *info_fp)
{
    int course_id;
    printf("Enter the course ID: ");
    scanf(" %i", &course_id);

    int id, midterm1, midterm2, final, current_course_id;
    float GPA;
    int count = 0;
    float grade = 0, sum = 0;
    int status_fscanf;
    /*Reset file pointer to the beginning*/ 
    fseek(info_fp, 0, SEEK_SET);

    /*Iterate through each record in the file*/ 
    while ((status_fscanf = fscanf(info_fp, "%i ; %i ; %i ; %i ; %i", &id, &midterm1, &midterm2, &final, &current_course_id)) != EOF)
    {
        if (status_fscanf == 4 && final == course_id) /*Meaning 108*/
        {
            GPA = calculate_GPA(midterm1, midterm2, final);
        }   
        else if (status_fscanf == 5 && course_id == current_course_id) /*Meaning 102*/
        {
            GPA = calculate_GPA((midterm1 + midterm2) / 2, final, course_id);
        }
        else 
        {
            continue;
        }
        printf("Student ID: %i, GPA: %.2f\n", id, GPA);

        count++;
        sum += GPA;
    }

    /*Checking if any students were found for the given course ID*/
    if (count == 0)
    {
        printf("No students found for course ID: %i\n", course_id);
    }
    else
    {
        /*Calculate and print the average GPA*/
        float average = (float)sum / count;
        printf("Average GPA for course ID %i: %.2f\n", course_id, average);
    }
}

void displayMenu(FILE *info_fp, FILE *id_fp)
{
    char option;
    int loopCheck = 1;
    do
    {
        /*Setting the files at the beginning for each iteration*/
        fseek(info_fp, 0, SEEK_SET);
        fseek(id_fp, 0, SEEK_SET);

        printf("'p': Print all the users pass or fail.\n");
        printf("'n': Print only the user with a specific first initial.\n");
        printf("'g': Calculate the GPA of a given student.\n");
        printf("'c': Print whether each user passed or failed the same class. \n");
        printf("'t': Print the number of classes for the instructor.\n");
        printf("'d': Print the department of all persons according to the role.\n");
        printf("'l': Print the course_id of a given student.\n");
        printf("'e': Quit the program.\n");
        scanf(" %c", &option);

        switch (option)
        {
        case 'p':
            printAllPassFail(info_fp);
            break;
        case 'n':
            if (printAllPassFail_W_inital(id_fp, info_fp) == FILE_OP_ERROR)
                loopCheck = 0; /*If it returns an error exit the programme*/
            break;
        case 'g':
            printStudentGrade(info_fp);
            break;
        case 'c':
            printWholeClass(info_fp);
            break;
        case 't': 
            numberOfClasses(info_fp);
            break;
        case 'd':
            if (findAllWithSameRole(id_fp) == FILE_OP_ERROR) 
                loopCheck = 0; /*If it returns an error exit the programme*/
            break;
        case 'l':
            printCourseID(info_fp);
            break;
        case 'e': 
            loopCheck = 0;
            break;
        default: printf("Invalid option. Enter again.\n");
            break;
        }
        /*If user not enters 'e' before*/
        if (loopCheck)
        {
            printf("Do you want to continue? (yes: y, no: n): ");
            scanf(" %c", &option);
            if (option == 'n' || option == 'N') loopCheck = 0;
        }
    } while (loopCheck);
    printf("Exiting...");   
}

void calc_whole_average(FILE *info_fp)
{
    fseek(info_fp, 0, SEEK_SET);

    int id, midterm1, midterm2, final = -1, course_id = -1;
    int sumFinal_102 = 0, sumFinal_108 = 0;
    int sumMidterm_102 = 0, sumMidterm_108 = 0;
    int count102 = 0, count108 = 0;

    char c;
    int status_of_fscanf;
    while ((status_of_fscanf = fscanf(info_fp, "%i ; %i ; %i ; %i ; %i", &id, &midterm1, &midterm2, &final, &course_id)) != EOF)
    {
        if (status_of_fscanf == 5) /*Meaning we are in 102*/
        {
            sumFinal_102 += final;
            sumMidterm_102 += (midterm1 + midterm2) / 2;
            count102++;
        }
        else if (status_of_fscanf == 4) /*Meaning we are in 108*/
        {
            sumFinal_108 += midterm2; /*Midterm2 is final for 108*/
            sumMidterm_108 += midterm1;
            count108++;
        }
    }
    
    FinalAverage_102 = (float) sumFinal_102 / count102;
    FinalAverage_108 = (float) sumFinal_108 / count108;
    MidtermAverage_102 = (float) sumMidterm_102 / count102;
    MidtermAverage_108 = (float) sumMidterm_108 / count108;
}

int main()  
{
    FILE *info_fp = fopen("first.txt", "r");
    
    if (info_fp == NULL) return 1;

    FILE *id_fp = fopen("second.txt", "r");

    if (id_fp == NULL) return 1;     
    /*First calculate the average scores for each course*/
    calc_whole_average(info_fp);
    /*then display menu*/
    displayMenu(info_fp, id_fp);

    fclose(info_fp);
    fclose(id_fp);
}
