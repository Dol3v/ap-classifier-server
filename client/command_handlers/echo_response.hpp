

#include "command_handler.hpp"

namespace client_command {
    /// simply prints the server's response
    class EchoResponse : public CommandHandler {
    private:
        int num_responses;
    public:
        EchoResponse(int conn_fd, int num_responses = 1) : CommandHandler(conn_fd), num_responses(num_responses) {}
        virtual void execute();
    };
}
