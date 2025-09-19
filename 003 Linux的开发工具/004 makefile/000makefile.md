# make/Makefile

一个项目有成白上千个 .c（.cpp） 文本源码，要将所有文件通过 gcc（g++，下文统一用gcc代表这个系列的编译器） 编译生成一个可执行程序，要怎么做？若其中的一部分被修改过，如何知道哪些被修改过？如何确定哪些是继承和被继承、调用和被调用的关系？

在 windows 中有成熟的 IDE 完成这些工作，在 Linux 中则是使用 `make` 和 makefile。

> 会不会写makefile，从一个侧面说明了一个人是否具备完成大型工程的能力。
>
> makefile 有自己的一套语法体系。可尝试自学，但只需要会用就行。

`make` 是一条**命令**，makefile 是一个文件，两个搭配使用，完成项目自动化构建。

`make` 是一个命令工具，是一个解释 makefile 中指令的命令工具，一般来说，大多数的 IDE 都有这个命令，比如：Delphi的 make，Visual C++的 nmake，Linux下GNU的 make。可见，makefile 都成为了一种在工程方面的编译方法。

这个项目的成白上千个源文件按类型、功能、模块分别放在若干个目录中，makefile 定义了一系列的规则来指定，哪些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，甚至于进行更复杂的功能操作。

makefile 带来的好处就是“自动化编译”，一旦写好，只需要一个 `make` 命令，整个工程完全自动编译，极大的提高了软件开发的效率。

> 但 makefile 也因为任何情况都按照程序员预留的处理方式执行，无法灵活应对所有情况，于是就有了 cMake。

## 简单的 makefile

在编译 c 代码时，可以新建一个名为 `makefile` 文件且只能是这个名，首字母可以大写。

在 `makefile` ，首行为生成的可执行程序的名字，以及从哪些代码中来。这是写一个依赖关系。

然后第 2 行以一个 <kbd>Tab</kbd> 键开头（不能是4个空格），对应的是 gcc 命令。这是写一个依赖方法（通过指令实现）。类似的依赖方法可以写若干个。

之后输入 `make` 命令，就会调用 `makefile` 中的指令。

```bash
[Bjarne@VM-8-8-centos cppTest]$ touch makefile
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  makefile
[Bjarne@VM-8-8-centos cppTest]$ vim makefile
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
a.exe:a.c
	gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ make  # 也可以用make a.exe
gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  a.exe  makefile
[Bjarne@VM-8-8-centos cppTest]$ 
```

在首次使用`make`时，`make` 会扫描 makefile 这个文件，它会**自顶向下扫描**，直到发现第1个 `目标文件:源文件`，就默认形成这个目标文件（按照事先准备好的依赖方法，这里是 gcc 指令）。

> 可以理解为，makefile 就是脚本，`make` 就是脚本解释器。类比就是 python 解释器 和 .py 源码。

`make` 的一次执行会默认生成一个目标文件。生成目标文件后，除非修改源文件（具体来说是**修改源文件的时间属性**），否则不会再生成目标文件。

可以输入 `make a.exe`，也可以直接 `make`，但直接 `make` 只会执行第1个依赖关系。

```bash
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  makefile
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
a.exe:a.c
	gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ make
gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ make
make: `a.exe' is up to date.
[Bjarne@VM-8-8-centos cppTest]$ touch a.c
[Bjarne@VM-8-8-centos cppTest]$ make a.exe
gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ make a.exe
make: `a.exe' is up to date.
[Bjarne@VM-8-8-centos cppTest]$ 

```





## 依赖关系和依赖方法

假设某个高中生，大学毕业后原来的电脑和设备不行，于是更换了一个新的手机，但有了电话之后，大学生一般是除了要钱也不会打电话给家人。

若开头就是一句“我是你儿子”，则只表明**依赖关系**，并没有给出要钱的方法，也就是**依赖方法**。这时就得要让家里人将钱打到指定账户中。

所以 $依赖关系 + 依赖方法 = 描述清楚一件事的原因和做法$，如此就能让计算机达到自己想要的目标。

在计算机中也是依赖关系和依赖方法的关系。包括 makefile 。

makefile 的一个依赖关系可以有不止一个指令，通过回车即可分隔开。

```bash
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  makefile
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
a.exe:a.o
	gcc -E a.c -o a.i
	gcc -S a.i -o a.s
	gcc -c a.s -o a.o
	gcc a.o -o a.exe

.PHONY:clean
clean:
	rm -f a.exe a.i a.s a.o

[Bjarne@VM-8-8-centos cppTest]$ make
cc    -c -o a.o a.c
gcc -E a.c -o a.i
gcc -S a.i -o a.s
gcc -c a.s -o a.o
gcc a.o -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  a.exe  a.i  a.o  a.s  makefile
[Bjarne@VM-8-8-centos cppTest]$ 

```



## PHONY伪目标

代码编译完成之后，中间文件还需要清理（毕竟不是直接从源码到二进制，会产生中间文件）。

在之前的 `makefile` 的基础上，在第 4 行新增内容：`.PHONY:clean`（`.PHONY`大致读 “匪您” ，这是一个关键字，修饰后面的伪目标），这是构建一个**伪目标**的概念。第5行：`clean:`是第4行的依赖方法。在这里可以加指令`rm -f exe` 表示清理生成的可执行程序。

这时输入`make` 可生成代码，输入`make clean` 可清理生成的代码。

```bash
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
a.exe:a.c
	gcc a.c -o a.exe

.PHONY:clean
clean:
	rm -f a.exe

[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  a.exe  makefile
[Bjarne@VM-8-8-centos cppTest]$ make clean  # 也可以直接用make a.exe
rm -f a.exe
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  makefile
[Bjarne@VM-8-8-centos cppTest]$ 

```

若将`.PHONY:clean` 连同附属文本 和 `a.exe:a.c` 连同附属文本更换顺序，使用 `make` 会默认执行 `clean`。即`.PHONY` 不会影响 `make` 的默认执行顺序。

```bash
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  a.exe  a.i  a.o  a.s  makefile
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
.PHONY:clean
clean:
	rm -f a.exe a.i a.s a.o

a.exe:a.o
	gcc -E a.c -o a.i
	gcc -S a.i -o a.s
	gcc -c a.s -o a.o
	gcc a.o -o a.exe

[Bjarne@VM-8-8-centos cppTest]$ make
rm -f a.exe a.i a.s a.o
[Bjarne@VM-8-8-centos cppTest]$ 
```

生成可执行程序时会检测重复，编译后就不能再进行编译，想要编译就得修改源码。也就是说 `make` 会识别文件的新、旧。

将`.PHONY`删掉后使用 `make`、`make a.exe`和`make clean` 都不影响。加上`.PHONY` 将会**使依赖方法执行时不受修改时间的限制**。

> 编译可以算是比较费时的时，每次将成百上千的 cpp 文件进行编译很耗时，所以这些自动化工具为了提升效率，会不让没修改过的文件进行编译。

```bash
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
.PHONY:a.exe 
a.exe:a.c
	gcc a.c -o a.exe

[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  a.exe  makefile
[Bjarne@VM-8-8-centos cppTest]$ make
gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ make
gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ 
```

## PHONY的原理和文件的时间

几个概念：

1. 写代码叫开发，站在其他人的角度我就是在一个记事本中修改东西，这个东西就是源代码，或者说源文件。
2. 当修改源文件时，部分属性会发生变化，其中就包括修改时间（Change）。

`make` 会对比源文件（源码）的修改时间和目标文件（可执行程序）的修改时间。重新生成（编译）的本质是将源文件再次变成目标文件。

第1次编译的时候，一定是先有源文件，才有目标文件，所以**源码的修改时间小于程序的修改时间**，除非源文件发生修改。

> 在代码变成可执行程序，可执行程序在使用这个过程中，绝大多数情况下修改一下代码再重新编译就能解决，只有极少数情况需要清理中间文件（或者说清理项目）。
>
> 这个情况容易发生在多个源码的情况。

文件 = 内容 + 属性，因此文件的各种时间要么和内容有关，要么和属性有关。

首先先了解文件的时间属性：

```bash
[Bjarne@VM-8-8-centos cppTest]$ stat a.c
  File: ‘a.c’
  Size: 172       	Blocks: 8          IO Block: 4096   regular file
Device: fd01h/64769d	Inode: 788282      Links: 1
Access: (0664/-rw-rw-r--)  Uid: ( 1001/  Bjarne)   Gid: ( 1001/  Bjarne)
Access: 2025-09-11 19:35:02.369936115 +0800
Modify: 2025-09-11 19:35:01.782933192 +0800
Change: 2025-09-11 19:35:01.782933192 +0800
 Birth: -
[Bjarne@VM-8-8-centos cppTest]$ 

```

`Access` （英文含义进入）：文件的最近访问时间。当对文件的内容进行访问（`cat`、`vim`、打开、进入目录、ls显示）时，特别是内容级别的访问时，会对文件的 `Access` 时间进行修改。

`Modify`（英文含义改变）：文件的内容的最近修改时间。

`Change`：文件的属性的最近修改时间。

> 即ACM 时间，非 ACM（美国计算机协会）和 ACM （国际计算机程序设计大赛）。
>
> 下文为描述方便，称A时间为 `Access` 时间，称C时间为 `Change` 时间，称M时间为 `Modify` 时间。

若对属性做修改，内容可能没变化；但对内容修改时，会和其他时间进行联动修改。

对属性做修改，例如权限：

```bash
[Bjarne@VM-8-8-centos cppTest]$ stat a.c
# 省略
Access: 2025-09-11 20:22:42.280033042 +0800
Modify: 2025-09-11 20:22:42.277033027 +0800
Change: 2025-09-11 20:22:42.277033027 +0800
# 省略
[Bjarne@VM-8-8-centos cppTest]$ ll|grep a.c
-rw-rw-r-- 1 Bjarne Bjarne  172 Sep 11 20:22 a.c
[Bjarne@VM-8-8-centos cppTest]$ chmod o-r a.c
[Bjarne@VM-8-8-centos cppTest]$ stat a.c
# 省略
Access: 2025-09-11 20:22:42.280033042 +0800
Modify: 2025-09-11 20:22:42.277033027 +0800
Change: 2025-09-11 20:38:06.361628841 +0800 # 修改权限，Change发生变化
# 省略
[Bjarne@VM-8-8-centos cppTest]$ 

```

使用`vim` 对文件进行修改时，所有时间都发生了变化。而且 `M` 时间和 `C` 时间几乎是同时修改。因为先打开文件，是先访问文件，再修改，所以 `A` 时间先改，`M`、`C` 时间后改。

```bash
[Bjarne@VM-8-8-centos cppTest]$ stat a.c|grep 2025
Access: 2025-09-11 20:44:40.634545882 +0800
Modify: 2025-09-11 20:44:40.632545872 +0800
Change: 2025-09-11 20:44:40.632545872 +0800
[Bjarne@VM-8-8-centos cppTest]$ vim a.c
[Bjarne@VM-8-8-centos cppTest]$ stat a.c|grep 2025
Access: 2025-09-11 20:57:29.568287542 +0800 # 时间上和MC时间不完全相同
Modify: 2025-09-11 20:57:29.565287528 +0800
Change: 2025-09-11 20:57:29.565287528 +0800
[Bjarne@VM-8-8-centos cppTest]$ 

```

对文件的操作，查看文件的次数是最多的，主要是各种用户和系统查看。

目前看到的所有文件（Linux + windows）都在磁盘上，所以说更改时间的本质是访问磁盘。

>  在计算机中，CPU的运行速度是最快的，然后就是CPU的周边部件例如高速缓存和寄存器，然后是内存，最后是磁盘。
>
>  其中磁盘、键盘、网卡、显示屏，这样的设备都是外设，磁盘的访问效率低下。
>
>  而文件的查看次数非常多，查看的本质就是访问磁盘。Linux 系统充满大量访问磁盘的操作（IO居多），会变相减慢系统效率。
>
>  于是为了增加效率，Linux 对A时间的修改机制进行了更改，需要访问一定的次数才能更改A时间，甚至直接不会更改。这取决于 Linux 的版本。

若只修改一个时间，可以用`touch`指令更改。

```bash
[Bjarne@VM-8-8-centos cppTest]$ stat a.c|grep 2025
Access: 2025-09-11 21:13:31.461982689 +0800
Modify: 2025-09-11 21:13:29.797974567 +0800
Change: 2025-09-11 21:13:29.797974567 +0800
[Bjarne@VM-8-8-centos cppTest]$ touch -a a.c # 只更改A时间
[Bjarne@VM-8-8-centos cppTest]$ stat a.c|grep 2025
Access: 2025-09-11 21:14:51.462373216 +0800
Modify: 2025-09-11 21:13:29.797974567 +0800
Change: 2025-09-11 21:14:50.587368944 +0800
[Bjarne@VM-8-8-centos cppTest]$ 

```

`man` 手册说的`touch` 的 `-a` 选项只会更改 A 时间，但实际上 C 时间也改了，这是因为 A **时间也是属性**，A 时间改了，C 时间也会改。`-m` 选项同样是只更改M时间，但实际上M时间也是属性，所以M时间也会和C时间的更改同步。

## makefile 的拓展

1. 在makefile 文件中，若在指令前加 `@` 符号，可以让指令不打印在交互界面。
2. makefile 用 # 作为行注释开头。
3. makefile 允许设置变量，但变量的定义不建议设置空格。之后使用时可以`$(变量名)` 进行占位，`make` 会用变量的真实名去替代（类似 c 语言的宏）。
4. makefile中，`依赖关系:依赖方法` 中的依赖关系和依赖方法，在后续指令中可以用 `$^`（`:`右侧的所有内容） 和 `$@` 简写。
5. `make` 和 makefile 可以对依赖关系进行推导。

### 指令不打印在交互界面

在makefile 文件中，若在指令前加 `@` 符号，可以让指令不打印在交互界面。

```bash
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  makefile
[Bjarne@VM-8-8-centos cppTest]$ vim makefile
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
a.exe:a.o
	@gcc a.o -o a.exe
a.o:a.s
	@gcc -c a.s -o a.o
a.s:a.i
	@gcc -S a.i -o a.s
a.i:a.c
	@gcc -E a.c -o a.i

.PHONY:clean
clean:
	rm -f a.exe

[Bjarne@VM-8-8-centos cppTest]$ make
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  a.exe  a.i  a.o  a.s  makefile
[Bjarne@VM-8-8-centos cppTest]$ 
```

### 行注释开头

makefile 用 `#` 作为行注释开头。Python 也选用 `#` 作为行注释的开头。

```bash
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
# a.exe:a.o
# 	@gcc a.o -o a.exe
# a.o:a.s
# 	@gcc -c a.s -o a.o
# a.s:a.i
# 	@gcc -S a.i -o a.s
# a.i:a.c
#	@gcc -E a.c -o a.i
a.exe:a.c
	@gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ make
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  a.exe  makefile
[Bjarne@VM-8-8-centos cppTest]$ 

```

### makefile 的变量和替换机制

makefile 允许设置变量，和Python类似，都是`变量名 = 变量值`，但变量的定义时不建议在`=`左、右设置空格。之后使用时可以`$(变量名)` 进行占位，`make` 会用变量的真实名去替代（类似 c 语言的宏）。

```bash
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  makefile
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
cc=gcc 
aim=a.exe 
src=a.c

a.exe:a.c
	$(cc) $(src) -o $(aim)
[Bjarne@VM-8-8-centos cppTest]$ make
gcc  a.c -o a.exe 
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  a.exe  makefile
[Bjarne@VM-8-8-centos cppTest]$ 
```

makefile中，`依赖关系:依赖方法` 中的依赖关系和依赖方法，在后续指令中可以用 `$^`（`:`右侧的所有内容） 和 `$@` （`:`左侧的所有内容）简写。

```bash
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  makefile
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
cc=gcc 
aim=a.exe 
src=a.c

a.exe:a.c
	gcc $^ -o $@

[Bjarne@VM-8-8-centos cppTest]$ make
gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ 
```

### 自动推导

`make` 和 makefile 可以对依赖关系进行推导。

```bash
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
a.exe:a.o
	gcc a.o -o a.exe

.PHONY:clean
clean:
	rm -f a.exe

[Bjarne@VM-8-8-centos cppTest]$ make
cc    -c -o a.o a.c
gcc a.o -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  a.exe  a.o  makefile
[Bjarne@VM-8-8-centos cppTest]$ 

```

可以看到，系统默认形成了`a.o` 文件。其中 `cc` 是 gcc 的一个链接文件。这是因为 `make` 有一套内置的隐式规则，知道如何从 `.c` 文件生成 `.o` 文件，当 `make` 发现需要 `a.o` 但 makefile 中没有明确定义如何生成 `a.o` 时，它会自动使用隐式规则。关于隐式规则，后期有机会再介绍。

```bash
[Bjarne@VM-8-8-centos cppTest]$ which cc
/usr/bin/cc
[Bjarne@VM-8-8-centos cppTest]$ ll /usr/bin/|grep cc
# 省略不重要的文件
lrwxrwxrwx    1 root root           3 Jan  8  2021 cc -> gcc
-rwxr-xr-x    1 root root     4142768 Nov  5  2016 ccmake
-rwxr-xr-x    2 root root      768608 Sep 30  2020 gcc
# 省略不重要的文件
[Bjarne@VM-8-8-centos cppTest]$ 

```

若将剩下的生成中间文件的指令补齐，`make` 在找不到依赖文件的情况下，会继续往下找，因此会生成中间文件（类似入栈、出栈的过程）。

一般不建议生成中间文件，直接通过 gcc 直接生成文件即可。因为生成中间文件比较费时。

```bash
[Bjarne@VM-8-8-centos cppTest]$ cat makefile
a.exe:a.o
	gcc a.o -o a.exe
a.o:a.s
	gcc -c a.s -o a.o
a.s:a.i
	gcc -S a.i -o a.s
a.i:a.c
	gcc -E a.c -o a.i

.PHONY:clean
clean:
	rm -f a.exe

[Bjarne@VM-8-8-centos cppTest]$ make
gcc -E a.c -o a.i
gcc -S a.i -o a.s
gcc -c a.s -o a.o
gcc a.o -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  a.exe  a.i  a.o  a.s  makefile
[Bjarne@VM-8-8-centos cppTest]$ 

```



# 实战小程序：进度条

进度条的应用通常是在下载、登录时使用。

进度条样式效果：

```bash
[###           ][xx%][\]
[==>           ][xx%][\]
```

实现进度条，首先需要了解2个概念：缓冲区和回车换行（`\n`）。

## 缓冲区原理的应用

首先是执行3个c程序带来的现象：

带 `sleep`函数的程序 （Linux的需要展开`unistd.h`），c程序的`printf`输出的字符串带`\n`，消息会立马刷新，且命令提示符（`[用户名@主机名 目录名]$`）会出现在下一行。

```bash
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  makefile
[Bjarne@VM-8-8-centos cppTest]$ cat a.c
#include <stdio.h>
#include <unistd.h>

int main(){
    printf("Can you see me?\n");
    sleep(2);
    return 0;
}
[Bjarne@VM-8-8-centos cppTest]$ make
gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ ./a.exe
Can you see me?  # 这里会先暂停2秒，再输出下一条命令提示符，文本无法展示过程
[Bjarne@VM-8-8-centos cppTest]$ 

```

而不带`\n`则会先暂停一下，再刷新消息，且命令提示符会紧随其后。

```bash
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  makefile
[Bjarne@VM-8-8-centos cppTest]$ cat a.c
#include <stdio.h>
#include <unistd.h>

int main(){
    printf("Can you see me?");
    sleep(2);
    return 0;
}
[Bjarne@VM-8-8-centos cppTest]$ make
gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ ./a.exe
Can you see me?[Bjarne@VM-8-8-centos cppTest]$ 
# 这里会先暂停2秒，再输出下一条命令提示符，文本无法展示过程

```

若带`fflush`函数对 `stdout` 进行强制刷新，则会先显示 `printf` 的字符串，再睡眠。

> 查询`fflush` ，在c语言中用于刷新流。
>
> `\n` 也是一种刷新流的策略：行刷新。

```bash
[Bjarne@VM-8-8-centos cppTest]$ ls
a.c  a.exe  makefile
[Bjarne@VM-8-8-centos cppTest]$ cat a.c
#include <stdio.h>
#include <unistd.h>

int main(){
    printf("Can you see me?");
    fflush(stdout);
    sleep(2);
    return 0;
}
[Bjarne@VM-8-8-centos cppTest]$ make
gcc a.c -o a.exe
[Bjarne@VM-8-8-centos cppTest]$ ./a.exe
Can you see me?[Bjarne@VM-8-8-centos cppTest]$ 
# 这里会先暂停2秒，再输出下一条命令提示符，文本无法展示过程

```

对3个现象进行分析：

c语言的函数内的语句是从上往下执行。所以不存在先执行 `sleep`，再执行 `printf` 的情况。但不带`\n`时，会看到 `sleep` 先运行的假象，这种情况只能是 `printf` 早已经运行，但字符串没有被显示出来。

合理的解释：

对C/C++语言，会针对标准输出`stdout`，Linux 会给程序提供默认的缓冲区。在`sleep`期间，字符串在输出缓冲区中。C/C++ 程序在运行时会默认打开三个文件流：`stdin`、`stdout`、`stderr`。其中`stdout` 就是输出缓冲区（标准输出流）。

关于缓冲区，涉及文件的更深层次的内容，后期会单独开一篇。



## 回车换行\n

c语言的转义运算符`\n`，即回车换行其实是做了2件事：换到下一行和将光标放在下一行的最开头。

老式回车键是想通过形式来告诉用户回车的原理。

<img src="./makefile001.png" alt="image-20250916200559130"  />

c语言的这2个转义字符`\r`是只回车，`\n`是 ”回车 + 换行“。

验证只回车`\r`而设计的倒计时程序：

```c
#include<stdio.h>
#include<unistd.h>

int main(){
    int cnt = 9;
    while(cnt){
        printf("%d\r",cnt);//输出完成后，光标会回到开头
        fflush(stdout);//没有fflush不会输出
        cnt--;
        sleep(1);
    }
    return 0;
}
```

第7行的`printf`，数据暂时被缓冲起来，直到最后一直没显示，且光标回到开头将数据清理掉，或干脆就不输出。解决方法是加`fflush`刷新`stdout`。如此便完成了一个倒计时程序。

这些c程序向显示器打印的并不是数字，而是字符。将各种数据转换成字符并输出是`printf`的工作，所以`printf`是格式化控制函数。

这就意味着，若之前的程序的`cnt` 初始化为10，则会输出`{90,80,...,10}`。这是倒计时程序不希望出现的 bug，所以可以用`%-2d` 进行格式控制，`-` 会让输出的字符进行左对齐，不加`-` 则是右对齐。

能显示2位数倒计时的程序如下：

```c
#include<stdio.h>
#include<unistd.h>

int main(){
    int cnt = 10;
    while(cnt){
        printf("%-2d\r",cnt);//输出倒计时
        fflush(stdout);
        cnt--;
        sleep(1);
    }
    return 0;
}

```

> 所以将显示器称为字符设备，因为它显示的只是字符，不是整型，也不是浮点数。
>
> 从键盘中得到的数据同理也是字符，哪怕是数字，这种读取方式是格式化输入，键盘也被称之为字符设备。
>
> c程序在进行打印时是要将数据打印到显示器上，直接刷新到显示器上时，每次刷新本质都是访问外设的过程，效率太低，所以c语言先暂时将字符串存储在缓冲区。
>
> 类比的话是快递小哥，一次将所有的快递给小哥，小哥一次将所有快递递交给用户。强制刷新输出流相当于是给小哥压力，让小哥及时反馈信息。

拥有这些准备知识，就可以开始尝试写进度条了。

## 简易进度条的实现

进度条要求：

1. 限制一段范围。
2. 在这段范围中推进自定义的符号（可以是`#`，也可以是`==>`）。
3. 在末尾携带进度百分比。
4. 给出旋转光标给用户反馈进度条一直在推进。

进度条样式效果：

```bash
[###           ][xx%][\]
[==>           ][xx%][\]
```

实现这个样式，可以用`printf("[][][]\r");`输出，因为是`\r`，所以`printf` 的下一句需要是 `fflush(stdout)`。之后就是对进度条的进度操作。

`sleep` 在 Linux 中是以秒为单位，这对进度条来说太慢，需要使用`usleep` 。`usleep` 相比`sleep` 是以微秒为单位，让程序挂起（或者说睡眠）一段时间，1000微秒等于1毫秒。

三个版本：

1. 简单原理版本。
2. 实际工程实践版本。
3. C语言扩展--带颜色。

简单原理版本（不进行封装和声明、定义分离）：

```c
#include<stdio.h>
#include<unistd.h>
#include<string.h>

const char *rotat = "|/-\\";//进度条旋转表示程序正常推进

int main(){
    int rate = 0;//进度百分比
    char bar[101] = {'\0'};//进度条本体
    int num = strlen(rotat);

    while(rate<=100){
        //%-100s 是为了打印长度为100的进度条，-是左对齐。
        printf("[%-100s][%d%%][%c]\r", bar, rate, rotat[rate%num]);
        fflush(stdout);
        usleep(1000*50);//50毫秒为单位，可修改
        bar[rate++] = '#';
    }
    printf("\n");
    return 0;
}

```

## 工程实践版本

无论是任何进度条，一定是和某种任务关联。例如下载、压缩、解压、删除文件等。

所以整理进度条函数的逻辑：

1. 给进度 `rate`，进度条函数会根据进度 `rate` 刷新进度条，之后在调用进度条函数的地方进行任务记录、任务进行。
2. 先执行任务时，再打印进度条时，循环可设置进度为 $100\%$ 时停止。
3. 可能会出现任务暂停在某个进度的情况（例如网络卡顿），这种情况可以通过操控进度的值让最后的光标进行旋转，“迷惑” 用户。
4. 这里采用101个长度的`char` 数组模拟进度条，`floor(rate*100)`  作为进度条的填充，则有效进度条范围是`[0,99]`，当进度达到 $99\%$ 时理应填满整个进度条。
5. 进度有可能从 0.99 跳到 1，或者1.1 等，所以当进度大于$100\%$ 时直接输出$100\%$ 。
6. 可选择自定义的进度条，例如`==>`。

参考程序如下：

```cpp
#include<stdio.h>
#include<unistd.h>
#include<string.h>

typedef void(*callshow_v)(double);//进度条展示版本

void process(double rate){
    const char *rotat = "|/-\\";//进度条旋转表示程序正常推进
    static char bar[101] = {'\0'};//进度条本体
    static size_t cnt = 0;//控制进度条的旋转
    int rlen = strlen(rotat);
    if(rate < 100.0){//决定采用浮点数时，要考虑不同下载速度的情况
        if(rate<99.0){
            bar[(int)rate] = '=';
            bar[(int)rate+1] = '>';
            int tmp = (int)rate-1;
            while(tmp>=0&&bar[tmp]!='='){
                bar[tmp]='=';
                --tmp;
            }
        }
        else{
            bar[99]='=';
            bar[100]='\0';
        }
        printf("[%-100s][%5.1lf%%][%c]\r",bar,rate,rotat[cnt%rlen]);
        ++cnt;
        fflush(stdout);
    }
    else{
		rate = 100.0;
        bar[99]='=';
        bar[100]='\0';
		int tmp = 98;//假设用的是更先进的通信技术例如5G网
        while(tmp>=0&&bar[tmp]!='='){
			bar[tmp]='=';
            --tmp;
        }
        printf("[%-100s][%5.1lf%%][%c]\n",bar,rate,rotat[0]); 
        memset(bar,'\0',sizeof(bar));
    }
}

void download(callshow_v cs){
    int aim = 1024*1024*10;//10MB 待下载数据
    int v = 1024;//下载速度为1KB
    int ret = 1024*1024*9;//下载进度记录
    //int cnt = 500;
    while(ret<aim){//达到目标时应该停止，因为这里是先下载再记录
        ret += v;//正在下载
        usleep(1000*10);
        double rate = ret*100.0/aim;
        //while(cnt&&rate>=66.0){//模拟网路卡顿的情况
        //    ret-=v;
        //    rate = ret*100.0/aim;
        //    --cnt;
        //}
        cs(rate);
    }
}

int main(){
    download(process);
    //download(process);//模拟多次下载
    //download(process);
    return 0;
}

```

C语言可以使用格式化语句输出简单的彩色代码片段。例如，蓝底红字：

```c
printf("[\033[31;44m%-100s\033[0m][%5.1lf%%][%c]\r",bar,rate,rotat[cnt%rlen]); 
```

具体参考[【C语言】C语言基础——printf带颜格式化输出（Linux）_printf格式化输出文本颜色-CSDN博客](https://blog.csdn.net/MendyMeng/article/details/121524491?spm=1001.2014.3001.5506)。
