//
// Created by ivan on 14.05.20.
//
#include <iostream>

using namespace std;

bool get_opponent_move() {
    string type;
    cin >> type;
    if (type == "over") {
        return false;
    }
    if (type == "move") {
        int x, y;
        cin >> x >> y;
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
            x++;
            cout << "move " << x << " " << y << "\n";
        }
        if (number == 2) {
            x--;
            cout << "move " << x << " "  << y << "\n";
        }
        if (!get_opponent_move())
            break;
    }
    return 0;
}