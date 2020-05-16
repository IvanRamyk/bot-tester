//#include <gtest/gtest.h>
#include <iostream>
#include "tournament/tournament.h"

int main() {
    //testing::InitGoogleTest();
    //RUN_ALL_TESTS();
    std::cout << "Project complied!" << std::endl;
    tournament("players/names.txt");
    return 0;
}
