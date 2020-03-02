#define WIN32_LEAN_AND_MEAN
/*#pragma comment(lib, "ws2_32.lib")*/

#include <windows.h>
#include <WinSock2.h>
#include <iostream>
#include <assert.h>
#include "../TcpClient1/DataPackage.h"

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
        char recvBuffer[1024] = {};
        //首次只接DataHeader长度的数据（长度和命令类型），之后再次接收详细数据
        int iRecv = recv( _clientSock, recvBuffer, sizeof(DataHeader), 0); 
        if (iRecv <= 0)
        {
            cout << "客户端已退出,任务结束." << endl;
            break;
        }

        DataHeader* pHeader = (DataHeader*)recvBuffer;
        //ASSERT(header != NULL);

        switch (pHeader->m_cmdType )
        {
            case CMD_LOGIN:
            {
                //收到数据recv()，使用数据偏移，再次接收未收完的数据
                recv(_clientSock, (char*)&recvBuffer + sizeof(DataHeader), pHeader->m_dataLength - sizeof(DataHeader), 0);
                Login* pRecvlg = (Login*)recvBuffer;
                cout << "收到命令：CMD_LOGIN, 数据长度：" << pRecvlg->m_dataLength
                    << ", 用户名：" << pRecvlg->m_userNmae << ", 密码：" << pRecvlg->m_password << endl;
                //返回，发送数据send()
                LoginResult sendlg;
                send(_clientSock, (const char*)(&sendlg), sizeof(LoginResult), 0);
                break;
            }
            case CMD_LOGOUT:
            {
                //收到数据recv()，使用数据偏移，再次接收未收完的数据
                recv(_clientSock, recvBuffer + sizeof(DataHeader), pHeader->m_dataLength - sizeof(DataHeader), 0);
                Logout* pRecvlg = (Logout*)recvBuffer;
                cout << "收到命令：CMD_LOGOUT, 数据长度：" << pRecvlg->m_dataLength
                    << ", 用户名：" << pRecvlg->m_userNmae << endl;
                //返回，发送数据send()
                LogoutResult sendlg;
                send(_clientSock, (const char*)(&sendlg), sizeof(LogoutResult), 0);
                break;
            }
            default:
            {
                DataHeader sendHeader;
                sendHeader.m_cmdType = CMD_ERROR;
                send(_clientSock, (const char*)&sendHeader, sizeof(DataHeader), 0);
                break;
            }
        }

    }

    //6.关闭SOCKET
    closesocket(_sock);

    /**********************************************/
    WSACleanup();

    system("pause");
    return 0;
}