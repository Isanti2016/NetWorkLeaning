#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <iostream>

using namespace std;

int main()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    int iResult = WSAStartup(ver, &dat);    //��������Winsock Dll����
    if ( NO_ERROR != iResult )
    {
        cout << "����WSAStartup()����Winsocke dll����..." << endl;
        WSACleanup();
        return 1;
    }
    /**********************************************/
    //1.����socket()
    SOCKET _client = socket(AF_INET, SOCK_STREAM, 0);
    if ( INVALID_SOCKET ==_client )
    {
        cout << "���󣬴���Socketʧ��..." << endl;
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "����Socket�ɹ�." << endl;
    }

    //2.����connect()
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    _sin.sin_port = htons(4567);

    iResult = connect( _client, (sockaddr*)(&_sin), sizeof(sockaddr_in));
    if ( SOCKET_ERROR == iResult )
    {
        cout << "�������ӳ���..." << endl;
        closesocket(_client);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "���ӷ������ɹ�." << endl;
    }

    while (true)
    {
        //3.�������
        char sendBuffer[128] = {};
        scanf("%s", sendBuffer);

        if ( 0 == strcmp(sendBuffer, "exit") )
        {
            break;
        }
        else
        {
            //4.��������
            send(_client, sendBuffer, strlen(sendBuffer) + 1, 0);
        }

        //5.����recv()
        char recvBuffer[256] = {};
        int iRecv = recv(_client, recvBuffer, sizeof(recvBuffer), 0);
        if (iRecv > 0 )
        {
            cout << "�յ����ݣ�" << recvBuffer << endl;
        }

    }



    //6.�ر�closesocket()
    closesocket(_client);

    /**********************************************/
    WSACleanup();

    system("pause");
    return 0;
}