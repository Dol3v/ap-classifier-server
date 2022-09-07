
#include <iostream>
#include "handle_upload.hpp"
#include "../../common.hpp"

void client_command::HandleUpload::execute() {
    auto prompt = common::recv_string(this->conn_fd);
    std::cout << prompt;
    std::string path;
    std::cin >> path;
    std::cout << path << std::endl;
    try {
        common::file_upload(path, this->conn_fd);
    } catch (std::logic_error &error) {
        std::cout << error.what() << "\n";
        return;
    }

    std::cout << common::recv_string(this->conn_fd);
    std::cin >> path;
    try {
        common::file_upload(path, this->conn_fd);
    } catch (std::logic_error &error) {
        std::cout << error.what() << "\n";
        return;
    }
}
