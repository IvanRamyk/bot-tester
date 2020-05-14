//
// Created by ivan on 01.05.20.
//

#ifndef TESTER_QUORIDOR_HPP
#define TESTER_QUORIDOR_HPP

#include <set>
#include <map>
#include <iostream>
#include <sstream>
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

inline bool operator < (Partition a, Partition b) {
    if (a.direction != b.direction)
        return a.direction < b.direction;
    if (a.start_point.x != b.start_point.x)
        return a.start_point.x < b.start_point.x;
    return a.start_point.y < b.start_point.y;

}



struct Move {
    int player_number;
    Direction direction;
};

class Quoridor {
private:
    Player players[4]{};
    unsigned int acting_player;
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

    Direction _diffToDirection(int dx, int dy) {
        Direction direction;
        if (dx == 1 && dy == 0)
            direction = Up;
        else if (dx == -1 && dy == 0)
            direction = Down;
        else if (dx == 0 && dy == 1)
            direction = Right;
        else if (dx == 0 && dy == -1)
            direction = Left;
        else if (dx == -1 && dy == -1)
            direction = DownLeft;
        else if (dx == 1 && dy == -1)
            direction = UpLeft;
        else if (dx == 1 && dy == 1)
            direction = UpRight;
        else if (dx == -1 && dy == 1)
            direction = DownRight;
        else if (dx == 2 && dy == 0)
            direction = UpUp;
        else if (dx == -2 && dy == 0)
            direction = DownDown;
        else if (dx == 0 && dy == 2)
            direction = RightRight;
        else if (dx == 0 && dy == -2)
            direction = LeftLeft;
        else
            direction = None;
        return direction;
    }

public:
    explicit Quoridor(int cnt_players = 2);

    bool draw();

    bool firstPlayerVictory();

    bool secondPlayerVictory();

    bool move(Move move);

    bool setPartition(Partition partition);

    std::pair<bool, std::string> makeMove(std::string _move) {
        std::string log = _move;
        std::stringstream stream(_move);
        std::string type;
        stream >> type;
        if (type == "move") {
            int x, y;
            stream >> x >> y;
            int cur_x = players[acting_player].position.x;
            int cur_y = players[acting_player].position.y;
            int player = acting_player;
            Direction direction = _diffToDirection(x - cur_x, y - cur_y);
            if (direction == None)
                return {false, log + "\nplayer " + std::to_string(acting_player + 1) + " made an impossible move."};
            bool result =  move({player, direction});
            if (!result) {
                return {false, log + "\nplayer " + std::to_string(acting_player + 1) + " made an impossible move."};
            }
            if (firstPlayerVictory()) {
                return {false, log + "\nplayer " + std::to_string(1) + " won."};
            }
            if (secondPlayerVictory()) {
                return {false, log + "\nplayer " + std::to_string(2) + " won."};
            }
            if (draw()) {
                return {false, log + "\nDraw."};
            }
            return {true, log};
        }
    }
};


#endif //TESTER_QUORIDOR_HPP
