//
// Created by ivan on 16.05.20.
//
#include <iostream>

using namespace std;
int x_, y_;

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
    }
    return true;
}

int main() {
    int number, x, y;
    cin >> number;
    if (number == 1) {
        x = 1;
        y = 5;
    }
    else  {
        get_opponent_move();
        x = 9;
        y = 5;
    }
    while (true) {
        if (number == 1) {
            if (x + 1 == x_ && y == y_)
                x+=2;
            else
                x++;
            cout << "move " << x << " " << y << "\n";
        }
        if (number == 2) {
            if (x - 1 == x_ && y == y_)
                x-=2;
            else
                x--;
            cout << "move " << x << " " << y << "\n";
        }
        if (!get_opponent_move())
            break;
    }
    return 0;
}