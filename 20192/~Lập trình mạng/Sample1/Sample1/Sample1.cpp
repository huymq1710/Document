
#include <iostream>
#include <WinSock2.h>
using namespace std;
using namespace std;
int main()
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int r = WSAStartup(version, &data); //LPWSADATA = *WSADATA
	if (r == 0) {
		printf("Khoi tao thanh cong!\n");
	}
	else {
		printf("Khoi tao that bai!\n");
	}
	WSACleanup();
	

}

