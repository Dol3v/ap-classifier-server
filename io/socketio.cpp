
#include <iostream>
#include "socketio.hpp"
#include "../common.hpp"

std::string io::SocketIo::read() {
    try {
        return common::recv_string(this->sock_fd);
    } catch (std::runtime_error& error) {
        throw socket_closed_error();
    }
}

void io::SocketIo::write(const std::string &data) {
    try {
        common::send_string(this->sock_fd, data);
    } catch (std::runtime_error &) {
        throw socket_closed_error();
    }
}

io::SocketIo::~SocketIo() {
    ::close(this->sock_fd);
}
