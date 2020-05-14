#include "../game/Quoridor.hpp"
#include <gtest/gtest.h>
//
// Created by ivan on 02.05.20.
//
TEST(Quoridor, Init) {
    Quoridor game;
    EXPECT_TRUE(!game.move({0, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Down}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(!game.move({0, Down}));
    EXPECT_TRUE(!game.move({0, Down}));
    EXPECT_TRUE(game.move({0, Left}));
}

TEST(Quoridor, JumpDown){
    Quoridor game;
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(!game.move({0, Up}));
}

TEST(Quoridor, JumpUp){
    Quoridor game;
    EXPECT_TRUE(game.setPartition({{3,3}, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(game.move({1, Down}));
    EXPECT_TRUE(game.move({0, Up}));
    EXPECT_TRUE(!game.move({1, Down}));
}

TEST(Quoridor, PartitionCollisionRight) {
    Quoridor game;
    EXPECT_TRUE(game.setPartition({{4, 4}, Right}));
    EXPECT_FALSE(game.setPartition({{4, 5}, Right}));
    EXPECT_TRUE(game.setPartition({{4, 6}, Right}));
    EXPECT_TRUE(game.setPartition({{4, 10}, Left}));

}

TEST(Quoridor, PartitionCollisionLeft) {
    Quoridor game;
    EXPECT_TRUE(game.setPartition({{4, 5}, Left}));
    EXPECT_FALSE(game.setPartition({{4, 4}, Left}));
    EXPECT_FALSE(game.setPartition({{4, 2}, Left}));
    EXPECT_TRUE(game.setPartition({{4, 3}, Left}));

}

TEST(Quoridor, PartitionCollisionUp) {
    Quoridor game;
    EXPECT_TRUE(game.setPartition({{6, 5}, Up}));
    EXPECT_FALSE(game.setPartition({{3, 1}, Up}));
    EXPECT_FALSE(game.setPartition({{3, 10}, Up}));
    EXPECT_TRUE(game.setPartition({{3, 2}, Up}));
    EXPECT_TRUE(game.setPartition({{3, 9}, Up}));
    EXPECT_FALSE(game.setPartition({{7, 5}, Up}));
    EXPECT_TRUE(game.setPartition({{8, 5}, Up}));

}

TEST(Quoridor, PartitionCollisionDown) {
    Quoridor game;
    EXPECT_TRUE(game.setPartition({{8, 5}, Down}));
    EXPECT_FALSE(game.setPartition({{3, 1}, Down}));
    EXPECT_FALSE(game.setPartition({{3, 10}, Down}));
    EXPECT_TRUE(game.setPartition({{3, 2}, Down}));
    EXPECT_TRUE(game.setPartition({{3, 9}, Down}));
    EXPECT_FALSE(game.setPartition({{9, 5}, Down}));
    EXPECT_TRUE(game.setPartition({{10, 5}, Down}));

}

TEST(Quoridor, PartitionIntersection) {
    Quoridor game;
    EXPECT_TRUE(game.setPartition({{4, 4}, Right}));
    EXPECT_FALSE(game.setPartition({{5, 5}, Down}));
}

TEST(Quoridor, Connection) {
    Quoridor game;
    EXPECT_TRUE(game.setPartition({{2,1}, Right}));
    EXPECT_TRUE(game.setPartition({{2,3}, Right}));
    EXPECT_TRUE(game.setPartition({{2,5}, Right}));
    EXPECT_TRUE(game.setPartition({{2,7}, Right}));
    EXPECT_TRUE(game.setPartition({{2,9}, Up}));
    EXPECT_FALSE(game.setPartition({{4,8}, Right}));
    EXPECT_TRUE(game.setPartition({{4,7}, Right}));
}

TEST(Quoridor, Draw) {
    Quoridor game;
    for (int i = 0; i < 50; ++i) {
        if (i % 4 == 0) {
            EXPECT_TRUE(game.move({0, Up}));
        }
        if (i % 4 == 1) {
            EXPECT_TRUE(game.move({1, Down}));
        }
        if (i % 4 == 2) {
            EXPECT_TRUE(game.move({0, Down}));
        }
        if (i % 4 == 3) {
            EXPECT_TRUE(game.move({1, Up}));
        }
    }
    EXPECT_TRUE(game.draw());

}

TEST(Quoridor, Draw2) {
    Quoridor game;
    for (int i = 0; i < 50; ++i) {
        if (i == 25) {
            EXPECT_TRUE(game.setPartition({{2,2}, Right}));
            EXPECT_TRUE(game.setPartition({{7,2}, Right}));
            EXPECT_TRUE(game.move({1, Down}));
            continue;
        }
        if (i % 4 == 0) {
            EXPECT_TRUE(game.move({0, Up}));
        }
        if (i % 4 == 1) {
            EXPECT_TRUE(game.move({1, Down}));
        }
        if (i % 4 == 2) {
            EXPECT_TRUE(game.move({0, Down}));
        }
        if (i % 4 == 3) {
            EXPECT_TRUE(game.move({1, Up}));
        }
    }
    EXPECT_FALSE(game.draw());

}



TEST(Quoridor, Exeption) {
    try {
        Quoridor game(3);
    }
    catch (std::invalid_argument& e) {
        //std::cout << e.what() << "\n";
    }
}

TEST(Quoridor, logs) {
    Quoridor game;
    auto res = game.makeMove("move 2 5");
    EXPECT_TRUE(res.first &&  res.second == "move 2 5");
    res = game.makeMove("move 2 4");
    EXPECT_TRUE(!res.first);
    //std::cout << res.second << "\n";
    res = game.makeMove("partition 2 2 2 4");
    //std::cout << res.second << "\n";
    EXPECT_TRUE(res.first && res.second == "partition 2 2 2 4");
}