#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <chrono>

using std::vector;
using byte = std::int16_t;
using std::string;
using std::to_string;
using std::cout;
using std::cin;
using std::endl;

template<class T>
T& min(T& lhs, T& rhs) {
	return (lhs < rhs ? lhs : rhs);
}

struct point {
	byte x;
	byte y;

	point up() const {
		return point{ x + 1, y };
	}

	point down() const {
		return point{ x - 1, y };
	}

	point left() const {
		return point{ x, y - 1 };
	}

	point right() const {
		return point{ x, y + 1 };
	}

	bool is_ok() const {
		if (x <= 0 || y <= 0) {
			return false;
		}
		if (x > 9 || y > 9) {
			return false;
		}
		return true;
	}

	point& operator=(const point& pnt) {
		this->x = pnt.x;
		this->y = pnt.y;
		return *this;
	}

	string to_string() {
		return std::to_string(x) + " " + std::to_string(y);
	}
};

std::ostream& operator<<(std::ostream& out, const point& pnt) {
	out << "(" << pnt.x << ", " << pnt.y << ")";
	return out;
}

bool operator==(const point& lhs, const point& rhs) {
	return(lhs.x == rhs.x && lhs.y == rhs.y);
}

struct border {
	point beg;
	point end;

	string to_string() {
		return beg.to_string() + " " + end.to_string();
	}
};

struct BoardState {
	vector<border> borders;
	point first_player{ 1, 5 };
	point second_player{ 9, 5 };

	bool is_valid_border(const border& bord) {
		for (const border& b : borders) {
			if (bord.beg == b.beg && bord.end == b.end)
				return false;

			if (bord.beg == point{ b.beg.x - 1, b.beg.y - 1 } && bord.end == point{ b.end.x + 1 , b.end.y + 1 })
				return false;
			else if (bord.beg == point{ b.end.x - 1, b.end.y - 1 } && bord.end == point{ b.beg.x + 1 , b.beg.y + 1 })
				return false;
			else if (bord.beg.x == bord.end.x && b.beg.x == b.end.x && (b.beg == bord.beg.right() || b.end == bord.beg.right()))
				return false;
			else if (bord.beg.y == bord.end.y && b.beg.y == b.end.y && (b.beg == bord.beg.up() || b.end == bord.beg.up()))
				return false;
		}
		return true;
	}
};

using way = vector<point>;

bool is_neighbours(const point& first, const point& second, const vector<border>& borders) {
	byte dx{ 0 }, dy{ 0 };
	for (const border& bord : borders) {
		dx = bord.end.x - bord.beg.x;
		dy = bord.end.y - bord.beg.y;
		if (dx == 0 && dy == 2) {
			if (first.down() == second && (first == bord.beg || first == bord.beg.right()))
				return false;
			else if (second.down() == first && (second == bord.beg || second == bord.beg.right()))
				return false;
		}
		else if (dx == 0 && dy == -2) {
			if (first.down() == second && (first == bord.end || first == bord.end.right()))
				return false;
			else if (second.down() == first && (second == bord.end || second == bord.end.right()))
				return false;
		}
		else if (dx == 2 && dy == 0) {
			if (first.left() == second && (first == bord.beg || first == bord.beg.up()))
				return false;
			else if (second.left() == first && (second == bord.beg || second == bord.beg.up()))
				return false;
		}
		else if (dx == -2 && dy == 0) {
			if (first.left() == second && (first == bord.end || first == bord.end.up()))
				return false;
			else if (second.left() == first && (second == bord.end || second == bord.end.up()))
				return false;
		}
	}
	return true;
}

vector<point> find_finish(const byte(&map)[9][9], const byte& player_number) {
	byte x = (player_number == 1 ? 8 : 0);

	vector<point> res{ {x + 1, 1} };
	byte min_val = map[x][0];

	for (byte y = 1; y < 9; y++) {
		if ((map[x][y] < min_val && map[x][y] != 0) || min_val == 0) {
			res = vector<point>{ {x + 1, y + 1} };
			min_val = map[x][y];
		}
		else if (map[x][y] == min_val)
			res.push_back(point{ x + 1, y + 1 });
	}
	return res;
}

vector<way> back_way(const point& now, const byte(&map)[9][9], const vector<border>& borders) {
	if (map[now.x - 1][now.y - 1] == 1)
		return { {{now}} };
	vector<way> result;

	auto now_value = map[now.x - 1][now.y - 1];

	for (const auto& my_point : { now.up(), now.down(), now.left(), now.right() }) {
		auto my_value = map[my_point.x - 1][my_point.y - 1];
		if (my_point.is_ok() && my_value == now_value - 1 && is_neighbours(my_point, now, borders)) {
			for (auto& my_way : back_way(my_point, map, borders)) {
				my_way.push_back(now);
				result.push_back(my_way);
			}
		}
	}
	return result;
}

vector<way> shortest_ways(const vector<border>& borders, const point& player_position, const byte& player_number) {
	byte field[9][9] = { 0 };
	field[player_position.x - 1][player_position.y - 1] = 1;

	vector<point> search{ player_position };
	byte current = 0;

	while (current < search.size())
	{
		point now = search[current++];
		for (const auto& next : { now.up(), now.down(), now.left(), now.right() }) {
			if (field[next.x - 1][next.y - 1] == 0 && next.is_ok() && is_neighbours(now, next, borders)) {
				search.push_back(next);
				field[next.x - 1][next.y - 1] = field[now.x - 1][now.y - 1] + 1;
			}
		}
	}

	vector<way> ways;
	for (auto& finish : find_finish(field, player_number)) {
		way my_way{ finish };
		for (const auto& my_way : back_way(finish, field, borders)) {
			ways.push_back(my_way);
		}
	}
	return ways;
}

byte benefit(const way& player_1, const way& player_2, const byte& player_number) {
	byte size_1 = byte(player_1.size());
	byte size_2 = byte(player_2.size());
	for (byte i = 0, my_min = min(size_1, size_2) - 1; i < my_min; i++) {
		if (player_1[i + 1] == player_2[i + 1]) {
			if (player_number == 1)
				size_2--;
			else
				size_1--;
			break;
		}
		else if (player_number == 1 && player_1[i + 1] == player_2[i]) {
			size_1--;
			break;
		}
		else if (player_number == 2 && player_2[i + 1] == player_1[i]) {
			size_2--;
			break;
		}
	}
	return size_1 - size_2;
}

string our_move(BoardState& board_state, int player_number, byte& border_count) {
	way best_way;
	byte max_benefit = INT16_MIN;
	for (const way& w1 : shortest_ways(board_state.borders, board_state.first_player, 1))
		for (const way& w2 : shortest_ways(board_state.borders, board_state.second_player, 2)) {
			byte cur_benefit = (player_number == 1 ? -1 : 1) * benefit(w1, w2, player_number);
			if (cur_benefit > max_benefit) {
				max_benefit = cur_benefit;
				best_way = (player_number == 1 ? w1 : w2);
			}
		}

	if (max_benefit < 0 && border_count < 10) {
		border best_border;
		byte new_max_benefit = INT16_MIN;

		vector<border> temp = board_state.borders;
		for (byte delta = 0; delta < 2; delta++)
			for (byte i = 1; i <= 8; i++)
				for (byte j = 2; j <= 9; j++) {
					border bord = (delta == 0 ? border{ i, j, i + 2, j } : border{ j, i, j, i + 2 });
					if (board_state.is_valid_border(bord)) {
						temp.push_back(bord);
						for (const way& w1 : shortest_ways(temp, board_state.first_player, 1))
							for (const way& w2 : shortest_ways(temp, board_state.second_player, 2)) {
								byte cur_benefit = (player_number == 1 ? -1 : 1) * benefit(w1, w2, player_number);
								if (cur_benefit > new_max_benefit) {
									new_max_benefit = cur_benefit;
									best_border = bord;
								}
							}
						temp.pop_back();
					}
				}
		if (new_max_benefit > max_benefit) {
			board_state.borders.push_back(best_border);
			border_count++;
			return "partition " + best_border.to_string();
		}
	}

	point& opponent = (player_number == 1 ? board_state.second_player : board_state.first_player);
	point& me = (player_number == 1 ? board_state.first_player : board_state.second_player);
	point& new_point = (best_way[1] == opponent ? best_way[2] : best_way[1]);
	me = new_point;
	return "move " + me.to_string();


}

void round(BoardState& board_state, const byte& player_number, byte& border_count)
{
	if (player_number == 1) {
		cout << our_move(board_state, player_number, border_count) << endl;
	}

	string opponents_move;
	cin >> opponents_move;

	if (opponents_move == "move") {
		byte x, y;
		cin >> x >> y;

		if (player_number == 1)
			board_state.second_player = point{ x, y };
		else
			board_state.first_player = point{ x, y };
	}
	else {
		byte x1, y1, x2, y2;
		cin >> x1 >> y1 >> x2 >> y2;

		board_state.borders.push_back(border{ x1, y1, x2, y2 });
	}

	if (player_number == 2) {
		cout << our_move(board_state, player_number, border_count) << endl;
	}
}

int main()
{
	byte player_number = 1;
	byte border_count = 0;

	BoardState board_state;

	cin >> player_number;
	while (true) {
		round(board_state, player_number, border_count);
	}

}