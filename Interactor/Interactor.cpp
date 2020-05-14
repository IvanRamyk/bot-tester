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
GameStatus* Interactor<Quoridor>::nextMove(const std::string &prevMove) {
    if(isMove(prevMove)){
        Move playerMove = getMove(prevMove);
        bool result = Game->move(playerMove);
        if(result){
           return new GameStatus(0,"");
        }
    }else if(isPartition(prevMove)){

    }
}
