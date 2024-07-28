#include<iostream>
using namespace std;



//Store Pieces Characterss
const char EMPTY = ' ';
const char PAWN = 'P';
const char ROOK = 'R';
const char KNIGHT = 'N';
const char BISHOP = 'B';
const char QUEEN = 'Q';
const char KING = 'K';

class Displaychess{
	char board[63];
public:
	Displaychess(){
        initBoard();
    }

    void initBoard(){
        // Set up the board with pieces in their initial positions

        // Black pieces
        board[0] = tolower(ROOK);
        board[1] = tolower(KNIGHT);
        board[2] = tolower(BISHOP);
        board[3] = tolower(QUEEN);
        board[4] = tolower(KING);
        board[5] = tolower(BISHOP);
        board[6] = tolower(KNIGHT);
        board[7] = tolower(ROOK);
        for (int i = 8; i <= 15; i++) {
            board[i] = tolower(PAWN);
        }

        // Empty spaces in the middle
        for (int i = 16; i <= 47; i++) {
            board[i] = EMPTY;

        }

        // White pieces
        for (int i = 48; i <= 55; i++) {
            board[i] = PAWN;
        }
        board[56] = ROOK;
        board[57] = KNIGHT;
        board[58] = BISHOP;
        board[59] = QUEEN;
        board[60] = KING;
        board[61] = BISHOP;
        board[62] = KNIGHT;
        board[63] = ROOK;
        
    }


	void displayboard() {
		for (int i = 1; i < 65; i++) {
			cout << "[" << board[i - 1] << "]";
			if (i % 8 == 0 && i > 0) {
				cout << endl;
			}
		}
	}
};
int main() {
	Displaychess chessboard;
	chessboard.displayboard();
	return 0;
}