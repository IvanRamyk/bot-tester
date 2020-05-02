//
// Created by ivan on 01.05.20.
//

#ifndef TESTER_QUORIDOR_HPP
#define TESTER_QUORIDOR_HPP

#include <set>
#include <map>
#include <iostream>
using std::set;
using std::map;

enum Direction {
    Left, Right, Up, Down, UpLeft, UpRight, DownLeft, DownRight
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
    Player players[2]{};
    unsigned  int acting_player;
    map<Partition, int> partitions;// with length 1, direction only Right or Up (for more strict definition)
    static Partition _downToUp(Partition p) {
        int x = p.start_point.x - 2;
        int y = p.start_point.y;
        return {{x, y}, Up};
    }
    static Partition _leftToRight(Partition p) {
        int x = p.start_point.x;
        int y = p.start_point.y - 2;
        return {{x, y}, Right};
    }
    static std::pair<Partition, Partition> _splitPartition(Partition partition) {
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
    void _passMove() {
        acting_player ^= unsigned (1);
    }
    static bool _onField(Partition partition) {
        int x = partition.start_point.x;
        int y = partition.start_point.y;
        if (partition.direction == Right) {
            if (y < 1 || y > 9)
                return false;
            if (x < 2 || x > 9)
                return false;
        }
        if (partition.direction == Up) {
            if (x < 1 || x > 9)
                return false;
            if (y < 2 || y > 9)
                return false;
        }
        return true;
    }
    bool _moveUp(int x, int y) {
        return  x + 1 <= 9 && partitions.count({{x + 1, y}, Right}) == 0;
    }
    bool _moveDown(int x, int y) {
        return x - 1 >= 1 && partitions.count({{x , y}, Right}) == 0;
    }
    bool _moveRight(int x, int y) {
        return y + 1 <= 9 && partitions.count({{x, y + 1}, Up}) == 0;
    }
    bool _moveLeft(int x, int y) {
        return y - 1 >= 1 && partitions.count({{x, y}, Up}) == 0;
    }
    bool _moveUpLeft(int x, int y) {
        bool can = false;
        if (_diff_x() == 1 && _diff_y() == 0 && _moveUp(x, y) && !_moveUp(x + 1, y) && _moveLeft(x + 1, y))// opponent higher
            can = true;
        if (_diff_x() == 0 && _diff_y() == -1 && _moveLeft(x, y) && !_moveLeft(x, y - 1) && _moveUp(x, y - 1))
            can = true;
        return can;
    }
    bool _moveUpRight(int x, int y) {
        bool can = false;
        if (_diff_x() == 1 && _diff_y() == 0 && _moveUp(x, y) && !_moveUp(x + 1, y) && _moveRight(x + 1, y))// opponent higher
            can = true;
        if (_diff_x() == 0 && _diff_y() == 1 && _moveRight(x, y) && !_moveRight(x, y + 1) && _moveUp(x, y + 1))
            can = true;
        return can;
    }
    bool _moveDownRight(int x, int y) {
        bool can = false;
        if (_diff_x() == -1 && _diff_y() == 0 && _moveDown(x, y) && !_moveDown(x - 1, y) && _moveRight(x - 1, y))// opponent lower
            can = true;
        if (_diff_x() == 0 && _diff_y() == 1 && _moveRight(x, y) && !_moveRight(x, y + 1) && _moveDown(x, y + 1))
            can = true;
        return can;
    }
    bool _moveDownLeft(int x, int y) {
        bool can = false;
        if (_diff_x() == -1 && _diff_y() == 0 && _moveDown(x, y) && !_moveDown(x - 1, y) && _moveLeft(x - 1, y))// opponent lower
            can = true;
        if (_diff_x() == 0 && _diff_y() == -1 && _moveLeft(x, y) && !_moveLeft(x, y - 1) && _moveDown(x, y - 1))
            can = true;
        return can;
    }
    int _diff_x() {
        return players[acting_player ^ unsigned(1)].position.x - players[acting_player].position.x;
    }
    int _diff_y() {
        return players[acting_player ^ unsigned(1)].position.y - players[acting_player].position.y;
    }
    bool _check_path(int x, int y, int target, set<std::pair<int, int>> &used) {
        if (x == target)
            return true;
        used.insert({x, y});
        if (_moveRight(x, y) && !used.count({x, y + 1}))
            if (_check_path(x, y + 1, target, used))
                return true;
        if (_moveLeft(x, y) && !used.count({x, y - 1}))
            if (_check_path(x, y - 1, target, used))
                return true;
        if (_moveUp(x, y) && !used.count({x + 1, y}))
            if (_check_path(x + 1, y, target, used))
                return true;
        if (_moveDown(x, y) && !used.count({x - 1, y}))
            if (_check_path(x - 1, y, target, used))
                return true;
        return false;
    }

    bool _check_path() {
        set <std::pair<int, int>> used;
        bool res =  _check_path(players[0].position.x, players[0].position.y, 9, used);
        used.clear();
        return res && _check_path(players[1].position.x, players[1].position.y, 1, used);
    }
public:
    Quoridor() {
        players[0].position = {1, 5};
        players[1].position = {9, 5};
        acting_player = 0;
    }

    bool first_player_victory() {
        return players[0].position.x == 9;
    }

    bool second_player_victory() {
        return players[1].position.x == 1;
    }

    bool move(Move move) {
        if (first_player_victory() || second_player_victory())
            return false;
        if (move.player_number != acting_player)
            return false;
        bool move_committed = false;
        int x = players[acting_player].position.x;
        int y = players[acting_player].position.y;
        if (move.direction == Up &&  _moveUp(x, y)) {
            if (_diff_y() == 0 && _diff_x() == 1) {
                if (_moveUp(x + 1, y)) {
                    players[acting_player].position.x += 2;
                    move_committed = true;
                }
            }
            else {
                players[acting_player].position.x++;
                move_committed = true;
            }
        }
        if (move.direction == Down && _moveDown(x, y)) {
            if (_diff_y() == 0 && _diff_x() == -1) {
                if (_moveUp(x - 1, y)) {
                    players[acting_player].position.x -= 2;
                    move_committed = true;
                }
            }
            else {
                players[acting_player].position.x--;
                move_committed = true;
            }
        }
        if (move.direction == Right && _moveRight(x, y)) {
            players[acting_player].position.y++;
            move_committed = true;
        }
        if (move.direction == Left && _moveLeft(x, y)) {
            players[acting_player].position.y--;
            move_committed = true;
        }
        if (move.direction == UpLeft && _moveUpLeft(x, y)) {
            players[acting_player].position.y--;
            players[acting_player].position.x++;
            move_committed = true;
        }
        if (move.direction == UpRight && _moveUpRight(x, y)) {
            players[acting_player].position.y++;
            players[acting_player].position.x++;
            move_committed = true;
        }
        if (move.direction == DownRight && _moveDownRight(x, y)) {
            players[acting_player].position.y++;
            players[acting_player].position.x--;
            move_committed = true;
        }
        if (move.direction == DownLeft && _moveDownLeft(x, y)) {
            players[acting_player].position.y--;
            players[acting_player].position.x--;
            move_committed = true;
        }
        if (!move_committed)
            return false;
        _passMove();
        return true;
    }

    bool setPartition(Partition partition) {
        if (first_player_victory() || second_player_victory())
            return false;
        std::pair<Partition, Partition> pars = _splitPartition(partition);
        if (!_onField(pars.first) || !_onField(pars.second))
            return false;
        int x_center = pars.second.start_point.x;
        int y_center = pars.second.start_point.y;
        if (pars.first.direction == Up &&
                partitions.count({{x_center, y_center - 1}, Right})
                && partitions.count({{x_center, y_center}, Right})
                && partitions[{{x_center, y_center - 1}, Right}] == partitions[{{x_center, y_center}, Right}])
                return false;
        if (pars.first.direction == Right &&
            partitions.count({{x_center - 1, y_center}, Up})
            && partitions.count({{x_center, y_center}, Up})
            && partitions[{{x_center - 1, y_center}, Up}] == partitions[{{x_center, y_center}, Up}])
            return false;
        if (partitions.count(pars.first) || partitions.count(pars.second))
            return false;
        auto temp = partitions;
        partitions.insert({pars.first, partitions.size()});
        partitions.insert({pars.second, partitions.size() - 1});
        if (!_check_path()) {
           partitions = temp;
           return false;
        }
        _passMove();
        return true;
    }
};


#endif //TESTER_QUORIDOR_HPP
