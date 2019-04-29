# **基于C++实现的简易在线oj**
## **运行环境**
### Linux
### GCC 4.9+
- Centos下gcc升级方法
```
//这时偷懒的一种做法
yum install centos-release-scl -y
yum install devtoolset-7 -y
scl enable devtoolset-7 bash
echo $PATH
//将输出的内容复制
vim ~/.bashrc
//添加一行
export PATH=复制的内容
//保存退出后，重写打开当前终端或者在终端输入 source ~/.bashrc 即可生效
gcc --version
```
- Debian 一般默认版本即可
### make

## **第三方库**
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [httplib](https://github.com/yhirose/cpp-httplib)
- [ctemplate](https://github.com/olafvdspek/ctemplate)
- boost
- 源码已下载至lib中，需自行编译安装，httplib只需要头文件即可，项目中已包含可不必下载。

## **run**
   `./oj_server`
## **查看端口状态**
   netstat -anp | grep 9092
