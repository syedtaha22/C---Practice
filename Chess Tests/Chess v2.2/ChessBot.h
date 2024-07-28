#pragma once
#include "ChessBoard.h"

class ChessBot{
    char color;


public:

    ChessBot (char pieceColor) : color(pieceColor){}
    string GenerateMove(vector<string> possibleMoves);
    void PlayMove(string move, ChessBoard &board);


};