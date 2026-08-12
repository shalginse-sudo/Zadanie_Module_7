#include "wsa_sock.h"
#include "tcp_client.h"
#include "tcp_server.h"

#include <thread>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    try {
        WinSock winsock;

        constexpr unsigned short port = 54674;
        const std::string ip = "127.0.0.1";

        TcpServer server(ip, port);
        server.start();

        std::thread server_thread([&server]() { server.run(); });

        TcpClient client(ip, port);
        client.connect_to_server();

        const std::string message = "Hello, server!";

        std::cout << "Client sends: " << message << '\n';

        client.send_message(message);

        std::string response =
            client.receive_message();

        std::cout << "Client received: "
            << response << '\n';

        server_thread.join();
    } catch(const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}