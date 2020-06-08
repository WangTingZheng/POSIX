#include <sys/shm.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <errno.h>
#include <wait.h>



#define MYKEY 0x1a0a
#define POOLNUM 5
#define POOLLEN 1000
#define COPYMODE 0644 //定义目标文件模式
int sem_id;
union semun 
{
	int val;   
	struct semid_ds  *buf;  
	unsigned short *array;
}sem_union;

struct BufferPool
{
	char Buffer[POOLNUM][POOLLEN];
	int Index[POOLNUM];
};

struct BufferPool bufferPool;
int create_sem()
{
	int semid;
	semid = semget(MYKEY,1,IPC_CREAT|0660);
	return semid;
}

int init_sem(int sem_id, int init_value)
{
	union semun sem_union;
	sem_union.val = init_value;
	if (semctl(sem_id,0,SETVAL, sem_union) == -1)
	{
		perror("Initialize semaphore");
		return -1;
	}
	return 1;
}

int semaphore_p(int sem_id, short sem_no)
{
	struct sembuf sem_b;
	sem_b.sem_num = sem_no;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id,&sem_b,1)==-1)
	{
		perror("semaphore_p failed");
		return 0;
	}
	return 1;
}

int semaphore_v(int sem_id, short sem_no)
{
	struct sembuf sem_b;
	sem_b.sem_num = sem_no;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id,&sem_b,1) == -1)
	{
		perror("semaphore_v failed");
		return 0;
	}
	return 1;
}

int del_sem(int sem_id)
{
	union semun sem_union;
	if (semctl(sem_id,0,IPC_RMID, sem_union)==-1)
	{
		perror("Delete semaphore");
		return -1;
	}
	return 1;
}

void *consumer(void *dir_path)
{
	int pool=-1;
	char *file_path_in;
    file_path_in =  (char *) dir_path;
	int in_fd;
	if((in_fd = open(file_path_in, O_RDONLY ) ) == -1 ) { //以只读方式打开源文件
        perror("file open");
        exit( -1 );
    }
    
    while(pool !=-1)
    {
    	for (int i = 0; i < POOLNUM; ++i)
		{
			semaphore_p(sem_id, i);
			if(bufferPool.Index[i] == 1)
			{
				pool = i;
				break;
			}
			else
			{
				semaphore_v(sem_id, i);
			}
		}
		if (pool == -1)
		{
			sleep(10);
		}
    }
	char n_char[POOLLEN];
	if(read(in_fd, n_char,sizeof(n_char)) < 0){
		perror("read");
		printf( "文件:%s发生读取错误\n", file_path_in);
        exit(-1);
	}
	bufferPool.Index[pool] = 0;
	semaphore_v(sem_id, pool);
}

void *producer(void *dir_path)
{
	int pool=-1;
	char *file_path_out;
    file_path_out =  (char *) dir_path;
	int out_fd;
	if ( ( out_fd = creat(file_path_out, COPYMODE ) ) == -1 ) { //以拷贝模式打开目标文件
        perror( "file copy" );
        exit( -1 );
    }

    while(1)
    {
    	for (int i = 0; i < POOLNUM; ++i)
		{
			semaphore_p(sem_id, i);
			if(bufferPool.Index[i] == 0)
			{
				pool = i;
				break;
			}
			else
			{
				semaphore_v(sem_id, i);
			}	
		}
		if (pool == -1)
		{
			sleep(10);
		}
    }
    if(write(out_fd, bufferPool.Buffer[pool], POOLLEN) != POOLLEN){
    	 perror("write");
    	 printf( "文件:%s发生copy错误\n", file_path_out);
         exit(-1);
    }
	bufferPool.Index[pool] = 1;
	semaphore_v(sem_id, pool);	
}

int main(int argc, char  *argv[])
{
	int id = create_sem();
	init_sem(id,0);
	pthread_t consumerid, producerid;
	pthread_create(&consumerid, NULL, consumer, argv[1]);
	pthread_create(&producerid, NULL, producer, argv[2]);
	pthread_join(consumerid,NULL);
	pthread_join(producerid,NULL);
	return 0;
}