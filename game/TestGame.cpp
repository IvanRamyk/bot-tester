//
// Created by sadoffnick on 16.05.20.
//

#include "TestGame.h"

std::pair <bool, std::string> TestGame::makeMove(const std::string &playerMove) {
    if(playerMove == "0" || playerMove == "over"){
        win = (moving ? 1 : 2);
        return {0, playerMove};
    }
    moving ^= 1;
    return {1,playerMove};

}

int TestGame::winner(){
    return win;
}