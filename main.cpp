#include <iostream>
#include "game/Quoridor.hpp"
#include "Interactor/Interactor.h"
#include "client/Client.h"

int main() {
    auto* game = new Quoridor();
    auto* interactor = new Interactor<Quoridor>(game);

    std::cout << "I've done my branch!\n";
    return 0;
}
