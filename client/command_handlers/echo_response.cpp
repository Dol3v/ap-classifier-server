#include <iostream>
#include "echo_response.hpp"
#include "../../common.hpp"

void client_command::EchoResponse::execute() {
    for (int i = 0; i < this->num_responses; i++) {
        std::cout << common::recv_string(this->conn_fd);
    }
}
