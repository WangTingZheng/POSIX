## POSIX API 实验

### 内容概况

```bash
src
├ cp
│   └ cp.c
├ list
│   └ list.c
├ ls
│   ├ ls.c
│   └ README.md
├ philosopher
│   ├ config.h
│   ├ init
│   │   ├ util.c
│   │   └ util.h
│   ├ input
│   │   ├ input.c
│   │   └ input.h
│   ├ main.c
│   └ philosopher
│       ├ philosopher.c
│       └ philosopher.h
├ pthread
│   ├ pthread
│   └ pthread.c
├ README.md
└ thread
    └ thread.c
```

### 编译

```bash
cd build
cmake ../
make
```

### 清理

#### make

```bash
cd build
make clean
```

#### cmake

```bash
rm -rf build
```

