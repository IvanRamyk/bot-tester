#include <iostream>
#include <fstream>
#include "../server/Server.hpp"

int getPortName(char* argument){
    int id = 0;
    int result = 0;
    while(argument[id]!='\0'){
        if(!isdigit(argument[id]))
            return -1;
        result *= 10;
        result += (int) (argument[id] - '0');
        id++;
        if(id > 6)
            return -1;
    }
    if(result < 1000 || result > 9999)
        return -1;
    return result;
}

int main(int argc, char** argv) {
    Server server;
    Server server2;

    std::cout << argv[0] << "|" << argv[1] << "|" << argv[2] << std::endl;
    int serverPortName = getPortName(argv[1]);
    int myPortName = getPortName(argv[2]);
    std::string filename = "logs/errorLog_" + std::string(argv[1]) + "_" + std::string(argv[2]) + ".txt";
    const char* charFilename = filename.c_str();
    std::ofstream errorLog(charFilename);
    if(serverPortName == -1){
        errorLog << "CLIENT ERROR: Incorrect server port name :" << argv[1] << std::endl;
        return 0;
    }
    if(myPortName == -1){
        errorLog << "CLIENT ERROR: Incorrect client port name :" << argv[2] << std::endl;
        return 0;
    }
    std::cout << "Bot listening on: " << myPortName << std::endl;
    // std::ofstream ofs("log.txt");
    std::string message;
    message = server.receive(myPortName);
    usleep(100);
    while(message != "over") {
        message = server.receive(myPortName);
        usleep(100);
        std::cout << message;
        if(message == "2"){
            continue;
        }else if (message == "over"){
            break;
        }
        getline(std::cin, message); //FIXME time check
        server.send(serverPortName, message);
    }
    return 0;
}