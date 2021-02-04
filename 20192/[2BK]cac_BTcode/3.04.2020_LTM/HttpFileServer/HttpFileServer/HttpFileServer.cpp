#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>
#include <iostream>
#include <iostream>
char* g_html = NULL;

void SendFile(SOCKET c, char* path)
{
	char header[1024];
	memset(header, 0, 1024);
	strcpy(header, "HTTP/1.1 200 OK\n");
	char* pattern = strstr(path, "VINHLT");
	pattern[0] = 0;
	if (_stricmp(path + strlen(path) - 4, ".jpg") == 0)
	{
		sprintf(header + strlen(header), "Content-Type: image/jpeg\n");
	}
	else if (_stricmp(path + strlen(path) - 4, ".mp4") == 0)
	{
		sprintf(header + strlen(header), "Content-Type: video/mp4\n");
	}
	else if (_stricmp(path + strlen(path) - 4, ".mp3") == 0)
	{
		sprintf(header + strlen(header), "Content-Type: audio/mp3\n");
	}
	else
	{
		sprintf(header + strlen(header), "Content-Type: application/octet-stream\n");
	}
	char buffer[1024];
	FILE* f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	int flen = ftell(f);
	fseek(f, 0, SEEK_SET);
	sprintf(header + strlen(header), "Content-Length: %d\n\n", flen);
	send(c, header, strlen(header), 0);
	while (!feof(f))
	{
		int r = fread(buffer, 1, sizeof(buffer), f);
		send(c, buffer, r, 0);
	}
	fclose(f);
}

void ScanFolder(char* path)
{
	WIN32_FIND_DATAA DATA;
	char fullpath[1024];
	memset(fullpath, 0, sizeof(fullpath));
	if (path[strlen(path) - 1] == '/')
		sprintf(fullpath, "C:%s*.*", path);
	else
		sprintf(fullpath, "C:%s/*.*", path);

	HANDLE hFind = FindFirstFileA(fullpath, &DATA);
	char tmpHtml[1024];
	memset(tmpHtml, 0, sizeof(tmpHtml));

	if (DATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		if (path[strlen(path) - 1] == '/')
		{
			sprintf(tmpHtml, "<a href=\"%s%s\"><b>%s</b></a><br>", path, DATA.cFileName, DATA.cFileName);
		}
		else
		{
			sprintf(tmpHtml, "<a href=\"%s/%s\"><b>%s</b></a><br>", path, DATA.cFileName, DATA.cFileName);
		}
	}
	else
	{
		if (path[strlen(path) - 1] == '/')
			sprintf(tmpHtml, "<a href=\"%s%sVINHLT\">%s</a><br>", path, DATA.cFileName, DATA.cFileName);
		else
			sprintf(tmpHtml, "<a href=\"%s/%sVINHLT\">%s</a><br>", path, DATA.cFileName, DATA.cFileName);
	}

	int oldlen = g_html != NULL ? strlen(g_html) : 0;
	g_html = (char*)realloc(g_html, oldlen + strlen(tmpHtml) + 1);
	sprintf(g_html + oldlen, "%s", tmpHtml);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		while (FindNextFileA(hFind, &DATA) == TRUE)
		{
			memset(tmpHtml, 0, sizeof(tmpHtml));

			if (DATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (path[strlen(path) - 1] == '/')
					sprintf(tmpHtml, "<a href=\"%s%s\"><b>%s</b></a><br>", path, DATA.cFileName, DATA.cFileName);
				else
					sprintf(tmpHtml, "<a href=\"%s/%s\"><b>%s</b></a><br>", path, DATA.cFileName, DATA.cFileName);
			}
			else
			{
				if (path[strlen(path) - 1] == '/')
					sprintf(tmpHtml, "<a href=\"%s%sVINHLT\">%s</a><br>", path, DATA.cFileName, DATA.cFileName);
				else
					sprintf(tmpHtml, "<a href=\"%s/%sVINHLT\">%s</a><br>", path, DATA.cFileName, DATA.cFileName);
			}
			int oldlen = g_html != NULL ? strlen(g_html) : 0;
			g_html = (char*)realloc(g_html, oldlen + strlen(tmpHtml) + 1);
			sprintf(g_html + oldlen, "%s", tmpHtml);
		}
	}
}

int FindBoundaryPattern(char* pattern, char* data, int len, int from)
//Tim mau pattern trong du lieu data bat dau tu vi tri from, len la tong so byte cua data
{
	for (int i = from; i < len; i++)
	{
		if (pattern[0] == data[i])
		{
			int j = 0;
			for (j = 0; j < strlen(pattern); j++)
			{
				if (pattern[j] != data[j + i])
				{
					break;
				}
			}
			if (j == strlen(pattern))
			{
				return i;
			}
		}
	}

	return -1;
}

DWORD WINAPI ClientThread(LPVOID param)
{
	SOCKET c = (SOCKET)param;
	char* buffer = (char*)calloc(1024, 1); //Cap phat dong 1024 byte
	memset(buffer, 0, sizeof(buffer));
	int r = recv(c, buffer, 1024, 0);
	char ACTION[1024];
	char PATH[1024];
	char VER[1024];
	char BOUNDARY[1024];
	char LENGTH[1024];
	memset(ACTION, 0, sizeof(ACTION));
	memset(PATH, 0, sizeof(PATH));
	memset(VER, 0, sizeof(VER));
	memset(BOUNDARY, 0, sizeof(BOUNDARY));
	memset(LENGTH, 0, sizeof(LENGTH));

	sscanf(buffer, "%s%s%s", ACTION, PATH, VER);
	if (g_html != NULL)
	{
		free(g_html);
		g_html = NULL;
	}

	if (strcmp(ACTION, "POST") == 0)
	{
		FILE* f = NULL;
		char* tmp = strstr(buffer, "boundary=") + strlen("boundary=");
		sscanf(tmp, "%s", BOUNDARY);
		tmp = strstr(buffer, "Content-Length:") + strlen("Content-Length:");
		sscanf(tmp, "%s", LENGTH);
		int length = atoi(LENGTH);
		buffer = (char*)realloc(buffer, 1024 + length); //Chac chan du boi vi 1024 byte thua de chua header
		int r1 = 0;
		int received = r;
		do
		{
			r1 = recv(c, buffer + received, 1024, 0);
			if (r1 > 0)
			{
				received += r1;
			}
			else
				break;
		} while (received < length);

		length = received; //Tong so byte nhan trong ca hai lan goi recv

		int found = FindBoundaryPattern(BOUNDARY, buffer, length, 0); //Boundary o tren phan HEADER cua HTTP
		while (found)
		{
			found = FindBoundaryPattern(BOUNDARY, buffer, length, found + 1); //Boundary bat dau phan du lieu
			if (found >= 0)
			{
				//Tach ten file tu metadata sau boundary
				char* filename = strstr(buffer + found, "filename=\"");
				if (filename != NULL)
				{
					char fnamevalue[1024];
					memset(fnamevalue, 0, sizeof(fnamevalue));
					strcpy(fnamevalue, "C:\\Temp\\");
					sscanf(filename + strlen("filename=\""), "%s", fnamevalue + strlen(fnamevalue));
					fnamevalue[strlen(fnamevalue) - 1] = 0;
					f = fopen(fnamevalue, "wb");
				}

				char* linebreak = strstr(buffer + found, "\r\n\r\n");
				if (linebreak != NULL)
					found = linebreak - buffer + 4; //Bo qua doan du lieu bao gom 4 dau \r\n\r\n
				else
					found += strlen(BOUNDARY);

				int next = FindBoundaryPattern(BOUNDARY, buffer, length, found + 1); //Vi tri tiep theo
				if (next >= 0)
				{
					fwrite(buffer + found, 1, next - found, f);
				}
				else
				{
					next = length - 1;
					fwrite(buffer + found, 1, next - found, f);
					break;
				}
			}
		}
		free(buffer); //Neu dung calloc/malloc/realloc thi phai free de thu hoi bo nho
		fclose(f);
		char* response = (char*)"HTTP/1.1 200 OK\n\n";
		send(c, response, strlen(response), 0);
	}

	if (strcmp(ACTION, "GET") == 0)
	{
		while (strstr(PATH, "%20") != NULL)
		{
			char* found = strstr(PATH, "%20");
			found[0] = ' ';
			strcpy(found + 1, found + 3);
		}
		if (strstr(PATH, "VINHLT") != NULL)
		{
			SendFile(c, PATH);
		}
		else
		{
			char* header = (char*)"HTTP/1.1 200 OK\nServer: LTM\nContent-Type: text/html\n\n";
			send(c, header, strlen(header), 0);
			char* input = (char*)"<form action=\"/\" method=\"post\" enctype=\"multipart/form-data\"><input type=\"file\" id=\"txtmyid\" name=\"txtmyname\"><br><input type=\"submit\"></form><br>";
			send(c, input, strlen(input), 0);
			ScanFolder(PATH);
			g_html = (char*)realloc(g_html, strlen(g_html) + 8);
			sprintf(g_html + strlen(g_html), "</html>");
			send(c, g_html, strlen(g_html), 0);
		}
	}
	closesocket(c);
	return 0;
}

void main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(80);
	std::cout << saddr.sin_port << " " << ntohl(saddr.sin_port);
	saddr.sin_addr.S_un.S_addr = 0;  //htons(INADDR_ANY)
	bind(s, (sockaddr*)&saddr, sizeof(saddr));
	listen(s, 10);
	while (0 == 0)
	{
		DWORD ID = 0;
		SOCKADDR_IN caddr;
		int clen = sizeof(caddr);
		SOCKET c = accept(s, (sockaddr*)&caddr, &clen);
		CreateThread(NULL, 0, ClientThread, (LPVOID)c, 0, &ID);
	}
}