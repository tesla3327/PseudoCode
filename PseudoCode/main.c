//
//  main.c
//  PseudoCode
//
//  Created by Michael Thiessen on 2013-07-08.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helperFunctions.h"
#include "variableManager.h"
#include "expression.h"
#include "constants.h"



//------------------ Functions ----------------------
void interpretProgramFile(char *programFilePath);

void parseSingleOperandInstruction(char *token);
void parseAssignmentInstruction(char *token);
void parseControlFlow(char *token, FILE *file);
void setup();

int main(int argc, const char * argv[])
{
    // Setup
    setup();
    
    // Open up a program file 
    interpretProgramFile((char *)FILE_PATH);

    printf("\nEnd of program.\n");
    return 0;
}

void setup()
{
    // Setup our TRUE and FALSE variables
    char *trueName = malloc(sizeof(char) * 5);
    trueName = "true";
    
    char *falseName = malloc(sizeof(char) * 6);
    falseName = "false";
    
    variableDefineNewVariable(trueName);
    variableDefineNewVariable(falseName);
    
    int *var = variableReturnVariable(trueName);
    *var = 1;
    
    var = variableReturnVariable(falseName);
    *var = 0;    
}

void interpretProgramFile(char *programFilePath)
{
    assert(programFilePath != NULL);
    
    FILE *file;
    char line[LINE_MAX];
    char *token = "";
    
    // Attempt to open the file
    file = fopen(programFilePath, "r");
    if (!file)
    {
        printf("ERR: Failed to open the file '%s'\n", programFilePath);
        return;
    }
    
    // Process line by line
    while (fgets(line, LINE_MAX, file))
    {
//        printf("%s", line);
        
        // Tokenize splitting on whitespace
        token = strtok(line, " ");
        while (token != NULL)
        {
            // Check to see how many operands the current instruction requires
            if (isStringInArray(token, (char **)SINGLE_OPERAND_INSTRUCTIONS, NUM_SINGLE_OPERAND_INSTRUCTIONS))
            {
                parseSingleOperandInstruction(token);
            }
            else if (isStringInArray(token, (char **)ASSIGNMENT_INSTRUCTIONS, NUM_ASSIGNMENT_INSTRUCTIONS))
            {
                parseAssignmentInstruction(token);
            }
            else if (isStringInArray(token, (char **)CONTROL_FLOW, NUM_CONTROL_FLOW))
            {
                parseControlFlow(token, file);
            }
            
            token = strtok(NULL, " ");
        }
    }
}

void parseControlFlow(char *token, FILE *file)
{
    assert(token != NULL);
    assert(file != NULL);
    
    // Setup memory
    int ifBlockCount;
    char *ch = malloc(sizeof(char) * strlen(token));
    char *line = malloc(sizeof(char) * LINE_MAX);
    strcpy(ch, token);
    
    // IF...THEN
    if (strcmp(ch, CONTROL_FLOW[0]) == 0)
    {
        // If the expression is false, we need to skip past
        // the end of the IF block
        token = strtok(NULL, "");
        trimwhitespace(token);
        
        if (!evaluateIntExpression(token))
        {
            fgets(line, LINE_MAX, file);
            trimwhitespace(line);
            
            ifBlockCount = 1;
            
            while (ifBlockCount != 0)
            {
                fgets(line, LINE_MAX, file);
                trimwhitespace(line);
                
                // Check if we hit an IF line
                if (line[0] == 'I' && line[1] == 'F')
                {
                    ifBlockCount++;
                }
                else if (strcmp(line, "END") == 0)
                {
                    ifBlockCount--;
                }
            }
        }
    }
    
    // Deallocate memory
    free(ch);
    free(line);
}

void parseSingleOperandInstruction(char *token)
{
    assert(token != NULL);
    
    char *operand = strtok(NULL, " ");
    int endChar = (int)strlen(operand) - 1;
    
    // Remove extraneous return characters
    if (operand[endChar] == '\n')
    {
        operand[endChar] = '\0';
    }
    
    // VAR - define a variable
    if (strcmp(token, SINGLE_OPERAND_INSTRUCTIONS[0]) == 0)
    {
        variableDefineNewVariable(operand);
    }
    // PRINT - print the contents of a variable
    else if (strcmp(token, SINGLE_OPERAND_INSTRUCTIONS[1]) == 0)
    {
        int *var = (int *)variableReturnVariable(operand);
        
        if (var != NULL)
        {
            printf("%i\n", *var);
        }
    }
}

void parseAssignmentInstruction(char *token)
{
    assert(token != NULL);

    int *var = NULL;
    char *varName = strtok(NULL, " ");
    char *assignmentOp = strtok(NULL, " ");
    
    // Get the operand and remove and carriage returns
    char *operand = strtok(NULL, "");
    int endChar = (int)strlen(operand) - 1;
    
    if (operand[endChar] == '\n')
    {
        operand[endChar] = '\0';
    }
    
    // Do some simple error checks
    if (varName == NULL)
    {
        printf("ERR: Cannot assign a value to nothing.\n");
        return;
    }
    else if (assignmentOp == NULL || assignmentOp[0] != '=')
    {
        printf("ERR: Incorrect assignment syntax. Assignment operator '=' required.\n");
        return;
    }
    else if (operand == NULL)
    {
        printf("ERR: No value to assign to the variable.\n");
        return;
    }
    
    // Assignment
    var = variableReturnVariable(varName);
    *var = evaluateIntExpression(operand);
}
































