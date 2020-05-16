//#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>
#include "tournament/tournament.h"

bool comp(std::pair<std::string, int> a, std::pair<std::string, int> b){
    return a.second > b.second;
}

int main() {
    //testing::InitGoogleTest();
    //RUN_ALL_TESTS();
    std::cout << "Project complied!" << std::endl;
    auto result = tournament("players/names.txt");
    std::cout << "\n\n\n------------------------------------------" << std::endl;
    std::cout << "\t\tTOURNAMENT HAS ENDED!" << std::endl;
    std::cout << "FINAL RESULTS: " << std::endl;
    sort(result.begin(), result.end(), comp);
    for(auto r : result){
        std::cout << r.first << " : " << r.second << std::endl;
    }
    return 0;
}
