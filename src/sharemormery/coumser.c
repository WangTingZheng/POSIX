#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#define TEXT_SZ 1024

struct sharedstruct
{
	int flag;//非0：表示可读，0表示可写
	char content[TEXT_SZ];//记录写入和读取的字符串
};

int main()
{
	int running = 1;  //刚开始设置要运行
	void *shm = NULL; //共享内存指针
	struct sharedstruct *shared; //共享内存结构体
	int shmid; //共享内存id
	shmid = shmget((key_t)1234, sizeof(struct sharedstruct), 0666|IPC_CREAT); //创建共享内存，返回id
	if (shmid == -1) 
	{
		exit(EXIT_FAILURE);
	}
	shm = shmat(shmid, 0,0); //映射共享内存，返回共享内存地址
	if (shm == (void*)-1)
	{
		exit(EXIT_FAILURE);
	}
	shared =  (struct sharedstruct*)shm; //转化为共享内存结构体
	while(running)//如果可以运行
	{
		if (shared->flag!=0) //如果是可读的
		{
			printf("You wrote:%s\n", shared->content); //提示并打印共享内存的内容，代表已经读取
			sleep(rand()%3); //休眠随机的一段时间
			shared->flag = 0; //设置为可写
			if (strncmp(shared->content," end",3) == 0) // 如果达到最后了，就无需运行了
				running = 0;
		}else //如果不可读，可写
		{
			sleep(1); //休眠等着
		}
		if (shmdt(shm)==-1)
		{
			exit(EXIT_FAILURE);
		}
		if (shmctl(shmid, IPC_RMID,0)==-1)
		{
			exit(EXIT_FAILURE);
		}
		exit(EXIT_FAILURE);
	}
	return 0;
}