//
//  client.cpp
//  C socket
//
//  Created by ppdfour on 2018/6/5.
//  Copyright © 2018年 ppdfour. All rights reserved.
//
//
//#include <stdio.h>
//#include <netinet/in.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <string.h>
//#include <unistd.h>
//#define BUFFER_SIZE 1024
//#define FILE_NAME_MAX_SIZE 512
//
//int main (int argc, const char * argv[])
//{
//    struct sockaddr_in client_addr;
//    client_addr.sin_len = sizeof(struct sockaddr_in);
//    client_addr.sin_family = AF_INET;
//    client_addr.sin_port = htons(11332);
//    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//    bzero(&(client_addr.sin_zero),8);
//
//    int client_socket = socket(AF_INET, SOCK_STREAM, 0);//返回新套接口的描述符
//    if (client_socket == -1) {
//        perror("socket error");
//        return 1;
//    }
//    char recv_msg[BUFFER_SIZE];
//    char reply_msg[BUFFER_SIZE];
//
//    if (connect(client_socket, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_in))==0)     {
//        //connect 成功之后，其实系统将你创建的socket绑定到一个系统分配的端口上，且其为全相关，包含服务器端的信息，可以用来和服务器端进行通信。
//        fd_set client_fd_set;
//        struct timeval tv;
//        tv.tv_sec = 20;
//        tv.tv_usec = 0;
//        while (1) {
//
//            FD_ZERO(&client_fd_set);//清空文件描述符集
//            FD_SET(STDIN_FILENO, &client_fd_set);//增加一个新的文件描述符
//            FD_SET(client_socket, &client_fd_set);
//
//            int ret = select(client_socket + 1, &client_fd_set, NULL, NULL, &tv);//～；可读文件描述符集合；可写文件描述符集合；异常文件描述符集合；～//返回发生变化的描述符
//            if (ret < 0 ) {
//                printf("select 出错!\n");
//                continue;
//            }
//            else if(ret ==0){
//                printf("select 超时!\n");
//                continue;
//            }
//            else{
//                if (FD_ISSET(STDIN_FILENO, &client_fd_set)) {//测试该文件描述符是否在指定的集合中
//                    printf("Please Input File Name On Server:\t");
//                    bzero(reply_msg, BUFFER_SIZE);
//                    fgets(reply_msg, BUFFER_SIZE, stdin);
//
//                    // 输入文件名 并放到缓冲区buffer中等待发送
//
//
//                    // 向服务器发送buffer中的数据
//                    if(send(client_socket, reply_msg, BUFFER_SIZE, 0) < 0)
//                    {
//
//                        perror("Send File Name Failed:");
//                        _exit(1);
//                    }
//                    printf("%s",reply_msg);
//                    bzero(reply_msg, BUFFER_SIZE);
//                    long byte_num = recv(client_socket,recv_msg,BUFFER_SIZE,0);
//                    if (byte_num==0) {
//                        printf("file not found!");
//                    }
//                    else if(byte_num>0){
//                    char path[100]="/Users/ppdfour/ppstudy/计算机网络/homework/课设/client file/client file/client file/";
//                    // 打开文件，准备写入
//                    FILE *fp = fopen(strcat(path, reply_msg), "w");
//                    if(NULL == fp)
//                    {
//                        printf("File:\t%s Can Not Open To Write\n", recv_msg);
//                        _exit(1);
//                    }
//                    // 从服务器接收数据到buffer中
//                    // 每接收一段数据，便将其写入文件中，循环直到文件接收完并写完为止
//                    else{
//                        byte_num=0;
//                        while((byte_num = recv(client_socket,recv_msg, BUFFER_SIZE, 0)) > 0)
//                        {
//                            fwrite(recv_msg, sizeof(char), byte_num, fp);
//                            bzero(recv_msg, BUFFER_SIZE);
//                        }
//                        printf("Receive File:\t%s From Server IP Successful!\n",recv_msg); // 接收成功后，关闭文件，关闭socket
//                        fclose(fp);
//                    }
//                }
//                }
//            }//else
//        }//while
//    }//if
//    printf("Hello, World!\n");
//    return 0;
//}







//  main.cpp
//  client file
//
//  Created by ppdfour on 2018/6/7.
//  Copyright © 2018年 ppdfour. All rights reserved.
#include <iostream>
#include <string.h>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SERVER_PORT 11332
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
using namespace std;

int main()
{
    // 声明并初始化一个客户端的socket地址结构
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
//    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr.sin_port = htons(11332);
    client_addr.sin_addr.s_addr = inet_addr("172.24.57.8");
    client_addr.sin_len = sizeof(struct sockaddr_in);

    // 创建socket，返回值为socket描述符
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket < 0)
    {
        perror("Create Socket Failed:");
        exit(1);
    }


    // 声明一个服务器端的socket地址结构，并用服务器那边的IP地址及端口对其进行初始化，用于后面的连接
//    struct sockaddr_in server_addr;
//    bzero(&server_addr, sizeof(server_addr));
//    server_addr.sin_family = AF_INET;
//    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) == 0)
//    {
//        perror("Server IP Address Error:");
//        exit(1);
//    }
//    server_addr.sin_port = htons(SERVER_PORT);
//    socklen_t server_addr_length = sizeof(server_addr);

    // 向服务器发起连接，连接成功后client_socket_fd代表了客户端和服务器的一个socket连接
//    if(connect(client_socket_fd, (struct sockaddr*)&server_addr, server_addr_length) < 0)
//    {
//        perror("Can Not Connect To Server IP:");
//        exit(0);
//    }

    if (connect(client_socket, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_in))==0)
    {   //connect 成功之后，创建的socket会绑定到系统分配的端口上，且其为全相关，包含服务器端的信息，可以用来和客户端进行通信。
        // 输入文件名并放到缓冲区buffer中等待发送
        char file_name[FILE_NAME_MAX_SIZE+1];
        bzero(file_name, FILE_NAME_MAX_SIZE+1);
        printf("Please Input File Name On Server:\t");
        scanf("%s", file_name);

        char buffer[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);
        strncpy(buffer, file_name, strlen(file_name)>BUFFER_SIZE?BUFFER_SIZE:strlen(file_name));

        // 向服务器发送buffer中的数据
        if(send(client_socket, buffer, BUFFER_SIZE, 0) < 0)
        {
            perror("Send File Name Failed:");
            exit(1);
        }
        char path[100]="/Users/ppdfour/ppstudy/计算机网络/homework/课设/client file/client file/client file/";
        // 打开文件，准备写入
        FILE *fp = fopen(strcat(path, file_name), "w");
        if(NULL == fp)
        {
            printf("File:\t%s Can Not Open To Write\n", file_name);
            exit(1);
        }

        // 从buffer中接收一段数据，写入文件中，循环直到文件接收完并写完为止
        bzero(buffer, BUFFER_SIZE);
        long int length = 0;
        length=recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (length==0){
            cout<<"file not found!"<<endl;
        }
        else {
            while(length>0)
            {
                
                if(fwrite(buffer, sizeof(char), BUFFER_SIZE, fp) < length)
                {
                    printf("File:\t%s Write Failed\n", file_name);
                    break;
                }
//                printf("%s",buffer);
                bzero(buffer, BUFFER_SIZE);
                length=recv(client_socket, buffer, BUFFER_SIZE, 0);
            }

            // 接收成功后，关闭文件，关闭socket
            printf("Receive File:\t%s From Server IP Successful!\n", file_name);
            fclose(fp);
            close(client_socket);
        }
    }
    return 0;
}



