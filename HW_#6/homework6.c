#include <stdio.h>

#define MAX_LENGTH 500
#define MAX_ADDITION 5
#define MIN_VALUE 50
#define MAX_VALUE 200

void calculate_mode(int *frequencies, int A, int B);
float calculate_median(const int *frequencys, int A, int B);
float calculate_average(const int *frequencies, int A, int B);
int find_size_of_array(const int *A);
void draw_histogram(int *requencys, const int *number_array, int size, int A, int B);
void calculate_frequencys(int *frequencys, const int *number_array);
int findMaxFreq(const int *frequencys, int A, int B);
int addNumbers(int *number_array, const int *additionalNums, int number_array_size, int additional_nums_size);

int addNumbers(int *number_array, const int *additionalNums, int number_array_size, int additional_nums_size)
{
    int i, j = 0;
    for (i = number_array_size; i < number_array_size + additional_nums_size; ++i, ++j)
    {
        number_array[i] = additionalNums[j]; /*Append the additional numbers to end of number_array*/
    }
    number_array[i] = -1;   /*Indicating the end of number_array*/

    return i; /*New size*/
}

int findMaxFreq(const int *frequencys, int A, int B)
{
    int i, max = frequencys[A];        /*Assign first element of interval*/
    for (i = A + 1; i <= B; ++i)       /*Iterate through all elements inside interval*/
    {
        if (max < frequencys[i]) max = frequencys[i];   /*update max*/
    }
    return max;
}

void calculate_frequencys(int *frequencys, const int *number_array)
{
    /*Set the frequencys to 0*/
    int k;
    for (k = 0; k < MAX_LENGTH; ++k)
    {
        frequencys[k] = 0;
    }
    /*Calculate them*/
    int i;
    for (i = 0; i < MAX_LENGTH; ++i)
    {
        int number = number_array[i];
        frequencys[number]++;
    }
}

void draw_histogram(int *frequencys, const int *number_array, int size, int A, int B)
{
    calculate_frequencys(frequencys, number_array);

    int maxFreq = findMaxFreq(frequencys, A, B);    /*This indicates amont of rows*/

    int i, j;
    for (i = maxFreq; i > 0; --i)    /*Iterate through max row to min*/
    {
        for (j = A; j <= B; ++j)     /*iterate thtough frequencys array*/
        {
            if (frequencys[j] == i)  /*if at top*/
            {   
                printf("*");         /*print the star and*/
                frequencys[j]--;     /*decrease the frequency in order to print it the row below*/
            }
            else printf(" ");        /*print space if it not star*/
        }
        printf("\n");                /*go to next line*/
    }
    /*Set the frequencies back on track since we broke it above*/
    calculate_frequencys(frequencys, number_array);
}

int find_size_of_array(const int *A)
{
    int i, count = 0;
    for (i = 0; A[i] != -1; ++i) count++;
    return count;   /*not including -1*/
}

float calculate_average(const int *frequencies, int A, int B)
{
    int i, sum = 0, total_num = 0;
    float average;
    for (i = A; i <= B; i++)
    {
        sum += frequencies[i] * i;      /*sum of contents*/
        total_num += frequencies[i];    /*sum of total number of elements*/
    }
    average = (float) sum / total_num;  /*find average*/
    return average;
}

float calculate_median(const int *frequencies, int A, int B)
{
    int temp[MAX_LENGTH];
    int i, j, k = 0;
    /*Store the interval in a temporary array*/
    for(i = A; i <= B; ++i)
    {
        for (j = 0; j < frequencies[i]; ++j)
        {
            temp[k] = i;
            ++k;
        }
    }
    int size = k;
    if  (size % 2 == 0) return (temp[size / 2 - 1] + temp[size / 2]) / 2.0; /*If even*/
    else                return temp[size / 2];                              /*If odd*/
}

void calculate_mode(int *frequencies, int A, int B)
{
    int maxFreq = findMaxFreq(frequencies, A, B);   /*Find the maximum frequensy value*/
    int i;
    for (i = A; i <= B; i++)    /*Iterate through all elements*/
    {
        if (frequencies[i] == maxFreq)  /*if current is equal to maxFreq print it*/
        {
            printf("%i ", i);
        }
    }
}

int main()
{   
    int frequencys[MAX_LENGTH] = {0};
    int number_array[MAX_LENGTH] = {
        50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 26, 77, 78, 79, 80, 80, 82, 83, 14, 15, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 26, 97, 98, 99, 100, 101, 102, 103, 104, 115, 106, 107, 108, 109,
        110, 111, 112, 113, 114, 125, 116, 117, 118, 119, 120, 121, 122, 123, 124, 121, 126, 127, 128, 129, 130, 131, 132, 133, 134, 132, 136, 137, 138, 139, 140, 141, 142, 143, 143, 143, 146, 147, 148, 149, 150, 151, 152, 123, 154, 155, 156, 157, 158, 159,
        160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 113, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 163, 124, 191, 126, 127, 158, 199, 200, -1
    };

    int A, B;

    /*Loop for getting the valid A and B values*/
    printf("Enter A and B values (A >= 50 and B <= 200 and A < B): ");
    for (scanf("%i %i", &A, &B); (A < MIN_VALUE || B > MAX_VALUE || A >= B); scanf("%i %i", &A, &B))
    {
        printf("Invalid A and B values. Enter again!! (A >= 50 and B <= 200 and A < B)\n");
    }

    int size = find_size_of_array(number_array);            /*Finding the size of number_array*/

    draw_histogram(frequencys, number_array, size, A, B);   /*draw histogram*/

    int choice;
    int additionalNums[MAX_ADDITION];
    printf("Would you like to add new numbers? (Press 1 for yes, Press 0 for no): ");
    scanf(" %i", &choice);

    if (choice)
    {
        printf("\nEnter numbers: ");
        int numberToAdd, i = 0;
        /*Get the numbers*/
        do
        {
            scanf(" %i", &numberToAdd);
            additionalNums[i] = numberToAdd;
            ++i;
            if (i >= MAX_ADDITION) break;   /*break if reached maximum count*/
        } while (numberToAdd != -1);

        int newSize = addNumbers(number_array, additionalNums, size, i);    /*append the numbers*/

        draw_histogram(frequencys, number_array, newSize, A, B);    /*draw histogram*/
    }

    printf("Average: %.2f\n", calculate_average(frequencys, A, B));
    printf("Median: %.2f\n", calculate_median(frequencys, A, B));
    calculate_mode(frequencys, A, B);
}