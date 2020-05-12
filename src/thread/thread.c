#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <stdlib.h>

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
void dealFileInDir(char path[]);
char *mergeString(char first[], char last[]);
NodePtr head;


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
    NodePtr temp = head;
    while(temp->next->next != NULL)
    {
        temp = temp->next;
    }
    NodePtr res = temp->next;
    temp->next = NULL;
    free(temp->next);
    return res;
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

void read_dir(char *dir_path)
{
    DIR *cwd = NULL;                                                    //路径文件指针
    struct dirent *det = NULL;                                          //记录读到的文件信息
    struct stat statbuf = {0};                                          //记录文件的属性
    printf("New path %s\n", dir_path);
    chdir(dir_path);                                                    //!非常重要，否则无法获取文件属性
    cwd = opendir(dir_path);                                            //打开目录
    while (NULL != (det = readdir(cwd)))                                //遍历当前目录
    {
        stat(det->d_name, &statbuf);//保存符号链接指向文件本身的信息进statbuf
        if (strcmp(det->d_name,".") !=0 && strcmp(det->d_name,"..")!=0) {
            if (S_ISDIR(statbuf.st_mode)) {
                printf("dir %s\n", det->d_name);
                push(head, createNode(mergeString(dir_path, det->d_name)));
            } else {
                dealFileInDir(mergeString(dir_path, det->d_name));
            }
        }
    }
    while(isEmpty(head) == 0)
    {
        NodePtr deal = pop(head);
        read_dir(deal->dir.dir_path);
        free(deal->dir.dir_path);
    }
    closedir(cwd);
}

void dealFileInDir(char *file_path)
{
    printf("no dir:%s\n", file_path);
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

int main(int argc, char *argv[])
{
    head = createNode("head");
    printf("%s\n", argv[0]);
    read_dir("/home/tingzheng");
    return 0;
}