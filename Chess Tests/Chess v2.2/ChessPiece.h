#pragma once
#include "GlobalVariables.h"

class ChessPiece {
public:
    char type;
    char color;
    bool firstMove;
    bool isSlidingPiece;
    ChessPiece(char pieceType = EMPTY, char pieceColor = EMPTY, bool isFirstMove = true)
        : type(pieceType), color(pieceColor), firstMove(true) {
            isSlidingPiece = (toupper(type) == ROOK) || (toupper(type)  == BISHOP) || (toupper(type)  == QUEEN);
        }
};

