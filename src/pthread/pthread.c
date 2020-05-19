#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>


#define COPYMODE 0644 //定义目标文件模式
#define BUF 4096 //定义缓冲区大小


typedef struct DIRs
{
    char dir_path[1024];
}Dir;

typedef struct NODE
{
    Dir dir;
    struct NODE *next;
}Node, *NodePtr;



void put(NodePtr head, Node *node);
NodePtr pop(NodePtr head);
NodePtr createNode(char dir_path[]);
void dealFileInDir(char *file_path);
char * mergeString(char first[], char last[]);
void *copyFile(void *arg);
void dealDir(char *dir_path);
char* get_file_name(char *file_path);
void copy_file_function(char *src, char *dst);
NodePtr head;
char *dst;

void push(NodePtr head, NodePtr node)
{
    NodePtr temp =  head;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = node;
}

NodePtr pop(Node *head)
{
    /*
    NodePtr temp = head->next;
    *head = *temp;
    temp->next =NULL;
    return temp;*/
    NodePtr temp = head;
    while(temp->next->next != NULL)
    {
        temp = temp->next;
    }
    NodePtr res = temp->next;
    temp->next = NULL;
    free(temp->next);
    return res;
    //return res;
}

NodePtr createNode(char dir_path[])
{
    NodePtr node = (NodePtr)malloc(sizeof(Node));
    Dir dir;
    strcpy(dir.dir_path, dir_path);
    node->dir = dir;
    node->next = NULL;
    return node;
}

void printNode(NodePtr node)
{
    printf("path is %s\n", node->dir.dir_path);
}

void printHeap(NodePtr head)
{
    NodePtr temp = head;
    while(temp !=NULL)
    {
        printNode(temp);
        temp = temp->next;
    }
}

int isEmpty(NodePtr head)
{
    if(head->next == NULL)
        return 1;
    return 0;
}

void *read_dir(void *dir_path)
{
    char *path;
    path =  (char *) dir_path;
    DIR *cwd = NULL;                                                    //路径文件指针
    struct dirent *det = NULL;                                          //记录读到的文件信息
    struct stat statbuf = {0};                                          //记录文件的属性
    printf("New path %s\n", path);
    chdir(path);                                                    //!非常重要，否则无法获取文件属性
    cwd = opendir(path);                                            //打开目录
    while (NULL != (det = readdir(cwd)))                                //遍历当前目录
    {
        stat(det->d_name, &statbuf);//保存符号链接指向文件本身的信息进statbuf
        if (strcmp(det->d_name,".") !=0 && strcmp(det->d_name,"..")!=0) {
            if (S_ISDIR(statbuf.st_mode)) {
                printf("dir %s\n", det->d_name);
                push(head, createNode(mergeString(path, det->d_name)));
            } else {
                dealFileInDir(mergeString(path, det->d_name));
            }
        }
    }
    while(isEmpty(head) == 0)
    {
        NodePtr deal = pop(head);
        dealDir(deal->dir.dir_path);
        free(deal->dir.dir_path);
    }
    closedir(cwd);
}

void dealDir(char *dir_path)
{
    pthread_t id;
    pthread_create(&id, NULL, read_dir, dir_path);
    pthread_join(id,NULL);
}
void dealFileInDir(char *file_path)
{
    pthread_t id;
    pthread_create(&id, NULL, copyFile, file_path);
    pthread_join(id,NULL);
}

void *copyFile(void *arg)
{
    char *file;
    file =  (char *) arg;
    printf("%s\n", file);
    copy_file_function(file, mergeString(dst, get_file_name(file)));
}

char *mergeString(char first[], char last[])
{
    if (first[sizeof(first-1)] != '/' && last[0] != '/')
    {
        char *temp = "/";
        first = mergeString(first,temp);
    }
    char *name = (char *) malloc(strlen(first) + strlen(last));
    strcpy(name, first);
    strcat(name, last);
    return name;
}


char* get_file_name(char *file_path)
{
    char *fn = (char *)malloc(sizeof(fn[0]));
    char *p;
    //上句假设以某种方式获得的全文件名在pathname中,"..."中只是举例
    strcpy(fn,(p=strrchr(file_path,'/')) ? p+1 : file_path);
    //上句函数第2实参这样写以防止文件在当前目录下时因p=NULL而出错
    return fn;
}
void do_help()
{
    printf("pthread --s source_path --t target_path\n");
}

void copy_file_function(char *src, char *dst)
{
    int in_fd = -1, out_fd = -1; //定义文件标识符

    if( ( in_fd = open( src, O_RDONLY ) ) == -1 ) { //以只读方式打开源文件
        perror( "file open" );
        exit( -1 );
    }

    if ( ( out_fd = creat( dst, COPYMODE ) ) == -1 ) { //以拷贝模式打开目标文件
        perror( "file copy" );
        exit( -1 );
    }

    char n_chars[BUF];  //设置文件缓冲数组
    int len = 0;

    while( ( len = read( in_fd, n_chars, sizeof( n_chars ) ) ) > 0 ) { //读取源文件，如果源文件读取没有到头，读到头为0，发生错误为-1
        if ( write( out_fd, n_chars, len ) != len ) { //写入目标文件
            printf( "文件:%s发生copy错误\n", dst );
            exit( -1 );
        }
    }

    if( len == -1 ) {   //如果发生错误
        printf( "读取%s文件错误\n", src );
        exit( -1 );
    }

    if( close( in_fd ) == -1 ) {      //关闭源文件
        printf( "文件%s关闭失败\n", src );
        exit( -1 );
    }
    if( close( out_fd ) == -1 ) {   //关闭目标文件
        printf( "文件%s关闭失败\n", dst );
        exit( -1 );
    }
}


int main(int argc, char *argv[])
{
    if (argc <5 ||strcmp(argv[1],"--s") !=0 || strcmp(argv[3],"--t") != 0)
    {
        do_help();
        return 0;
    }
    head = createNode("NULL");
    dst = argv[4];
    pthread_t id;
    pthread_create(&id, NULL, read_dir, argv[2]);
    pthread_join(id,NULL);
    return 0;
}