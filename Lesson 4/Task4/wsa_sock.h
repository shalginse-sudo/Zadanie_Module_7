#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdexcept>

class WinSock
{
public:
    WinSock();
    ~WinSock();

    WinSock(const WinSock &) = delete;
    WinSock &operator=(const WinSock &) = delete;
};