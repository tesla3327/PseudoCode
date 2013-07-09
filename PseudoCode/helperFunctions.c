//
//  helperFunctions.c
//  PseudoCode
//
//  Created by Michael Thiessen on 2013-07-08.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#include "helperFunctions.h"

// Simple linear search
int isStringInArray(char* string, char** array, int numElements)
{
    for (int i = 0; i < numElements; i++)
    {
        if (strcmp(string, array[i]) == 0)
        {
            return 1;
        }
    }
    
    return 0;
}


















