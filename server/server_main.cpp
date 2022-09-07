#include "server.hpp"
#include "../common.hpp"

int main() {
    net::Server server(PORT, SERVER_IP, 20);
    server.start();
}