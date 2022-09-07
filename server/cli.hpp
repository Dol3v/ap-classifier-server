#pragma once

#include <vector>
#include "commands/command.hpp"

class CLI {
private:
    std::vector<command::Command*> commands;
    io::DefaultIo& default_io;
    int client_id;
    ClientData* client_data;
public:
    CLI(io::DefaultIo &dio, int client_id);

    void print_menu();
    void start();
    ~CLI();
};
