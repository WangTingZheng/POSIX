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

struct sharedstruct  //定义共享内存结构体
{
	int flag;//非0：表示可读，0表示可写
	char content[TEXT_SZ];//记录写入和读取的字符串
};

int main()
{
	int running = 1; //开始应该是可以运行的
	void *shm = NULL;  //  共享内存
	struct sharedstruct *shared = NULL;  //共享内存结构体
	char buffer[BUFSIZ+1];  //缓冲区
	int shmid;  //共享内存id
	shmid = shmget((key_t)1234, sizeof(struct sharedstruct), 0666|IPC_CREAT);  //创建一个共享内存，返回id
	if (shmid == -1)  //如果出错，就退出
	{
		exit(EXIT_FAILURE);
	}
	shm = shmat(shmid,(void*)0,0); //链接共享内存，返回指针（内存地址）
	if (shm == (void*)-1)
	{
		exit(EXIT_FAILURE);
	}
	printf("Memory attached at %X\n", (int)shm);  
	shared =  (struct sharedstruct*)shm;  //转化为共享内存结构体变量
	while(running){ //如果还需要继续运行
		if (shared->flag == 1)  //如果共享内存可读不可写
		{
			sleep(1);  //休眠一段时间
			printf("Waiting...\n"); //打印提示
		}
		else //如果可写
		{
			printf("Enter some text:");  //提示输入
			fgets(buffer, BUFSIZ, stdin); //获取输入到缓冲区
			strncpy(shared->content,buffer,TEXT_SZ); //把缓冲区的数据复制到共享内存的context中
			shared->flag = 1; //flag置1代表以及写入，可以进行读取
		}
		if (strncmp(buffer , "end" , 3) == 0)  //如果buffer到达最后，说明已经复制完成，停止运行
		{
			running = 0;
		}
		if (shmdt(shm) == -1)
		{
			exit(EXIT_FAILURE);
		}
		if (shmctl(shmid, IPC_RMID,0)==-1)
		{
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	return 0;
}