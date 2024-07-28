#pragma once
#include "GlobalConstants.h"
#include<iostream>
using namespace std;

class ChessPiece {
public:
    char type;
    char color;
    bool firstMove;
    ChessPiece(char pieceType = EMPTY, char pieceColor = EMPTY, bool isFirstMove = true)
        : type(pieceType), color(pieceColor), firstMove(true) {}
};

