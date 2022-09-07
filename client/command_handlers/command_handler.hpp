#pragma once

#include "../../io/defaultio.hpp"

namespace client_command {

    /// client command handler
    class CommandHandler {
    public:
        explicit CommandHandler(int conn_fd);
        virtual ~CommandHandler() = default;
    protected:
        int conn_fd;
    public:
      virtual void execute() = 0;
    };

}
