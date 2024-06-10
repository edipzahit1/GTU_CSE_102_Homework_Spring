#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 5

typedef enum
{
    INT,
    DOUBLE,
    FLOAT,
    CHAR
} ValueType;

typedef union
{
    int int_V;
    double double_V;
    float float_V;
    char char_V;
} Value;

typedef struct
{
    char *key;
    Value *values;
    ValueType type;   /* i have added this in order to print properly */
    int value_count;  /* to determine how many values in a item       */
} Item;

typedef struct
{
    Item *items;
    int capacity;
    int curr_size;
} CustomDict;

CustomDict *create_dict();
int read_csv(CustomDict *dict, const char *filename);
ValueType getValueType(const char *type);
void free_dict(CustomDict *dict);
int getItemCount(FILE *ptr);
int getValueCount(char *line);
void print_dict(const CustomDict *dict);
void fillValues(Value *values, ValueType vt, char *p);
void add_item(CustomDict* dict, char* key, Value* value, ValueType vt, int value_count);
void resize_dict(CustomDict *dict);
void delete_item(CustomDict *dict, char *key);
void set_value(CustomDict *dict, char *key, Value *values, ValueType vt, int value_count);
Value *search_item(CustomDict *dict, char *key);
void sort_dict(CustomDict *dict);
void skip_whitespace(char **p);

void free_dict(CustomDict *dict)
{
    int i;
    for (i = 0; i < dict->curr_size; i++)
    {
        free(dict->items[i].key);
        free(dict->items[i].values);
    }
    free(dict->items);
    free(dict);
}

CustomDict *create_dict()
{
    CustomDict *dict = (CustomDict *)malloc(sizeof(CustomDict));
    if (dict == NULL)
    {
        printf("Memory allocation error.\n");
        return NULL;
    }
    dict->capacity = INITIAL_CAPACITY;
    dict->items = (Item *)malloc(sizeof(Item) * dict->capacity);
    dict->curr_size = 0;
    return dict;
}

void skip_whitespace(char **p) 
{
    while (**p == ' ' || **p == '\t' || **p == '\n' || **p == '\r') 
        (*p)++;
}

int read_csv(CustomDict *dict, const char *filename)
{
    if (dict == NULL || filename == NULL)
        return 0;

    FILE *ptr = fopen(filename, "r");
    if (ptr == NULL)
        return 0;

    char line[100];
    while (fgets(line, sizeof(line), ptr) != NULL)
    {
        char type[10], key[50];

        /* Parse the type */ 
        char *p = line;
        skip_whitespace(&p);
        char *typePtr = type;
        while (*p != ',' && *p != '\0')
        {
            *typePtr++ = *p++;
        }
        *typePtr = '\0';
        if (*p == ',') p++; /* Skip comma */

        /* Parse the key*/
        skip_whitespace(&p);
        char *keyPtr = key;
        while (*p != ',' && *p != '\0')
        {
            *keyPtr++ = *p++;
        }
        *keyPtr = '\0';
        if (*p == ',') p++; /* Skip comma */

        if (search_item(dict, key) != NULL) /* Check if item already exist with that key */
            continue;

        ValueType vt = getValueType(type);
        int value_count = getValueCount(p);

        Value *values = (Value *)malloc(sizeof(Value) * value_count);
        fillValues(values, vt, p);

        if (dict->curr_size >= dict->capacity)
            resize_dict(dict);

        dict->items[dict->curr_size].key = (char *)malloc(strlen(key) + 1);
        strcpy(dict->items[dict->curr_size].key, key);
        
        dict->items[dict->curr_size].values = values;
        dict->items[dict->curr_size].type = vt;
        dict->items[dict->curr_size].value_count = value_count;
        dict->curr_size++;
    }
    fclose(ptr);
    return 1;
}

void fillValues(Value *values, ValueType vt, char *p) 
{
    int valueCount = 0;
    while (*p != '\0') 
    {
        /* Extract the value */
        char *valueStr = p;
        while (*p != ',' && *p != '\0') p++;
        if (*p == ',') *p++ = '\0';

        switch (vt) {
            case INT:
                values[valueCount].int_V = atoi(valueStr);
                break;
            case FLOAT:
                values[valueCount].float_V = atof(valueStr);
                break;
            case DOUBLE:
                values[valueCount].double_V = atof(valueStr);
                break;
            case CHAR:
                while (*valueStr == ' ' || *valueStr == '\t') valueStr++;
                values[valueCount].char_V = *valueStr;
                break;
        }
        valueCount++;
    }
}

int getItemCount(FILE *ptr)
{
    int itemCount = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), ptr) != NULL)
        itemCount++;
    fseek(ptr, 0, SEEK_SET);
    return itemCount;
}

int getValueCount(char *line)
{
    int valCount = 0;
    while (*line != '\0')
    {
        if (*line == ',')
            valCount++;
        line++;
    }
    return valCount + 1;
}

ValueType getValueType(const char *type)
{
    if (strcmp(type, "int") == 0)
        return INT;
    else if (strcmp(type, "float") == 0)
        return FLOAT;
    else if (strcmp(type, "double") == 0)
        return DOUBLE;
    else if(strcmp(type, "char") == 0)
        return CHAR;
}

void print_dict(const CustomDict *dict) 
{
    int i, j;
    for (i = 0; i < dict->curr_size; i++) 
    {
        printf("Key: %15s ", dict->items[i].key);
        printf(": ");
        for (j = 0; j < dict->items[i].value_count; j++) 
        {
            switch (dict->items[i].type) 
            {
                case INT:
                    printf("%i ", dict->items[i].values[j].int_V);
                    break;
                case FLOAT:
                    printf("%.2f ", dict->items[i].values[j].float_V);
                    break;
                case DOUBLE:
                    printf("%.2lf ", dict->items[i].values[j].double_V);
                    break;
                case CHAR:
                    printf("%c ", dict->items[i].values[j].char_V);
                    break;
            }
        }
        printf("\n");
    }
}

void add_item(CustomDict *dict, char *key, Value *values, ValueType vt, int value_count)
{
    int i, j;
    for (i = 0; i < dict->curr_size; i++) 
    {
        if (strcmp(dict->items[i].key, key) == 0) 
        {
            free(dict->items[i].values); /*free the previous values*/
            dict->items[i].values = (Value *)malloc(sizeof(Value) * value_count);  /*allocate memory then copy the values*/
            for (j = 0; j < value_count; j++)
                dict->items[i].values[j] = values[j];
            
            dict->items[i].type = vt;
            dict->items[i].value_count = value_count;
            return;
        }
    }
    if (dict->capacity == dict->curr_size)
        resize_dict(dict);

    dict->items[dict->curr_size].key = (char *)malloc(strlen(key) + 1);
    strcpy(dict->items[dict->curr_size].key, key);

    dict->items[dict->curr_size].values = (Value *)malloc(sizeof(Value) * value_count); /*allocate memory then copy the values*/
    for (j = 0; j < value_count; j++)
        dict->items[dict->curr_size].values[j] = values[j];

    dict->items[dict->curr_size].type = vt;
    dict->items[dict->curr_size].value_count = value_count;
    dict->curr_size++;
}

void delete_item(CustomDict *dict, char *key)
{
    if (dict->curr_size == 0)
    {
        printf("Dictionary is empty.\n");
        return;
    }
    int i, index = -1;
    for (i = 0; i < dict->curr_size; i++)
    {
        if (strcmp(dict->items[i].key, key) == 0)
        {
            index = i;
            break;
        }
    }            
    if (index != -1)
    {
        free(dict->items[index].key);
        free(dict->items[index].values);
        /*shift other items*/
        for (i = index; i < dict->curr_size - 1; i++)
            dict->items[i] = dict->items[i + 1];
        dict->curr_size--;
    }
    else
    {
        printf("No values with the key: %s\n", key);
    }
}

void set_value(CustomDict *dict, char *key, Value *values, ValueType vt, int value_count)
{
    int i, j;
    for (i = 0; i < dict->curr_size; i++) 
    {
        if (strcmp(dict->items[i].key, key) == 0) 
        {
            free(dict->items[i].values);
            dict->items[i].values = (Value *)malloc(sizeof(Value) * value_count);
            for (j = 0; j < value_count; j++)
                dict->items[i].values[j] = values[j];
            dict->items[i].type = vt;
            dict->items[i].value_count = value_count;
            return;
        }
    }
    printf("No such item with the key : %s\n", key);
    return;
}

Value *search_item(CustomDict *dict, char *key)
{
    int i;
    for (i = 0; i < dict->curr_size; ++i)
    {
        if (strcmp(dict->items[i].key, key) == 0)
            return dict->items[i].values;
    }
    return NULL;   
}   

void sort_dict(CustomDict *dict)
{
    int i, j;
    for (i = 0; i < dict->curr_size - 1; ++i)
    {
        for (j = i + 1; j < dict->curr_size; ++j)
        {
            if (strcmp(dict->items[i].key, dict->items[j].key) > 0)
            {
                Item temp = dict->items[i];
                dict->items[i] = dict->items[j];
                dict->items[j] = temp;
            }
        }
    }
}

void resize_dict(CustomDict *dict)
{
    dict->capacity *= 2;
    dict->items = (Item *)realloc(dict->items, sizeof(Item) * dict->capacity);
}
