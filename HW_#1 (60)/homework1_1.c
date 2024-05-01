#include <stdio.h>

//FIX THIS CODE BEFORE PUSHING

int indexOfValue(char *alphabet, char value);

int main() 
{
    char alphabet[61] = "abcdefghijklmnopqrstuvwxyz(<=+)[*/]{>!-}?\\&|\%_;\"#.\'0123456789";
    int alphabetLength = sizeof(alphabet) / sizeof(char);
    long long int studentID = 220104004042;
    int sum = 0;

    /*getting the student id*/
    while (studentID > 0 || sum > 9) 
    {
        /*if there is no digits and sum is greater than 9  
        set sum zero and assign studentID to current sum*/
        if (studentID == 0) {   
            studentID = sum;
            sum = 0;
        }
        sum = sum + studentID % 10; /*get the last digit*/
        studentID = studentID / 10; /*remove the last digit*/
    }

    int openCurlyBracesCount = 0;
    int closeCurlyBracesCount = 0;
    char frontInput, backInput;
    int commentCharacterCount = 0;
    int inComment = 0;

    while (openCurlyBracesCount == 0 || closeCurlyBracesCount != openCurlyBracesCount) 
    {

        scanf("%c", &frontInput);

        /*here i use backInput only for comment lines*/
        if (frontInput == '/' && (scanf("%c", &backInput) == 1) && backInput == '*') {

            /*checking whether we are still inside the comment
            and increment counter*/
            inComment = 1;
            while (scanf("%c", &frontInput) == 1) 
            {
                if (frontInput == '*' && (scanf("%c", &backInput) == 1) && backInput == '/') {
                    inComment = 0;
                    break;
                }
                /*Dont count spaces*/
                if (backInput == ' ' || backInput == '\t' || frontInput == ' ' || frontInput == '\t') {
                    continue;
                } 
                commentCharacterCount++;
            }
            
            int reversed = 0;
            while (commentCharacterCount > 0) /*Reverse the number*/
            {         
                int digit = commentCharacterCount % 10;
                reversed = reversed * 10 + digit;
                commentCharacterCount /= 10;
            }

            while (reversed > 0)  /*print the total character*/
            {     
                int digit = reversed % 10;
                if ((indexOfValue(alphabet, frontInput) != -1)) {
                    int encryptedIndex = indexOfValue(alphabet, digit + '0') + sum;  /*getting the index*/
                    /*if it exceeds our alphabet we set the correct position for index
                    with decrementing length of alphabet*/
                    while (encryptedIndex >= alphabetLength) 
                    {
                        encryptedIndex -= alphabetLength;
                    }
                    printf("@ %c", alphabet[encryptedIndex]);
                } 
                reversed /= 10;
            }   
            commentCharacterCount = 0;  /*set character 0 for another comment line*/
        }

        /*counting when to end*/
        if (frontInput == '{') {
            openCurlyBracesCount++;
        } 
        else if (frontInput == '}') {
            closeCurlyBracesCount++;
        }

        /*I am doing this because after getting comment line
         * and / characters will be in process respectively
        and this conditions skip them*/
        if (frontInput == '*' || frontInput == '/') {
            inComment = 1;
        } else {
            inComment = 0;
        }

        if (inComment) {
            continue;
        }
        
        /*printing the encrypted code*/
        if ((indexOfValue(alphabet, frontInput) != -1)) {
            int encryptedIndex = indexOfValue(alphabet, frontInput) + sum;  /*getting the index*/
            /*if it exceeds our alphabet we set the correct position for index
            with decrementing length of alphabet*/
            while (encryptedIndex >= alphabetLength) 
            {
                encryptedIndex -= alphabetLength;
            }
            printf("%c", alphabet[encryptedIndex]);
        } 
        else {
            printf("%c", frontInput);   /*if it is not in alphabet simply print it*/
        }        
    }

}

/*returns -1 if character is not in alphabet, returns index otherwise*/
int indexOfValue(char *alphabet, char value) {
    int i = 0;
    while (alphabet[i] != '\0') {
        if (alphabet[i] == value)
            return i;
        ++i;
    }
    return -1;
}
