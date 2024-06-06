#include <stdio.h>
#include <string.h>

/*
*   The array instructionHandlers is holding all the instructions with their 
*   shortname such as "LDAI"    
*   and correspounding opcode for that shortname
*   and a pointer to function for that specific operation
*/

#define REG_SIZE 8
#define MAX_INSTRUCTION 50
#define MAX_MEMORY 256
#define STACK_INDEX 100

typedef struct  /*This struct is general parameters an instruction can take*/
{
    int reg1;
    int reg2;
    int memoryAddress;
    int val;
} Parameters;

typedef void (*OperationFunc)(Parameters *param);   /*This is the function pointer*/

typedef struct  /*This struct is to create an instruction handler*/
{
    char name[5];
    int opcode;
    OperationFunc operation;    /*to point the current operation i put a pointer to a function*/
} InstructionHandler;

int programCounter = 0;
char regA[REG_SIZE + 1] = "00000000";
char regB[REG_SIZE + 1] = "00000000";
char instructionRegister[REG_SIZE * 3 + 1];
char programMemory[MAX_MEMORY][REG_SIZE + 1];

const char registerA_address[REG_SIZE + 1] = "01100110";    /*This is for execute part in order to distinguish registers*/
const char registerB_address[REG_SIZE + 1] = "01100111";

int to_binary(int decimal);
int to_decimal(char *binary);
int getOpcode(char *opName);
void get_binary_opcode(char *op, char *binary);
void get_binary_params(char *firstParam, char *first_param_str);
void loadToMemory(char *op, char *firstParam, char *secondParam, int *address);
void execute();

/*--------------INSTRUCTIONS---------------*/
void ADD(Parameters *param) { param->reg1 = param->reg1 + param->reg2; }
void ADDI(Parameters *param) { param->reg1 = param->reg1 + param->val; }
void ADDM(Parameters *param) { param->reg1 = param->reg1 + param->memoryAddress; }
void AND(Parameters *param) { param->reg1 = param->reg1 & param->reg2; }
void ANDI(Parameters *param) { param->reg1 = param->reg1 & param->val; }
void ANDM(Parameters *param) { param->reg1 = param->reg1 & param->memoryAddress; }
void OR(Parameters *param) { param->reg1 = param->reg1 | param->reg2; }
void ORI(Parameters *param) { param->reg1 = param->reg1 | param->val; }
void ORM(Parameters *param) { param->reg1 = param->reg1 | param->memoryAddress; }
void SUB(Parameters *param) { param->reg1 = param->reg1 - param->reg2; }
void SUBI(Parameters *param) { param->reg1 = param->reg1 - param->val; }
void SUBM(Parameters *param) { param->reg1 = param->reg1 - param->memoryAddress; }
void INC(Parameters *param) { param->reg1++; }
void DEC(Parameters *param) { param->reg1--; }
void SHL(Parameters *param) { param->reg1 = param->reg1 << param->val; }
void SHR(Parameters *param) { param->reg1 = param->reg1 >> param->val; }
void BRE(Parameters *param)
{
    if (param->reg1 == param->reg2)
        programCounter = param->reg1;
}
void BRN(Parameters *param)
{
    if (param->reg1 != param->reg2)
        programCounter = param->reg1;
}
void J(Parameters *param)   { programCounter = param->reg1 + param->reg2; }
void NOR(Parameters *param) { param->reg1 = ~(param->reg1 | param->reg2); }
void LDA(Parameters *param) { param->reg1 = param->val;}
void STR(Parameters *param) { param->memoryAddress = param->reg1; }
void SWP(Parameters *param)
{
    int temp = param->reg1;
    param->reg1 = param->reg2;
    param->reg2 = temp;
}
void LESS(Parameters *param)
{
    if (param->reg1 >= param->reg2)
        param->reg1 = 0;
    else 
        param->reg1 = 1;
}
void GRT(Parameters *param)
{
    if (param->reg1 <= param->reg2)
        param->reg1 = 0;
    else 
        param->reg1 = 1;
}
void PRI(Parameters *param) { printf("%i", param->reg1); }
void PRII(Parameters *param) { printf("%i", param->val); }
void PRIM(Parameters *param) { printf("%i", param->memoryAddress); }
void LDAI(Parameters *param) { param->reg1 = param->val;}
void EXIT(Parameters *param) { param->reg1 = -1; } /*Indicate end*/
/*--------------INSTRUCTIONS---------------*/

InstructionHandler instructionHandlers[] =
{
    {"ADD", 1, ADD},    {"ADDI", 2, ADDI},  {"ADDM", 3, ADDM}, 
    {"AND", 4, AND},    {"ANDI", 5, ANDI},  {"ANDM", 6, ANDM}, 
    {"OR", 7, OR},      {"ORI", 8, ORI},    {"ORM", 9, ORM}, 
    {"SUB", 10, SUB},   {"SUBI", 11, SUBI}, {"SUBM", 12, SUBM},
    {"INC", 13, INC},   {"DEC", 14, DEC},   {"SHL", 15, SHL}, 
    {"SHR", 16, SHR},   {"BRE", 17, BRE},   {"BRN", 18, BRN}, 
    {"J", 19, J},       {"NOR", 20, NOR},   {"LDA", 21, LDA}, 
    {"STR", 22, STR},   {"SWP", 23, SWP},   {"LESS", 24, LESS}, 
    {"GRT", 25, GRT},   {"PRI", 26, PRI},   {"PRII", 27, PRII}, 
    {"PRIM", 28, PRIM}, {"LDAI", 29, LDAI}, {"EXIT", 30, EXIT}
};

void simulate(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Can not locate file.\n");
        return;
    }
    char line[100];
    int i;
    /*Set things for another file*/
    programCounter = 0;
    strcpy(regA, registerA_address);
    strcpy(regB, registerB_address);   
    for (i = 0; i < MAX_MEMORY; i++) 
    {
        strcpy(programMemory[i], "00000000");  
    }
    /*This is where i manually fill the memory*/
    strcpy(programMemory[STACK_INDEX], "00000101");
    strcpy(programMemory[STACK_INDEX + 1], "00000110");

    int address = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char op[5], firstParam[5] = "@", secondParam[5] = "@";
        sscanf(line, "%s %s %s", op, firstParam, secondParam);

        if (address < MAX_MEMORY)
            loadToMemory(op, firstParam, secondParam, &address);
        else
        {
            printf("OVERFLOW\n");
            return;
        }
    }
    printf("Memory: \n");
    for (i = 0; i < 30; ++i)
    {
        printf("%s\n", programMemory[i]);
    }

    printf("OUTPUT:\n");
    execute();

    fclose(file);
}

void execute()
{
    strcpy(regA, registerA_address);
    strcpy(regB, registerB_address);
    int isEXIT = 1;
    do
    {
        char *opcode = programMemory[programCounter];
        char *first_param = programMemory[programCounter + 1];
        char *second_param = programMemory[programCounter + 2];

        int opcode_decimal = to_decimal(opcode);
        int inst_index = opcode_decimal - 1;

        InstructionHandler handler = instructionHandlers[inst_index];
        Parameters params;
        /*
            First set the params variable for execution
            check for the addresses to put reg1 and reg2 correct registers
            and simply place second parameters to memory address and val fields
        */
        params.reg1 = strcmp(first_param, registerA_address) == 0 ? to_decimal(regA) : to_decimal(regB);
        params.reg2 = strcmp(second_param, registerA_address) == 0 ? to_decimal(regA) : to_decimal(regB);
        params.memoryAddress = to_decimal(second_param);
        params.val = to_decimal(second_param);

        /*if (strcmp(second_param, "00000000") == 0)
            params.val = to_decimal(first_param);   *//*FOR PRINT STATEMENTS*/

        /*Execute current execution with parameters*/
        handler.operation(&params);

        /*
            Update the registers after execution since the changes 
            made up there are not effected the actual registers
        */
        if (strcmp(first_param, registerA_address) == 0)
            snprintf(regA, REG_SIZE + 1, "%08i", to_binary(params.reg1));
        else if (strcmp(first_param, registerB_address) == 0)
            snprintf(regB, REG_SIZE + 1, "%08i", to_binary(params.reg1));
        else if (strcmp(second_param, registerA_address) == 0)
            snprintf(regA, REG_SIZE + 1, "%08i", to_binary(params.reg2));
        else if (strcmp(second_param, registerB_address) == 0)
            snprintf(regB, REG_SIZE + 1, "%08i", to_binary(params.reg2));
        
        if (params.reg1 == -1)
            isEXIT = 0;
        
        programCounter += 3;
    } while (programCounter < MAX_MEMORY && isEXIT);
}

void loadToMemory(char *op, char *firstParam, char *secondParam, int *address)
{
    char opcode_str[REG_SIZE + 1];
    char first_param_str[REG_SIZE + 1];
    char second_param_str[REG_SIZE + 1];

    get_binary_opcode(op, opcode_str);
    get_binary_params(firstParam, first_param_str);
    get_binary_params(secondParam, second_param_str);
    /*
        if there is a load operation to registers load it
        in order to fill the memory correctly
    */
    if (firstParam[0] == 'A' || firstParam[0] == 'B')
    {
        /*These checks for values that does not update registers during loading the program*/
        if (strcmp(op, "J") != 0 && strcmp(op, "STR") != 0 &&
            strcmp(op, "SWP") != 0 && strcmp(op, "PRI") != 0 &&
            strcmp(op, "PRII") != 0 && strcmp(op, "PRIM") != 0 && strcmp(op, "EXIT") != 0)
        {
            if (firstParam[0] == 'A')
                strcpy(regA, second_param_str);
            else if (firstParam[0] == 'B')
                strcpy(regB, second_param_str);
        }
    }
    snprintf(programMemory[(*address)++], REG_SIZE + 1, "%s", opcode_str);
    snprintf(programMemory[(*address)++], REG_SIZE + 1, "%s", first_param_str);
    snprintf(programMemory[(*address)++], REG_SIZE + 1, "%s", second_param_str);
}

void get_binary_params(char *param, char *binary_str)
{
    if (param[0] == 'A')
        strcpy(binary_str, registerA_address);
    else if (param[0] == 'B')
        strcpy(binary_str, registerB_address);
    else if (strstr(param, "M[") != NULL)
    {
        int index = 0; 
        if (param[2] == 'A')
            index = to_decimal(regA);
        else if (param[2] == 'B') 
            index = to_decimal(regB);
        
        if (index >= 0 && index < MAX_MEMORY)
            strcpy(binary_str, programMemory[index]);
        else
            strcpy(binary_str, "00000000");
    }
    else if (param[0] == '@')
    {
        snprintf(binary_str, REG_SIZE + 1, "%08i", 0);
    }
    else
    {
        int num;
        sscanf(param, "%i", &num);
        snprintf(binary_str, REG_SIZE + 1, "%08i", to_binary(num));
    }
}

void get_binary_opcode(char *op, char *binary)
{
    int opcode = getOpcode(op);
    if (opcode == -1)
    {
        strcpy(binary, "00000000");
        return;
    }
    snprintf(binary, REG_SIZE + 1, "%08i", opcode);
}

int getOpcode(char *opName)
{
    int numHandlers = sizeof(instructionHandlers) / sizeof(InstructionHandler);
    int i;
    for (i = 0; i < numHandlers; i++)
    {
        if (strcmp(opName, instructionHandlers[i].name) == 0)
            return to_binary(i + 1); /*return correspounding address*/
    }
    return -1;
}

int to_binary(int decimal)
{
    int binary = 0, base = 1;
    while (decimal > 0)
    {
        int last_digit = decimal % 2;
        binary += last_digit * base;
        base *= 10;
        decimal /= 2;
    }
    return binary;
}

int to_decimal(char *binary)
{
    int decimal = 0;
    int i;
    int binary_length = strlen(binary);
    for (i = 0; i < binary_length; i++)
    {
        decimal = decimal * 2 + (binary[i] - '0');
    }
    return decimal;
}

int main()
{
    char filename[20];
    int choice = 1;

    printf("Binary to decimal : %i\nDecimal to binary: %i\n", to_decimal("00001001"), to_binary(25));

    printf("Enter file name (.asm) :\n");
    do
    {
        while (1)
        {
            scanf("%s", filename);
            if (strstr(filename, ".asm") != NULL)
                break;
            printf("Invalid file name. Enter again :\n");
        }
        simulate(filename);
        printf("\nDo you want to execute another assembly code: 1 for yes 0 for no:\n");
        scanf("%i", &choice);
    } while (choice);
}
