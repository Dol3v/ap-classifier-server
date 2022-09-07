#include "common.hpp"
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <iostream>

void common::send_string(int sock_fd, const std::string& contents) {
    send_with_prelude(sock_fd, contents.data(), contents.size() + 1);
}

void common::send_with_prelude(int sock_fd, const void *data, unsigned int length) {
    std::string header;
    std::stringstream ss(header);
    ss << "length:" << length << "\n";
    // packet specifying length of next one
    ssize_t sent_bytes = ::send(sock_fd, ss.str().c_str(), ss.str().size(), 0);
    if (sent_bytes < ss.width()) {
        throw std::logic_error("Couldn't send prelude packet");
    }
    // data packet
    sent_bytes = ::send(sock_fd, data, length, 0);
    if (sent_bytes < length) {
        throw std::logic_error("Couldn't sent data packet");
    }
}

int common::parse_prelude(const std::string &contents) {
    const static std::string length_prelude = "length:";
    auto without_length_prefix = contents.substr(length_prelude.size());
    return std::stoi(without_length_prefix);
}

void common::file_upload(const std::string &source_path, int sock_fd) {
    std::ifstream file(source_path, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    if (!file || size == -1) {
        common::send_string(sock_fd, "failed");
        throw std::logic_error("couldn't open file");
    }
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size))
    {
        std::cout << "buffer size: " << buffer.size() << std::endl;
        common::send_with_prelude(sock_fd, buffer.data(), buffer.size());
    }
}

void common::file_recv_to_path(const std::string &destination_path, int sock_fd) {
    std::ofstream dest_file(destination_path, std::fstream::trunc | std::fstream::out | std::fstream::in);
    std::cout << "received file to path\n";
    dest_file << common::recv_string(sock_fd);
    dest_file.close();
}

std::string common::recv_string(int conn_fd) {
    auto prelude = recv_until(conn_fd, '\n');
    auto size = parse_prelude(prelude);

    std::vector<char> contents;
    contents.resize(size);
    if(!::read(conn_fd, contents.data(), size)) {
        throw std::runtime_error("socket was closed\n");
    }

    // the actual size contains the null terminator, which is both unnecessary for the std::string ctor
    // and creates some *really* weird bugs when it's included
    return std::string(contents.data(), size - 1);
}

std::string common::recv_until(int sock_fd, char end_of_data) {
    char curr;
    ssize_t read_bytes;
    std::string data;
    std::stringstream ss(data);

    while (true) {
        read_bytes = ::read(sock_fd, &curr, 1);
        if (read_bytes == -1) {
            throw std::runtime_error("couldn't read from socket");
        }

        if (curr == end_of_data) {
            break;
        }

        if (read_bytes == 0) {
            throw std::runtime_error("socket closed");
        }
        ss << curr;
    }
    return ss.str();
}
