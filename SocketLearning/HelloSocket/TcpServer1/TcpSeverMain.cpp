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
        cout << "����WSAStartup()����Winsocke dll����..." << endl;
        return 1;
    }
    /**********************************************/

    //1.����socket()
    SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( INVALID_SOCKET == _sock )
    {
        cout << "���󣬴���socketʧ��...\n";
        WSACleanup();
        return 1;
    }

    //2.�󶨶˿�bind()
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_addr.S_un.S_addr = INADDR_ANY; //inet_addr("127.0.0.1")
    _sin.sin_port = htons(4567);
    if ( SOCKET_ERROR == bind(_sock, (sockaddr*)(&_sin), sizeof(sockaddr_in)) )
    {
        cout<<"���󣬰󶨶˿�ʧ��...." << endl;
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "�󶨶˿ڳɹ�." << endl;
    }

    //3.�����˿�listen()
    if ( SOCKET_ERROR == listen(_sock, 5) )
    {
        cout << "���󣬼���ʧ��..." << endl;
        closesocket(_sock);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "���ڼ���." << endl;
    }

    //4.�ȴ��ͻ�������accept()
    SOCKET _clientSock = INVALID_SOCKET;
    sockaddr_in _clientAdd = {};
    int iClientLen = sizeof(sockaddr_in);   //�˴���Ȼ�ó���ָ�룬������Ϊ�˷��أ���ʶ��ʱsockaddr_in�Ĵ�С
    _clientSock = accept(_sock, (sockaddr*)(&_clientAdd), &iClientLen);
    if (INVALID_SOCKET == _clientSock)
    {
        cout << "���󣬽��յ���Ч�ͻ���..." << endl;
    }
    else
    {
        cout << "�µĿͻ��˼���, Socket��"<< (int)_clientSock <<", IP: " << inet_ntoa(_clientAdd.sin_addr) << endl;
    }
    
    while (true)
    {
        //5.��������
        char recvBuffer[128] = {};
        int iRecv = recv( _clientSock, recvBuffer, sizeof(recvBuffer), 0);
        if (iRecv <= 0)
        {
            cout << "�ͻ������˳�,�������." << endl;
            break;
        }

        cout << "�յ����" << recvBuffer << endl;

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

        //6.��ͻ��˷�������send()
        send(_clientSock, sendBuffer, strlen(sendBuffer) + 1, 0);
    }

    //6.�ر�SOCKET
    closesocket(_sock);

    /**********************************************/
    WSACleanup();

    system("pause");
    return 0;
}