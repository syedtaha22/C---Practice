
#include "ChessBoard.h"


class ChessBot {
    char color;

public:
    ChessBot(char pieceColor) : color(pieceColor) {}

    string GenerateMove(vector<string> possibleMoves) {
        srand(time(0));
        int randomNumber = rand() % possibleMoves.size();
        return possibleMoves[randomNumber];
    }

    void PlayMove(string move, ChessBoard& board) {
        // Extracting coordinates
        int fromCol = move[0] - 'a';
        int fromRow = 7 - (move[1] - '1');
        int toCol = move[2] - 'a';
        int toRow = 7 - (move[3] - '1');
        int fromTile = fromRow * 8 + fromCol;
        int toTile = toRow * 8 + toCol;
        string Player = (color == 'W') ? "White " : "Black ";
        cout << Player << " Plays: " << move << endl;
        board.MakeCompleteMove(fromTile, toTile);
        board.DisplayScores();
        board.DisplayBoard();

    }

};


bool GameIsEnded(ChessBoard board) {
    char playerColor = board.isCurrentPlayerWhite() ? 'W' : 'B';

    if (board.isCheck(board, playerColor)) {
        if (board.isCheckmate(board,playerColor)) {
            cout << "\n\n\n";
            string winner = (board.isCurrentPlayerWhite() ? "Black" : "White");
            cout << "Check Mate. Winner is " << winner << endl << endl;
            return true;

        }
        cout << endl << (board.isCurrentPlayerWhite() ? "White" : "Black") << "'s King is in Check" << endl;
    }

    if (!board.isCheck(board, playerColor) && board.GetAllPossibleMoves(playerColor).size() == 0) {
        cout << "\n\n\n";
        cout << "---Stalemate---" << endl;
        return true;
     

    }
    if (board.calculatePlayerScore('W') < 106 && board.calculatePlayerScore('B') < 106) {
        cout << "\n\n\n";
        cout << "---Draw---" << endl;
        return true;
    }

    return false;

}



 int main() {
     auto start = high_resolution_clock::now();
     ChessBoard chessboard;
     ChessBot Black('B');


     //string initialFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
     // string initialFEN = "K7/7r/6q1/8/8/8/8/2K5 b KQkq - 0 1";
      string initialFEN = "K1k5/8/8/8/8/8/8/8 b KQkq - 0 1";

     chessboard.initializeBoardFromFEN(initialFEN);
     chessboard.DisplayScores();
     chessboard.DisplayBoard();
     int fromTile, toTile;

     string move;

     while(true){
         cout << "\n\n" << (chessboard.isCurrentPlayerWhite() ? "White" : "Black") << "'s turn." << endl;


         do {
             cout << "Enter your move (e.g., e2e4): ";
             cin >> move;

         } while (!chessboard.isValidMove(move, fromTile, toTile));

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

//int main() {
//    auto start = high_resolution_clock::now();
//    ChessBoard chessboard;
//    ChessBot White('W');
//    ChessBot Black('B');
//
//
//    string initialFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
//    // string initialFEN = "K7/7r/6q1/8/8/8/8/2K5 b KQkq - 0 1";
//
//    chessboard.initializeBoardFromFEN(initialFEN);
//    chessboard.DisplayScores();
//    chessboard.DisplayBoard();
//    int fromTile = 0, toTile = 0;
//
//    string move;
//    int total_turns = 0;
//    
//
//    while (true) {
//
//        vector<string> PossibleMovesForWhite = chessboard.GetAllPossibleMovesInChessNotation(chessboard.isCurrentPlayerWhite() ? 'W' : 'B');
//        cout << "\n\n" << (chessboard.isCurrentPlayerWhite() ? "White" : "Black") << "'s turn." << endl;
//        
//        cout << "Total Possible Moves: " << PossibleMovesForWhite.size() << endl;
//
//        move = White.GenerateMove(PossibleMovesForWhite);
//        White.PlayMove(move, chessboard);
//
//        if (GameIsEnded(chessboard)) {
//            break;
//        }
//
//
//        vector<string> PossibleMovesForBlack = chessboard.GetAllPossibleMovesInChessNotation(chessboard.isCurrentPlayerWhite() ? 'W' : 'B');
//        cout << "\n\n" << (chessboard.isCurrentPlayerWhite() ? "White" : "Black") << "'s turn." << endl;
//        cout << "Total Possible Moves: " << PossibleMovesForBlack.size() << endl;
//
//        move = Black.GenerateMove(PossibleMovesForBlack);
//        Black.PlayMove(move, chessboard);
//
//        if (GameIsEnded(chessboard)) {
//            break;
//        }
//
//        total_turns++;
//
//
//    }
//
//    cout << "\nTurn Total Turns Taken: " << total_turns << endl;
//    auto end = high_resolution_clock::now();
//    auto duration = duration_cast<milliseconds>(end - start);
//    cout << "Time taken to execute main function: " << duration.count() << " milliseconds" << endl;
//    return 0;
//}

