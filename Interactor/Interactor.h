//
// Created by sadoffnick on 11.05.20.
//

#ifndef TESTER_INTERACTOR_H
#define TESTER_INTERACTOR_H

#include <string>
#include <fstream>
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
    bool recordLog = false;
    bool writeLog = true;


public:
    explicit Interactor<GameInstance>(GameInstance* gameInstance){
        Game = gameInstance;
    }
    int playGame(const std::string& gameName);
};

bool isDraw(const std::string& result){
    if(result.length() < 4)
        return false;
    std::string word = "";
    for(int i = 0; i < 4; i++){
        word = result[result.length
    }
}

template <class Game>
int Interactor<Game>::playGame(const std::string& gameName) {
    std::ofstream ofs("gameLog_" + gameName);
    server.send(8002, "1");
    server.send(8003, "2");
    int currentPlayer = 8002;
    int nextPlayer = 8003;
    int serverPort = 8004;
    while(true) {
        std::string playerMove = server.receive(serverPort);
        auto result = Game->makeMove();
        sleep(0.1);
        if(recordLog)
            ofs << result.second << std::endl;
        else if(writeLog)
            std::cout << result.second << std::endl;
        if (!result.first) {
            server.send(currentPlayer, "-1"); //FIXME: Game over SIGNAL, maybe should be who won
            server.send(nextPlayer, "-1");
            return Game->winner();
        }
        server.send(nextPlayer, result.second);
        std::swap(currentPlayer, nextPlayer);
    }
}



#endif //TESTER_INTERACTOR_H
