#include "../Interactor/Interactor.h"
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>

const bool FULL_LOG = true;

const int CNT_GAMES = 5;

const std::string clientPath = "client/cmake-build-debug/testClient";

template <class T>
int playGame(Interactor<T>* i, const std::string& gameName, int serverPort, int currentPlayer, int nextPlayer){
    return i->playGame(gameName, serverPort, currentPlayer, nextPlayer);
}

int playBattle(const std::string& firstPlayer, const std::string& secondPlayer){
    std::cout << "Battle against " << firstPlayer << " and " << secondPlayer << std::endl;
    int serverPort = 5036,
            currentBotPort = serverPort+1,
            nextBotPort = serverPort+2;

    //std::string pipeline = "mkfifo pipeline && ";



    std::string commandOne = "cat pipeline | ./" + clientPath + " ";
    commandOne += std::to_string(serverPort) + " " + std::to_string(currentBotPort) + " | ";
    commandOne += firstPlayer + " > pipeline";

    std::string commandTwo = "cat pipeline2 | ./" + clientPath + " ";//= pipeline + "2\n .
    commandTwo += std::to_string(serverPort) + " " + std::to_string(nextBotPort) + " | ";
    commandTwo += secondPlayer + " > pipeline2";

    std::ofstream file1("script1.sh");
    file1 << commandOne << std::endl;
    std::cout << commandOne << std::endl;
    std::ofstream file2("script2.sh");
    std::cout << commandTwo << std::endl;
    file2 << commandTwo << std::endl;
    file1.close();
    file2.close();
    usleep(2000000);
    auto* game = new Quoridor();
    auto* interactor = new Interactor<Quoridor>(game);
    //auto fut = std::async (std::launch::async, system, "./script2.sh");
    //auto fut1 = std::async (std::launch::async, system, "./script1.sh");
    std::thread th1(system, "./script1.sh");
    std::thread th2(system, "./script2.sh");
    usleep(2000000);
    //freopen("game.txt", "w", stdout);
    int win = interactor->playGame(firstPlayer + "_" + secondPlayer, serverPort, currentBotPort, nextBotPort);
    //fclose(stdout);
    std::cout << "Battle was successful! (" << win << ")\n"  << std::endl;
    system("./clear.sh");
    th1.detach();
    th2.detach();

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
                usleep(1000000);
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
                usleep(1000000);
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
            if(firstWin > secondWin){
                result[firstPlayer].second += 3;
            }else if(firstWin < secondWin){
                result[secondPlayer].second += 3;
            }else{
                result[firstPlayer].second++;
                result[secondPlayer].second++;
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

