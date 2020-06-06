//
// Created by sadoffnick on 16.05.20.
//

#ifndef TESTER_TESTGAME_H
#define TESTER_TESTGAME_H

#include <string>


class TestGame {
private:
    int win = 0;
    int moving = 0;
public:
    std::pair <bool, std::string> makeMove(const std::string& playerMove);
    int winner();
};




#endif //TESTER_TESTGAME_H
