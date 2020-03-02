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
        char recvBuffer[1024] = {};
        //�״�ֻ��DataHeader���ȵ����ݣ����Ⱥ��������ͣ���֮���ٴν�����ϸ����
        int iRecv = recv( _clientSock, recvBuffer, sizeof(DataHeader), 0); 
        if (iRecv <= 0)
        {
            cout << "�ͻ������˳�,�������." << endl;
            break;
        }

        DataHeader* pHeader = (DataHeader*)recvBuffer;
        //ASSERT(header != NULL);

        switch (pHeader->m_cmdType )
        {
            case CMD_LOGIN:
            {
                //�յ�����recv()��ʹ������ƫ�ƣ��ٴν���δ���������
                recv(_clientSock, (char*)&recvBuffer + sizeof(DataHeader), pHeader->m_dataLength - sizeof(DataHeader), 0);
                Login* pRecvlg = (Login*)recvBuffer;
                cout << "�յ����CMD_LOGIN, ���ݳ��ȣ�" << pRecvlg->m_dataLength
                    << ", �û�����" << pRecvlg->m_userNmae << ", ���룺" << pRecvlg->m_password << endl;
                //���أ���������send()
                LoginResult sendlg;
                send(_clientSock, (const char*)(&sendlg), sizeof(LoginResult), 0);
                break;
            }
            case CMD_LOGOUT:
            {
                //�յ�����recv()��ʹ������ƫ�ƣ��ٴν���δ���������
                recv(_clientSock, recvBuffer + sizeof(DataHeader), pHeader->m_dataLength - sizeof(DataHeader), 0);
                Logout* pRecvlg = (Logout*)recvBuffer;
                cout << "�յ����CMD_LOGOUT, ���ݳ��ȣ�" << pRecvlg->m_dataLength
                    << ", �û�����" << pRecvlg->m_userNmae << endl;
                //���أ���������send()
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

    //6.�ر�SOCKET
    closesocket(_sock);

    /**********************************************/
    WSACleanup();

    system("pause");
    return 0;
}