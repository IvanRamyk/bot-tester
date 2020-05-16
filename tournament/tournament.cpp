#include "../Interactor/Interactor.h"
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>

const bool FULL_LOG = true;

const int CNT_GAMES = 50;

const std::string clientPath = "client/cmake-build-debug/testClient";

template <class T>
int playGame(Interactor<T>* i, const std::string& gameName, int serverPort, int currentPlayer, int nextPlayer){
    return i->playGame(gameName, serverPort, currentPlayer, nextPlayer);
}

int playBattle(const std::string& firstPlayer, const std::string& secondPlayer){
    int serverPort = 8235,
            currentBotPort = serverPort+1,
            nextBotPort = currentBotPort;//serverPort+2;

    std::string pipeline = "mkfifo pipeline145";

    std::string commandOne = pipeline + "1\n pwd \n./" + clientPath + " ";
    commandOne += std::to_string(serverPort) + " " + std::to_string(currentBotPort) + " < pipeline1451 | ";
    commandOne += firstPlayer + " > pipeline1451";

   // std::string commandTwo = pipeline + "2\n ./" + clientPath + " ";
   // commandTwo += std::to_string(serverPort) + " " + std::to_string(currentBotPort) + " < pipeline1452 | ";
   // commandTwo += secondPlayer + " > pipeline1452";

    std::ofstream file1("script1.sh");
    file1 << commandOne << std::endl;
   // std::ofstream file2("script2.sh");
   // file2 << commandTwo << std::endl;
    file1.close();
   // file2.close();

    auto* game = new TestGame();
    auto* interactor = new Interactor<TestGame>(game);

    auto fut = std::async (std::launch::async, system, "./script2.sh");
    auto handle = std::async(std::launch::async, playGame<TestGame>, interactor, firstPlayer + "_" + secondPlayer, serverPort, currentBotPort, nextBotPort);
    int x = 7;
    //auto fut1 = std::async (std::launch::async, system, "./script1.sh");
    usleep(2000);
    // sleep(3);
    int win = handle.get();//interactor->playGame(firstPlayer + "_" + secondPlayer, serverPort, currentBotPort, nextBotPort);
    std::cout << "Battle was successful!" << std::endl;
    return win;
}


std::vector <std::pair <std::string, int>> tournament(const std::string& file_path){
    std::vector <std::pair <std::string, int>> result;
    std::ifstream ifs(file_path);
    std::vector <std::string> players;
    std::string plr;
    std::string teamName;
    int cntPlayers = 0;
    while(getline(ifs, plr)){
        getline(ifs, teamName);
        players.push_back(plr);
        result.emplace_back(teamName, 0);
        cntPlayers++;
    }
    std::string bashCommand;
    std::string playersRoot = "players/";
    for(int firstPlayer = 0; firstPlayer < cntPlayers; firstPlayer++){
        for(int secondPlayer = firstPlayer+1; secondPlayer < cntPlayers; secondPlayer++){
            int firstWin = 0, secondWin = 0, tie = 0, winner = 0;
            for(int i = 0; i < CNT_GAMES; i++){
                winner = playBattle(players[firstPlayer], players[secondPlayer]);
                switch(winner){
                    case 0:
                        tie++;
                        break;
                    case 1:
                        firstWin++;
                        std::cout << "First player wins!\n";
                        break;
                    case 2:
                        secondWin++;
                        std::cout << "Second player wins!\n";
                        break;
                    default:
                        break;
                }
                winner = playBattle(players[secondPlayer], players[firstPlayer]);
                switch(winner){
                    case 0:
                        tie++;
                        break;
                    case 1:
                        secondWin++;
                        std::cout << "Second player wins!\n";
                        break;
                    case 2:
                        firstWin++;
                        std::cout << "First player wins!\n";
                        break;
                    default:
                        break;
                }
            }
            if(FULL_LOG){
                if(firstWin > secondWin){
                    std::cout << "First player won with result (" + std::to_string(firstWin)
                    +  ":" + std::to_string(secondWin) + ")\n";
                }else if(firstWin < secondWin){
                    std::cout << "Second player won with result (" + std::to_string(secondWin)
                                 +  ":" + std::to_string(firstWin) + ")\n";
                }else{
                    std::cout << "It's a tie! " + std::to_string(tie) + " draw matches.\n";
                }
            }
        }
    }
    return result;
}

