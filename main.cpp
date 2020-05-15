#include <iostream>
#include <gtest/gtest.h>
#include "game/Quoridor.hpp"
#include "Interactor/Interactor.h"

int main() {
    /*auto* game = new Quoridor();
    auto* interactor = new Interactor<Quoridor>(game);

    std::cout << "I've done my branch!\n";*/
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
    return 0;
}
