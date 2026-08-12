#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>

class TcpClient
{
public:
    TcpClient(const std::string &ip, unsigned short port);
    ~TcpClient();

    TcpClient(const TcpClient &) = delete;
    TcpClient &operator=(const TcpClient &) = delete;

    void connect_to_server();

    void send_message(const std::string &message);
    std::string receive_message();

private:
    static constexpr int BUFFER_SIZE = 1024;

    void create_socket();
    void close();

private:
    std::string ip_;
    unsigned short port_;
    SOCKET socket_ = INVALID_SOCKET;
};