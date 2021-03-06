# **基于C++实现的简易在线oj**
## **运行环境**
### Linux
### make
### GCC 4.9+
- Centos下gcc升级方法
  ```
  //这是偷懒的一种做法
  yum install centos-release-scl -y
  yum install devtoolset-7 -y
  scl enable devtoolset-7 bash   // 只有本次有效 
  //在 ~/.bash_profile 里面 添加一行 每次打开终端都会自动执行 即可永久生效
  gcc --version
  ```
- Debian 一般默认版本即可

## **第三方库**
- [**httplib**](https://github.com/yhirose/cpp-httplib) 只需要使用头文件
- [**jsoncpp**](https://github.com/open-source-parsers/jsoncpp) 已链接静态库

        自行编译方法，解压缩后输入以下命令
        先安装scons
            sudo yum install scons
            或者
            sudo apt-get install scons
        也可以源码编译，先安装 python
            sudo yum install python2.7
            或者
            sudo apt-get install python2.7
            然后 进入 lib 文件夹下，解压scons
            cd scons
            sudo python setup.py install
        再编译 jsoncpp
            scons platform=linux-gcc

- [**ctemplate**](https://github.com/olafvdspek/ctemplate) 已链接静态库

        自行编译方法，解压缩后输入以下命令
        ./autogen.sh && ./configure
        make

- **boost** 需进行安装

        yum install boost
        yum install boost-devel
        yum install boost-doc
        或者
        apt-get install boost
        apt-get install boost-devel
        apt-get install boost-doc

## **服务端运行**
   `chmod 755 ./start && 。/start`
## **查看端口状态**
   `netstat -anp | grep 12121`
## **客户端运行**
   `ip:12121/`
