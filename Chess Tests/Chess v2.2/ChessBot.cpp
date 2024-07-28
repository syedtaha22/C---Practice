#include "ChessBot.h"

string ChessBot::GenerateMove(vector<string> possibleMoves){

    srand(time(0));
    int randomNumber = rand() % possibleMoves.size();

    return possibleMoves[randomNumber];
}

void ChessBot::PlayMove(string move, ChessBoard &board){
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
