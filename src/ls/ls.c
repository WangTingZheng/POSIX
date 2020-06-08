#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>


struct mode
{
    int showPoint;  //是否打印.和..
    int showIgnore; //是否打印隐藏文件
    int isSymlink;  //打印软连接指示的文件/打印软连接本身的信息
};

struct mode print_mode;                     //定义打印模式结构体

/*
* 打印命令函数
*/
void print_help()
{
	printf("不指定目录\n");
	printf("	myls 打印出当前目录的文件信息\n");
	printf("指定目录\n");
	printf("	myls dir_path 打印出这个目录下的文件信息\n");
	printf("选项\n");
	printf("	--p(point)   是否显示当前目录本身. 和上级目录\n");
	printf("	--i(ignore)  是否显示隐藏文件\n");
	printf("	--s(symlink) 是否显示符号链接文件本身的属性还是符号链接指向文件的属性\n");
}


void myls_l(char *pathname);
void print_dir(struct dirent *det);



int main(int argc, char *argv[])
{
    char buf[1024] = "";	                    //初始化路径缓冲区
    /*
    for (int j = 0; j < argc; j++)
    {
    	printf("第%d个为%s\n",j, argv[j]);
    }
    */
    if (argc >= 3)							//如果用户输入了三个值
    {
    	for(int i=2 ; i<argc;i++)			//因为myls只需要2个参数，主命令myls 和目录路径，从第三个参数开始，都是打印模式选择命令
		{ 
	        if(strcmp(argv[i],"--p") == 0)  //如果有--p命令，代表要打印.和..
	        {
	           print_mode.showPoint = 1;    //设置标志位
	        } else if(strcmp(argv[i], "--i") == 0)  //如果有--i，代表要打印隐藏文件
	        {
	           print_mode.showIgnore = 1;   //设置标志位
	        }else if(strcmp(argv[i], "--s") == 0) //如果有--s，代表要打印符号链接文件本身的信息
	        {
	        	print_mode.isSymlink = 1;  //设置标志位
	        }else if(strcmp(argv[i], "--h") == 0)  //如果有--h，代表需要打印帮助文本
	        {
	        	print_help(); //打印帮助文本
				return 0; //退出整个程序
	        }
	    }
	    myls_l(argv[1]); //打印输入的目录内的文件/目录信息
    } else if(argc == 2)  			//如果用户输入了两个值
    {
        if(strcmp(argv[1],"--p") == 0)  //如果有--p命令，代表要打印.和..
        {
            print_mode.showPoint = 1;										//设置标志位
            getcwd(buf, sizeof(buf));										//获得当前路径
            myls_l(buf);													//打印目录下的信息
        } else if(strcmp(argv[1], "--i") == 0)								//如果有--i，代表要打印隐藏文件
        {
            print_mode.showIgnore = 1;										//设置标志位
            getcwd(buf, sizeof(buf));										//获得当前路径
            myls_l(buf);													//打印目录下的信息
        }else if (strcmp(argv[1], "--s") == 0)								//如果有--s，代表要打印符号链接文件本身的信息
        {
        	print_mode.isSymlink = 1;										//设置标志位
            getcwd(buf, sizeof(buf));										//获得当前路径
            myls_l(buf);													//打印目录下的信息
        } else if(strcmp(argv[1], "--h") == 0)								 //如果有--h，代表需要打印帮助文本
		{
	       	print_help();													//打印帮助信息
			return 0;														//退出整个程序
		}
        else
        {
            myls_l(argv[1]);												//打印参数里传过来的目录下的信息
        }
    } else if(argc == 1)													//如果只有1个参数
    {
        getcwd(buf, sizeof(buf));											//获得当前路径
        myls_l(buf);														//打印当前目录下的信息
    }
    return 0;
}



/*
* 打印文件结构体中的信息
* det: 文件结构体指针
*/
void print_dir(struct dirent *det)
{
    struct stat statbuf = {0};											//记录文件的属性
    struct passwd *pwd = NULL;											//记录文件所属用户
    struct group *grp = NULL;											//记录文件所属用户组
    struct tm mtm = {0};
    if (print_mode.isSymlink == 1)										//如果要打印符号链接本身的信息
    {
		lstat(det->d_name, &statbuf);									//保存符号链接文件本身的信息进statbuf
    }
    else{
    	stat(det->d_name, &statbuf);									//保存符号链接指向文件本身的信息进statbuf
    }
    //st_mode															//文件类型
    if(S_ISREG(statbuf.st_mode))			                  			//如果是普通文件							
        printf("-");
    else if(S_ISDIR(statbuf.st_mode))									//如果是目录文件
        printf("d");
    else if(S_ISCHR(statbuf.st_mode))									//如果是字符设备
        printf("c");
    else if(S_ISBLK(statbuf.st_mode))									 //如果是块设备
        printf("b");
    else if(S_ISFIFO(statbuf.st_mode))									 //如果是FIFO
        printf("p");
    else if(S_ISLNK(statbuf.st_mode))									 //如果是符号链接
        printf("l");
    else if(S_ISSOCK(statbuf.st_mode))									 //如果是套接字
        printf("s");
    //st_mode															//文件权限
    if(statbuf.st_mode &S_IRUSR)
        printf("r");
    else
        printf("-");
    if(statbuf.st_mode & S_IWUSR)
        printf("w");
    else
        printf("-");
    if(statbuf.st_mode & S_IXUSR)
        printf("x");
    else
        printf("-");

    if(statbuf.st_mode & S_IRGRP)
        printf("r");
    else
        printf("-");
    if(statbuf.st_mode & S_IWGRP)
        printf("w");
    else
        printf("-");
    if(statbuf.st_mode & S_IXGRP)
        printf("x");
    else
        printf("-");

    if(statbuf.st_mode & S_IROTH)
        printf("r");
    else
        printf("-");
    if(statbuf.st_mode & S_IWOTH)
        printf("w");
    else
        printf("-");
    if(statbuf.st_mode & S_IXOTH)
        printf("x");
    else
        printf("-");
    //st_nlink													//硬链接数
    printf("%2ld ", statbuf.st_nlink);
    //st_uid													//属主
    pwd = getpwuid(statbuf.st_uid);
    printf("%s ", pwd->pw_name);
    //st_gid													//属组
    grp = getgrgid(statbuf.st_gid);
    printf("%s ", grp->gr_name);
    //st_size													//文件大小
    printf("%5ld ", statbuf.st_size);
    //st_mtim													//最后修改时间
    localtime_r(&(statbuf.st_mtime), &mtm);
    printf("%d月", mtm.tm_mon + 1);
    printf("%4d ", mtm.tm_mday);
    printf("%02d:", mtm.tm_hour);
    printf("%02d ", mtm.tm_min);
    //st_name													//文件名
    printf("%s\n", det->d_name);
}

void myls_l(char *pathname) {
    //printf("%s\n", pathname);
    //printf("p：%d i:%d\n",print_mode.showPoint, print_mode.showIgnore);
    

    DIR *cwd = NULL;                                                    //路径文件指针
    struct dirent *det = NULL;                                          //记录读到的文件信息
    chdir(pathname);                                                    //!非常重要，否则无法获取文件属性
    cwd = opendir(pathname);											//打开目录
    while (NULL != (det = readdir(cwd)))                                //遍历当前目录
    {
        int isprint = 1;												//初始化是否打印本文件的标志位，默认打印
        if (print_mode.showPoint == 0)                                  //如果不展示.和..
        {
            if (strcmp(det->d_name, ".") == 0 || strcmp(det->d_name, "..") == 0)  //如果此时的目录. 或..，则不打印
            {
                isprint = 0;											//设置标志位为不打印
            }
        }
        if (print_mode.showIgnore == 0)  //如果不展示隐藏文件
        {
            if (det->d_name[0] == '.' && strcmp(det->d_name, ".") != 0 && strcmp(det->d_name, "..") != 0) //如果不是隐藏文件
                isprint = 0;											//设置标志位为不打印
        }
        if (isprint == 1)												//如果不打印本次遍历的文件信息
            print_dir(det);
    }
    closedir(cwd);                                                      //关闭目录
}