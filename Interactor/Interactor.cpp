//
// Created by sadoffnick on 11.05.20.
//

#include "Interactor.h"



bool isMove(const std::string& move) {
    if (move.length() < 4)
        return false;
    std::string temp = "";
    for(int i = 0; i < 4; i++)
        temp += move[i];
    return temp == "move";
}

bool isPartition(const std::string& move){
    if(move.length() < 9)
        return false;
    std::string temp = "";
    for(int i = 0; i < 9; i++)
        temp += move[i];
    return temp == "partition";
}

Move getMove(const std::string& move){

}

template<>
