#include <iostream>
#include <fstream>


#include <stdio.h>
#include <stdlib.h>
#include "Client.h"


std::string second_update_message(const std::string& message){
    return message + " a";
}

std::string tcp_send_and_wait(std::string message){
    if(message == "-1")
        return message;
    return second_update_message(message);
}

void Client::speakWithPlayer() {
    int moves = 3;
    std::string message = "hel lo";
    std::ofstream ofs("log1.txt");
    while(message != "-1"){
        ofs << "Client message: " << message << std::endl;
        std::cout << message << std::endl;
        getline(std::cin,message);
        ofs << "Bot message: " << message << std::endl;
        message = tcp_send_and_wait(message);
        ofs << "Server response message: " << message << std::endl;
        if(message == "-1"){
            //std::cout <<  << std::endl;
            return;
        }
    }
}