#include<iostream>
#include<cassert>
#include<set>
#include<map>
#include<vector>
#include<queue>
#include<random>

using namespace std;

std::random_device rd;

bool flipCoin(int p_false, int p_true){
    std::uniform_int_distribution<int> uid(1, p_false + p_true);
    if(uid(rd) <= p_false)
        return false;
    return true;
}

enum MoveType {
    INVALID = 0,
    OVER = 1,
    MOVE = 2,
    PARTITION = 3,
};

struct Point {
    int x, y;
    Point(int x = -1, int y = -1): x(x), y(y){}
    Point(const Point & p): x(p.x), y(p.y){}
};

struct Delta {
    int dx, dy;
    Delta(int dx = 0, int dy = 0): dx(dx), dy(dy){}
    int manhattan(){
        return abs(dx) + abs(dy);
    }
};
bool operator == (const Delta & a, const Delta & b){
    return a.dx == b.dx && a.dy == b.dy;
}

const Delta D_UP(1, 0), D_DOWN(-1, 0), D_LEFT(0, -1), D_RIGHT(0, 1);

Point operator + (Point p, Delta d){
    return Point(p.x + d.dx, p.y + d.dy);
}
Delta operator - (Point p1, Point p2){
    return Delta(p1.x - p2.x, p1.y - p2.y);
}

istream & operator >> (istream & in, Point & p){
    in >> p.x >> p.y;
    return in;
}
ostream & operator << (ostream & out, const Point & p){
    out << p.x << ' ' << p.y;
    return out;
}

bool operator < (const Point & p1, const Point & p2){
    return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
}
bool operator == (const Point & p1, const Point & p2){
    return p1.x == p2.x && p1.y == p2.y;
}

struct Fence {
    Point a, b;
    Fence(): a(), b() {}
    Fence(Point a, Point b): a(a), b(b){}
    Fence(const Fence & f): a(f.a), b(f.b){}

    Point center() const {
        return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
    }
    bool isVertical() const {
        return a.x != b.x;
    }
    bool isHorisontal() const {
        return a.x == b.x;
    }

    static Fence FromCenter(int x, int y, bool horisontal){
        if(horisontal)
            return Fence(Point(x, y - 1), Point(x, y + 1));
        return Fence(Point(x - 1, y), Point(x + 1, y));
    }
};

istream & operator >> (istream & in, Fence & f){
    in >> f.a >> f.b;
    return in;
}
ostream & operator << (ostream & out, const Fence & f){
    out << f.a << ' ' << f.b;
    return out;
}
bool operator < (const Fence & f1, const Fence & f2){
    return f1.a < f2.a || (f1.a == f2.a && f1.b < f2.b);
}

struct Move {
    MoveType moveType;
    Point p;
    Fence f;
    Move(const Move & m): moveType(m.moveType), p(m.p), f(m.f) {}
    Move(MoveType moveType = MoveType::INVALID){
        assert(moveType == MoveType::INVALID || moveType == MoveType::OVER);
    }
    Move(MoveType moveType, Point p): moveType(moveType), p(p)
    {
        assert(moveType == MoveType::MOVE);
        assert(p.x != -1);
        assert(p.y != -1);
    }
    Move(MoveType moveType, Fence f): moveType(moveType), f(f)
    {
        assert(moveType == MoveType::PARTITION);
        assert(f.a.x != -1);
        assert(f.a.y != -1);
        assert(f.b.x != -1);
        assert(f.b.y != -1);
    }

    bool isOver() const {
        return moveType == MoveType::OVER;
    }

    bool isMove() const {
        return moveType == MoveType::MOVE;
    }
    bool isPartition() const {
        return moveType == MoveType::PARTITION;
    }
};

istream & operator >> (istream & in, Move & m){
    string type;
    in >> type;
    if (type == "move"){
        m.moveType = MoveType::MOVE;
        in >> m.p;
    } else if (type == "partition"){
        m.moveType = MoveType::PARTITION;
        in >> m.f;
    } else if (type == "over"){
        m.moveType = MoveType::OVER;
    }

    return in;
}

ostream & operator << (ostream & out, const Move & m){
    switch (m.moveType)
    {
        case MoveType::MOVE:
            out << "move " << m.p;
            break;
        case MoveType::PARTITION:
            out << "partition " << m.f;
            break;
        default:
            assert(false && "Given move is not 'move' or 'partition'");
            break;
    }
    return out;
}

struct Field{
    Point player1, player2;
    map<Point, Fence> verticalFences, horisontalFences;

    Field(): player1(1, 5), player2(9, 5) {}
    void addFence(const Fence & f){
        if(f.isVertical())
            verticalFences[f.center()] = f;
        else
            horisontalFences[f.center()] = f;
    }
    void removeFence(const Fence & f){
        if(f.isVertical())
            verticalFences.erase(f.center());
        else
            horisontalFences.erase(f.center());
    }

    bool isValidFence(const Fence & f){
        Point center = f.center();
        if(verticalFences.count(center))
            return false;
        if(horisontalFences.count(center))
            return false;
        if(f.isHorisontal()){
            return horisontalFences.count(center + D_LEFT) == 0 &&
                   horisontalFences.count(center + D_RIGHT) == 0;
        }
        return verticalFences.count(center + D_UP) == 0 &&
               verticalFences.count(center + D_DOWN) == 0;
    }
    bool isValidMove(Point from, Point to){
        if(to.x < 1 || to.x > 9 || to.y < 1 || to.y > 9)
            return false;
        if(from == to)
            return false;
        if(abs(from.x - to.x) + abs(from.y - to.y) > 1)
            return false;
        Point mx = max(from, to);
        // horisontal move
        if(from.x == to.x){
            return verticalFences.count(mx) == 0 && verticalFences.count(mx + D_UP) == 0;
        } else {
            return horisontalFences.count(mx) == 0 && horisontalFences.count(mx + D_RIGHT) == 0;
        }
    }

    int getDistance(const Point & playerPos, int goalHorisontal){
        vector<vector<int>> dist(11, vector<int>(11, -1));
        queue<Point> Q;
        Q.push(playerPos);
        dist[playerPos.x][playerPos.y] = 0;
        while(!Q.empty()){
            Point p = Q.front();
            Q.pop();
            int d = dist[p.x][p.y];
            if(p.x == goalHorisontal){
                return d;
            }
            for(Delta delta: {D_RIGHT, D_LEFT, D_UP, D_DOWN}){
                Point pnext = p + delta;
                if(isValidMove(p, pnext) && dist[pnext.x][pnext.y] == -1){
                    dist[pnext.x][pnext.y] = d + 1;
                    Q.push(pnext);
                }
            }
        }
        return 100500;
    }
    void addJumpMoves(
            const Point & player,
            const Point & other,
            vector<vector<int>> & dist,
            vector<vector<Point>> & from,
            queue<Point> & Q){
        if((player - other).manhattan() != 1)
            return;
        if(!isValidMove(player, other))
            return;

        Delta delta = other - player;
        Point landingPoint = other + delta;
        if(isValidMove(other, landingPoint)){
            dist[landingPoint.x][landingPoint.y] = 1;
            from[landingPoint.x][landingPoint.y] = player;
            Q.push(landingPoint);
            return;
        }
        Delta d1, d2;
        if(delta == D_DOWN || delta == D_UP)
            d1 = D_LEFT, d2 = D_RIGHT;
        else
            d1 = D_UP, d2 = D_DOWN;

        landingPoint = other + d1;
        if(isValidMove(other, landingPoint)){
            dist[landingPoint.x][landingPoint.y] = 1;
            from[landingPoint.x][landingPoint.y] = player;
            Q.push(landingPoint);
        }
        landingPoint = other + d2;
        if(isValidMove(other, landingPoint)){
            dist[landingPoint.x][landingPoint.y] = 1;
            from[landingPoint.x][landingPoint.y] = player;
            Q.push(landingPoint);
        }
    }

    Point getShortestPathMove(const Point & playerPos, const Point & otherPlayerPos, int goalHorisontal){
        vector<vector<int>> dist(11, vector<int>(11, -1));
        vector<vector<Point>> from(11, vector<Point>(11));

        queue<Point> Q;
        Q.push(playerPos);
        dist[playerPos.x][playerPos.y] = 0;
        addJumpMoves(playerPos, otherPlayerPos, dist, from, Q);
        while(!Q.empty()){
            Point p = Q.front();
            Q.pop();
            int d = dist[p.x][p.y];
            if(p.x == goalHorisontal){
                while(dist[p.x][p.y] != 1){
                    p = from[p.x][p.y];
                }
                return p;
            }
            for(Delta delta: {D_RIGHT, D_LEFT, D_UP, D_DOWN}){
                Point pnext = p + delta;
                if(isValidMove(p, pnext) && dist[pnext.x][pnext.y] == -1){
                    dist[pnext.x][pnext.y] = d + 1;
                    from[pnext.x][pnext.y] = p;

                    Q.push(pnext);
                }
            }
        }
        assert(false && "IMPOSSIBLE MOVE!");
    }
    Point getShortestPathMove(int player){
        if(player == 1)
            return getShortestPathMove(player1, player2, 9);
        return getShortestPathMove(player2, player1, 1);
    }
    int getDistancePlayer1(){
        return getDistance(player1, 9);
    }
    int getDistancePlayer2(){
        return getDistance(player2, 1);
    }
    int getDistanceForPlayer(int player){
        if(player == 1)
            return getDistancePlayer1();
        return getDistancePlayer2();
    }
    int getDistDiff(int player){
        int otherPlayer = player ^ 2 ^ 1;
        int d1 = getDistanceForPlayer(player);
        int d2 = getDistanceForPlayer(otherPlayer);
        if(d1 == 100500 || d2 == 100500)
            return 100500;
        return d1 - d2;
    }
    Fence getBestPartition(int player){
        Fence bestChoice;
        int bestDiff = 1000;
        int bestOtherPlayerDistance = -1000;
        int gotCnt = 0;
        for(int horisontal = 0; horisontal <= 1; horisontal++)
            for(int cx = 2; cx <= 9; cx++)
                for(int cy = 2; cy <= 9; cy++){
                    Fence f = Fence::FromCenter(cx, cy, horisontal);
                    if(!isValidFence(f))
                        continue;
                    addFence(f);
                    int diff = getDistDiff(player);
                    if(diff == bestDiff){
                        int otherPlayerDistance = getDistanceForPlayer(player ^ 2 ^ 1);
                        if(otherPlayerDistance == bestOtherPlayerDistance && flipCoin(gotCnt, 1)){
                            bestChoice = f;
                            gotCnt++;
                        }

                        if(otherPlayerDistance > bestOtherPlayerDistance){
                            bestOtherPlayerDistance = otherPlayerDistance;
                            bestChoice = f;
                            gotCnt = 1;
                        }
                    }
                    if(diff != 100500 && diff < bestDiff){
                        bestDiff = diff;
                        bestChoice = f;
                        bestOtherPlayerDistance = getDistanceForPlayer(player ^ 2 ^ 1);
                        gotCnt = 1;
                    }

                    removeFence(f);
                }
        return bestChoice;
    }
};

struct Game{
    int player;
    int currentPlayer;
    int partitionsLeft = 10;
    Field f;
    Game(): currentPlayer(1), partitionsLeft(10){}


    void readPlayer(istream & in){
        in >> player;
    }
    void play(istream & in, ostream & out){
        Move m;
        if(player == 1){
            m = getBestMove();
            addMove(m);
            out << m << endl;
        }
        while(true){
            in >> m;
            if(m.isOver())
                return;
            addMove(m);

            m = getBestMove();
            addMove(m);
            out << m << endl;
        }

    }
    void addMove(const Move & m){
        if(m.isMove()){
            if(currentPlayer == 1)
                f.player1 = m.p;
            else
                f.player2 = m.p;
        } else if(m.isPartition()){
            f.addFence(m.f);
        }
        currentPlayer = currentPlayer ^ 2 ^ 1;
    }
    Move getBestMove(){
        int distDifference = f.getDistDiff(player);
        if(distDifference <= 0 || partitionsLeft == 0){
            return Move(MoveType::MOVE, f.getShortestPathMove(player));
        }
        partitionsLeft--;
        return Move(MoveType::PARTITION, f.getBestPartition(player));
    }
};



int main(){
    Game G;
    G.readPlayer(cin);
    G.play(cin, cout);
}