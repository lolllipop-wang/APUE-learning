# 如何编译apue环境

## 准备工作
### 需要gcc 和 make
  $ sudo apt-get install gcc make
### 如果是Linux系统，可能需要安装libbsd
  $ sudo apt-get install libbsd-dev
  
## 下面开始编译
### 首先将resource目录中的压缩文件解压
  $ tar -zxvf src.3e.tar.gz
  $ cd ./apue.3e
### 执行make
  $ make

### make 完成后，将apue.h和libapue.a分别移动到/usr/include和/usr/lib(方便起见)， 移动到其他文件夹， 编译书上的程序时再指定include 和 lib目录。
  $ sudo cp ./include/apue.h /usr/include/
  $ sudo cp ./lib/libapue.a /usr/lib/
### 实例 cc -o demo demo.c -lapue
