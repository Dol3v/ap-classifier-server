#pragma once

#include <atomic>
#include <mutex>
#include "cli.hpp"

namespace net {
    /// A TCP Server
    class Server {
    private:
        unsigned int port;
        int sock_fd;
        std::string ip;
        int total_joined_clients;
        int timeout_s;
        int backlog;
        std::atomic<int> active_clients;
        // lock on socket operations
        std::mutex socket_lock;

        /// listens for incoming connections
        void listen() const;

        /// accepts a connection and returns the connection fd
        int accept();

    public:
        Server(unsigned int port, const std::string &ip, int timeout_s = 5, int backlog = 3);

        /// starts up the server
        void start();

        ~Server();
    };

    /// called on SIGALRM
    void timeout_handler(int sig);
}