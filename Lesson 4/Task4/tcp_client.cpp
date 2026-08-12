#include "tcp_client.h"

#include <string>
#include <stdexcept>
#include <iostream>

TcpClient::TcpClient(const std::string &ip, unsigned short port)
    : ip_(ip)
    , port_(port)
{}

TcpClient::~TcpClient()
{
    close();
}

void TcpClient::create_socket()
{
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(socket_ == INVALID_SOCKET) {
        throw std::runtime_error("client socket failed: " + std::to_string(WSAGetLastError()));
    }
}

void TcpClient::close()
{
    if(socket_ != INVALID_SOCKET) {
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
    }
}

void TcpClient::connect_to_server()
{
    create_socket();

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port_);

    if(inet_pton(AF_INET, ip_.c_str(), &address.sin_addr) != 1) {
        throw std::runtime_error("invalid server address");
    }

    if(connect(socket_, reinterpret_cast<sockaddr *>(&address), sizeof(address)) == SOCKET_ERROR) {
        throw std::runtime_error("connect failed: " + std::to_string(WSAGetLastError()));
    }

    std::cout << "Client connected\n";
}

void TcpClient::send_message(const std::string &message)
{
    int total_sent = 0;

    while(total_sent < static_cast<int>(message.size())) {
        int sent = send(socket_,
                        message.data() + total_sent,
                        static_cast<int>(message.size()) - total_sent,
                        0);

        if(sent == SOCKET_ERROR) {
            throw std::runtime_error("send failed: " + std::to_string(WSAGetLastError()));
        }

        total_sent += sent;
    }
}

std::string TcpClient::receive_message()
{
    char buffer[BUFFER_SIZE];

    int received = recv(socket_, buffer, sizeof(buffer), 0);

    if(received == SOCKET_ERROR) {
        throw std::runtime_error("recv failed: " + std::to_string(WSAGetLastError()));
    }

    if(received == 0) {
        return {};
    }

    return std::string(buffer, received);
}