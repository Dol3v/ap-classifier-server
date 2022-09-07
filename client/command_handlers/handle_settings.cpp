
#include <iostream>
#include <limits>
#include "handle_settings.hpp"
#include "../../common.hpp"

namespace client_command {
    void HandleSettings::execute() {
        // flushing cin
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // printing prompt
        std::cout << common::recv_string(this->conn_fd);
        // reading user input
        std::string user_settings;
        std::getline(std::cin, user_settings);
        if (user_settings.empty()) {
            common::send_string(this->conn_fd, "none");
            return;
        }

        common::send_string(this->conn_fd, user_settings);
        auto response = common::recv_string(this->conn_fd);
        std::cout << response;
    }
} // client_command