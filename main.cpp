#include <iostream>
#include "Quoridor.hpp"
int main() {
    Quoridor game;
    game.setPartition(Partition({1, 1}, Up));
    return 0;
}
