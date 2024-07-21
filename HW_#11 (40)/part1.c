#include <stdio.h>

int numPathsHome(int street, int avenue)
{
    if (street == 1 && avenue == 1)
        return 1;

    int leftPath = 0, upPath = 0;
    if (avenue > 1)
        leftPath = numPathsHome(street, avenue - 1);
    if (street > 1)
        upPath = numPathsHome(street - 1, avenue);
    return upPath + leftPath;
}

int main()
{
    int street, avenue;
    printf("Enter the street number:\n");
    scanf("%i", &street);
    printf("Enter the avenue number:\n");
    scanf("%i", &avenue);
    printf("Number of paths: %i\n", numPathsHome(street, avenue));
}