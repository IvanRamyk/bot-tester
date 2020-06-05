#include <bits/stdc++.h>
//#define DEBUG cout
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,avx,avx2")

using namespace std;



class Common
{
public:
    static int convertCoordXStr(string coordXStr)
    {
        if(coordXStr == "A")
            return 1;
        if(coordXStr == "B")
            return 2;
        if(coordXStr == "C")
            return 3;
        if(coordXStr == "D")
            return 4;
        if(coordXStr == "E")
            return 5;
        if(coordXStr == "F")
            return 6;
        if(coordXStr == "G")
            return 7;
        if(coordXStr == "H")
            return 8;
        if(coordXStr == "I")
            return 9;
        return -1;

}

static string convertCoordX(int coordX)
{
    if(coordX == 1)
        return "A";
    if(coordX == 2)
        return "B";
    if(coordX == 3)
        return "C";
    if(coordX == 4)
        return "D";
    if(coordX == 5)
        return "E";
    if(coordX == 6)
        return "F";
    if(coordX == 7)
        return "G";
    if(coordX == 8)
        return "H";
    if(coordX == 9)
        return "I";
    return " ";
}

};
void printMove(string str)
{
    if(str.size() == 2)
    {
        string tmp;
        tmp+=str[0];
        cout << "move " << str[1] << " " << Common::convertCoordXStr(tmp) << endl;
    }
    else
    {
        int x1,y1,x2,y2;
        string tmp;
        tmp+=str[0];
        y1 = Common::convertCoordXStr(tmp);
        x1 = str[1] - '0';
        if(str[2] == 'h')
            x1++;
        else y1++;
        if(str[2] == 'v')
            x2 = x1 + 2, y2 = y1 ;
        else x2 = x1,y2 = y1 + 2;
        cout << "partition " << x1 << " " << y1 << " " <<  x2 << " " <<  y2 << endl;
    }
}

string getMove()
{
    string tmp;
    cin >> tmp;
    if(tmp == "move")
    {
        int x,y;
        cin >> x >> y;
        return Common::convertCoordX(y) + to_string(x);
    }
    else
    {
        int x1,x2,y1,y2;
        cin >> y1 >> x1 >> y2 >> x2;
        if(x2 == x1 + 2)
        tmp = Common::convertCoordX(x1) + to_string(y1-1);
        else tmp = Common::convertCoordX(x1-1) + to_string(y1);
        if(x2 == x1 + 2)
            tmp+='h';
        else tmp+='v';
//printMove(tmp);
       // cout << tmp;
        return tmp;
    }
}
class Square
{
public:
    string coordXStr;
    int coordX;
    int coordY;
    int unaryCoord;

    Square(int coordX, int coordY)
    {
        this->coordXStr = Common::convertCoordX(coordX);
        this->coordX = coordX;
        this->coordY = coordY;
        unaryCoord = (coordY-1)*9+coordX-1;
    }

    Square(Square* s)
    {
        this->coordXStr = s->coordXStr;
        this->coordX = s->getCoordinateX();
        this->coordY = s->getCoordinateY();
        this->unaryCoord = s->getUnaryCoord();
    }

    int getCoordinateX()
    {
        return coordX;
    }

    int getCoordinateY()
    {
        return this->coordY;
    }

    int getUnaryCoord()
    {
        return unaryCoord;
    }

    string toString()
    {
        string tmp = this->coordXStr;
        tmp+=to_string(this->coordY);
        return tmp;
    }
};

class Player
{
public:
    int id;
    Square* position;
    int fences;
    unordered_set <string> neighbours;

    Player(Square* position, int id, unordered_set<string> neighbours)
    {
        this->position = position;
        this->id = id;
        this->neighbours = neighbours;
        this->fences = 10;
    }

    Player(Player* player)
    {
        this->id = player->getId();
        this->position = new Square(player->getPosition());
        this->fences = player->getFences();
        this->neighbours = player->neighbours;
    }

    int getFences()
    {
        return this->fences;
    }

    Square* getPosition()
    {
        return this->position;
    }

    int getId()
    {
        return this->id;
    }

    void decreaseFences()
    {
        this->fences--;
    }

    void setPosition(Square* position)
    {
        this->position = position;
    }

    unordered_set<string>& getNeighbours()
    {
        return this->neighbours;
    }

    void setNeighbours(unordered_set<string>& s)
    {
        this->neighbours = s;
    }

    void addNeighbour(string& s)
    {
        neighbours.insert(s);
    }

    void removeNeighbour(string& s)
    {
        neighbours.erase(s);
    }
};

class Board {
public:
    unordered_map <string,unordered_set <string>> edges;
    vector <Square*> squares;
    unordered_set  <string> fences;
    char graphicBoard[18][18];
    Player* player1;
    Player* player2;

    static const int IN_PROGRESS;
    int P1 = 0;
    int P2 = 1;
    static const string POS_P1;
    static const string POS_P2;

     Board() {
        this->edges.clear();
        createNodes();
        createEdges();
        player1 = new Player(getSquare(POS_P1), P1, getEdges(POS_P1));
        player2 = new Player(getSquare(POS_P2), P2, getEdges(POS_P2));
        fences.clear();

        for (Square* i : squares) {
            if(i)
            if (i->getCoordinateY() != 9 && i->getCoordinateX() != 9) {
                string tmp1 =i->toString() + "h";
                string tmp2 =i->toString() + "v";

                fences.insert(tmp1);
                fences.insert(tmp2);
            }
        }

    }

     Board(Board* b) {
        this->edges.clear();
        for (auto s : b->edges) {
            this->edges[s.first] =  b->edges[s.first];
        }
        this->squares = b->squares;
        for(int i=0;i<18;i++)
        for(int j=0;j<18;j++)
        this->graphicBoard[i][j] = b->graphicBoard[i][j];
        this->player1 = new Player(b->player1);
        this->player2 = new Player(b->player2);
        this->fences = b->fences;
    }

     void createNodes() {
        this->squares = vector <Square*>(82);
        for (int i = 1; i <= 9; ++i) {
            for (int j = 1; j <= 9; ++j) {
                Square* n = new Square(j, i);
                squares[n->getUnaryCoord()] = n;
            }
        }
    }

     void createEdges() {
       unordered_set<string> v;
        for (int i = 1; i <= 9; ++i) {
            for (int j = 1; j <= 9; ++j) {
                v.clear();
                Square* n = squares[(j - 1) * 9 + i - 1];
                int coordX = n->getCoordinateX();
                int coordY = n->getCoordinateY();
                int pos = n->getUnaryCoord();
                if (coordX > 1) {
                    v.insert(squares[pos - 1]->toString());
                }
                if (coordX < 9) {
                    v.insert(squares[pos + 1]->toString());
                }
                if (coordY > 1) {
                    v.insert(squares[pos - 9]->toString());
                }
                if (coordY < 9) {
                    v.insert(squares[pos + 9]->toString());
                }
                edges[n->toString()] = v;
            }
        }
    }

     Square* getSquare(string id) {
        string tmp1,tmp2;//id.substring(0, 1).toUpperCase()
        tmp1+= toupper(id[0]);
        tmp2+= id[1];
        int x = Common::convertCoordXStr(tmp1);
        int y = stoi(tmp2);
        return squares[(y - 1) * 9 + x - 1];
    }

     Square* getSquare(int unaryCoord) {
        return squares[unaryCoord];
    }

     unordered_set <string>& getEdges(string id) {
        return this->edges[id];
    }

    unordered_set<string>& getFences() {
        return this->fences;
    }

     Player* getPlayer1() {
        return player1;
    }

     Player* getPlayer2() {
        return player2;
    }

     void removeNeighbour(Square* n1, Square* n2) {
       unordered_set<string> neighbours = edges[n1->toString()];
        neighbours.erase(n2->toString());
        edges[n1->toString()] = neighbours;

        neighbours = edges[n2->toString()];
        neighbours.erase(n1->toString());
        edges[n2->toString()] = neighbours;
    }

     pair <bool, int> bfs(Square* source, int playerId) {
        int sourcePos = source->getUnaryCoord();
        if ((sourcePos >= 72 && sourcePos <= 80 && playerId == 0) || (sourcePos >= 0 && sourcePos <= 8 && playerId == 1)) {
            return {true, 0};
        }

        int distance[81] = {0};
        bool visited[81] = {0};
        int previous[81] = {0};
        for (int i = 0; i < 81; ++i) {
            distance[i] = INT_MAX;
            previous[i] = -1;
            visited[i] = false;
        }

        int sourceCoord = source->getUnaryCoord();
        visited[sourceCoord] = true;
        distance[sourceCoord] = 0;
        previous[sourceCoord] = -1;
        queue<Square*> q;
        q.push(source);
        Square* n;
        int index1, index2;
        while (!q.empty()) {
            n = q.front();
            q.pop();
            index1 = n->getUnaryCoord();
            for (string s : getEdges(n->toString())) {
                Square* i = getSquare(s);
                index2 = i->getUnaryCoord();
                if ((index2 >= 72 && index2 <= 80 && playerId == 0) || (index2 >= 0 && index2 <= 8 && playerId == 1)) {
                    visited[index2] = true;
                    distance[index2] = distance[index1] + 1;
                    previous[index2] = index1;
                    return {true, distance[index2]};
                } else if (!visited[index2]) {
                    visited[index2] = true;
                    distance[index2] = distance[index1] + 1;
                    previous[index2] = index1;
                    q.push(i);
                }
            }
        }
        return {false, NULL};
    }

     bool checkPath() {
        return bfs(player1->getPosition(), 0).first &&
                bfs(player2->getPosition(), 1).first;
    }

     void addNeighbour(Square* n1, Square* n2) {
       unordered_set<string> aux = edges[n1->toString()];
        aux.insert(n2->toString());
        edges[n1->toString()] = aux;
        aux = edges[n2->toString()];
        aux.insert(n1->toString());
        edges[n2->toString()] =  aux;
    }

     bool addFence(int idPlayer, string id) {
        string tmp1,tmp2;
        tmp1 += toupper(id[0]);
        tmp2 += tolower(id[2]);
        string pos = tmp1 + id[1] + tmp2;
        if (fences.find(pos) != fences.end()) {
            Square* n1 = getSquare(id.substr(0, 2));
            Square* n2 = squares[n1->getUnaryCoord() + 9];
            Square* n3 = squares[n2->getUnaryCoord() + 1];
            Square* n4 = squares[n1->getUnaryCoord() + 1];
            int auxx = ((n1->getCoordinateX() - 1) * 2);
            int auxy = (16 - ((n1->getCoordinateY() - 1) * 2));
            if (pos.substr(2, 3) == "h") {
                removeNeighbour(n1, n2);
                removeNeighbour(n3, n4);
                if (!checkPath()) {
                    addNeighbour(n1, n2);
                    addNeighbour(n3, n4);
                    return false;
                }
                fences.erase(pos);
                auxy = auxy - 1;
                for (int i = 0; i < 3; ++i) {
                    graphicBoard[auxy][auxx + i] = '-';
                }
                fences.erase(n1->toString() + "v");
                if (n2->getCoordinateX() != 9) {
                    fences.erase(n4->toString() + "h");
                }
                if (n1->getCoordinateX() > 1) {
                    fences.erase(squares[n1->getUnaryCoord() - 1]->toString() + "h");
                }
            } else {
                removeNeighbour(n1, n4);
                removeNeighbour(n2, n3);
                if (!checkPath()) {
                    addNeighbour(n1, n4);
                    addNeighbour(n2, n3);
                    return false;
                }
                fences.erase(pos);
                auxx = auxx + 1;
                for (int i = 0; i < 3; ++i) {
                    graphicBoard[auxy - i][auxx] = '|';
                }
                fences.erase(n1->toString() + "h");
                if (n2->getCoordinateY() != 9) {
                    fences.erase(n2->toString() + "v");
                }
                if (n1->getCoordinateY() > 1) {
                    fences.erase(squares[n1->getUnaryCoord() - 9]->toString() + "v");
                }
            }
            getPlayerTurn(idPlayer)->decreaseFences();
            updateNeighbours();
            return true;
        }
        return false;
    }

     bool checkMove(Player* p, string dest) {
        return p->getNeighbours().find(dest) != p->getNeighbours().end();
    }

     Player* getPlayerTurn(int idPlayer) {
        if (idPlayer == player1->getId()) return player1;
        return player2;
    }

     void updateNeighbours() {
        string pos1 = player1->getPosition()->toString();
        string pos2 = player2->getPosition()->toString();
       unordered_set<string> n1 = getEdges(pos1);
       unordered_set<string> n2 = getEdges(pos2);
        if (n1.find(pos2)!=n1.end()) {
            n1.erase(pos2);
            n2.erase(pos1);
           unordered_set<string> n1Tmp = n1;
            int diff = getSquare(pos2)->getUnaryCoord() - getSquare(pos1)->getUnaryCoord();
            if (player2->getPosition()->getCoordinateY() > 1 && player2->getPosition()->getCoordinateY() < 9) {
                string jump = getSquare(getSquare(pos2)->getUnaryCoord() + diff)->toString();
                if (n2.find(jump)!=n2.end()) {
                    n1.insert(jump);
                } else {
                    for(auto all:n2)
                        n1.insert(all);
                }
            }
            if (player1->getPosition()->getCoordinateY() > 1 && player1->getPosition()->getCoordinateY() < 9) {
                string jump = getSquare(getSquare(pos1)->getUnaryCoord() - diff)->toString();
                if (n1.find(jump)!=n1.end()) {
                    n2.insert(jump);
                } else {
                    for(auto all:n1Tmp)
                    n2.insert(all);
                }
            }
        }
        player1->setNeighbours(n1);
        player2->setNeighbours(n2);
    }

     bool movePlayer(int idPlayer, string position) {
        Square* src = getPlayerTurn(idPlayer)->getPosition();
        Square* dest = getSquare(position);
        if (checkMove(getPlayerTurn(idPlayer), dest->toString())) {
            getPlayerTurn(idPlayer)->setPosition(dest);
            updateNeighbours();
            graphicBoard[16 - ((src->getCoordinateY() - 1) * 2)][(src->getCoordinateX() - 1) * 2] = ' ';
            graphicBoard[16 - ((dest->getCoordinateY() - 1) * 2)][(dest->getCoordinateX() - 1) * 2] = (char) (idPlayer + 1);
            return true;
        }
        return false;
    }

     vector <string> getPossibleMoves(int player) {

        vector <string> moves;
        if (player == 0) {
            for(auto all:player1->getNeighbours())
                moves.push_back(all);
            if (player1->getFences() > 0)
             for(auto all:this->fences)
                moves.push_back(all);
        } else {
            for(auto all:player2->getNeighbours())
                moves.push_back(all);
            if (player2->getFences() > 0)
            for(auto all:this->fences)
                moves.push_back(all);
        }

        return moves;
    }

     bool performMove(int player, string pos) {
        switch (pos.size()) {
            case 2:
                return movePlayer(player, pos);
            case 3:
                return addFence(player, pos);
            default:
                return false;
        }
    }

     int playerWon() {
        int coordinateY = this->player1->getPosition()->getCoordinateY();
        if (coordinateY == 9) return P1 + 1;
        int coordinateY2 = player2->getPosition()->getCoordinateY();
        if (coordinateY2 == 1) return P2 + 1;
        return IN_PROGRESS;
    }

};

const string Board::POS_P1 = "E1";
const string Board::POS_P2 = "E9";
const int Board::IN_PROGRESS = -1;

int ggg =0;
class Node
{
public:
    string move;
    int player;
    int visitCount;
    int winScore;
    Node* parent;
    vector <Node*> children;

    Node()
    {
        this->parent = NULL;
        this->children.clear();
        this->winScore = 0;
        this->visitCount = 0;
        this->player = 0;
    }

    Node(Node* node)
    {
        this->move = node->move;
        this->player = node->player;
        this->visitCount = node->visitCount;
        this->winScore = node->winScore;
        this->parent = node->getParent();
        this->children = node->getChildren();
    }

    vector < Node* >& getChildren()
    {
        return this->children;
    }

    Node* getParent()
    {
        return this->parent;
    }

    int getPlayer()
    {
        return this->player;
    }

    string& getMove()
    {
        return this->move;
    }

    int& getVisitCount()
    {
        return this->visitCount;
    }

    int& getWinScore()
    {
        return this->winScore;
    }

    void setPlayer(int player)
    {
        this->player = player;
    }

    void setMove(string& move)
    {
        this->move = move;
    }

    void setParent(Node* parent)
    {
        this->parent = parent;
    }

    void setChildren(vector <Node*>& children)
    {
        this->children = children;
    }

    Node* getRandomChildNode()
    {
        int possibleMoves = this->children.size();
        return this->children[rand()%(possibleMoves)];
    }

    void incrementVisit()
    {
        this->visitCount += 1;
    }

    void addScore(int& score)
    {
        this->winScore += score;
    }

    vector <Node*> getAllPossibleNodes(Board* board)
    {
        vector <Node*> possibleNodes;
        vector <string> availablePositions = board->getPossibleMoves((player+1)%2);
        Board* tmpBoard = new Board(board);
        ggg=0;
        for (int i =0;i< 3;i++)
        {
            auto p = availablePositions[i];
            Node* newNode = new Node();
            int start =clock();
            newNode->setPlayer((this->player+1)%2);
            newNode->setMove(p);

            if(tmpBoard->performMove(newNode->getPlayer(), p))
                possibleNodes.push_back(newNode);
            tmpBoard = new Board(board);
        }

        for (int i =0;i< 2;i++)
        {
            auto p = availablePositions[rand()%(availablePositions.size())];
            Node* newNode = new Node();
            int start =clock();
            newNode->setPlayer((this->player+1)%2);
            newNode->setMove(p);

            if(tmpBoard->performMove(newNode->getPlayer(), p))
                possibleNodes.push_back(newNode);
            tmpBoard = new Board(board);
        }

        return possibleNodes;
    }

    int getOpponent()
    {
        return (this->player+1)%2;
    }

    void togglePlayer()
    {
        this->player = (this->player+1)%2;
    }

    Node* getChildWithMaxScore()
    {

        pair <int,Node*> ans = {INT_MIN, NULL};
        for(auto child: this->children)
        {
            if(ans.first < child->winScore)
            ans = {child->winScore, child};
        }
        return ans.second;
    }

    void randomPlay(Board* board)
    {
        int selectRandom = rand()%(board->getPossibleMoves(player).size());
        board->performMove(this->player, board->getPossibleMoves(player)[selectRandom]);
    }

    void heuristicDecision(Board* board)
    {
        Player* p1 = board->getPlayerTurn(player);
        Player* p2 = board->getPlayerTurn(getOpponent());
        int distanceGoalP1 = board->bfs(p1->getPosition(), p1->getId()).second;
        int distanceGoalP2 = board->bfs(p2->getPosition(), p2->getId()).second;
        if (distanceGoalP1 <= distanceGoalP2 || p1->getFences() == 0)
        {
            pair <string, int> result = {"", 81};
            for (string s : p1->getNeighbours())
            {
                pair<bool, int> t = board->bfs(board->getSquare(s), player);
                if (t.second < result.second)
                {
                    result.first = s;
                    result.second = t.second;
                }
            }
            board->performMove(player, result.first);
        }
        else
        {
            int selectRandom = rand()%(board->getPossibleMoves(player).size());
            board->performMove(this->player, board->getPossibleMoves(player)[selectRandom]);
        }
    }

};

 class Tree {
public:
    Node* root;

    Tree() {
        root = new Node();
    }

    Tree(Node* root) {
        this->root = root;
    }

    Node* getRoot() {
        return this->root;
    }

    void setRoot(Node* root) {
        this->root = root;
    }

    void addChild(Node* parent, Node* child) {
        parent->getChildren().push_back(child);
    }
};

class UCT
{
public:
    static double uctValue(int totalVisit, double nodeWinScore, int nodeVisit)
    {
        if (nodeVisit == 0)
        {
            return INT_MAX;
        }
        return (nodeWinScore / (double) nodeVisit) + 1.41 * sqrt(log(totalVisit) / (double) nodeVisit);
    }

    static Node* findBestNodeWithUCT(Node* node)
    {
        int parentVisit = node->getVisitCount();
        pair <int,Node*> ans = {INT_MIN,NULL};
        for(auto el:node->getChildren())
        {
            if(uctValue(parentVisit, el->getWinScore(), el->getVisitCount()) > ans.first)
                ans = {uctValue(parentVisit, el->getWinScore(), el->getVisitCount()),el};
            }
              return ans.second;
    }
};

class MonteCarloTreeSearch {
public:
    int WIN_SCORE;
    int SIMULATIONS;

    MonteCarloTreeSearch(int simulations) {
        this->SIMULATIONS = 4;
        this->WIN_SCORE = 10;
    }

    Board* findNextMove(Board* board, int player) {
        Board* originalBoard = new Board(board);

        int opponent = (player + 1) % 2;
        Tree* tree = new Tree();
        Node* rootNode = tree->getRoot();
        rootNode->setPlayer(opponent);
        int simulations = 0;
        int start_time,end_time;
        while (simulations < SIMULATIONS) {

            board = new Board(originalBoard);
            ++simulations;

            // Phase 1 - Selection

            Node* promisingNode = selectPromisingNode(rootNode, board);


            // Phase 2 - Expansion

            if (board->playerWon() == Board::IN_PROGRESS) {
                expandNode(promisingNode, board);
            }

            // Phase 3 - Simulation

            Node* nodeToExplore = promisingNode;
            if (promisingNode->getChildren().size() > 0) {
                nodeToExplore = promisingNode->getRandomChildNode();
            }
            board->performMove(nodeToExplore->getPlayer(), nodeToExplore->getMove());
            int playoutResult = simulateRandomPlayout(nodeToExplore, board);

            // Phase 4 - Update

            backPropagation(nodeToExplore, playoutResult);


        }
        Node* bestNode = rootNode->getChildWithMaxScore();
        originalBoard->performMove(bestNode->getPlayer(), bestNode->getMove());
        printMove(bestNode->getMove());

        return originalBoard;
    }

    Node* selectPromisingNode(Node* rootNode, Board* board) {
        Node* node = rootNode;
        while (node->getChildren().size() != 0) {
            node = UCT::findBestNodeWithUCT(node);
            board->performMove(node->getPlayer(), node->getMove());
        }
        return node;
    }

     void expandNode(Node* node, Board* board) {
        vector <Node*> possibleNodes = node->getAllPossibleNodes(board);

        for(auto child:possibleNodes)
        {
            Node* newNode = new Node(child);
            newNode->setParent(node);
            newNode->setPlayer(node->getOpponent());
            node->getChildren().push_back(newNode);
        }
    }

     void backPropagation(Node* nodeToExplore, int playerNo) {
        Node* tempNode = nodeToExplore;
        while (tempNode != NULL) {
            tempNode->incrementVisit();
            if (tempNode->getPlayer() + 1 == playerNo)
                tempNode->addScore(WIN_SCORE);
            tempNode = tempNode->getParent();
        }
    }

     int simulateRandomPlayout(Node* node, Board* board) {
        Node* tempNode = new Node(node);
        int boardStatus = board->playerWon();
        while (boardStatus == Board::IN_PROGRESS) {
            tempNode->togglePlayer();
            //tempNode.randomPlay(board);
            tempNode->heuristicDecision(board);
            boardStatus = board->playerWon();
        }
        return boardStatus;
    }
};

class Game
{
public:
    int turn;
    Board* board;
    MonteCarloTreeSearch* mcts2;

    Game()
    {
        board = new Board();
        mcts2 = new MonteCarloTreeSearch(6);
        turn = 0;
    }

    int startGame(int ps)
    {
        string input;
        while(board->playerWon() == Board::IN_PROGRESS)
        {
            if(turn == ps-1)
            {
                input = getMove();
                while (!validInput(input))
                {
                    return 1;
                }
                board->performMove(turn, input);
            }
            else
            {
                Player* p = board->getPlayerTurn(turn);

                if(p->getFences() > 0)
                {
                    board = mcts2->findNextMove(board, turn);
                }
                else
                {
                    pair <string,int> result = {" ", 81};
                    for (string s : board->getPossibleMoves(turn))
                    {
                        pair<bool,int> t = board->bfs(board->getSquare(s),turn);
                        if(t.second < result.second)
                        {
                            result.first = s;
                            result.second = t.second;
                        }
                    }

                    board->performMove(turn,result.first);
                    printMove(result.first);
                }


            }
            turn = (turn+1)%2;
            //cout << "GGG " << ggg <<"\n";
        }

        cout << "FINISHED Player " << board->playerWon() << " won" << endl;
        return board->playerWon();
    }

    bool validInput(string input)
    {
        if(input.size() == 2)
            return board->movePlayer(turn,input);
        if(input.size() == 3)
            if(board->getPlayerTurn(turn)->getFences() > 0 && board->addFence(turn,input))
                return true;

        return false;
    }
};


int main()
{
    srand( time( 0 ) );
    int x;
    Game* g;
    int result[] = {0, 0};
    for (int i = 0; i < 1; ++i) {
        cin >> x;
        if(x==2)
            x=1;
        else x=2;
        g = new Game();
        g->startGame(x);
    }


}

