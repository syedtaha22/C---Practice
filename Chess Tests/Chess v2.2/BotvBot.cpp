// #include "ChessBoard.h"
#include "ChessBot.h"

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
    ChessBot White('W');
    ChessBot Black('B');


    string initialFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    // string initialFEN = "K7/7r/6q1/8/8/8/8/2K5 b KQkq - 0 1";

    chessboard.initializeBoardFromFEN(initialFEN);
    chessboard.DisplayScores();
    chessboard.DisplayBoard();
    int fromTile, toTile;

    string move;

    while(true){
        
        vector<string> PossibleMovesForWhite = chessboard.GetAllPossibleMovesInChessNotation(chessboard.isCurrentPlayerWhite() ? 'W' : 'B');
        cout << "\n\n" << (chessboard.isCurrentPlayerWhite() ? "White" : "Black") << "'s turn." << endl;
        cout << "Total Possible Moves: " <<  PossibleMovesForWhite.size() << endl;

        move = White.GenerateMove(PossibleMovesForWhite);
        White.PlayMove(move, chessboard);

        if(GameIsEnded(chessboard)){    
            break;
        }


        vector<string> PossibleMovesForBlack = chessboard.GetAllPossibleMovesInChessNotation(chessboard.isCurrentPlayerWhite() ? 'W' : 'B');
        cout << "\n\n" << (chessboard.isCurrentPlayerWhite() ? "White" : "Black") << "'s turn." << endl;
        cout << "Total Possible Moves: " <<  PossibleMovesForBlack.size() << endl;

        move = Black.GenerateMove(PossibleMovesForBlack);
        Black.PlayMove(move, chessboard);

        if(GameIsEnded(chessboard)){    
            break;
        }


    }

    cout << "\n\n\n";
    string winner = (chessboard.isCurrentPlayerWhite() ? "Black" : "White");
    cout <<"Winner is " << winner << endl << endl;
    

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "\n\nTime taken to execute main function: " << duration.count() << " milliseconds" << endl;
    return 0;
}