
#include <iostream>
#include <thread>
#include "receive_results.hpp"
#include "../../common.hpp"

namespace client_command {

    void FileReceive::execute() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "enter local path:" << std::endl;
        std::string local_path;
        std::getline(std::cin, local_path);
        common::file_recv_to_path(local_path, this->conn_fd);
    }
} // client_command