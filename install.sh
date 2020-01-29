#!/bin/bash

# 获取系统的类型，Centos Debian Mac 需要的静态库是不一样的
# 目前只有 Centos Debian Mac 三个系统的静态库

function get_system_type()
{
    echo $(uname)
}

function get_linux_distro()
{
    # Linux 主要区别就是 Debian 和 Centos
    # 其他系统后面再添加
    if grep -Eq "Ubuntu" /etc/*-release; then
        echo "Debian"
    elif grep -Eq "Deepin" /etc/*-release; then
        echo "Debian"
    elif grep -Eq "Debian" /etc/*-release; then
        echo "Debian"
    elif grep -Eq "CentOS" /etc/*-release; then
        echo "CentOS"
    else
        echo "UnKnow"
    fi
}

function main()
{
    echo "---------- start install ----------"
    system_type=`get_system_type`
    if [ ${system_type} == "Darwin" ]; then
        # mac
        echo "cp ./lib/Mac/* ./lib/"
        cp ./lib/Mac/* ./lib/
    elif [ ${system_type} == "Linux" ]; then
        # linux
        system_distro=`get_linux_distro`
        if [ ${system_distro} == "CentOS" ]; then
            echo "cp ./lib/Centos/* ./lib/"
            cp ./lib/Centos/* ./lib/
        elif [ ${system_distro} == "Debian" ]; then
            echo "cp ./lib/Centos/* ./lib/"
            cp ./lib/Debian/* ./lib/
        else 
            echo "UnKnow System Return"
        fi
    else 
        echo "UnKnow System Return"
    fi
}

# main 函数启动
main
