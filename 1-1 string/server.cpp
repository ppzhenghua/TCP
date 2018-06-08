//
//  server.cpp
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
    struct sockaddr_in server_addr;
    server_addr.sin_len = sizeof(struct sockaddr_in);
    server_addr.sin_family = AF_INET;                           //Address families AF_INET互联网地址簇
    server_addr.sin_port = htons(11332);                        //主机字节序转网络字节序
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");       //10进制ip换成长整型
    bzero(&(server_addr.sin_zero),8);                           //前8个字节置为0
    
    //创建socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);        //soket返回一个新创建的soket的描述符
    if (server_socket == -1) {
        perror("socket error");
        return 1;
    }
    
    //绑定socket：将创建的socket绑定到本地的IP地址和端口，此socket是半相关的，只是负责侦听客户端的连接请求，并不能用于和客户端通信
    int bind_result = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_result == -1) {
        perror("bind error");
        return 1;
    }
    
    //listen侦听（socket，等待接受的队列大小）connect请求时,完成三次握手后先将连接放到这个队列中，直到被accept处理。如果队列满了，又有新的连接的时候，对方可能会收到出错信息。
    if (listen(server_socket, 5) == -1) {                       //等待链接队列的最大长度
        perror("listen error");
        return 1;
    }
    
    struct sockaddr_in client_address;                          //给一会想要accept的的客户端一个名字和空间
    socklen_t address_len;
    int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &address_len);
    //返回的client_socket为一个全相关的socket，包含client的地址和端口信息，服务器通过client_socket可以和客户端进行通信。
    if (client_socket == -1) {
        perror("accept error");
        return -1;
    }
    
    char recv_msg[1024];                                        //发送消息的缓冲区
    char reply_msg[1024];                                       //接受消息的缓冲区
    
    while (1) {
        bzero(recv_msg, 1024);                                  //清空缓冲区
        bzero(reply_msg, 1024);
        
        printf("reply:");
        scanf("%s",reply_msg);
        send(client_socket, reply_msg, 1024, 0);                //send()返回值是发送数据的总数
        
        long byte_num = recv(client_socket,recv_msg,1024,0);    //recv()返回值是从客户端接口copy到缓冲区的字节数
        recv_msg[byte_num] = '\0';
        printf("client said:%s\n",recv_msg);
        
    }
    
    return 0;
}
