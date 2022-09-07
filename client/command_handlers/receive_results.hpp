#pragma once

#include "command_handler.hpp"

namespace client_command {
    
    /// receive results command
    class FileReceive : public CommandHandler {
    public:
        FileReceive(int conn_fd) : CommandHandler(conn_fd) {}
        virtual void execute();
    };

} // client_command
