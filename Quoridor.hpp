//
// Created by ivan on 01.05.20.
//

#ifndef TESTER_QUORIDOR_HPP
#define TESTER_QUORIDOR_HPP

#include <set>
using std::set;

enum Direction {
    Left, Right, Up, Down
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
    bool operator < ( Partition b) {
        Partition a = *this;
        if (a.direction != b.direction)
            return a.direction < b.direction;
        if (a.start_point.x != b.start_point.x)
            return a.start_point.x < b.start_point.x;
        return a.start_point.y < b.start_point.y;

    }
};



struct Move {
    int player_number;
    Direction direction;
};

class Quoridor {
private:
    Player player1, player2;
    int acting_player;
    set<Partition> partitions;// with length 1, direction only Right or Up (for more strict definition)
    Partition _downToUp(Partition p) {
        int x = p.start_point.x - 2;
        int y = p.start_point.y;
        return {{x, y}, Up};
    }
    Partition _leftToRight(Partition p) {
        int x = p.start_point.x;
        int y = p.start_point.y - 2;
        return {{x, y}, Right};
    }
    std::pair<Partition, Partition> _splitPartition(Partition partition) {
        if (partition.direction == Left)
            partition = _leftToRight(partition);
        if (partition.direction == Down)
            partition = _downToUp(partition);
        int x1, x2 = partition.start_point.x; x1 = x2;
        int y1, y2 = partition.start_point.y; y1 = y2;
        if (partition.direction == Up)
            x2 += 2;
        if (partition.direction == Right)
            y2 += 2;
        return {Partition({x1, y1}, partition.direction),
                Partition({(x1 + x2) / 2, (y1 + y2) / 2}, partition.direction)};
    }
public:
    Quoridor() {
        player1.position = {1, 5};
        player2.position = {9, 5};
        acting_player = 1;
    }

    bool first_player_victory() {
        return player1.position.x == 9;
    }

    bool second_player_victory() {
        return player2.position.x == 1;
    }

    bool move(Move) {

    }

    bool setPartition(Partition partition) {
        std::pair<Partition, Partition> pars = _splitPartition(partition);
        partitions.insert(pars.first);
        partitions.insert(pars.second);
    }
};


#endif //TESTER_QUORIDOR_HPP
