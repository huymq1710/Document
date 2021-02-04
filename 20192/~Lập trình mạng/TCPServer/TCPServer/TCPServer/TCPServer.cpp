#include <iostream>
#include <WinSock2.h>
void SendTCP(SOCKET c, char* buffer, int len) {
    int s = send(c, buffer, len, 0);
    //TH ma s < r : dlieu chua dc gui het
    while (s < len)
    {
        s += send(c, buffer + s, len - s, 0);
    }
}

int RecvTCP(SOCKET c, char* buffer, int len, int count) { //len: bike max coiunt bike muon nhan
    int r = recv(c, buffer, len, 0); //r bike da nhan
    printf("%d bytes received\n",r);
    while (r<count && r < len) //hoac la nhan het, hoac la nhan qua max bo nho cap phat
    {
        r += recv(c, buffer + r, len, 0);
        printf("%d bytes received\n", r);

    }
    return r;
}

int main()
{
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //s la socket le tan
    struct sockaddr_in saddr; //khai bao dia chi va cong ma le tan dung do
    saddr.sin_family = AF_INET; //kieu cua dia chi: la IPV4
    saddr.sin_port = htons(8888); //co them ham htons ms chay
    saddr.sin_addr.S_un.S_addr = 0; //Co 3 cach gan o bai 1, day su cach 4 
    //Vay la da co du thong tin socket le tan o cong 8888
    
    bind(s, (sockaddr*)&saddr, sizeof(saddr));
    //ham blind sd tham so sockaddr nhung khoi tao s van su dung sockaddr_in
    //ko khai bao sockaddr de do ep kieu: sockaddr = mang[14] => dien thong tin kho
    
    listen(s, 10); //hang doi co 10 ghe
    
    struct sockaddr_in caddr; //khach den
    int clen = sizeof(caddr);
    SOCKET c= accept(s, (sockaddr*)&caddr, &clen); //sd socket c de trao doi dl

    char* buffer = (char*)calloc(1024, 1); //bo dem de nhan dlieu
    while (0 == 0) {
        int r= RecvTCP(c, buffer, 1024, 32); //Nhan du 32 bike ms tra ve
        SendTCP(c, buffer, r);
    }


}


