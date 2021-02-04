#include <iostream>
#include <stdio.h>

using namespace std;

#include<WinSock2.h>
int main()
{
  
    SOCKET c = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    struct sockaddr_in saddr; //khai bao dia chi va cong ma le tan dung do
    saddr.sin_family = AF_INET; //kieu cua dia chi: la IPV4
    saddr.sin_port = htons(8888); //co them ham htons ms chay
    saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); 
    //Vay la da co du thong tin socket le tan o cong 8888
    connect(c, (sockaddr*)&saddr, sizeof(saddr));
    char* buffer = (char*)calloc(1024, 1);
    while (0==0)
    {
        memset(buffer, 0, 1024);
        gets(buffer);

    }
}

