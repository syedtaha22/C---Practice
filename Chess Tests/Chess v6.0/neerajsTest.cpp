#include<iostream>
#include<string>
#include<exception>
using namespace std;

enum Piece { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, EMPTY };
enum Colour { BLACK, WHITE, NOCOL };

class Square {
    Piece piece;
    Colour colour;
    int x_axis, y_axis;
public:
    Square() {
        piece = EMPTY;
        colour = NOCOL;
    }
    void setSpace(Square*);
    void setEmpty();
    void setPieceandColour(Piece, Colour);
    Piece getPiece();
    Colour getColour();
    void setx(int x) { x_axis = x; }
    void sety(int y) { y_axis = y; }
    int getx() { return x_axis; }
    int gety() { return y_axis; }
};

class Board {
    Square board[8][8];
    Colour turn = WHITE;
public:
    bool moveking(Square* fromthis, Square* tothat);
    bool movequeen(Square* fromthis, Square* tothat);
    bool movebishop(Square* fromthis, Square* tothat);
    bool moveknight(Square* fromthis, Square* tothat);
    bool moverook(Square* fromthis, Square* tothat);
    bool movepawn(Square* fromthis, Square* tothat);
    bool makemove(int a1, int b1, int a2, int b2);
    void printboard();
    Square* getSquare(int x, int y) { return &board[x][y]; }
    void setSquare(Square* s1, int x, int y) { board[x][y] = *s1; }
    bool domove();
    void setboard();
    bool playgame();
};

void Square::setSpace(Square* s1) {
    colour = s1->getColour();
    piece = s1->getPiece();
}

void Square::setEmpty() {
    colour = NOCOL;
    piece = EMPTY;
}

Piece Square::getPiece() {
    return piece;
}

Colour Square::getColour() {
    return colour;
}

void Square::setPieceandColour(Piece p, Colour c) {
    piece = p;
    colour = c;
}

void Board::printboard() {
    cout << "-------Neeraj's Chess-------" << endl;
    for (int i = 0; i < 8; i++) {
        cout << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            Piece p = board[i][j].getPiece();
            Colour c = board[i][j].getColour();
            switch (p) {
            case KING: (c == WHITE) ? cout << "[K]" : cout << "[k]";
                break;
            case QUEEN: (c == WHITE) ? cout << "[Q]" : cout << "[q]";
                break;
            case BISHOP: (c == WHITE) ? cout << "[B]" : cout << "[b]";
                break;
            case ROOK: (c == WHITE) ? cout << "[R]" : cout << "[r]";
                break;
            case PAWN: (c == WHITE) ? cout << "[P]" : cout << "[p]";
                break;
            case KNIGHT: (c == WHITE) ? cout << "[N]" : cout << "[n]";
                break;
            case EMPTY: cout << "[ ]";
                break;
            }
        }
        cout << endl;
    }
    cout << "   a  b  c  d  e  f  g  h" << endl;
}

bool Board::domove() {
    string move;
    int x1, x2, y1, y2;
    bool stop = false;
    while (!stop) {
        if (turn == WHITE) {
            cout << "White's Turn" << endl;
        }
        else {
            cout << "Black's Move " << endl;
        }
        cin >> move;
        if (move.length() != 4) {
            cout << "Invalid input format. Please use the format 'e2e4'." << endl;
            continue;
        }
        x1 = move[0] - 'a';
        y1 = 7 - (move[1] - '1');
        x2 = move[2] - 'a';
        y2 = 7 - (move[3] - '1');
        if (x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7 || x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7) {
            cout << "One of your inputs was out of bounds." << endl;
            continue;
        }
        if (getSquare(x1, y1)->getColour() == turn) {
            if (makemove(x1, y1, x2, y2) == false) {
                cout << "Invalid move" << endl;
            }
            else {
                stop = true;
            }
        }
        else {
            cout << "You can't move the opponent's piece, try again." << endl;
        }
        if (getSquare(x2, y2)->getPiece() == KING) {
            if (getSquare(x1, y1)->getColour() == WHITE) {
                cout << "WHITE WINS" << endl;
                return false;
            }
            else {
                cout << "BLACK WINS" << endl;
                return false;
            }
        }
    }
    turn = (turn == BLACK) ? WHITE : BLACK;
    return true;
}

void Board::setboard() {
    board[0][0].setPieceandColour(ROOK, WHITE);
    board[0][1].setPieceandColour(KNIGHT, WHITE);
    board[0][2].setPieceandColour(BISHOP, WHITE);
    board[0][3].setPieceandColour(QUEEN, WHITE);
    board[0][4].setPieceandColour(KING, WHITE);
    board[0][5].setPieceandColour(BISHOP, WHITE);
    board[0][6].setPieceandColour(KNIGHT, WHITE);
    board[0][7].setPieceandColour(ROOK, WHITE);
    board[7][0].setPieceandColour(ROOK, BLACK);
    board[7][1].setPieceandColour(KNIGHT, BLACK);
    board[7][2].setPieceandColour(BISHOP, BLACK);
    board[7][3].setPieceandColour(QUEEN, BLACK);
    board[7][4].setPieceandColour(KING, BLACK);
    board[7][5].setPieceandColour(BISHOP, BLACK);
    board[7][6].setPieceandColour(KNIGHT, BLACK);
    board[7][7].setPieceandColour(ROOK, BLACK);
    for (int i = 0; i < 8; i++) {
        board[1][i].setPieceandColour(PAWN, WHITE);
        board[6][i].setPieceandColour(PAWN, BLACK);
    }
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j].setPieceandColour(EMPTY, NOCOL);
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j].setx(i);
            board[i][j].sety(j);
        }
    }
}

bool Board::playgame() {
    system("cls");
    printboard();
    return domove();
}

bool Board::moveking(Square* from, Square* to) {
    if (abs(to->getx() - from->getx()) <= 1 && abs(to->gety() - from->gety()) <= 1) {
        to->setSpace(from);
        from->setEmpty();
        return true;
    }
    return false;
}

bool Board::movequeen(Square* from, Square* to) {
    int fromx = from->getx();
    int fromy = from->gety();
    int tox = to->getx();
    int toy = to->gety();
    int yplus, xplus;
    if (fromx == tox) {
        yplus = (toy - fromy) / abs(toy - fromy);
        for (int i = fromy + yplus; i != toy; i += yplus) {
            if (board[fromx][i].getColour() != NOCOL) return false;
        }
    }
    else if (fromy == toy) {
        xplus = (tox - fromx) / abs(tox - fromx);
        for (int i = fromx + xplus; i != tox; i += xplus) {
            if (board[i][fromy].getColour() != NOCOL) return false;
        }
    }
    else if ((tox - fromx) == (toy - fromy) || (tox - fromx) == (fromy - toy)) {
        xplus = (tox - fromx) / abs(tox - fromx);
        yplus = (toy - fromy) / abs(toy - fromy);
        for (int i = fromx + xplus, j = fromy + yplus; i != tox; i += xplus, j += yplus) {
            if (board[i][j].getColour() != NOCOL) return false;
        }
    }
    else {
        return false;
    }
    to->setSpace(from);
    from->setEmpty();
    return true;
}

bool Board::movebishop(Square* from, Square* to) {
    int fromx = from->getx();
    int fromy = from->gety();
    int tox = to->getx();
    int toy = to->gety();
    int yplus, xplus;
    if ((tox - fromx) == (toy - fromy) || (tox - fromx) == (fromy - toy)) {
        xplus = (tox - fromx) / abs(tox - fromx);
        yplus = (toy - fromy) / abs(toy - fromy);
        for (int i = fromx + xplus, j = fromy + yplus; i != tox; i += xplus, j += yplus) {
            if (board[i][j].getColour() != NOCOL) return false;
        }
    }
    else {
        return false;
    }
    to->setSpace(from);
    from->setEmpty();
    return true;
}

bool Board::moveknight(Square* from, Square* to) {
    int x = abs(from->getx() - to->getx());
    int y = abs(from->gety() - to->gety());
    if ((x == 1 && y == 2) || (x == 2 && y == 1)) {
        to->setSpace(from);
        from->setEmpty();
        return true;
    }
    return false;
}

bool Board::moverook(Square* from, Square* to) {
    int fromx = from->getx();
    int fromy = from->gety();
    int tox = to->getx();
    int toy = to->gety();
    int yplus, xplus;
    if (fromx == tox) {
        yplus = (toy - fromy) / abs(toy - fromy);
        for (int i = fromy + yplus; i != toy; i += yplus) {
            if (board[fromx][i].getColour() != NOCOL) return false;
        }
    }
    else if (fromy == toy) {
        xplus = (tox - fromx) / abs(tox - fromx);
        for (int i = fromx + xplus; i != tox; i += xplus) {
            if (board[i][fromy].getColour() != NOCOL) return false;
        }
    }
    else {
        return false;
    }
       to->setSpace(from);
       from->setEmpty();
    return true;
}

bool Board::movepawn(Square* from, Square* to) {
    int fromx = from->getx();
    int fromy = from->gety();
    int tox = to->getx();
    int toy = to->gety();
    if (from->getColour() == WHITE) {
        if (fromy == 1 && toy == 3 && fromx == tox) {
            if (board[2][fromx].getPiece() == EMPTY && board[3][fromx].getPiece() == EMPTY) {
                to->setSpace(from);
                from->setEmpty();
                return true;
            }
        }
        else if (toy == fromy + 1 && fromx == tox && to->getPiece() == EMPTY) {
            to->setSpace(from);
            from->setEmpty();
            return true;
        }
        else if (toy == fromy + 1 && (tox == fromx + 1 || tox == fromx - 1) && to->getColour() == BLACK) {
            to->setSpace(from);
            from->setEmpty();
            return true;
        }
    }
    else {
        if (fromy == 6 && toy == 4 && fromx == tox) {
            if (board[5][fromx].getPiece() == EMPTY && board[4][fromx].getPiece() == EMPTY) {
                to->setSpace(from);
                from->setEmpty();
                return true;
            }
        }
        else if (toy == fromy - 1 && fromx == tox && to->getPiece() == EMPTY) {
            to->setSpace(from);
            from->setEmpty();
            return true;
        }
        else if (toy == fromy - 1 && (tox == fromx + 1 || tox == fromx - 1) && to->getColour() == WHITE) {
            to->setSpace(from);
            from->setEmpty();
            return true;
        }
    }
    return false;
}

bool Board::makemove(int a1, int b1, int a2, int b2) {
    Square* s1 = getSquare(a1, b1);
    Square* s2 = getSquare(a2, b2);
    Piece p = s1->getPiece();
    switch (p) {
    case KING:
        return moveking(s1, s2);
    case QUEEN:
        return movequeen(s1, s2);
    case BISHOP:
        return movebishop(s1, s2);
    case KNIGHT:
        return moveknight(s1, s2);
    case ROOK:
        return moverook(s1, s2);
    case PAWN:
        return movepawn(s1, s2);
    case EMPTY:
        return false;
    }
    return false;
}

int main() {
    try {
        Board b1;
        char ch;
        while (1) {
            b1.setboard();
            while (b1.playgame());
            cout << "Do you want to play again? (y/n): ";
            cin >> ch;
            if (ch != 'y') break;
        }
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }
    return 0;
}
