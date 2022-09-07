#pragma once
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "command_handlers/command_handler.hpp"

namespace net {
    /// TCP Client.
    class Client {
    private:
        uint16_t port;
        std::string server_ip;
        int conn_fd;
        std::vector<client_command::CommandHandler*> command_handlers;
    public:
        Client(const std::string& server_ip, uint16_t port);
        ~Client();
        void start();
        int sock_fd;
    };

    void start_client();
}