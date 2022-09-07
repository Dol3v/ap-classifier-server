#pragma once

#include "command_handler.hpp"

namespace client_command {
    class HandleUpload : public CommandHandler {
    public:
        HandleUpload(int conn_fd) : CommandHandler(conn_fd) {}

        virtual void execute();
    };
}
