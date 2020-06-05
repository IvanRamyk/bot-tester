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

std::string getRidOfZeroes(std::string message){
    std::string result = "";
    int id = 0;
    while(id < message.length() && message[id] == '0') id++;
    while(id < message.length()) result += message[id++];
    return result;
}

std::string waitForInput(int timing){
    std::chrono::system_clock::time_point two_seconds_passed
            = std::chrono::system_clock::now() + std::chrono::milliseconds(timing);
    std::promise<std::string> p;
    std::future<std::string> TL_cin = p.get_future();
    std::thread([](std::promise<std::string> p)
                {
                    std::string s;
                    getline(std::cin, s);
                    p.set_value_at_thread_exit(s);
                },
                std::move(p)
    ).detach();

    if(std::future_status::ready == TL_cin.wait_until(two_seconds_passed))
    { return TL_cin.get(); }
    else
    { return "TL";}
}

int main(int argc, char** argv) {
    Server server;
    Server server2;

    int serverPortName = getPortName(argv[1]);
    int myPortName = getPortName(argv[2]);
    std::string filename = "logs/errorLog_" + std::string(argv[1]) + "_" + std::string(argv[2]) + ".txt";
    const char* charFilename = filename.c_str();
    std::ofstream errorLog(charFilename);
    if(serverPortName == -1){
        errorLog << "CLIENT ERROR: Incorrect server port name :" << argv[1] << std::endl;
        return 0;
    }
    //errorLog << argv[0] << "|" << argv[1] << "|" << argv[2] << std::endl;
    if(myPortName == -1){
        errorLog << "CLIENT ERROR: Incorrect client port name :" << argv[2] << std::endl;
        return 0;
    }
    errorLog << "Bot listening on: " << myPortName << std::endl;
    //std::ofstream ofs("log.txt");
    std::string message;
    //message = server.receive(myPortName);
    //usleep(100);
    message = server.receive(myPortName);
    //errorLog << "Client catched: " << message << std::endl;
    usleep(100000);
    std::cout << message << std::endl;
    if(message == "1"){
        if (message == "over" || message == "TL"){
            return 0;
        }
        message = waitForInput(300);
    //    errorLog << "Participant has answered:" << std::endl;//FIXME time check
        message = getRidOfZeroes(message);

    //    errorLog << "Cleared answer with: " << message << std::endl;
        server.send(serverPortName, message);
    }
    while(message != "over") {
     //   errorLog << "Bot now waits for messages" << std::endl;
        message = server.receive(myPortName);
        usleep(100000);
    //    errorLog << "Bot got the message: " << message << std::endl;
        std::cout << message << std::endl;
        usleep(100000);
        //if(message == "2"){
        //    continue;
        //}else
        if (message == "over"){
            break;
        }
        message = waitForInput(150);
     //   errorLog << "Participant answers with: ";// << message << std::endl;//FIXME time check
        message = getRidOfZeroes(message);

    //    errorLog << "Cleared answer with: " << message << std::endl;
        server.send(serverPortName, message);
    }
    errorLog << "Bot says good-bye!\n";
    return 0;
}