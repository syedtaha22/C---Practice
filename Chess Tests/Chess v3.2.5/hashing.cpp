#include<iostream>
#include<unordered_map>
#include <cmath>
#include <random> 

using namespace std;

const int Total_tiles = 64;

const char EMPTY = ' ';
const char PAWN = 'P';
const char ROOK = 'R';
const char KNIGHT = 'N';
const char BISHOP = 'B';
const char QUEEN = 'Q';
const char KING = 'K';


class ChessPiece {
public:
    char type;
    char color;
    bool firstMove;
    ChessPiece(char pieceType = EMPTY, char pieceColor = EMPTY, bool isFirstMove = true)
        : type(pieceType), color(pieceColor), firstMove(true) {}
};


class ChessBoard {
private:

    const int pawnScore = 1;
    const int rookScore = 5;
    const int knightScore = 3;
    const int bishopScore = 3;
    const int queenScore = 9;
    const int kingScore = 100;
    bool currentPlayerIsWhite;

public:

    ChessPiece board[Total_tiles];

    void initializeBoardFromFEN(const string& fen) {
        int row = 0, col = 0;

        for (char symbol : fen) {
            if (symbol == ' ') {
                break;  // Stop parsing after the board section
            }

            if (symbol == '/') {
                row++;
                col = 0;
            }
            else if (isdigit(symbol)) {
                col += (symbol - '0');
            }
            else {
                char pieceColor = isupper(symbol) ? 'W' : 'B';
                board[row * 8 + col] = ChessPiece((symbol), pieceColor);
                col++;
            }
        }

        // Set the current player based on FEN
        currentPlayerIsWhite = (fen.find("w") != string::npos);

        // ...code for additional FEN features
    }

    void DisplayBoard() const {
        cout << "8 ";
        for (int i = 0; i < Total_tiles; i++) {
            // cout << (i/8) + 1  << " ";
            cout << "[" << board[i].type << "]";
            if ((i + 1) % 8 == 0 && (i + 1) < Total_tiles) {
                cout << endl;
                cout << 8 - (i + 1) / 8 << " ";
            }

            // cout << endl;
        }
        cout << endl;
        cout << "   a  b  c  d  e  f  g  h";
    }


};

class TranspositionTables{

    uint64_t zobristKeys[64][12]; // 64 squares, 12 piece types
    unordered_map<uint64_t, pair<float, int>> transpositionTable; // <hash, <score, depth>>


public:

    TranspositionTables(){
        initZobristKeys();
    }

    void initZobristKeys() {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);

        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 12; ++j) {
                zobristKeys[i][j] = dis(gen);
            }
        }
    }

    // Compute hash value for the current board position
    uint64_t computeHash(const ChessBoard& board) const {
        uint64_t hash = 0;
        for (int i = 0; i < 64; ++i) {
            ChessPiece piece = board.board[i];
            if (piece.type != EMPTY) {
                int index = (piece.color == 'W') ? (piece.type - 'A') : (piece.type - 'a' + 6);
                hash ^= zobristKeys[i][index];
            }
        }
        return hash;
    }

    // Search in transposition table
    pair<float, int> lookupTranspositionTable(uint64_t hash) const {
        auto it = transpositionTable.find(hash);
        if (it != transpositionTable.end()) {
            return it->second;
        }
        return { -INFINITY, -1 }; // Not found
    }

    // Store in transposition table
    void storeTranspositionTable(uint64_t hash, float score, int depth) {
        transpositionTable[hash] = { score, depth };
    }

    bool isValuePresent(uint64_t hash) {

        for (const auto& key : transpositionTable) {
            if (key.first == hash) {
                return true;
            }
        }
        return false;
    }

};

int main(){

    ChessBoard chessboard;

    TranspositionTables table;
    string FEN = "rnbqkbnr/ppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    chessboard.initializeBoardFromFEN(FEN);
    chessboard.DisplayBoard();
    auto hash = table.computeHash(chessboard);
    table.storeTranspositionTable(hash, 10, 2);
    pair<float,int> lookup = table.lookupTranspositionTable(2);

    cout << endl << hash << endl;
    cout << lookup.first << " " << lookup.second << endl;

    cout << table.isValuePresent(hash + 1) << endl;
    





    return 0;
}