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

int main (int argc, const char * argv[])
{
    struct sockaddr_in client_addr;
    client_addr.sin_len = sizeof(struct sockaddr_in);
    client_addr.sin_family = AF_INET;                                   //Address families AF_INET互联网地址簇
    client_addr.sin_port = htons(11332);                                //端口号11332从主机字节序转网络字节序
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");               //10进制ip换成长整型
    bzero(&(client_addr.sin_zero),8);                                   //将前8个字节置为0
    
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);                //创建socket
    if (client_socket == -1) {
        perror("socket error");
        return 1;
    }
    char recv_msg[1024];            //接受消息的缓冲区
    char reply_msg[1024];           //发送消息的缓冲区
    
    if (connect(client_socket, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_in))==0)     {
        //connect 成功之后，创建的socket会绑定到系统分配的端口上，且其为全相关，包含服务器端的信息，可以用来和客户端进行通信。
        while (1) {
            bzero(recv_msg, 1024);          //请空缓冲区
            bzero(reply_msg, 1024);
            long byte_num = recv(client_socket,recv_msg,1024,0);        //recv返回从server端口copy来的总子节数，是长整型
            recv_msg[byte_num] = '\0';
            printf("server said:%s\n",recv_msg);
            
            printf("reply:");
            scanf("%s",reply_msg);
            if (send(client_socket, reply_msg, 1024, 0) == -1) {        //把发送消息缓冲区的内容传到端口
                perror("send error");
            }
        }
        
    }
    return 0;
}
//全相关与半相关：半相关（half-association）是指一个三元组 (协议,本地IP地址,本地端口),通过这个三元组就可以唯一标识一个网络中的进程,一般用于listening socket。但是实际进行通信的过程，至少需要两个进程，且它们所使用的协议必须一致，所以一个完成的网络通信至少需要一个五元组表示(协议,本地地址,本地端口,远端地址,远端端口)，这样的五元组叫做全相关。
