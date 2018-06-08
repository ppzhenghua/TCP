//
//  try.cpp
//  server
//
//  Created by ppdfour on 2018/6/7.
//  Copyright © 2018年 ppdfour. All rights reserved.
//
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SERVER_PORT 11332
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512

int main(void)
{
    // 声明并初始化一个服务器端的socket地址结构
    struct sockaddr_in server_addr;
    server_addr.sin_len = sizeof(struct sockaddr_in);
    server_addr.sin_family = AF_INET;                                   //Address families AF_INET互联网地址簇
    server_addr.sin_port = htons(SERVER_PORT);                          //主机字节序转网络字节序
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");               //10进制ip换成长整型
    bzero(&(server_addr.sin_zero),8);                                   //前8个字节置为0
    
    //创建socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);                //soket返回一个新创建的soket的描述符
    if (server_socket == -1) {
        perror("socket error");
        return 1;
    }
//    int opt = 1;
//    setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    //绑定socket：将创建的socket绑定到本地的IP地址和端口，此socket是半相关的，只是负责侦听客户端的连接请求，并不能用于和客户端通信
    int bind_result = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_result == -1) {
        perror("bind error");
        return 1;
    }
    
    // socket监听listen
    if (listen(server_socket, 5) == -1) {                              //等待链接队列的最大长度
        perror("listen error");
        return 1;
    }
    
    while(1)
    {
        // 定义客户端的socket地址结构
        struct sockaddr_in client_addr;
        socklen_t client_addr_length = sizeof(client_addr);
        
        // 接受连接请求，返回一个新的socket(描述符)，这个新socket用于同连接的客户端通信
        // accept函数会把连接到的客户端信息写到client_addr中
        int new_server_socket_fd = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_length);
        if(new_server_socket_fd < 0)
        {
            perror("Server Accept Failed:");
            break;
        }
        
        // recv函数接收数据到缓冲区buffer中，返回值是从客户端接口copy到缓冲区的字节数
        char buffer[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);
        if(recv(new_server_socket_fd, buffer, BUFFER_SIZE, 0) < 0)
        {
            perror("Server Recieve Data Failed:");
            break;
        }
        
        // 将文件从buffer拷贝到file_name中
        char file_name[FILE_NAME_MAX_SIZE+1];
        bzero(file_name, FILE_NAME_MAX_SIZE+1);
        strncpy(file_name, buffer, strlen(buffer)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(buffer));
        printf("%s\n", file_name);
        
        // 打开文件并读取文件数据
        char path[100]="/Users/ppdfour/ppstudy/计算机网络/homework/课设/server file/server file/server file/";
        FILE *fp = fopen(strcat(path, file_name), "r");
        if(NULL == fp)
        {
            printf("File:%s Not Found\n", file_name);
        }
        else
        {
            bzero(buffer, BUFFER_SIZE);
            long int length = 0;
            // 每读取一段数据，便将其发送给客户端，循环直到文件读完为止
            while((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
            {
                if(send(new_server_socket_fd, buffer, length, 0) < 0)
                {
                    printf("Send File:%s Failed./n", file_name);
                    break;
                }
                bzero(buffer, BUFFER_SIZE);
            }
            
            // 关闭文件
            fclose(fp);
            printf("File:%s Transfer Successful!\n", file_name);
        }
        // 关闭与客户端的连接
        close(new_server_socket_fd);
    }
    // 关闭监听用的socket
    close(server_socket);
    return 0;
}

