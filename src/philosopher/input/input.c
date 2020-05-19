//
// Created by 14037 on 2020/4/22.
//

#include "input.h"

int getInput()
{
    int data;
    if(scanf("%d",&data))
    {
        return data;
    }
    return validInputCode;
}

void setPhilosopherNumberWithInput()
{
    int res;
    res = getInput();
    if(res != validInputCode)
    {
        printf("you got an philosopher number: %d\n ", res);
        setPhilosopherNumber(res);
    }
    else {
        printf("Input code valid\n");
    }
}
