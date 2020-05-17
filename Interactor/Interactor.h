//
// Created by sadoffnick on 11.05.20.
//

#ifndef TESTER_INTERACTOR_H
#define TESTER_INTERACTOR_H

#include <string>
#include <fstream>
#include "../game/TestGame.h"
#include "../game/Quoridor.hpp"
#include "../server/Server.hpp"

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
    Server server;
    bool recordLog = true;
    bool writeLog = true;


public:
    explicit Interactor<GameInstance>(GameInstance* gameInstance){
        Game = gameInstance;
    }
    int playGame(const std::string& gameName, int serverPort, int currentPlayer, int nextPlayer);
};

bool isDraw(const std::string& result){
    if(result.length() < 4)
        return false;
    std::string word = "";
    for(int i = 4; i > 0; i--){
        word += result[result.length() - i];
    }
    return word == "draw";
}

template <class Game>
int Interactor<Game>::playGame(const std::string& gameName, int serverPort, int currentPlayer, int nextPlayer) {
    std::ofstream ofs("gameLog_" + gameName);
    server.send(currentPlayer, "1");
    server.send(nextPlayer, "2");
    usleep(100000);
    while(true) {
        std::string playerMove = server.receive(serverPort);
        ofs << "Server received : " << playerMove << std::endl;
        std::cout << "Server received : " << playerMove << std::endl;
        auto result = Game->makeMove(playerMove);
        std::cout << (result.first ? " and the game is on" : "but i didn't like it " ) << result.second << std::endl;
        usleep(100000);
        recordLog = true;
        writeLog = true;
        if(recordLog)
            ofs << result.second << std::endl;
        else if(writeLog)
            std::cout << result.second << std::endl;
        if (!result.first) {
            //if(playerMove != "over" && playerMove != "TL")
                server.send(currentPlayer, "over");
            server.send(nextPlayer, "over");
            ofs.close();
            return Game->winner();
        }
        server.send(nextPlayer, result.second);
        std::swap(currentPlayer, nextPlayer);
    }
}



#endif //TESTER_INTERACTOR_H
