//
// Created by ivan on 01.05.20.
//

#include "Quoridor.hpp"

Partition Quoridor::_downToUp(Partition p) {
    int x = p.start_point.x - 2;
    int y = p.start_point.y;
    return {{x, y}, Up};
}

Partition Quoridor::_leftToRight(Partition p) {
    int x = p.start_point.x;
    int y = p.start_point.y - 2;
    return {{x, y}, Right};
}

std::pair<Partition, Partition> Quoridor::_splitPartition(Partition partition) {
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

void Quoridor::_passMove() {
    acting_player = (acting_player + unsigned (1)) % player_number;
}

bool Quoridor::_onField(Partition partition) {
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

bool Quoridor::_moveUp(int x, int y) {
    return  x + 1 <= 9 && partitions.count({{x + 1, y}, Right}) == 0;
}

bool Quoridor::_moveDown(int x, int y) {
    return x - 1 >= 1 && partitions.count({{x , y}, Right}) == 0;
}

bool Quoridor::_moveRight(int x, int y) {
    return y + 1 <= 9 && partitions.count({{x, y + 1}, Up}) == 0;
}

bool Quoridor::_moveLeft(int x, int y) {
    return y - 1 >= 1 && partitions.count({{x, y}, Up}) == 0;
}

bool Quoridor::_moveUpLeft(int x, int y) {
    bool can = false;
    if (_diffX() == 1 && _diffY() == 0 && _moveUp(x, y) && !_moveUp(x + 1, y) && _moveLeft(x + 1, y))// opponent higher
        can = true;
    if (_diffX() == 0 && _diffY() == -1 && _moveLeft(x, y) && !_moveLeft(x, y - 1) && _moveUp(x, y - 1))
        can = true;
    return can;
}

bool Quoridor::_moveUpRight(int x, int y) {
    bool can = false;
    if (_diffX() == 1 && _diffY() == 0 && _moveUp(x, y) && !_moveUp(x + 1, y) && _moveRight(x + 1, y))// opponent higher
        can = true;
    if (_diffX() == 0 && _diffY() == 1 && _moveRight(x, y) && !_moveRight(x, y + 1) && _moveUp(x, y + 1))
        can = true;
    return can;
}

bool Quoridor::_moveDownRight(int x, int y) {
    bool can = false;
    if (_diffX() == -1 && _diffY() == 0 && _moveDown(x, y) && !_moveDown(x - 1, y) && _moveRight(x - 1, y))// opponent lower
        can = true;
    if (_diffX() == 0 && _diffY() == 1 && _moveRight(x, y) && !_moveRight(x, y + 1) && _moveDown(x, y + 1))
        can = true;
    return can;
}

bool Quoridor::_moveDownLeft(int x, int y) {
    bool can = false;
    if (_diffX() == -1 && _diffY() == 0 && _moveDown(x, y) && !_moveDown(x - 1, y) && _moveLeft(x - 1, y))// opponent lower
        can = true;
    if (_diffX() == 0 && _diffY() == -1 && _moveLeft(x, y) && !_moveLeft(x, y - 1) && _moveDown(x, y - 1))
        can = true;
    return can;
}

int Quoridor::_diffX() {
    return players[acting_player ^ unsigned(1)].position.x - players[acting_player].position.x;
}

int Quoridor::_diffY() {
    return players[acting_player ^ unsigned(1)].position.y - players[acting_player].position.y;
}

bool Quoridor::_checkPath(int x, int y, int target, set<std::pair<int, int>> &used) {
    if (x == target)
        return true;
    used.insert({x, y});
    if (_moveRight(x, y) && !used.count({x, y + 1}))
        if (_checkPath(x, y + 1, target, used))
            return true;
    if (_moveLeft(x, y) && !used.count({x, y - 1}))
        if (_checkPath(x, y - 1, target, used))
            return true;
    if (_moveUp(x, y) && !used.count({x + 1, y}))
        if (_checkPath(x + 1, y, target, used))
            return true;
    if (_moveDown(x, y) && !used.count({x - 1, y}))
        if (_checkPath(x - 1, y, target, used))
            return true;
    return false;
}

bool Quoridor::_checkPath() {
    set <std::pair<int, int>> used;
    bool res = _checkPath(players[0].position.x, players[0].position.y, 9, used);
    used.clear();
    return res && _checkPath(players[1].position.x, players[1].position.y, 1, used);
}

Quoridor::Quoridor(int cnt_players) {
    cnt_moves = 0;
    if (cnt_players != 2 && cnt_players != 4)
        throw std::invalid_argument("Only games with 2 or 4 players are possible to create");

    player_number = cnt_players;
    players[0].position = {1, 5};
    players[1].position = {9, 5};
    used_partitions[0] = 0;
    used_partitions[1] = 0;
    if (cnt_players == 4) {
        players[2].position = {5, 1};
        players[3].position = {5, 9};
        used_partitions[2] = 0;
        used_partitions[3] = 0;
    }
    acting_player = 0;
}

bool Quoridor::draw() {
    return cnt_moves >= 50;
}

bool Quoridor::firstPlayerVictory() {
    return players[0].position.x == 9;
}

bool Quoridor::secondPlayerVictory() {
    return players[1].position.x == 1;
}

bool Quoridor::move(Move move) {
    if (firstPlayerVictory() || secondPlayerVictory() || draw()) {
        return false;
    }
    if (move.player_number != acting_player)
        return false;
    bool move_committed = false;
    int x = players[acting_player].position.x;
    int y = players[acting_player].position.y;
    if (move.direction == Up &&  _moveUp(x, y)) {
        players[acting_player].position.x++;
        move_committed = true;
    }
    if (move.direction == UpUp && _moveUp(x, y)) {
        if (_diffY() == 0 && _diffX() == 1) {
            if (_moveUp(x + 1, y)) {
                players[acting_player].position.x += 2;
                move_committed = true;
            }
        }
    }
    if (move.direction == Down && _moveDown(x, y)) {
        players[acting_player].position.x--;
        move_committed = true;
    }
    if (move.direction == DownDown && _moveDown(x, y)) {
        if (_diffY() == 0 && _diffX() == -1) {
            if (_moveDown(x - 1, y)) {
                players[acting_player].position.x -= 2;
                move_committed = true;
            }
        }
    }
    if (move.direction == Right && _moveRight(x, y)) {
        players[acting_player].position.y++;
        move_committed = true;
    }
    if (move.direction == RightRight && _moveRight(x, y)) {
        if (_diffY() == 1 && _diffX() == 0) {
            if (_moveDown(x, y + 1)) {
                players[acting_player].position.y += 2;
                move_committed = true;
            }
        }
    }
    if (move.direction == Left && _moveLeft(x, y)) {
        players[acting_player].position.y++;
        move_committed = true;
    }
    if (move.direction == LeftLeft && _moveLeft(x, y)) {
        if (_diffY() == -1 && _diffX() == 0) {
            if (_moveDown(x, y - 1)) {
                players[acting_player].position.y -= 2;
                move_committed = true;
            }
        }
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
    ++cnt_moves;
    return true;
}

bool Quoridor::setPartition(Partition partition) {
    if (firstPlayerVictory() || secondPlayerVictory() || draw())
        return false;
    if (used_partitions[acting_player] == 10)
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
    if (!_checkPath()) {
        partitions = temp;
        return false;
    }
    cnt_moves = 0;
    used_partitions[acting_player]++;
    _passMove();
    return true;
}

Direction Quoridor::_diffToDirection(int dx, int dy) {
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

bool Quoridor::_checkFormat(const std::string &command) {
    std::string buffer, type;
    int cnt_params = 0;
    std::stringstream stream(command);
    while (stream >> buffer) {
        if (cnt_params == 0) {
            type = buffer;
            if (buffer != "move" && buffer != "partition")
                return false;
            cnt_params = 1;
        }
        else if (cnt_params == 1 || cnt_params == 2 || cnt_params == 3 || cnt_params == 4) {
            for (char i : buffer)
                if (!std::isdigit(i))
                    return false;
            cnt_params++;
        }
        else
            return false;
    }
    return ((type == "move" && cnt_params == 3) || (type == "partition" && cnt_params == 5));
}

int Quoridor::winner() {
    if (firstPlayerVictory())
        return 1;
    if (secondPlayerVictory())
        return 2;
    return 0;
}

std::pair<bool, std::string> Quoridor::makeMove(const std::string &_move) {
    std::string log = _move;
    if (!_checkFormat(_move)) {
        return  {false, "incorrect format of move"};
    }
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
    else  {
        int x1, y1, x2, y2;
        stream >> x1 >> y1 >> x2 >> y2;
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        if (y1 > y2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        int dx = x2 - x1;
        int dy = y2 - y1;
        Partition partition{};
        partition.start_point = {x1, x2};
        if (dy == 0 && dx == 2)
            partition.direction = Up;
        else if (dy == 2 && dx == 0)
            partition.direction = Right;
        else
            return {false, log + "\nplayer " + std::to_string(acting_player + 1) + " made an impossible move."};
        bool res = setPartition(partition);
        if (!res) {
            return {false, log + "\nplayer " + std::to_string(acting_player + 1) + " made an impossible move."};
        }
        return {true, log};
    }
}

bool operator<(Partition a, Partition b) {
    if (a.direction != b.direction)
        return a.direction < b.direction;
    if (a.start_point.x != b.start_point.x)
        return a.start_point.x < b.start_point.x;
    return a.start_point.y < b.start_point.y;

}
