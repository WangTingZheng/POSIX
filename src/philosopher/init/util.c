
//
// Created by 14037 on 2020/4/22.
//
#include "util.h"


/*
 * 设定哲学家的数量，哲学家的数量将存入数组中供其它程序调用
 * @param number: 哲学家的数量
 */
void setPhilosopherNumber(int number)
{
    if(number <= philosopherNumberMax)
    {
        philosopherNumber = number;
        printf("philosopher number has been set as %d!\n", number);
    } else
    {
        printf("Philosopher Number bigger than PhilosopherNumberMax %d, valid! reset!\n", philosopherNumberMax);
        return ;
    }
}

/*
 * 返回一个哲学家的左边叉子的序号
 * @param philosopherCode 哲学家的序号
 * return 一个整型变量，是该哲学家左边叉子的序号
 */
int returnLeft(int philosopherCode)
{
    return philosopherCode + 1;
}


/*
 * 返回一个哲学家的右边叉子的序号
 * @param philosopherCode 哲学家的序号
 * return 一个整型变量，是该哲学家右边叉子的序号
 */
int returnRight(int philosopherCode)
{
    if(philosopherCode != 0)
    {
        return philosopherCode;
    }
    return philosopherNumber;
}

