#pragma once


#include "command_handler.hpp"

namespace client_command {

    class HandleSettings : public CommandHandler {
    public:
        explicit HandleSettings(int conn_fd) : CommandHandler(conn_fd) {}
        virtual void execute();
    };

} // client_command
