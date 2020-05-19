#include "philosopher.h"
pthread_mutex_t  waiter;

_Noreturn void *eat_think(void *arg)
{
    int phi = *(int *)arg;
    int left,right;
    left = returnLeft(phi);
    right = returnRight(phi);

    for(;;){
        sleep(rand()%4);//思考0~3秒
        pthread_mutex_lock(&forkp[left]);//拿起左手的叉子
        printf("哲学家 %d 拿起左手的叉子 %d\n", phi, left);
        if (pthread_mutex_trylock(&forkp[right]) == EBUSY){//拿起右手的叉子
            pthread_mutex_unlock(&forkp[left]);//如果右边叉子被拿走放下左手的叉子
            printf("哲学家 %d 放下左手的叉子 %d\n", phi, left);
            continue;
        }
        printf("哲学家 %d 拿起右手的叉子 %d\n", phi, right);
        printf("哲学家 %d 在吃饭\n",phi);
        printf("--------------------------------\n");
        sleep(rand()%4);//吃0~3秒
        pthread_mutex_unlock(&forkp[left]);
        pthread_mutex_unlock(&forkp[right]);
    }
}

_Noreturn void *eat_think_odd_even(void *arg)
{
    int phi = *(int *)arg;
    int left,right;
    left = returnLeft(phi);
    right = returnRight(phi);

    for(;;){
        sleep(rand()%4);//思考0~3秒
        if (phi %2==0)
        {
            pthread_mutex_lock(&forkp[left]);//拿起左手的叉子
            printf("哲学家 %d 拿起左手的叉子 %d\n", phi, left);
            pthread_mutex_lock(&forkp[right]);//拿起左手的叉子
            printf("哲学家 %d 拿起右手的叉子 %d\n", phi, right);
        }
        else
        {
            pthread_mutex_lock(&forkp[right]);//拿起左手的叉子
            printf("哲学家 %d 拿起右手的叉子 %d\n", phi, right);
            pthread_mutex_lock(&forkp[left]);//拿起左手的叉子
            printf("哲学家 %d 拿起左手的叉子 %d\n", phi, left);
        }
        printf("哲学家 %d 在吃饭\n",phi);
        printf("--------------------------------\n");
        sleep(rand()%4);//吃0~3秒
        pthread_mutex_unlock(&forkp[left]);
        pthread_mutex_unlock(&forkp[right]);
    }
}

int is_available(pthread_mutex_t mutex, int p_id)
{
    int right = 1; //默认：所有哲学家都拿了右筷子
    int left = 1;  //默认：所有哲学家都拿起了左筷子
    for (int i = 0; i < philosopherNumber; ++i) {  //遍历所有哲学家
        if (i+1 != p_id)  //如果当前id不等于申请的哲学家的id，也是此次访问的是其它哲学家
        {
            if (mutex_philosopher[returnRight(i)] != i)  //如果当前哲学家的右叉子的占有者不等于哲学家，说明他没有选择右叉子
            {
                right = 0;   //说明其它哲学家不都选择右叉子，这里有两种情况，一是哲学家全选择左叉子，二是有哲学家选了左叉子，也有哲学家选择了右叉子
            }
            if (mutex_philosopher[returnLeft(i)] != i)//如果当前哲学家的右叉子的占有者不等于哲学家，说明他没有选择右叉子
            {
                left = 0; //说明其它哲学家不都选择左叉子，这里有两种情况，一是哲学家全选择右叉子，二是有哲学家选了右叉子，也有哲学家选择了左叉子
            }
        }
    }
    if ((right == 1 && left == 0)||(right == 0 && left == 1))//说明其它哲学家选择单一
    {
        return 0;
    }
    return 1;
}

void update(int philosopher, int mutex)
{
    mutex_philosopher[mutex] = philosopher;
}

_Noreturn void *eat_think_waiter(void *arg)
{
    int phi = *(int *)arg;
    int left,right;
    left = returnLeft(phi);
    right = returnRight(phi);

    for(;;){
        sleep(rand()%4);//思考0~3秒
        pthread_mutex_lock(&waiter);  //向一位服务生请求帮助，如果正在服务他人，就阻塞
        pthread_mutex_lock(&forkp[left]);//拿起左手的叉子
        printf("哲学家 %d 拿起左手的叉子 %d\n", phi, left);

        pthread_mutex_lock(&forkp[right]);//拿起左手的叉子
        printf("哲学家 %d 拿起左手的叉子 %d\n", phi, right);

        pthread_mutex_unlock(&waiter);

        printf("哲学家 %d 在吃饭\n",phi);
        printf("--------------------------------\n");
        sleep(rand()%4);//吃0~3秒
        pthread_mutex_unlock(&forkp[left]);
        pthread_mutex_unlock(&forkp[right]);
    }
}
/*
_Noreturn void *eat_think_waiter(void *arg)
{
    int phi = *(int *)arg;
    int left,right;
    left = returnLeft(phi);
    right = returnRight(phi);

    for(;;){
        int flag = 0;
        sleep(rand()%4);//思考0~3秒
        if (is_available(forkp[left], phi) == 1)
        {
            pthread_mutex_lock(&forkp[left]);//拿起左手的叉子
            update(phi, left);
            printf("哲学家 %d 拿起左手的叉子 %d\n", phi, left);
            flag ++;
        }


        if (is_available(forkp[right], phi) == 1)
        {
            pthread_mutex_lock(&forkp[right]);//拿起左手的叉子
            update(phi, right);
            printf("哲学家 %d 拿起左手的叉子 %d\n", phi, left);
            flag++;
        }
        if (flag == 2)
        {

        }
        printf("哲学家 %d 在吃饭\n",phi);
        printf("--------------------------------\n");
        sleep(rand()%4);//吃0~3秒
        pthread_mutex_unlock(&forkp[left]);
        pthread_mutex_unlock(&forkp[right]);
    }
}*/
int start(){
    pthread_t phiThread[philosopherNumber];


    for (int a = 0; a < philosopherNumber; a++)
    {
        pthread_mutex_init(&forkp[a], NULL);
        printf("thread %d inited\n", a);
    }

    for (int b = 0; b < philosopherNumber; b++)
    {
        int* temp = (int*)malloc(sizeof(int));
        *temp = b;
        pthread_create(&phiThread[b], NULL, eat_think, (void *)temp);
        printf("thread %d created\n", b);
    }

    for (int c = 0; c <philosopherNumber; c++)
        pthread_join(phiThread[c], NULL);
    return 0;
}