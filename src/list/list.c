/*==============================
*
*
* 文件名称： main.c
* 作者： 王听正
* 日期： 2020-5-11
* 功能： 链表的插入操作
*
*
===============================*/

#include <stdio.h> //输入输出printf要用的
#include <string.h>  //字符串处理函数
#include <stdlib.h>
#define DEAFULT_NAME "hlju"  //定义头节点学生名称
#define DEAFULT_AGE 22  //定义头节点学生年龄

typedef struct stuInfo{ //学生信息
    char stuName[10];  //学生姓名
    int Age;  //学生年龄
}ElemType;

 
typedef struct node{ //节点结构体
    ElemType data;  //学生信息
    struct node *next;  //下一个节点的地址
}ListNode, *ListPtr;



/*
* 创建头节点
* name[10]: 节点中的学生的姓名
* age: 节点中学生的年龄
* return: 一个链表节点
*/
ListNode CreateList()
{
    ListNode listNode;  //定义一个节点变量
    ElemType newStu;  //定义一个学生信息结构体变量
    newStu.Age = DEAFULT_AGE;  //给学生信息变量赋年龄值
    strcpy(newStu.stuName,DEAFULT_NAME); //给学生信息变量赋姓名值
    listNode.data = newStu; //把学生节点加入到链表节点中
    listNode.next = NULL; //把学生节点的下一个节点的地址设置为空
    return listNode; //返回节点
}


/*
* 定义链表中所有的学生信息
* head: 链表头节点的地址
*/
void PrintList(ListNode *head)
{
    ListPtr node = head->next; //从头节点的下一个开始
    while (node != NULL) //如果当前节点不为空，就可以打印，否则不打印
    {
        printf("name %s, age %d\n", node->data.stuName, node->data.Age); //打印学生名字和年龄
        node = node->next; //把节点指向下一个节点
    }
}


/*
* 在链表某位添加一个节点
* head: 要操作的链表的头节点地址
*/
void addNode(ListNode *head)
{
    ListPtr node = head;  //定义一个节点地址变量，把头节点的地址赋值给它
    while (node->next !=NULL) // 如果节点的下一个节点的地址不为空，则说明还没到最后一个节点
    {
        node  = node->next; // 把此刻的节点地址向后推
    }
    ListPtr add= (ListPtr)malloc(sizeof(ListNode));  //申请一段节点大小的内存空间
    printf("请输入要在末尾添加的节点的姓名和年龄，用空格隔开\n"); //提示输入节点学生信息
    scanf("%s %d", add->data.stuName, &(add->data.Age)); //接受用户输入的节点信息
    add->next = NULL;  //把加入的节点的下一跳节点地址设置为空
    node->next = add; //把原本链表最后的节点的下一跳地址设置为新加入的节点地址
    PrintList(head); // 打印链表，提示添加情况
}


/*
* 向链表中间插入一个节点
* head: 要操作的链表的头节点的地址
*/
void InsertList(ListNode *head)
{
   int index = 1;  //初始化节点位置
   printf("链表里的值有:\n"); //提示链表内容
   PrintList(head); 
   printf("你想插入到哪一个前面？\n"); //提示插入节点
   scanf("%d",&index); //接收用户输入的节点位置
   if(index< 0 || head == NULL)  //检查节点位置合法性，如果小于1或者链表不存在，报错
   {
       printf("插入失败，选择的位置不合法或者链表不存在");
       return;
   }
   ListPtr temp = head; //创建节点指针，以头节点为初始位置
   int i = 1; //初始化遍历节点位置到第一个
   while (i != index) //如果找到了节点位置，就退出循环，否则就继续循环
   {
       if (temp == NULL) //如果当前节点为空，说明到头了，还没找到节点
       {
           printf("找不到你想要的位置");
           return;
       }
       i ++;
       temp = temp->next;
   }
   ListPtr newNode = (ListPtr)malloc(sizeof(ListNode)); //跳出循环，并且还在程序里，说明已经找到了节点，申请节点大小的内存空间
   scanf("%s %d", newNode->data.stuName,&newNode->data.Age); //接收用户输入的节点信息
   newNode->next = temp->next; //把前面节点的下一个节点位置赋值给新节点的下一个节点位置指针变量
   temp->next = newNode; //把前面节点的下一跳节点位置设置为新插入节点的地址
}

int isCommand(char enter)
{	int num = (int)enter -48;
	if(num >=1 && num <=5)
		return 1;
	return 0;
}

/*
* 主函数
*/
int main(int argc, char* argv[])
{
    ListNode listNode; //定义链表头节点变量
    while(1) //构建死循环，便于重复输入命令
    {
        printf("1 create list\n");   //定义命令代号
        printf("2 printf list\n");
        printf("3 insert list\n");
        printf("4 add node\n");
        printf("5 quit\n");
        char command  = getchar(); //接收用户输入的值
        while (command == '\n' || isCommand(command) ==0 ) //如果输入的值不等于任何代号或者等于换行 
             command  = getchar(); //继续接收代号输入

        switch(command)
        {
            case '1': listNode  = CreateList();
                break;
            case '2': PrintList(&listNode);
                break;
            case '3': InsertList(&listNode);
                break;
            case '4': addNode(&listNode);
                break;
            case '5':
                return 0;
        }
    }
}
