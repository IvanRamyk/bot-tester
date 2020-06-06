#include <bits/stdc++.h>
#define all(x) (x).begin(), (x).end()
#define DBG cerr << __LINE__ << ": @" << __FUNCTION__ << "  "
#define VAR(x) #x << "=" << x << " "

using namespace std;
typedef pair<int, int> pii;
typedef vector<vector<int>> Graph;

pii to_coord(int v) { return {v / 9, v % 9}; }

int from_coord(pii c) { return c.first * 9 + c.second; }

pii operator+(pii a, pii b) { return {a.first + b.first, a.second + b.second}; }
bool operator==(pii a, pii b) { return a.first == b.first && a.second == b.second; }


Graph graph;
int player1, player2;
int wall1 = 8, wall2 = 0;
int our_walls = 10;
pii neighbors[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
pii diag[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

bool wall[9][9][2] = {}; // 0 is horizontal, 1 is vertical
set<pii> to_build;

bool in_bounds(pii coord) {
    return coord.first >= 0 && coord.second >= 0 && coord.first < 9 && coord.second < 9;
}

bool in_bounds(int pos) {
    return in_bounds(to_coord(pos));
}

void normalize(int& i1, int& j1, int& i2, int& j2) {
    if (i1 > i2) swap(i1, i2);
    if (j1 > j2) swap(j1, j2);
}

void display_to_build() {
    for (pii coord : to_build) {
        cout << "{" << coord.first << ", " << coord.second << "}, ";
    }
    cout << '\n';
}

void display_graph() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            pii coord_from = {i, j};
            cout << "(" << coord_from.first << ", " << coord_from.second << ") : {";
            for (int u : graph[from_coord(coord_from)]) {
                pii coord_to = to_coord(u);
                cout << "(" << coord_to.first<< ", " << coord_to.second << "), ";
            }
            cout << "}\n";
        }
    }
}

bool no_edge(pii from, pii to) {
    // cout << from.first << " " << from.second << " " << to.first << " " << to.second << endl;
    int u = from_coord(from), v = from_coord(to);
    return find(all(graph[u]), v) == graph[u].end();
}

void display_walls() {
    cout << ' ';
    for (int i = 0; i < 18; i++)
        cout << (i % 2 ? to_string(i /2 + 1) : " ");
    cout << '\n';
    for (int i = 18; i >= 0; i--) {
        cout << (i % 2 ? to_string(i /2 + 1) : " ");
        for (int j = 0; j < 19; j++) {
            // cout << i << " " << j << endl;
            if (i % 2) {
                if (j % 2) {
                    if (pii{i/2, j/2} == to_coord(player1)) {
                        cout << "1";
                    } else if (pii{i/2, j/2} == to_coord(player2)) {
                        cout << "2";
                    } else {
                        cout << " ";
                    }
                } else {
                    bool is_wall = j == 0 || j == 18 || no_edge({i/2, j/2}, {i/2, j/2 - 1});
                    cout << (is_wall ? "|" : " ");
                }
            } else {
                if (j % 2) {
                    bool is_wall = i == 0 || i == 18 || no_edge({i/2, j/2}, {i/2 - 1, j/2});
                    cout << (is_wall ? "-" : " ");
                } else {
                    cout << "·";
                }
            }
        }
        cout << '\n';
    }
}

void init_game() {
    player1 = from_coord({0, 4});
    player2 = from_coord({8, 4});
    // init graph
    graph.resize(81);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (const pii &inc : neighbors) {
                pii cell = pii(i, j) + inc;
                if (cell.first < 0 || cell.second < 0 || cell.first > 8 ||
                    cell.second > 8)
                    continue;
                int u = from_coord({i, j});
                int v = from_coord(cell);
                graph[u].push_back(v);
                //graph[v].push_back(u);
            }
        }
    }
    // init to_build
    to_build.insert({8, 4});
    to_build.insert({8, 5});
}

int shortest_path(int player, int wall) {
    queue<int> q;
    q.push(player);
    int dist[81];
    fill(dist, dist + 81, INT_MAX);
    dist[player] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : graph[u]) {
            if (dist[v] != INT_MAX)
                continue;
            dist[v] = dist[u] + 1;
            if (to_coord(v).first == wall)
                return dist[v];
            q.push(v);
        }
    }
    return INT_MAX;
}

int step(int player, int wall) {
    queue<int> q;
    q.push(player);
    int dist[81];
    int parents[81];
    fill(dist, dist + 81, INT_MAX);
    fill(parents, parents + 81, -1);
    dist[player] = 0;
    int end;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : graph[u]) {
            if (dist[v] != INT_MAX)
                continue;
            dist[v] = dist[u] + 1;
            parents[v] = u;
            if (to_coord(v).first == wall) {
                end = v;
                goto endwhile;
            }
            q.push(v);
        }
    }
endwhile:
    // cout << "DIST: \n";
    // for(int i = 8;i  >= 0;i--){
    //     for(int j = 0;j < 9;j++){
    //         cout << dist[from_coord({i, j})] << ' ';
    //     }
    //     cout << '\n';
    // }
    // cout << "PARENTS: \n";
    // for(int i = 8;i  >= 0;i--){
    //     for(int j = 0;j < 9;j++){
    //         cout << parents[from_coord({i, j})] << ' ';
    //     }
    //     cout << '\n';
    // }
    for (int u = end; u != player; u = parents[u]) {
        if (parents[u] == player)
            return u;
    }
}

void remove_edge(pii coord1, pii coord2) {
    int u = from_coord(coord1), v = from_coord(coord2);
    graph[u].erase(find(all(graph[u]), v));
    graph[v].erase(find(all(graph[v]), u));
}

void add_edge(pii coord1, pii coord2){
    int u = from_coord(coord1), v = from_coord(coord2);
    graph[v].push_back(u);
    graph[u].push_back(v);
}

void add_to_build(int i, int j) {
    // DBG << '\n';
    for (auto step : diag) {
        pii p = pii{i, j} + step;
        if (in_bounds(p)) to_build.insert(p);
    }
    for (auto step : neighbors) {
        pii p = pii{i, j} + step;
        if (in_bounds(p)) to_build.insert(p);
    }
    to_build.insert({i, j});
}

void make_wall(int i1, int j1, int i2, int j2) {
    // DBG << 1 << '\n';
    normalize(i1, j1, i2, j2);
    if (i1 == i2) {
        if (!i1) return;
        if(j2 - j1 == 2)
            --j2;
        remove_edge({i1, j1}, {i1 - 1, j1});
        remove_edge({i1, j1 + 1}, {i1 - 1, j1 + 1});
        wall[i1][j1][0] = 1;
        // wall[i1][j1+1][0] = 1;
        to_build.insert({i1, j1 + 2});
    } else if (j1 == j2) {
        if (!j1) return;
        if(i2 - i1 == 2)
            --i2;
        remove_edge({i1, j1}, {i1, j1 - 1});
        remove_edge({i1 + 1, j1}, {i1 + 1, j1 - 1});
        wall[i1][j1][1] = 1;
        // wall[i1+1][j1][1] = 1;
        to_build.insert({i1 + 2, j1});
    }
    add_to_build(i1, j1);
    add_to_build(i2, j2);
}

void remove_wall(int i1, int j1, int i2, int j2) {
    // DBG << '\n';
    normalize(i1, j1, i2, j2);
    if (i1 == i2) {
        if(!i1) return;
        if(j2 - j1 == 2)
            --j2;
        add_edge({i1, j1}, {i1 - 1, j1});
        add_edge({i1, j1 + 1}, {i1 - 1, j1 + 1});
        wall[i1][j1][0] = 0;
    } else {
        if(!j1) return;
        if(i2 - i1 == 2)
            --i2;
        add_edge({i1, j1}, {i1, j1 - 1});
        add_edge({i1 + 1, j1}, {i1 + 1, j1 - 1});
        wall[i1][j1][1] = 0;
    }
}

void read_move() {
    // DBG << '\n';
    string operation;
    cin >> operation;
    if (operation == "over")
        exit(0);
    else if (operation == "move") {
        int i, j;
        cin >> i >> j;
        player2 = from_coord({i - 1, j - 1});
        for(auto step:diag)
            if(in_bounds(from_coord(pii{i - 1, j - 1} + step)))
                to_build.insert(pii{i - 1, j - 1} + step);
        for(auto step:neighbors)
            if(in_bounds(from_coord(pii{i - 1, j - 1} + step)))
                to_build.insert(pii{i - 1, j - 1} + step);
    } else if (operation == "partition") {
        int i1, i2, j1, j2;
        cin >> i1 >> j1 >> i2 >> j2;
        make_wall(i1 - 1, j1 - 1, i2 - 1, j2 - 1);
    }
}

void read_players() {
    int player;
    cin >> player;
    // cout << VAR(player1) << VAR(player2) << '\n';
    if (player == 2) {
        swap(player1, player2);
        swap(wall1, wall2);
        read_move();
    }
    // cout << VAR(player1) << VAR(player2) << '\n';
}

bool is_wall(int i, int j, int o){
    if(!in_bounds({i, j})) return false;
    return wall[i][j][o];
}

bool can_put(int i1, int j1, int i2, int j2) {
    // wall[i][j][0] - --
    // wall[i][j][1] - |
    if(i1 == i2 && i1 == 0)
        return 0;
    if(j1 == j2 && j1 == 0)
        return 0;
    normalize(i1, j1, i2, j2);
    if (i1 == i2)
        return !is_wall(i1, j1, 0) && 
               !is_wall(i1 - 1, j1 + 1, 1) && 
               !is_wall(i1, j1 - 1, 0) && 
               !is_wall(i1, j1 + 1, 0);
    else
        return !is_wall(i1, j1, 1) && 
               !is_wall(i1 + 1, j1 - 1, 0) && 
               !is_wall(i1 - 1, j1, 1) && 
               !is_wall(i1 + 1, j1, 1);
}

bool go(int player, pii step){
    pii cur = to_coord(player),
        to = cur + step;
    if(!in_bounds(to))
        return 0;
    return find(all(graph[player]), from_coord(to)) != graph[player].end();
}



pair<int, vector<int>> make_wall(int depth = 1, bool me = true) {
    // DBG << '\n';
    pair<int, vector<int>> mn = {100, vector<int>(0)},
                           mx = {-1, vector<int>(0)};
    auto cto_build = to_build;
    if(our_walls){
        for (auto point : cto_build) {
            for (auto step : neighbors) {
                pii to = point + step;
                if (!in_bounds(to) || !can_put(point.first, point.second, to.first, to.second))
                    continue;
                auto temp = to_build;
                make_wall(point.first, point.second, to.first, to.second);
                int sp1 = shortest_path(player1, wall1), sp2 = shortest_path(player2, wall2);
                if(sp1 != INT_MAX && sp2 != INT_MAX){
                    if (!depth) {
                        int dif = sp1 - sp2;
                        vector<int> cur = {point.first, point.second, to.first, to.second};
                        // for(auto el : cur)
                        if (dif < mn.first) {
                            mn = {dif, cur};
                        }
                        if (dif > mx.first) {
                            mx = {dif, cur};
                        }
                    } else {
                        pair<int, vector<int>> cur = make_wall(depth - 1, !me);
                        cur.second = {point.first, point.second, to.first, to.second};
                        if (cur.first < mn.first)
                            mn = cur;
                        if (cur.first > mx.first)
                            mx = cur;
                    }
                }
                remove_wall(point.first, point.second, to.first, to.second); 
                to_build = temp;
            }
        }
    }
    for(auto step:neighbors){
        if(me){
            if(!go(player1, step))
                continue;
            int temp = player1;
            player1 = from_coord(to_coord(player1) + step);
            if(!depth){
                int dif = shortest_path(player1, wall1) - shortest_path(player2, wall2);
                vector<int> cur = {to_coord(player1).first, to_coord(player1).second};
                if(dif < mn.first)
                    mn = {dif, cur};
                if(dif > mx.first)
                    mx = {dif, cur};
            }else{
                auto cur = make_wall(depth - 1, !me);
                cur.second = {to_coord(player1).first, to_coord(player1).second};
                if(cur.first < mn.first)
                    mn = cur;
                if(cur.first > mx.first)
                    mx = cur;
            }
            player1 = temp;
        }else{
             if(!go(player2, step))
                continue;
            int temp = player2;
            player2 = from_coord(to_coord(player2) + step);
            if(!depth){
                int dif = shortest_path(player1, wall1) - shortest_path(player2, wall2);
                vector<int> cur = {to_coord(player2).first, to_coord(player2).second};
                if(dif < mn.first)
                    mn = {dif, cur};
                if(dif > mx.first)
                    mx = {dif, cur};
            }else{
                auto cur = make_wall(depth - 1, !me);
                cur.second = {to_coord(player2).first, to_coord(player2).second};
                if(cur.first < mn.first)
                    mn = cur;
                if(cur.first > mx.first)
                    mx = cur;
            }
            player2 = temp;
        }
    }
    return me ? mn : mx;
}

bool First = true;

void make_move() {
    // DBG << '\n';
    auto cur = make_wall();
    if (!(First && player1 == from_coord({0, 4})) && cur.second.size() == 2) {
        //player1 = step(player1, wall1);
        player1 = from_coord({cur.second[0], cur.second[1]});
        if (player1 == player2) {
            if (to_coord(player1).first == wall1) {
                player1 -= 1;
            } else {
                player1 = step(player1, wall1);
            }
        }
        pii nxt = to_coord(player1);
        cout << "move " << nxt.first + 1 << ' ' << nxt.second + 1 << endl;
    } else {
        // actually put the wall
        if(First){
            // cout << "HERE\n";
            if(player1 == from_coord({0, 4})){
                cur.second = {8, 4, 8, 5};
                // cout << "HERE\n";
            }
            First = 0;
        }
        make_wall(cur.second[0], cur.second[1], cur.second[2], cur.second[3]);
        cout << "partition ";
        if(cur.second[0] == cur.second[2])
            cur.second[3]++;
        else
            cur.second[2]++;
        for (int i = 0; i < 4; i++) {
            cout << cur.second[i] + 1 << ' ';
        }
        cout << endl;
        our_walls--;
    }
}

int main(int argc, char **argv) {
    init_game();
    read_players();

    while (true) {
        auto t = clock();
        make_move();
        // cout << "IT TOOK: " << (clock()-t)/double(CLOCKS_PER_SEC) << "s\n";
        // display_walls();
        read_move();
        // display_walls();
    }

    return 0;
}
