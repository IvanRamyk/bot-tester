#include <iostream>
#include <gtest/gtest.h>
#include "server/Server.hpp"

int main() {
    Server server;
    std::string temp;
    std::cout << server.receive(8001);
    sleep(0.1);
    server.send(8002, "1");

    std::cout << server.receive(8001);
    sleep(0.1);
    server.send(8002, "3");

    std::cout << server.receive(8001);
    sleep(0.1);
    server.send(8002, "5");

    return 0;
}
