//
// Created by ivan on 07.05.20.
//

#include "testServer.hpp"

inline size_t read_complete(char * buff, const boost::system::error_code & err, size_t bytes)
{
    if ( err) return 0;
    bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
    return found ? 0 : 1;
}


std::string testServer::receive(int port) {
    boost::asio::ip::tcp::acceptor acceptor(service,  boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(),port));
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    boost::asio::ip::tcp::socket sock(service);
    acceptor.accept(sock);
    int bytes = read(sock, boost::asio::buffer(buff), boost::bind(read_complete,buff,_1,_2));
    std::string msg(buff, bytes);
    sock.write_some(boost::asio::buffer(msg));
    sock.close();
    std::string res = buff;
    res.pop_back();// removing endline from the end
    return res;
}

void testServer::send(int port, std::string message) {
    boost::asio::ip::tcp::endpoint ep( boost::asio::ip::address::from_string("127.0.0.1"), port);
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(boost::asio::buffer(std::string(message + '\n')));
    sock.close();
}
