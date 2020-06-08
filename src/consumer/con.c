#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#define MAX 5  //队列长度
#define LEN 1000

#define COPYMODE 0644 //定义目标文件模式
#define BUF 4096 //定义缓冲区大小


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t full;     //填充的个数
sem_t empty;    //空槽的个数

int top = 0;     //队尾
int bottom = 0;  //队头

typedef struct BufferPool{
    char Buffer[MAX][LEN];
    char Index[MAX];
    pthread_mutex_t Mutex_group[MAX];
}*BufferPoolPtr;

BufferPoolPtr  bufferPool;

_Noreturn void* produce(void* arg)
{
    while (1)
    {
        sem_wait(&empty);
        for (int j = 0; j < MAX; j++) {
            pthread_mutex_lock(&(bufferPool->Mutex_group[j]));
            if (bufferPool->Index[j] == 0){
                printf("%p produce puts hello in %d pool\n",arg, j);
                strcpy(bufferPool->Buffer[j],"hello");
                bufferPool->Index[j] = 1;
                pthread_mutex_unlock(&bufferPool->Mutex_group[j]);
                break;
            } else
            {
                pthread_mutex_unlock(&bufferPool->Mutex_group[j]);
                sleep(rand());
            }
        }
        sem_post(&full);
        sleep(1);
    }

    //return (void*)1;
}

_Noreturn void* consume(void* arg)
{
    while (1)
    {
        sem_wait(&full);
        //printf("consume working\n");
        for (int j = 0; j < MAX; j++) {
            pthread_mutex_lock(&bufferPool->Mutex_group[j]);
            if (bufferPool->Index[j] == 1){
                printf("%p consume show data  %s in pool%d\n",arg,bufferPool->Buffer[j], j);
                bufferPool->Index[j] = 0;
                pthread_mutex_unlock(&bufferPool->Mutex_group[j]);
                break;
            } else
            {
                pthread_mutex_unlock(&bufferPool->Mutex_group[j]);
                sleep(rand());
            }
        }
        sem_post(&empty);
        sleep(1);
    }


    //return (void*)2;
}

int main(int argc, char *argv[])
{
    pthread_t thid1;
    pthread_t thid2;
    pthread_t thid3;
    pthread_t thid4;

    int  ret1;
    int  ret2;
    int  ret3;
    int  ret4;
    bufferPool = (BufferPoolPtr)malloc(sizeof(BufferPoolPtr));
    //int shmid = shmget((key_t)1234,sizeof(struct BufferPool),0666|IPC_CREAT);
    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&bufferPool->Mutex_group[i],NULL);
    }


    //void *shm = shmat(shmid,0,0);
    //bufferPool = (BufferPoolPtr) shm;

    printf("=================start===================\n");
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, MAX);

    pthread_create(&thid1, NULL, produce, (void *) 1);
    pthread_create(&thid2, NULL, consume, (void *) 2);
    pthread_create(&thid3, NULL, produce, (void *) 3);
    pthread_create(&thid4, NULL, consume, (void *) 4);

   pthread_join(thid1, (void**)&ret1);
    pthread_join(thid2, (void**)&ret2);
    pthread_join(thid3, (void**)&ret3);
    pthread_join(thid4, (void**)&ret4);

    return 0;
}