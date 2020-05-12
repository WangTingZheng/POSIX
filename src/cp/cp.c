#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define COPYMODE 0644 //定义目标文件模式
#define BUF 4096 //定义缓冲区大小

int main(int argc, char *argv[])
{
    if ( argc != 3 ){//如果输入的参数不是3，则说明使用方法不正确
        printf( "usage:%s source destination\n", argv[0] );
        exit( -1 );
    }

    int in_fd = -1, out_fd = -1; //定义文件标识符

    if( ( in_fd = open( argv[1], O_RDONLY ) ) == -1 ) { //以只读方式打开源文件
        perror( "file open" );
        exit( -1 );
    }

    if ( ( out_fd = creat( argv[2], COPYMODE ) ) == -1 ) { //以拷贝模式打开目标文件
        perror( "file copy" );
        exit( -1 );
    }

    char n_chars[BUF];  //设置文件缓冲数组
    int len = 0;

    while( ( len = read( in_fd, n_chars, sizeof( n_chars ) ) ) > 0 ) { //读取源文件，如果源文件读取没有到头，读到头为0，发生错误为-1
        if ( write( out_fd, n_chars, len ) != len ) { //写入目标文件
            printf( "文件:%s发生copy错误\n", argv[2] );
            exit( -1 );
        }
    }

    if( len == -1 ) {   //如果发生错误
        printf( "读取%s文件错误\n", argv[1] );
        exit( -1 );
    }

    if( close( in_fd ) == -1 ) {      //关闭源文件
        printf( "文件%s关闭失败\n", argv[1] );
        exit( -1 );
    }
    if( close( out_fd ) == -1 ) {   //关闭目标文件
        printf( "文件%s关闭失败\n", argv[2] );
        exit( -1 );
    }
    return 0;
}
