#!/bin/bash

# 获取系统的类型，Centos Debian Mac 需要的静态库是不一样的
# 目前只有 Centos Debian Mac 三个系统的静态库

function kill_old_server()
{
    echo "---------- kill old server begin ----------"

    system_type=`get_system_type`
    if [ ${system_type} == "Darwin" ]; then
        # mac
        old_server_pid=$(lsof -i:9092 | grep oj_server | awk '{print $2}')
    elif [ ${system_type} == "Linux" ]; then
        # linux
        old_server_pid=$(ps -e | grep oj_server | awk '{print $1}')
    else 
        echo "Not support system type , can't kill old server : "${system_type}
        exit
    fi

    if [ "$old_server_pid" = "" ]; then
        echo "old oj_server not exit"
    else
        echo "process id : "${old_server_pid}

        kill -9 ${old_server_pid}
        if [ $? -eq 0 ];then
            echo "kill old oj_server success"
        else
            echo "kill old oj_server fail"
        fi
    fi
    echo "---------- kill old server end ----------"
}


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

function start_server()
{
    echo "---------- start server begin ----------"
    make
    kill_old_server
    ./oj_server >> /dev/null &
    echo "start oj_server success"
    echo "---------- start server end ----------" 
}

function start_mac_server()
{
    echo "cp ./lib/Mac/* ./lib/"
    cp ./lib/Mac/* ./lib/
    start_server
}

function start_centos_server()
{
    echo "cp ./lib/Centos/* ./lib/"
    cp ./lib/Centos/* ./lib/
    start_server
}

function start_debian_server()
{
    echo "cp ./lib/Debian/* ./lib/"
    cp ./lib/Debian/* ./lib/
    start_server
}

function start_linux_server()
{
    linux_distro=`get_linux_distro`
    echo "linux_distro is : "${linux_distro}
    if [ ${linux_distro} == "CentOS" ]; then
        # CentOS
        start_centos_server
    elif [ ${linux_distro} == "Debian" ]; then
        # Debian
        start_debian_server
    else 
        echo "Not support linux distro : "${linux_distro}
    fi
}

function main()
{
    color="$(tput setaf 4)"
    normal="$(tput sgr0)"
    printf "${color}"
    echo "==================== start.sh begin ===================="
    system_type=`get_system_type`
    echo "system_type is : "${system_type}
    if [ ${system_type} == "Darwin" ]; then
        # mac
        start_mac_server
    elif [ ${system_type} == "Linux" ]; then
        # linux
        start_linux_server
    else 
        echo "Not support system type : "${system_type}
    fi
    echo "==================== start.sh end ===================="
    printf "${normal}"
}

# main 函数启动
main
