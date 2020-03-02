#define WIN32_LEAN_AND_MEAN
/*#pragma comment(lib, "ws2_32.lib")*/

#include <windows.h>
#include <WinSock2.h>
#include <iostream>

using namespace std;


int main()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    if (NO_ERROR != WSAStartup(ver, &dat))
    {
        cout << "错误，WSAStartup()调用Winsocke dll出错..." << endl;
        return 1;
    }
    /**********************************************/

    //1.创建socket()
    SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( INVALID_SOCKET == _sock )
    {
        cout << "错误，创建socket失败...\n";
        WSACleanup();
        return 1;
    }

    //2.绑定端口bind()
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_addr.S_un.S_addr = INADDR_ANY; //inet_addr("127.0.0.1")
    _sin.sin_port = htons(4567);
    if ( SOCKET_ERROR == bind(_sock, (sockaddr*)(&_sin), sizeof(sockaddr_in)) )
    {
        cout<<"错误，绑定端口失败...." << endl;
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "绑定端口成功." << endl;
    }

    //3.监听端口listen()
    if ( SOCKET_ERROR == listen(_sock, 5) )
    {
        cout << "错误，监听失败..." << endl;
        closesocket(_sock);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "正在监听." << endl;
    }

    //4.等待客户端连接accept()
    SOCKET _clientSock = INVALID_SOCKET;
    sockaddr_in _clientAdd = {};
    int iClientLen = sizeof(sockaddr_in);   //此处虽然用长度指针，但不是为了返回，标识的时sockaddr_in的大小
    _clientSock = accept(_sock, (sockaddr*)(&_clientAdd), &iClientLen);
    if (INVALID_SOCKET == _clientSock)
    {
        cout << "错误，接收到无效客户端..." << endl;
    }
    else
    {
        cout << "新的客户端加入, Socket："<< (int)_clientSock <<", IP: " << inet_ntoa(_clientAdd.sin_addr) << endl;
    }
    
    while (true)
    {
        //5.接收数据
        char recvBuffer[128] = {};
        int iRecv = recv( _clientSock, recvBuffer, sizeof(recvBuffer), 0);
        if (iRecv <= 0)
        {
            cout << "客户端已退出,任务结束." << endl;
            break;
        }

        cout << "收到命令：" << recvBuffer << endl;

        char sendBuffer[128] = {};
        if ( 0 == strcmp(recvBuffer, "getName") )
        {
            strcpy_s(sendBuffer, "Xiao Qiang.\n");
        }
        else if ( 0 == strcmp(recvBuffer, "Age") )
        {
            strcpy_s(sendBuffer, "20.\n");
        }
        else
        {
            strcpy_s(sendBuffer, "Hello, I'm server!\n");
        }

        //6.向客户端发送数据send()
        send(_clientSock, sendBuffer, strlen(sendBuffer) + 1, 0);
    }

    //6.关闭SOCKET
    closesocket(_sock);

    /**********************************************/
    WSACleanup();

    system("pause");
    return 0;
}