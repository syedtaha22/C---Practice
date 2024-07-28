#include "ChessBoard.h"
// #include "ChessBot.h"

bool GameIsEnded(ChessBoard board){

    if(board.isCheck(board, (board.isCurrentPlayerWhite()) ? 'W' : 'B')){
        if(board.isCheckmate(board, (board.isCurrentPlayerWhite()) ? 'W' : 'B')){
            return true;

        }
        cout << endl << (board.isCurrentPlayerWhite() ? "White" : "Black") << "'s King is in Check" << endl;
    }

    if(board.calculatePlayerScore('W') < 106 && board.calculatePlayerScore('B') < 106){
        cout << "Draw" << endl;
        return true;
    }

    return false;
    
}


int main() {
    auto start = high_resolution_clock::now();
    ChessBoard chessboard;
    string initialFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    // string initialFEN = "3R4/4B3/3r4/8/3Q4/8/2n5/8 w KQkq - 0 1";

    chessboard.initializeBoardFromFEN(initialFEN);
    chessboard.DisplayScores();
    chessboard.DisplayBoard();
    int fromTile, toTile;

    string move[8] = {"e2e4", "a7a6", "d1f3", "a6a5", "f1c4", "a5a4", "f3f7", "a4a3" };
    for(int i = 0; i<8;i++){
        cout << "\n\n" << (chessboard.isCurrentPlayerWhite() ? "White" : "Black") << "'s turn." << endl;
        do {
            cout << "Enter your move (e.g., e2e4): " << move[i] << endl;

        } while (!chessboard.isValidMove(move[i], fromTile, toTile));
        chessboard.MakeCompleteMove(fromTile, toTile);
        chessboard.DisplayScores();
        chessboard.DisplayBoard();
        if(GameIsEnded(chessboard)){    
            break;
        }

    }

    cout << "\n\n\n";
    char winner = (chessboard.isCurrentPlayerWhite() ? 'B' : 'W');
    cout <<"Winner is " << winner << endl << endl;
    

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "\n\nTime taken to execute main function: " << duration.count() << " milliseconds" << endl;
    return 0;
}