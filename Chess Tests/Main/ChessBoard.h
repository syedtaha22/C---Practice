#pragma once
#include<iostream>
#include "ChessPiece.h"
using namespace std;

class ChessBoard {
private:
    ChessPiece board[8][8];
    bool currentPlayerIsWhite = true;
    // Scores for each piece type
    const int pawnScore = 1;
    const int rookScore = 5;
    const int knightScore = 3;
    const int bishopScore = 3;
    const int queenScore = 9;
    const int kingScore = 100;

    int getPieceScore(const ChessPiece& piece) const;
    int calculatePlayerScore(char playerColor) const;
    void promotePawn(int toRow, int toCol);
    bool isValidMoveFormat(const string& move) const;
    bool isValidPawnMove(const ChessPiece& piece, int fromRow, int fromCol, int toRow, int toCol, const ChessPiece& targetPiece) const;
    bool isValidBishopMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidRookMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidKnightMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidQueenMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isValidKingMove(int fromRow, int fromCol, int toRow, int toCol) const;


public:
    void initializeBoardFromFEN(const string& fen);
    void initializeBoard();
    void displayBoard() const;
    void makeMove(int fromRow, int fromCol, int toRow, int toCol);
    bool isCurrentPlayerWhite();
    bool isValidMove(const string& move, int& fromRow, int& fromCol, int& toRow, int& toCol) const;

};
