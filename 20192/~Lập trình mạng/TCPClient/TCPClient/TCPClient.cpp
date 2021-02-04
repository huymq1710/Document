#include <iostream>
#include <conio.h>
#include<stdio.h>
#include <WinSock2.h>
#include <string.h>
using namespace std;
int main()
{
    WSADATA   data;
    WSAStartup(MAKEWORD(2, 2), &data);
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //khoi tao dia chi server 192.168.1.6 va dang o cong 8888
    SOCKADDR_IN ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(8888);
    ServerAddr.sin_addr.s_addr = inet_addr("192.168.1.6");
    //ket noi
    connect(s, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));

    //Nhan du lieu
    char* buffer = (char*)calloc(1024, 1);
    while (strcmp(buffer,"quit") != 0) {
        fflush(stdin);
        memset(buffer, 0, 1024);
        cin.getline(buffer, 1024, '\n');
        send(s, buffer, strlen(buffer), 0);
    }
    free(buffer);
    buffer = NULL;
    //
    closesocket(s);
    WSACleanup();
}

