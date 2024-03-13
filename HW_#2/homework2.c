#include <stdio.h>

int main() 
{
    /*opening the files*/
    FILE *dataPtr = fopen("data.txt", "r");
    /*return 1 from main if a file ccant open*/
    if (dataPtr == NULL) return 1;

    FILE *testptr = fopen("test.txt", "r");

    if (testptr == NULL) return 1;

    FILE *testfinalPtr = fopen("test-final.txt", "r");

    if (testfinalPtr == NULL) return 1;

    int xCoordinate, yCoordinate, class;
    int max_x1 = 0, max_y1 = 0, min_x1 = 0, min_y1 = 0;
    int max_x0 = 0, max_y0 = 0, min_x0 = 0, min_y0 = 0;
    int real_label_r1, real_label_r2, real_label_r3, real_label_r4;

    int isFound_0 = 0, isFound_1 = 0;

    int error = 0, min_error = -1;
    int predicted_class;
    int seperator_X, seperator_Y;
    int real_sep_X = 0, real_sep_Y = 0;

    int correctClassCount = 0;
    int allPointCount = 0;

    int loopCheck = 1;
    char choice;
    do
    {
        printf("1) Display \"data.txt\"\n");
        printf("2) Train model\n");
        printf("3) Test model\n");
        printf("4) Enter 'q' for exit\n");
        scanf("%c", &choice);

        switch (choice)
        {
        case '1':
            /*Going back to beginning of the file */
            fseek(dataPtr, 0, SEEK_SET);
            printf("X Y Class\n");
            /*scanning all file and printing*/
            while (fscanf(dataPtr, "%i%i%i", &xCoordinate, &yCoordinate, &class) != EOF) {
                printf("%i %i %i\n", xCoordinate, yCoordinate, class);
            }
            break;
        case '2':
            fseek(dataPtr, 0, SEEK_SET);
            /*Here i am getting the initial values for min and max for each class*/
            while (fscanf(dataPtr, "%i%i%i", &xCoordinate, &yCoordinate, &class) != EOF && (!isFound_0 || !isFound_1))
            {
                if (class) {
                    isFound_1 = 1;
                    max_x1 = xCoordinate;
                    min_x1 = xCoordinate;
                    max_y1 = yCoordinate;
                    min_y1 = yCoordinate;
                } else {
                    isFound_0 = 1;
                    max_x0 = xCoordinate;
                    min_x0 = xCoordinate;
                    max_y0 = yCoordinate;
                    min_y0 = yCoordinate;
                }
            }

            /*going back to beginning of the file to scan all file*/
            fseek(dataPtr, 0, SEEK_SET);

            /*scanning the file and determining the min and max values*/
            while (fscanf(dataPtr, "%i%i%i", &xCoordinate, &yCoordinate, &class) != EOF)
            {
                if (class) {
                    if      (xCoordinate > max_x1)  max_x1 = xCoordinate;
                    else if (xCoordinate < min_x1)  min_x1 = xCoordinate; 
                    
                    if      (yCoordinate > max_y1)  max_y1 = yCoordinate;
                    else if (yCoordinate < min_y1)  min_y1 = yCoordinate;
                } else {
                    if      (xCoordinate > max_x0)  max_x0 = xCoordinate;
                    else if (xCoordinate < min_x0)  min_x0 = xCoordinate; 
                    
                    if      (yCoordinate > max_y0)  max_y0 = yCoordinate;
                    else if (yCoordinate < min_y0)  min_y0 = yCoordinate;
                }
            }
            
            printf("Class 0: X in [%i %i], Y in [%i %i]\n",min_x0, max_x0, min_y0, max_y0);
            printf("Class 1: X in [%i %i], Y in [%i %i]\n",min_x1, max_x1, min_y1, max_y1);

            /*going back to beginning of data.txt file*/
            fseek(dataPtr, 0, SEEK_SET);

            /*Here i am setting the initial and final values for the loop after this*/
            int minX = min_x0 < min_x1 ? min_x0 : min_x1;
            int maxX = max_x0 > max_x1 ? max_x0 : max_x1;
            int minY = min_y0 < min_y1 ? min_y0 : min_y1;
            int maxY = max_y0 > max_y1 ? max_y0 : max_y1;
            int seperator_X, seperator_Y, label_r1, label_r2, label_r3, label_r4;

            for (seperator_X = minX; seperator_X < maxX; seperator_X++) 
            {
                for (seperator_Y = minY; seperator_Y < maxY; seperator_Y++) 
                {
                    /*these 4 loops are for region labeling. Testing all possible 0-1 pairs*/
                    for (label_r1 = 0; label_r1 <= 1; label_r1++) 
                    {
                        for (label_r2 = 0; label_r2 <= 1; label_r2++) 
                        {
                            for (label_r3 = 0; label_r3 <= 1; label_r3++) 
                            {
                                for (label_r4 = 0; label_r4 <= 1; label_r4++) 
                                {
                                    int error = 0;
                                    fseek(dataPtr, 0, SEEK_SET);
                                    
                                    /*scanning the file and predicting their class label*/
                                    while (fscanf(dataPtr, "%i%i%i", &xCoordinate, &yCoordinate, &class) != EOF) {
                                        int predicted_class;
                                        if (xCoordinate < seperator_X) {
                                            if (yCoordinate > seperator_Y) {
                                                predicted_class = label_r1;
                                            } else {
                                                predicted_class = label_r2;
                                            }
                                        } else {
                                            if (yCoordinate > seperator_Y) {
                                                predicted_class = label_r3;
                                            } else {
                                                predicted_class = label_r4;
                                            }
                                        }
                                        /*increment error when encountering and missprediction*/
                                        if (predicted_class != class) {
                                            error++;
                                        }
                                    }

                                    /* Update best separator coordinates and labels if error is lower*/
                                    if (min_error == -1 || error < min_error) {
                                        real_sep_X = seperator_X;
                                        real_sep_Y = seperator_Y;
                                        real_label_r1 = label_r1;
                                        real_label_r2 = label_r2;
                                        real_label_r3 = label_r3;
                                        real_label_r4 = label_r4;
                                        min_error = error;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            printf("Seperator coordinates: X = %i, Y = %i\n"
                "Training error of the model = %i\n"
                "Labels of the regions: R1 = %i, R2 = %i, R3 = %i, R4 = %i\n\n", 
                real_sep_X, real_sep_Y, min_error, real_label_r1, real_label_r2, real_label_r3, real_label_r4);

            break;
        case '3':
            /*going back to begining of the file*/
            fseek(testptr, 0, SEEK_SET);
            printf("Test.txt predictions:\nX Y Predicted Class\n");
            while (fscanf(testptr, "%i%i%i", &xCoordinate, &yCoordinate, &class) != EOF)
            {
                /*determining predicted classes for every region and class label*/
                if (xCoordinate < real_sep_X) {
                    if (yCoordinate > real_sep_Y) {
                        predicted_class = real_label_r1;
                    } else {
                        predicted_class = real_label_r2;
                    }
                } else {
                    if (yCoordinate > real_sep_Y) {
                        predicted_class = real_label_r3;
                    } else {
                        predicted_class = real_label_r4;
                    }
                }
                printf("%i %i %i\n", xCoordinate, yCoordinate, predicted_class);
                /*counting the error*/
                if (predicted_class == class) {
                    correctClassCount++;
                }
                allPointCount++;
            }
            printf("Success = %.2f\n", (float) correctClassCount / allPointCount);

            fseek(testfinalPtr, 0, SEEK_SET);

            int xValue, yValue;
            printf("test-final.txt file \nX Y Predicted Class\n");
            while (fscanf(testfinalPtr, "%i%i", &xValue, &yValue) != EOF)
            {
                if (xValue < real_sep_X) {
                    if (xValue > real_sep_Y) {
                        predicted_class = real_label_r1;
                    } else {
                        predicted_class = real_label_r2;
                    }
                } else {
                    if (xValue > real_sep_Y) {
                        predicted_class = real_label_r3;
                    } else {
                        predicted_class = real_label_r4;
                    }
                }
                printf("%i %i %i\n", xValue, yValue, predicted_class);
            }

            break;
        case 'q':
            loopCheck = 0;
            break;
        default:
            printf("You have entered an invalid option enter again.\n");
            break;
        }
        /*cleaning the buffer because \n character still inside stdin*/
        fflush(stdin);
    } while (loopCheck);

    fclose(testptr);
    fclose(dataPtr);
    return 0;
}