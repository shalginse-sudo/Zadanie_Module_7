#include "tcp_server.h"

#include <string>
#include <iostream>
#include <stdexcept>

TcpServer::TcpServer(const std::string &ip, unsigned short port)
    : ip_(ip)
    , port_(port)
{}

TcpServer::~TcpServer()
{
    close();
}

void TcpServer::start()
{
    create_socket();
    bind_socket();
    listen_socket();

    std::cout << "Server started on " << ip_ << ":" << port_ << '\n';
}

void TcpServer::run()
{
    SOCKET client_socket = accept(socket_, nullptr, nullptr);

    if(client_socket == INVALID_SOCKET) {
        throw std::runtime_error("accept failed: " + std::to_string(WSAGetLastError()));
    }

    std::cout << "Server: client connected\n";

    process_client(client_socket);
    closesocket(client_socket);
}

void TcpServer::create_socket()
{
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(socket_ == INVALID_SOCKET) {
        throw std::runtime_error("server socket failed: " + std::to_string(WSAGetLastError()));
    }
}

void TcpServer::bind_socket()
{
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port_);

    if(inet_pton(AF_INET, ip_.c_str(), &address.sin_addr) != 1) {
        throw std::runtime_error("invalid server address");
    }

    if(bind(socket_, reinterpret_cast<sockaddr *>(&address), sizeof(address)) == SOCKET_ERROR) {
        throw std::runtime_error("bind failed: " + std::to_string(WSAGetLastError()));
    }
}

void TcpServer::listen_socket()
{
    if(listen(socket_, SOMAXCONN) == SOCKET_ERROR) {
        throw std::runtime_error("listen failed: " + std::to_string(WSAGetLastError()));
    }
}

void TcpServer::process_client(SOCKET client_socket)
{
    char buffer[BUFFER_SIZE];

    int received = recv(client_socket, buffer, sizeof(buffer), 0);

    if(received == SOCKET_ERROR) {
        throw std::runtime_error("recv failed: " + std::to_string(WSAGetLastError()));
    }

    if(received == 0) {
        std::cout << "Server: client disconnected\n";
        return;
    }

    std::string message(buffer, received);

    std::cout << "Server received: " << message << '\n';

    send_all(client_socket, message);
}

void TcpServer::send_all(SOCKET socket, const std::string &message)
{
    int total_sent = 0;

    while(total_sent < static_cast<int>(message.size())) {
        int sent = send(socket,
                        message.data() + total_sent,
                        static_cast<int>(message.size()) - total_sent,
                        0);

        if(sent == SOCKET_ERROR) {
            throw std::runtime_error("send failed: " + std::to_string(WSAGetLastError()));
        }

        total_sent += sent;
    }
}

void TcpServer::close()
{
    if(socket_ != INVALID_SOCKET) {
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
    }
}