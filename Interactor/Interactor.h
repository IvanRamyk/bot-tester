//
// Created by sadoffnick on 11.05.20.
//

#ifndef TESTER_INTERACTOR_H
#define TESTER_INTERACTOR_H

#include <string>
#include "../game/Quoridor.hpp"

struct GameStatus{
    bool gameOn;
    std::string preMove;
    std::string logInfo = "";

    GameStatus(bool _gameOn, const std::string& _preMove, const std::string& _log = ""){
        gameOn = _gameOn;
        preMove = _preMove;
        logInfo = _log;
    }

};

template <class GameInstance>
class Interactor {
    GameInstance* Game;
    void tcpSend();
    void tcpReceive();

public:
    explicit Interactor<GameInstance>(GameInstance* gameInstance){
        Game = gameInstance;
    }
    GameStatus startGame();
    GameStatus * nextMove(const std::string& prevMove);
};


#endif //TESTER_INTERACTOR_H
