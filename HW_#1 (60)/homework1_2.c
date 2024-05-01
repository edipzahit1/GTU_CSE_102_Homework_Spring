#include <stdio.h>

int indexOfValue(char *alphabet, char value);

int main() 
{
    char alphabet[61] = "abcdefghijklmnopqrstuvwxyz(<=+)[*/]{>!-}?\\&|\%_;\"#.\'0123456789";
    int alphabetLength = sizeof(alphabet) / sizeof(char);
    long long int studentID = 220104004042;
    int sum = 0;

    /*getting the key, sum will be the key*/
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
        
        if (frontInput == '@') {
            char isSpace;
            scanf("%c", &isSpace);

            if (isSpace == ' ') {
                char c;
                printf("/* There are ");
                while (scanf("%c", &c) && ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'i')))
                {
                    int decryptedIndex = indexOfValue(alphabet, c) - sum;
                    if (decryptedIndex <= 0) {
                        decryptedIndex = alphabetLength + decryptedIndex;
                    }
                    printf("%c", alphabet[decryptedIndex]);
                }
                printf(" characters in this comment line.*/");
            }
            continue;
        }

        /*counting when to end
        i adjusted the checking characters according to my key*/
        if (frontInput == '>') {
            openCurlyBracesCount++;
        } 
        else if (frontInput == '?') {
            closeCurlyBracesCount++;
        }

        /*printing the decrypted code*/
        if ((indexOfValue(alphabet, frontInput) != -1)) {
            int decryptedIndex = indexOfValue(alphabet, frontInput) - sum;  
            while (decryptedIndex >= alphabetLength) {
                decryptedIndex -= alphabetLength;
            }
            printf("%c", alphabet[decryptedIndex]);
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
