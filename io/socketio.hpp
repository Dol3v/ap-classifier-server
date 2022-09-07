#pragma once
#include <unistd.h>
#include <mutex>
#include "defaultio.hpp"

namespace io {
    class SocketIo : public io::DefaultIo {
    private:
        int sock_fd;
    public:
        explicit SocketIo(int sock_fd) : DefaultIo(), sock_fd(sock_fd) {}
        virtual std::string read();
        virtual void write(const std::string& data);
        virtual ~SocketIo();
    };

    class socket_closed_error : std::exception {
    public:
        const char* what() {
            return "socket was closed";
        }
    };
}
