#pragma once
#include "GlobalVariables.h"
#include "ChessPiece.h"

using namespace std;

class ChessBoard {
private:
    ChessPiece board[64];
    bool currentPlayerIsWhite = true;
    // Scores for each piece type
    const int pawnScore = 1;
    const int rookScore = 5;
    const int knightScore = 3;
    const int bishopScore = 3;
    const int queenScore = 9;
    const int kingScore = 100;


    int getPieceScore(const ChessPiece& piece) const;

    void promotePawn(int toTile);

    bool isValidMoveFormat(const string& move) const;

    bool isValidKingMove(int fromIndex, int toIndex, vector<int>& possibleMoves) const;
    bool isValidKnightMove(int fromIndex, int toIndex, vector<int>& possibleMoves) const;
    bool isValidPawnMove(int fromIndex, int toIndex, vector<int>& possibleMoves) const;
    bool isValidSlidingPieceMove(int fromIndex, int toIndex, vector<int>& possibleMoves) const;


public:

    int calculatePlayerScore(char playerColor) const;

    void ComputeKingMoves(int pieceIndex, vector<int>& possibleMoves) const;
    void ComputeKnightMoves(int pieceIndex, vector<int>& possibleMoves) const;
    void ComputePawnMoves(int pieceIndex, vector<int>& possibleMoves) const;
    void ComputeSlidingPieceMoves(int pieceIndex, vector<int>& possibleMoves) const;

    void initializeBoard();
    void initializeBoardFromFEN(const string& fen);

    void DisplayBoard() const;
    void DisplayScores();

    void MakeMove(int fromTile, int toTile);
    void MakeCompleteMove(int fromTile, int toTile);

    bool isCurrentPlayerWhite();
    bool isCheck(const ChessBoard& chessboard, char playerColor) const;
    bool isCheckmate(ChessBoard& chessboard, char playerColor) const;
    bool isValidMove(const string& move, int& fromTile, int& toTile) const;

    vector<int> GetAllPossibleMoves(char playerColor) const;
    vector<string> GetAllPossibleMovesInChessNotation(char playerColor);

    string ConvertToChessNotation(int fromTile, int toTile);
};

