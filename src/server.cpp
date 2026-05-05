#include "../include/server.h"
#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

void handle_client(int client_socket)
{
    char buffer[1024];

    std::cout << "Client connected: " << client_socket << std::endl;

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytes = read(client_socket, buffer, sizeof(buffer));

        if (bytes <= 0)
        {
            std::cout << "Client disconnected: " << client_socket << std::endl;
            close(client_socket);
            break;
        }

        std::string msg(buffer, bytes);
        std::cout << "Received: " << msg << std::endl;

        // Temporary response
        std::string response = "OK\n";
        send(client_socket, response.c_str(), response.size(), 0);
    }
}

void start_server(int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10);

    std::cout << "Redis-like server running on port " << port << std::endl;

    while (true)
    {
        int addrlen = sizeof(address);
        int client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

        std::thread(handle_client, client_socket).detach();
    }
}