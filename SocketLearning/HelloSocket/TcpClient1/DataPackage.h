#pragma once

#include <memory.h>
#include <stdio.h>

enum CMD_TYPE
{
    CMD_HEADER,
    CMD_LOGIN,
    CMD_LOGIN_RESULT,
    CMD_LOGOUT,
    CMD_LOGOUT_RESULT,
    CMD_ERROR
};

struct DataHeader
{
    short m_dataLength;
    short m_cmdType;

    DataHeader()
    {
        m_dataLength = sizeof(DataHeader);
        m_cmdType = CMD_HEADER;
    }
};

struct Login : public DataHeader
{
    char m_userNmae[32];
    char m_password[32];

    Login()
    {
        m_dataLength = sizeof(Login);
        m_cmdType = CMD_LOGIN;
        memset(m_userNmae, 0x00, sizeof(m_userNmae));
        memset(m_password, 0x00, sizeof(m_password));
    }
};

struct LoginResult : public DataHeader
{
    int m_iResult;

    LoginResult()
    {
        m_dataLength = sizeof(LoginResult);
        m_cmdType = CMD_LOGIN_RESULT;
        m_iResult = 0;
    }
};

struct Logout : public DataHeader
{
    char m_userNmae[32];

    Logout()
    {
        m_dataLength = sizeof(Logout);
        m_cmdType = CMD_LOGOUT;
        memset(m_userNmae, 0x00, sizeof(m_userNmae));
    }
};

struct LogoutResult : public DataHeader
{
    int m_iResult;

    LogoutResult()
    {
        m_dataLength = sizeof(LogoutResult);
        m_cmdType = CMD_LOGOUT_RESULT;
        m_iResult = 1;
    }
};