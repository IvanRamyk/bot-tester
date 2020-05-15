//
// Created by ivan on 16.05.20.
//
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <vector>

using namespace std;

int x_, y_;

struct partition {
    int x;
    int y;
    bool up; // 0 - right
};

bool operator <(partition a, partition b) {
    if (a.x != b.x)
        return a.x < b.x;
    if (a.y != b.y)
        return a.y < b.y;
    return a.up;
}

set <partition> s;
int number, x, y;

pair <int, int> bfs() {
    map <pair<int, int>, pair<int, int>> dist;
    queue <pair<int, int>> q;
    if (number == 1) {
        for (int i = 1; i <= 9; ++i) {
            q.push({9, i});
            dist[{9, i}] = {-1,-1};
        }
    }
    else {
        for (int i = 1; i <= 9; ++i) {
            q.push({1, i});
            dist[{1, i}] = {-1, -1};
        }
    }
    while (!q.empty()) {
        pair <int, int> cur = q.front();
        q.pop();
        int cur_x = cur.first;
        int cur_y = cur.second;
        vector <pair <int, int>> available;
        if (cur_x > 1 && !dist.count({cur_x - 1, cur_y}) && !s.count({cur_x, cur_y, 0}))
            available.push_back({cur_x - 1, cur_y});
        if (cur_x < 9 && !dist.count({cur_x + 1, cur_y}) && !s.count({cur_x + 1, cur_y, 0}))
            available.push_back({cur_x + 1, cur_y});
        if (cur_y > 1 && !dist.count({cur_x, cur_y - 1}) && !s.count({cur_x, cur_y, 1}))
            available.push_back({cur_x , cur_y - 1});
        if (cur_y < 9 && !dist.count({cur_x, cur_y + 1}) && !s.count({cur_x, cur_y + 1, 1}))
            available.push_back({cur_x, cur_y + 1});
        for (auto i : available) {
            dist[i] = cur;
            q.push(i);
        }
    }
    return dist[{x, y}];
}

void add_partition(int x1, int y1,int x2, int y2) {
    if (x1 > x2)
        swap(x1, x2);
    if (y1 > y2)
        swap(y1, y2);
    if (y1 == y2) {
        s.insert({x1, y1, 1});
        s.insert({x1 + 1, y1, 1});
    }
    else {
        s.insert({x1, y1, 0});
        s.insert({x1, y1 + 1, 0});
    }
}

bool get_opponent_move() {
    string type;
    cin >> type;
    if (type == "over") {
        return false;
    }
    if (type == "move") {
        cin >> x_ >> y_;
    }
    else  {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        add_partition(x1,y1,x2,y2);
    }
    return true;
}

std::string set_partition()  {
    if (number == 1) {
        add_partition(8,5,8,7);
        return "partition 8 5 8 7";
    }
    return "";
}

int main() {
    cin >> number;
    if (number == 1) {
        x = 1;
        y = 5;
        cout << set_partition() << "\n";
        get_opponent_move();
    }
    else  {
        x = 9;
        y = 5;
        get_opponent_move();
    }
    while (true) {
        auto move = bfs();
        if (move.first == x_ && move.second == y_)  {
            move.first = x + (move.first - x) * 2;
            move.second = y + (move.second - y) * 2;
        }
        x = move.first;
        y = move.second;
        cout << "move " << move.first << " " << move.second << "\n";
        if (!get_opponent_move())
            break;
    }
    return 0;
}