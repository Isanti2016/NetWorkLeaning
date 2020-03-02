#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <iostream>
#include "DataPackage.h"

using namespace std;

int main()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    int iResult = WSAStartup(ver, &dat);    //用于启动Winsock Dll进程
    if ( NO_ERROR != iResult )
    {
        cout << "错误，WSAStartup()调用Winsocke dll出错..." << endl;
        WSACleanup();
        return 1;
    }
    /**********************************************/
    //1.创建socket()
    SOCKET _client = socket(AF_INET, SOCK_STREAM, 0);
    if ( INVALID_SOCKET ==_client )
    {
        cout << "错误，创建Socket失败..." << endl;
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "创建Socket成功." << endl;
    }

    //2.连接connect()
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    _sin.sin_port = htons(4567);

    iResult = connect( _client, (sockaddr*)(&_sin), sizeof(sockaddr_in));
    if ( SOCKET_ERROR == iResult )
    {
        cout << "错误，连接出错..." << endl;
        closesocket(_client);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "连接服务器成功." << endl;
    }

    while (true)
    {
        //3.获得命令
        char sendBuffer[128] = {};
        scanf("%s", sendBuffer);

        if ( 0 == strcmp(sendBuffer, "exit") )
        {
            break;
        }
        else if (0 == strcmp(sendBuffer, "login"))
        {
            //4.发送数据send()
            Login sendLg;
            strcpy(sendLg.m_userNmae, "wbb");
            strcpy(sendLg.m_password, "wbbmima");
            send(_client, (const char*)&sendLg, sizeof(Login), 0);

            //5.接收recv()
            LoginResult recvlg = {};
            recv(_client, (char*)&recvlg, sizeof(LoginResult), 0);
            cout << "Login result: " << recvlg.m_iResult << endl;

        }
        else if (0 == strcmp(sendBuffer, "logout"))
        {
            //4.发送数据send()
            Logout sendLg;
            strcpy(sendLg.m_userNmae, "wbb");
            send(_client, (const char*)&sendLg, sizeof(Logout), 0);

            //5.接收recv()
            LogoutResult recvlg = {};
            recv(_client, (char*)&recvlg, sizeof(LogoutResult), 0);
            cout << "Logout result: " << recvlg.m_iResult << endl;
        }
        else
        {
            cout << "输入命令错误，请重新输入..." << endl;
        }

    }

    //6.关闭closesocket()
    closesocket(_client);

    /**********************************************/
    WSACleanup();

    system("pause");
    return 0;
}