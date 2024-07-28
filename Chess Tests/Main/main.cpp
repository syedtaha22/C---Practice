#include "ChessBoard.h"


int main() {
    ChessBoard chessboard;
    string initialFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    //string initialFEN = "8/8/8/4p1Kq/2k1P3/8/8/8 b - - 0 1";

    chessboard.initializeBoardFromFEN(initialFEN);
    chessboard.displayBoard();
    int fromRow, fromCol, toRow, toCol;
    while (true) {
        cout << "\n\n" << (chessboard.isCurrentPlayerWhite() ? "White" : "Black") << "'s turn." << endl;
        string move;
        do {
            cout << "Enter your move (e.g., e2e4): ";
            cin >> move;
        } while (!chessboard.isValidMove(move, fromRow, fromCol, toRow, toCol));

        chessboard.makeMove(fromRow, fromCol, toRow, toCol);

        cout << endl;
        chessboard.displayBoard();
    }
    return 0;
}

