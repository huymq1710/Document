#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
// AsyncSelect Example.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "AsyncSelect Example.h"
#include <WinSock2.h>
#include <stdio.h>

#define MAX_LOADSTRING 100
#define WM_ACCEPT WM_USER + 1
#define WM_READ_CLOSE WM_USER + 2
#define WM_DISCONNECT WM_USER + 3

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    MyWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

SOCKET s, data;
HWND hWnd;
SOCKET clients[1024];
int status[1024]; //0 neu da disconnect, 1 neu van dang connect
int count = 0;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASYNCSELECTEXAMPLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    WSADATA DATA;
    WSAStartup(MAKEWORD(2, 2), &DATA);

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(21);
    saddr.sin_addr.S_un.S_addr = 0;
    bind(s, (sockaddr*)&saddr, sizeof(saddr));
    listen(s, 10);
    WSAAsyncSelect(s, hWnd, WM_ACCEPT, FD_ACCEPT); //Sau lenh nay thi moi ket noi den ma can accept se lam phat sinh mot su kien vao cua so voi ma 1234

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASYNCSELECTEXAMPLE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MyWndProc; //Dia chi cua ham se duoc goi ra de xu ly cac su kien phat sinh tren cua so
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASYNCSELECTEXAMPLE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ASYNCSELECTEXAMPLE);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LONGLONG FileTime_to_POSIX(FILETIME ft)
{
    // takes the last modified date
    LARGE_INTEGER date, adjust;
    date.HighPart = ft.dwHighDateTime;
    date.LowPart = ft.dwLowDateTime;

    // 100-nanoseconds = milliseconds * 10000
    adjust.QuadPart = 11644473600000 * 10000;

    // removes the diff between 1970 and 1601
    date.QuadPart -= adjust.QuadPart;

    // converts back from 100-nanoseconds to seconds
    return date.QuadPart / 10000000;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    SOCKET s, c;
    SOCKADDR_IN caddr;
    int clen = sizeof(caddr);
    char buffer[1024];
    static SOCKADDR_IN data_addr;
    static char* currentDir = (char*)calloc(1024, 1);
    static char* content = (char*)calloc(1024 * 1024, 1);
    static char* WorkingDir = (char*)calloc(1024, 1);
    static char rootDir[1024];
    static char username[1024];
    static char password[1024];
    switch (message)
    {
    case WM_READ_CLOSE:
        c = (SOCKET)wParam;
        if (LOWORD(lParam) & FD_READ)
        {
            memset(buffer, 0, sizeof(buffer));
            recv(c, buffer, sizeof(buffer), 0);
            if (strstr(buffer, "USER"))
            {
                memset(username, 0, sizeof(username));
                sscanf(buffer + 5, "%s", username);
                char* tmp = (char*)"331 Can password\r\n";
                send(c, tmp, strlen(tmp), 0);
            }
            if (strstr(buffer, "PASS"))
            {
                memset(password, 0, sizeof(password));
                sscanf(buffer + 5, "%s", password);
                int found = 0;
                FILE* f = fopen("users.txt", "rt");
                while (!feof(f))
                {
                    char line[1024];
                    char _username[1024];
                    char _password[1024];
                    char _rootdir[1024];
                    memset(line, 0, sizeof(line));
                    memset(_username, 0, sizeof(_username));
                    memset(_password, 0, sizeof(_password));
                    memset(_rootdir, 0, sizeof(_rootdir));

                    fgets(line, sizeof(line), f);
                    sscanf(line, "%s%s", _username, _password);

                    if (strcmp(username, _username) == 0 && strcmp(password, _password) == 0)
                    {
                        found = 1;
                        strcpy(_rootdir, strstr(line, _password) + strlen(_password) + 1);
                        while (_rootdir[strlen(_rootdir) - 1] == '\r' ||
                            _rootdir[strlen(_rootdir) - 1] == '\n')
                        {
                            _rootdir[strlen(_rootdir) - 1] = 0;
                        }
                        memset(rootDir, 0, sizeof(rootDir));
                        strcpy(rootDir, _rootdir);
                        break;
                    }
                }
                fclose(f);
                if (found)
                {
                    char* tmp = (char*)"230 Login ngon roi\r\n";
                    send(c, tmp, strlen(tmp), 0);
                    strcpy(currentDir, "257 \"/\" is current directory.\r\n");
                    strcpy(content, "type=dir;modify=20200421102857; .NET\r\ntype=file;modify=20200421102857; Sample.doc\r\n");
                    strcpy(WorkingDir, rootDir);
                }
                else
                {
                    char* tmp = (char*)"530 Login failed\r\n";
                    send(c, tmp, strlen(tmp), 0);
                }
            }
            if (strstr(buffer, "SYST"))
            {
                char* tmp = (char*)"215 FTP CUA VINH\r\n";
                send(c, tmp, strlen(tmp), 0);
            }
            if (strstr(buffer, "FEAT"))
            {
                char* tmp = (char*)"211-Features:\r\n MDTM\r\n REST STREAM\r\n SIZE\r\n MLST type*; size*; modify*;\r\n MLSD\r\n UTF8\r\n CLNT\r\n MFMT\r\n EPSV\r\n EPRT\r\n211 End\r\n";
                send(c, tmp, strlen(tmp), 0);
            }
            if (strstr(buffer, "CLNT"))
            {
                char* tmp = (char*)"200 Khong can biet cai nay lam gi\r\n";
                send(c, tmp, strlen(tmp), 0);
            }
            if (strstr(buffer, "OPTS"))
            {
                char* tmp = (char*)"202 OK chap nhan truyen du lieu kieu nay\r\n";
                send(c, tmp, strlen(tmp), 0);
            }
            if (strstr(buffer, "PWD"))
            {
                char tmp[1024];
                memset(tmp, 0, sizeof(tmp));
                if (_stricmp(WorkingDir, rootDir) == 0)
                {
                    sprintf(tmp, "257 \"/\" is current directory\r\n");
                }
                else
                    sprintf(tmp, "257 \"%s\" is current directory\r\n", WorkingDir + strlen("C:/Temp"));

                send(c, tmp, strlen(tmp), 0);
            }
            if (strstr(buffer, "TYPE"))
            {
                char* tmp = (char*)"200 OK\r\n";
                send(c, tmp, strlen(tmp), 0);
            }
            if (strstr(buffer, "PORT"))
            {
                char* tmp = (char*)"200 OK\r\n";
                send(c, tmp, strlen(tmp), 0);
                for (int i = 0; i < strlen(buffer); i++)
                {
                    if (buffer[i] == ',') buffer[i] = ' ';
                }
                char port[1024];
                char ip1[1024], ip2[1024], ip3[1024], ip4[1024], p1[1024], p2[1024];
                sscanf(buffer, "%s%s%s%s%s%s%s", port, ip1, ip2, ip3, ip4, p1, p2);
                unsigned short uport = atoi(p1) * 256 + atoi(p2);
                data_addr.sin_family = AF_INET;
                data_addr.sin_port = htons(uport);
                data_addr.sin_addr.S_un.S_un_b.s_b1 = atoi(ip1);
                data_addr.sin_addr.S_un.S_un_b.s_b2 = atoi(ip2);
                data_addr.sin_addr.S_un.S_un_b.s_b3 = atoi(ip3);
                data_addr.sin_addr.S_un.S_un_b.s_b4 = atoi(ip4);
            }
            if (strstr(buffer, "MLSD"))
            {
                //Quet thu muc hien thoi WorkingDir va tao xau content
                char findPath[1024];
                WIN32_FIND_DATAA fData;
                memset(findPath, 0, sizeof(findPath));
                sprintf(findPath, "%s/*.*", WorkingDir);
                HANDLE hFind = FindFirstFileA(findPath, &fData);
                //type=dir;modify=20200421102857; .NET\r\n
                if (hFind != INVALID_HANDLE_VALUE)
                {
                    strcpy(content, "");
                    char* type = (char*)(fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? "dir" : "file");
                    sprintf(content, "type=%s;modify=%ld; %s\r\n", type, (long)FileTime_to_POSIX(fData.ftLastWriteTime), fData.cFileName);
                    while (FindNextFileA(hFind, &fData) == TRUE)
                    {
                        char* type = (char*)(fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? "dir" : "file");
                        sprintf(content + strlen(content), "type=%s;modify=%ld; %s\r\n", type, (long)FileTime_to_POSIX(fData.ftLastWriteTime), fData.cFileName);
                    }
                }

                char* tmp = (char*)"150 \r\n";
                send(c, tmp, strlen(tmp), 0);
                data = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                connect(data, (sockaddr*)&data_addr, sizeof(data_addr));
                send(data, content, strlen(content), 0);
                closesocket(data);
                char* tmp2 = (char*)"226 Successfully transferred \"/\"\r\n";
                send(c, tmp2, strlen(tmp2), 0);
            }
            if (strstr(buffer, "CWD"))
            {
                char dir[1024];
                memset(dir, 0, 1024);
                strncpy(dir, buffer + 4, strlen(buffer) - 6);
                if (strcmp(dir, "/") == 0) //Thu muc goc cua FTP Client
                {
                    strcpy(WorkingDir, rootDir);
                }
                else
                    sprintf(WorkingDir + strlen(WorkingDir), "/%s", dir);

                char* tmp = (char*)"250 CWD Successful\r\n";
                send(c, tmp, strlen(tmp), 0);
            }
            if (strstr(buffer, "CDUP"))
            {
                if (_stricmp(WorkingDir, rootDir) != 0)
                {
                    int i = 0;
                    for (i = strlen(WorkingDir) - 1; i > 0; i--)
                    {
                        if (WorkingDir[i] != '/')
                            WorkingDir[i] = 0;
                        else
                            break;
                    }
                    WorkingDir[i] = 0;
                }
                char* tmp = (char*)"200 CDUP successful. \"/\" is the current directory\r\n";
                send(c, tmp, strlen(tmp), 0);
            }
            if (strstr(buffer, "SIZE")) //SIZE <ten file>\r\n
            {
                char path[1024];
                memset(path, 0, sizeof(path));
                sprintf(path, "%s/", WorkingDir);
                strcpy(path + strlen(path), buffer + 5);
                while (path[strlen(path) - 1] == '\r' || path[strlen(path) - 1] == '\n')
                    path[strlen(path) - 1] = 0;

                FILE* f = fopen(path, "rb");
                fseek(f, 0, SEEK_END);
                long size = ftell(f);
                fclose(f);
                memset(path, 0, sizeof(path));
                sprintf(path, "213 %ld\r\n", size); //Tra loi lenh SIZE
                send(c, path, strlen(path), 0);
            }
            if (strstr(buffer, "RETR")) //RETR <ten file>\r\n
            {
                char* tmp = (char*)"150 \r\n";
                send(c, tmp, strlen(tmp), 0);

                char path[1024];
                memset(path, 0, sizeof(path));
                sprintf(path, "%s/", WorkingDir);
                strcpy(path + strlen(path), buffer + 5);
                while (path[strlen(path) - 1] == '\r' || path[strlen(path) - 1] == '\n')
                    path[strlen(path) - 1] = 0;
                data = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                connect(data, (sockaddr*)&data_addr, sizeof(data_addr));

                FILE* f = fopen(path, "rb");
                while (!feof(f))
                {
                    char fdata[1024];
                    int r = fread(fdata, 1, 1024, f);
                    send(data, fdata, r, 0);
                }
                fclose(f);

                closesocket(data);

                char* tmp2 = (char*)"226 Successfully transferred \"/\"\r\n";
                send(c, tmp2, strlen(tmp2), 0);
            }
            if (strstr(buffer, "STOR")) //STOR <ten file>\r\n
            {
                char* tmp = (char*)"150 \r\n";
                send(c, tmp, strlen(tmp), 0);

                char path[1024];
                memset(path, 0, sizeof(path));
                sprintf(path, "%s/", WorkingDir);
                strcpy(path + strlen(path), buffer + 5);
                while (path[strlen(path) - 1] == '\r' || path[strlen(path) - 1] == '\n')
                    path[strlen(path) - 1] = 0;
                data = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                connect(data, (sockaddr*)&data_addr, sizeof(data_addr));

                FILE* f = fopen(path, "wb");
                while (0 == 0)
                {
                    char fdata[1024];
                    int r = recv(data, fdata, 1024, 0);
                    if (r > 0)
                        fwrite(fdata, 1, r, f);
                    else
                        break;
                }
                fclose(f);

                closesocket(data);

                char* tmp2 = (char*)"226 Successfully transferred \"/\"\r\n";
                send(c, tmp2, strlen(tmp2), 0);
            }
            if (strstr(buffer, "QUIT"))
            {
                closesocket(c);
            }
        }
        if (LOWORD(lParam) & FD_CLOSE)
        {
            for (int i = 0; i < count; i++)
            {
                if (clients[i] == c)
                {
                    status[i] = 0; //Kenh nay da disconnect
                    break;
                }
            }
            char* msg = (char*)"Da co mot client roi room\n";
            for (int i = 0; i < count; i++)
            {
                if (clients[i] != c && status[i] == 1) //Chi gui cho cac client dang ket noi
                {
                    send(clients[i], msg, strlen(msg), 0);
                }
            }
        }
        break;
    case WM_ACCEPT:
        s = (SOCKET)wParam;
        c = accept(s, (sockaddr*)&caddr, &clen);
        {
            char* tmp = (char*)"220 READY\r\n";
            send(c, tmp, strlen(tmp), 0);
            clients[count] = c;
            status[count] = 1;
            count += 1;
            WSAAsyncSelect(c, hWnd, WM_READ_CLOSE, FD_READ | FD_CLOSE); //Sau lenh nay neu co du lieu tren socket c thi cua so se nhan duoc thong diep WM_READ
        }
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
