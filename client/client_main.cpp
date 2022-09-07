#include "client.hpp"
#include "../common.hpp"

int main() {
    net::Client client(SERVER_IP, PORT);
    client.start();
}