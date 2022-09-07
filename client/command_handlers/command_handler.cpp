

#include "command_handler.hpp"

namespace client_command {

    CommandHandler::CommandHandler(int conn_fd) : conn_fd(conn_fd) {}
} // CommandHandler