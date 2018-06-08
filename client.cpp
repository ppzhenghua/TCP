//
//  client.cpp
//  C socket
//
//  Created by ppdfour on 2018/6/5.
//  Copyright © 2018年 ppdfour. All rights reserved.
//

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512

int main (int argc, const char * argv[])
{
    struct sockaddr_in client_addr;
    client_addr.sin_len = sizeof(struct sockaddr_in);
    client_addr.sin_family = AF_INET;                           //Address families AF_INET互联网地址簇
    client_addr.sin_port = htons(11332);                        //主机字节序转网络字节序
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");     //10进制ip换成长整型
    bzero(&(client_addr.sin_zero),8);                           //前8个字节置为0
    
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);        //soket()返回一个新创建的soket的描述符
    if (client_socket == -1) {
        perror("socket error");
        return 1;
    }
    char recv_msg[BUFFER_SIZE];
    char reply_msg[BUFFER_SIZE];
    
    if (connect(client_socket, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_in))==0)     {
        //connect 成功之后，其实系统将你创建的socket绑定到一个系统分配的端口上，且其为全相关，包含服务器端的信息，可以用来和服务器端进行通信。
        fd_set client_fd_set;
        struct timeval tv;
        tv.tv_sec = 20;
        tv.tv_usec = 0;
        while (1) {
            FD_ZERO(&client_fd_set);                        //清空文件描述符集
            FD_SET(STDIN_FILENO, &client_fd_set);           //增加一个新的文件描述符
            FD_SET(client_socket, &client_fd_set);
            
            int ret = select(client_socket + 1, &client_fd_set, NULL, NULL, &tv);
            //～；可读文件描述符集合；可写文件描述符集合；异常文件描述符集合；～//返回发生变化的描述符
            if (ret < 0 ) {
                printf("select 出错!\n");
                continue;
            }
            else if(ret ==0){
                printf("select 超时!\n");
                continue;
            }
            else{
                if (FD_ISSET(STDIN_FILENO, &client_fd_set)) {                   //测试该文件描述符是否在指定的集合中
                    bzero(reply_msg, BUFFER_SIZE);
                    fgets(reply_msg, BUFFER_SIZE, stdin);
                    if (send(client_socket, reply_msg, BUFFER_SIZE, 0) == -1) {//send()返回值是发送数据的总数
                        perror("发送消息出错!\n");
                    }
                }
                if (FD_ISSET(client_socket, &client_fd_set)) {
                    bzero(recv_msg, BUFFER_SIZE);
                    long byte_num = recv(client_socket,recv_msg,BUFFER_SIZE,0);//recv()返回值是从客户端接口copy到缓冲区的字节数
                    if (byte_num > 0) {
                        if (byte_num > BUFFER_SIZE) {
                            byte_num = BUFFER_SIZE;
                        }
                        recv_msg[byte_num] = '\0';
                        printf("服务器:%s\n",recv_msg);
                    }
                    else if(byte_num < 0){
                        printf("接受消息出错!\n");
                    }
                    else{
                        printf("服务器端退出!\n");
                        _exit(0);
                    }

                }//if
            }//else
        }//while
    }//if
    return 0;
}

