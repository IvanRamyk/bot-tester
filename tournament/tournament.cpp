#include "../Interactor/Interactor.h"
#include "../server/Server.hpp"
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>

const bool FULL_LOG = true;

const int CNT_GAMES = 50;

std::vector <std::pair <std::string, int>> tournament(const std::string& file_path){
    std::vector <std::pair <std::string, int>> result;
    std::ifstream ifs("names.txt");
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
    std::string firstClient = "client/firstClient/Client";
    std::string secondClient = "client/secondClient/Client";
    std::string playersRoot = "players/";
    for(int firstPlayer = 0; firstPlayer < cntPlayers; firstPlayer++){
        for(int secondPlayer = firstPlayer+1; secondPlayer < cntPlayers; secondPlayer++){
            int firstWin = 0, secondWin = 0, tie = 0;


            //first vs. second
            bashCommand += "mkfifo firstPipeline && ";
            bashCommand += players[firstPlayer] + " < firstPipeline | ";
            bashCommand += "./" + firstClient + " > firstPipeline && ";

            bashCommand += "mkfifo secondPipeline && ";
            bashCommand += players[secondPlayer] + " < secondPipeline | ";
            bashCommand += "./" + secondClient + " > secondPipeline";

            std::string filename = "script_" + std::to_string(firstPlayer);
            filename += "_" + std::to_string(secondPlayer);
            std::ofstream ofs(filename);
            ofs << bashCommand;

            for(int i = 0; i < CNT_GAMES; i++){
                const char* connect = ("./" + filename).c_str();
                std::system(connect);
                auto* game = new Quoridor();
                auto* interactor = new Interactor<Quoridor>(game);
                std::string gameName = std::to_string(firstPlayer) + "_" + std::to_string(secondPlayer) + "_" + std::to_string(i);
                int winner = interactor->playGame(gameName);
                switch(winner){
                    case 0:
                        tie++;
                        break;
                    case 1:
                        firstWin++;
                        break;
                    case 2:
                        secondWin++;
                        break;
                    default:
                        break;
                }

            }
            //second vs. first

            bashCommand += "mkfifo firstPipeline && ";
            bashCommand += players[secondPlayer] + " < firstPipeline | ";
            bashCommand += "./" + firstClient + " > firstPipeline && ";

            bashCommand += "mkfifo secondPipeline && ";
            bashCommand += players[firstPlayer] + " < secondPipeline | ";
            bashCommand += "./" + secondClient + " > secondPipeline";

            filename = "script_" + std::to_string(secondPlayer);
            filename += "_" + std::to_string(firstPlayer);
            std::ofstream ofs1(filename);
            ofs1 << bashCommand;

            for(int i = 0; i < CNT_GAMES; i++){
                const char* connect = ("./" + filename).c_str();
                std::system(connect);
                auto* game = new Quoridor();
                auto* interactor = new Interactor<Quoridor>(game);
                std::string gameName = std::to_string(secondPlayer) + "_" + std::to_string(firstPlayer) + "_" + std::to_string(i);
                int winner = interactor->playGame(gameName);
                switch(winner){
                    case 0:
                        tie++;
                        break;
                    case 1:
                        secondWin++;
                        break;
                    case 2:
                        firstWin++;
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

