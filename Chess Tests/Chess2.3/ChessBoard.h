#pragma once
#include "ChessPiece.h"

class ChessBoard {
private:
    ChessPiece board[Total_tiles];
    bool currentPlayerIsWhite = true;

    vector<int> SlidingPieceMoveData;
    vector<int> PawnMoveData;
    vector<int> KnightMoveData;
    vector<int> KingMoveData;
    // Scores for each piece type
    const int pawnScore = 1;
    const int rookScore = 5;
    const int knightScore = 3;
    const int bishopScore = 3;
    const int queenScore = 9;
    const int kingScore = 100;

    int getPieceScore(const ChessPiece& piece) const;
    void promotePawn(int toTile);
    vector<int> FilterValidMoves(int fromIndex, vector<int> possibleMoves) const;

    bool isValidKingMove(int fromIndex, int toIndex, vector<int> possibleMoves) const;
    bool isValidKnightMove(int fromIndex, int toIndex, vector<int> possibleMoves) const;
    bool isValidPawnMove(int fromIndex, int toIndex, vector<int> possibleMoves) const;
    bool isValidSlidingPieceMove(int fromIndex, int toIndex, vector<int> possibleMoves) const;
    bool isValidMoveFormat(const string& move) const;




public:


    int calculatePlayerScore(char playerColor) const;
    void initializeBoard();
    void initializeBoardFromFEN(const string& fen);
    void DisplayScores();
    void DisplayBoard() const;
    void MakeCompleteMove(int fromTile, int toTile);
    void MakeMove(int fromTile, int toTile);
    void ComputeKingMoves(int pieceIndex, vector<int>& possibleMoves) const;
    void ComputeKnightMoves(int pieceIndex, vector<int>& possibleMoves) const;
    void ComputePawnMoves(int pieceIndex, vector<int>& possibleMoves) const;
    void ComputeSlidingPieceMoves(int pieceIndex, vector<int>& possibleMoves) const;
    bool isCurrentPlayerWhite();
    void NotAValidMove(ChessBoard CurrentBoard, char playercolor, char piecetype) const;
    bool isCheck(const ChessBoard& chessboard, char playerColor) const;
    bool isCheckmate(ChessBoard& chessboard, char playerColor) const;
    bool isValidMove(const string& move, int& fromTile, int& toTile) const;
    vector<int> GetAllPossibleMoves(char playerColor) const;
    vector<string> GetAllPossibleMovesInChessNotation(char playerColor);
    string ConvertToChessNotation(int fromTile, int toTile);
};