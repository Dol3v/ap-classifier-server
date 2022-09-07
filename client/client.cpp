#include <iostream>
#include <sstream>
#include <arpa/inet.h>
#include <cstring>
#include "client.hpp"
#include "../common.hpp"
#include "command_handlers/handle_upload.hpp"
#include "command_handlers/handle_settings.hpp"
#include "command_handlers/echo_response.hpp"
#include "command_handlers/receive_results.hpp"

net::Client::Client(const std::string& server_ip, uint16_t port) {
    if ((this->sock_fd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::logic_error("couldn't create socket");
    }

    struct sockaddr_in server_address{};
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_port = ::htons(port);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip.c_str());

    if ((this->conn_fd = ::connect(this->sock_fd, reinterpret_cast<struct sockaddr *>(&server_address),
                                   sizeof(server_address))) < 0) {
        throw std::logic_error("Failed to connect to server");
    }
    command_handlers.push_back(new client_command::HandleUpload(this->sock_fd));
    command_handlers.push_back(new client_command::HandleSettings(this->sock_fd));
    command_handlers.push_back(new client_command::EchoResponse(this->sock_fd, 2));
    command_handlers.push_back(new client_command::EchoResponse(this->sock_fd));
    command_handlers.push_back(new client_command::FileReceive(this->sock_fd));
    command_handlers.push_back(new client_command::EchoResponse(this->sock_fd));
    command_handlers.push_back(new client_command::EchoResponse(this->sock_fd));
}

net::Client::~Client() {
    if (this->sock_fd) {
        ::close(this->sock_fd);
    }
    if (this->conn_fd) {
        ::close(this->conn_fd);
    }
    for (auto* command : command_handlers) {
        delete command;
    }
}

void net::Client::start() {
    while (true) {
        auto menu = common::recv_string(this->sock_fd);
        std::cout << menu;
        int choice;
        std::cin >> choice;

        std::stringstream choice_str;
        choice_str << choice;
        common::send_string(this->sock_fd, choice_str.str());
        choice--;
        if (choice < this->command_handlers.size())
            this->command_handlers.at(choice)->execute();
        else
            std::cout << common::recv_string(this->sock_fd);
        if (choice == this->command_handlers.size() - 1)
            return;
    }
}
