#pragma once

#include <string>

#define PORT 5000
#define SERVER_IP "127.0.0.1"

/// Utility functions that are common to both the client and server.
namespace common {

    std::string recv_until(int sock_fd, char end_of_data);

    std::string recv_string(int conn_fd);

    void send_with_prelude(int sock_fd, const void *data, unsigned int length);

    int parse_prelude(const std::string &contents);

    void file_recv_to_path(const std::string &destination_path, int sock_fd);

    void file_upload(const std::string &source_path, int sock_fd);

    void send_string(int sock_fd, const std::string &contents);
}