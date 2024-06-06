#include <stdio.h>
#include "customDict.h"

int main() {
    CustomDict* myDict = create_dict();

    printf("After reading first file:\n");
    if (!read_csv(myDict, "data1.csv")) 
        return 1;

    print_dict(myDict);

    printf("After reading second file:\n");
    if (!read_csv(myDict, "data2.csv")) 
        return 1;

    print_dict(myDict);

    Value additionalValues[3] = {{.char_V = 'A'}, {.char_V = 'm'}, {.char_V = 'B'}};
    add_item(myDict, "Char Key", additionalValues, CHAR, 3);

    Value* searchedValue = search_item(myDict, "Float Key");
    if (searchedValue != NULL) 
        printf("Searched Value: %f\n", searchedValue->float_V);

    Value updatedVal;
    updatedVal.float_V = 6.28;
    set_value(myDict, "Float Key", &updatedVal, FLOAT, 1);

    print_dict(myDict);
    
    printf("After sorting:\n");
    sort_dict(myDict);
    print_dict(myDict);

    delete_item(myDict, "Integer Key");
    print_dict(myDict);

    free_dict(myDict);

    return 0;
}