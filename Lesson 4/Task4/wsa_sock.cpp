#include "wsa_sock.h"

WinSock::WinSock()
{
    WSADATA data{};
    if(WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
}

WinSock::~WinSock()
{
    WSACleanup();
}