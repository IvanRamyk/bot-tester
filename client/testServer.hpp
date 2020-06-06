//
// Created by ivan on 07.05.20.
//

#ifndef TESTER_SERVER_HPP
#define TESTER_SERVER_HPP


#include <ctime>
#include <string>
#include <iostream>

#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp>



using boost::asio::ip::tcp;
using namespace boost::placeholders;


class testServer {
private:
    boost::asio::io_service service;

public:
    testServer() {}

    std::string receive(int port);
    void send(int port, std::string message);
};


#endif //TESTER_SERVER_HPP
