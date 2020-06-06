//
// Created by sadoffnick on 16.05.20.
//

#ifndef TESTER_TOURNAMENT_H
#define TESTER_TOURNAMENT_H

#include <string>
#include <vector>

int playBattle(const std::string& firstPlayer, const std::string& secondPlayer);

std::vector <std::pair <std::string, int>> tournament(const std::string& file_path);

#endif //TESTER_TOURNAMENT_H
