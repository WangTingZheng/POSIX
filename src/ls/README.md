## ls
本文件是模仿ls -l命令，b基本用法如下
```
不指定目录
        myls 打印出当前目录的文件信息
指定目录
        myls dir_path 打印出这个目录下的文件信息
选项
        --p(point)   是否显示当前目录本身. 和上级目录
        --i(ignore)  是否显示隐藏文件
        --s(symlink) 是否显示符号链接文件本身的属性还是符号链接指向文件的属性
``` 