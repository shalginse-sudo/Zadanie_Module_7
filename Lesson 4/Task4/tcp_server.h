#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>

class TcpServer
{
public:
    TcpServer(const std::string &ip, unsigned short port);
    ~TcpServer();

    TcpServer(const TcpServer &) = delete;
    TcpServer &operator=(const TcpServer &) = delete;

    void start();

    void run();

private:
    static constexpr int BUFFER_SIZE = 1024;

    void create_socket();
    void bind_socket();
    void listen_socket();
    void process_client(SOCKET client_socket);

    static void send_all(SOCKET socket, const std::string &message);

    void close();

private:
    std::string ip_;
    unsigned short port_;
    SOCKET socket_ = INVALID_SOCKET;
};