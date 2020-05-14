//
// Created by ivan on 01.05.20.
//

#ifndef TESTER_QUORIDOR_HPP
#define TESTER_QUORIDOR_HPP

#include <set>
#include <map>
#include <iostream>
#include <sstream>
#include <cctype>
using std::set;
using std::map;

enum Direction {
    Left, Right, Up, Down, UpLeft, UpRight, DownLeft, DownRight, DownDown, UpUp, LeftLeft, RightRight, None
};

struct Position {
    int x, y;
    Position()= default;
};


struct Player {
    Position position;
};

struct Partition {
    Position start_point;
    Direction direction;
    Partition (Position pos, Direction dir): start_point(pos), direction(dir) {};
    Partition()= default;
};

inline bool operator < (Partition a, Partition b);



struct Move {
    int player_number;
    Direction direction;
};

class Quoridor {
private:
    Player players[4]{};
    unsigned int acting_player;
    int used_partitions[4];
    int player_number;
    int cnt_moves;
    map<Partition, int> partitions;// with length 1, direction only Right or Up (for more strict definition)
    static Partition _downToUp(Partition p);

    static Partition _leftToRight(Partition p);

    static std::pair<Partition, Partition> _splitPartition(Partition partition);

    void _passMove();

    static bool _onField(Partition partition);

    bool _moveUp(int x, int y);

    bool _moveDown(int x, int y);

    bool _moveRight(int x, int y);

    bool _moveLeft(int x, int y);

    bool _moveUpLeft(int x, int y);

    bool _moveUpRight(int x, int y);

    bool _moveDownRight(int x, int y);

    bool _moveDownLeft(int x, int y);

    int _diffX();

    int _diffY();

    bool _checkPath(int x, int y, int target, set<std::pair<int, int>> &used);

    bool _checkPath();

    static Direction _diffToDirection(int dx, int dy);

    static bool _checkFormat(const std::string& command);

public:
    explicit Quoridor(int cnt_players = 2);

    bool draw();

    bool firstPlayerVictory();

    bool secondPlayerVictory();

    bool move(Move move);

    bool setPartition(Partition partition);

    int winner();

    std::pair<bool, std::string> makeMove(const std::string& _move);
};


#endif //TESTER_QUORIDOR_HPP
