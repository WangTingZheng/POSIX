//
// Created by 14037 on 2020/4/22.
//

#include <pthread.h>
#include <stdio.h>
#include "../config.h"

#ifndef PHILOSOPHER_INIT_H
#define PHILOSOPHER_INIT_H

#endif //PHILOSOPHER_INIT_H



int philosopherNumber; //存放哲学家数量的变量
pthread_mutex_t forkp[philosopherNumberMax];
int mutex_philosopher[philosopherNumberMax];  //叉子对于的哲学家

void setPhilosopherNumber(int number);
int returnLeft(int philosopherCode);
int returnRight(int philosopherCode);
pthread_mutex_t* returnFork();