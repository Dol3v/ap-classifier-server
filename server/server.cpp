#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include <csignal>
#include <pthread.h>
#include <thread>
#include <iostream>
#include "server.hpp"
#include "../io/socketio.hpp"

net::Server::Server(unsigned int port, const std::string &ip, int timeout_s, int backlog) : ip(ip), port(port),
                                                                                            timeout_s(timeout_s),
                                                                                            active_clients(0),
                                                                                            backlog(backlog) {
    this->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sock_fd == 0) {
        throw std::runtime_error("couldn't create a socket");
    }

    struct sockaddr_in address{};
    memset(&address, 0, sizeof(address));

    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    address.sin_family = AF_INET;
    if (bind(this->sock_fd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0) {
        throw std::runtime_error("couldn't bind the socket to the requested ip/port");
    }
    this->total_joined_clients = 0;
}

void net::Server::listen() const {
    if (::listen(this->sock_fd, this->backlog) < 0) {
        throw std::runtime_error("couldn't listen for incoming connections");
    }
}

int net::Server::accept() {
    struct sigaction alarm_sigaction{};
    memset(&alarm_sigaction, 0, sizeof(struct sigaction));
    alarm_sigaction.sa_handler = timeout_handler;
    struct sigaction prev_sigaction{};

    sigaction(SIGALRM, &alarm_sigaction, &prev_sigaction);
    int accepted_fd = 0;

    while (true) {
        alarm(this->timeout_s);

        struct sockaddr_in accepted{};
        unsigned int accepted_len = 0;
        std::cout << "calling accept\n";
        this->socket_lock.lock();
        accepted_fd = ::accept(this->sock_fd, reinterpret_cast<sockaddr *>(&accepted), &accepted_len);
        this->socket_lock.unlock();
        if (accepted_fd < 0 | accepted_len < sizeof(struct sockaddr)) {
            if (errno == EINTR && this->active_clients == 0) {
                // process was interrupted by the alarm handler, changing errno and exiting
                std::cout << "[-] timeout\n" << std::endl;
                errno = ETIMEDOUT;
                return -1;
            } else if (errno == EINTR) {
                // received timeout but there are still other clients, hence we should try again
                continue;
            }
            std::cout << "[-] errno: " << errno << std::endl;
            throw std::runtime_error("couldn't accept incoming connection");
        }
        break;
    }

    std::cout << "[*] accepted connection: " << accepted_fd << std::endl;
    // turn off alarm, change signal handler back to normal and continue
    alarm(0);
    sigaction(SIGALRM, &prev_sigaction, nullptr);
    return accepted_fd;
}


net::Server::~Server() {
    if (this->sock_fd) {
        ::shutdown(this->sock_fd, ::SHUT_RDWR);
    }
}

void handle_client(int conn_fd, int client_id, std::atomic<int> &clients) {
    auto sio = std::make_shared<io::SocketIo>(conn_fd);
    auto cli = std::make_shared<CLI>(*sio, client_id);
    clients++;
    try {
        cli->start();

    } catch (io::socket_closed_error &socket_closed_error) {
        std::cout << "[-] client disconnected" << std::endl;
    } catch (std::exception &exception) {
        std::cout << "[-] internal server error: " << exception.what() << std::endl;
    }
    // if the cli.start() method ended => the client exited, hence we should
    // decrement the client count
    clients--;
}

void net::Server::start() {
    this->listen();
    while (true) {
        try {
            int conn_fd = this->accept();
            if (conn_fd < 0)
                break;
            std::thread(handle_client, conn_fd, this->total_joined_clients++, std::ref(this->active_clients)).detach();
        } catch (std::exception &exception) {
            std::cout << "failed with exception " << exception.what() << "\n";
        }
    }
}

void net::timeout_handler(int sig) {
    // no need to handle the alarm, just interrupt the normal code flow
}
