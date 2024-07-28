
#include<iostream>
#include <chrono>
#include<vector>
#include<cmath>
#include<string>

#include "raylib.h"

using namespace std;
using namespace std::chrono;

const char EMPTY = ' ';
const char PAWN = 'P';
const char ROOK = 'R';
const char KNIGHT = 'N';
const char BISHOP = 'B';
const char QUEEN = 'Q';
const char KING = 'K';



const int Total_tiles = 64;
enum class PromotionPiece { QUEEN, ROOK, KNIGHT, BISHOP };


class ChessPiece {
public:
    char type;
    char color;
    bool firstMove;
    bool isSlidingPiece;
    bool PawnMovedTwoSpaces;
    ChessPiece(char pieceType = EMPTY, char pieceColor = EMPTY, bool isFirstMove = true, bool pawnmovedtwospaces = false)
        : type(pieceType), color(pieceColor), firstMove(true), PawnMovedTwoSpaces(false) {
        isSlidingPiece = (toupper(type) == ROOK) || (toupper(type) == BISHOP) || (toupper(type) == QUEEN);
        PawnMovedTwoSpaces = false;
    }
};


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

public:
    int getPieceScore(const ChessPiece& piece) const {
        switch (toupper(piece.type)) {
        case PAWN:
            return pawnScore;
        case ROOK:
            return rookScore;
        case KNIGHT:
            return knightScore;
        case BISHOP:
            return bishopScore;
        case QUEEN:
            return queenScore;
        case KING:
            return kingScore;
        default:
            return 0;  // Unknown piece type
        }
    }

    void promotePawn(int toTile) {
        cout << "\nPawn promotion! Choose a piece (Q, R, N, B): ";
        char promotionChoice;
        cin >> promotionChoice;

        ChessPiece promotedPiece;

        switch (toupper(promotionChoice)) {
        case 'Q':
            promotedPiece = ChessPiece(currentPlayerIsWhite ? toupper(QUEEN) : tolower(QUEEN), currentPlayerIsWhite ? 'W' : 'B');
            cout << "Piece Promoted to Queen" << endl;
            break;
        case 'R':
            promotedPiece = ChessPiece(currentPlayerIsWhite ? toupper(ROOK) : tolower(ROOK), currentPlayerIsWhite ? 'W' : 'B');
            cout << "Piece Promoted to Rook" << endl;
            break;
        case 'N':
            promotedPiece = ChessPiece(currentPlayerIsWhite ? toupper(KNIGHT) : tolower(KNIGHT), currentPlayerIsWhite ? 'W' : 'B');
            cout << "Piece Promoted to Knight" << endl;
            break;
        case 'B':
            promotedPiece = ChessPiece(currentPlayerIsWhite ? toupper(BISHOP) : tolower(BISHOP), currentPlayerIsWhite ? 'W' : 'B');
            cout << "Piece Promoted to Bishop" << endl;
            break;
        default:
            cout << "Invalid choice. Defaulting to Queen promotion." << endl;
            promotedPiece = ChessPiece(currentPlayerIsWhite ? toupper(QUEEN) : tolower(QUEEN), currentPlayerIsWhite ? 'W' : 'B');
            cout << "Piece Promoted to Queen" << endl;
            break;
        }

        // Update the board with the promoted piece
        board[toTile] = promotedPiece;
    }

    bool isValidMoveFormat(const string& move) const {
        return (move.length() == 4 &&
            move[0] >= 'a' && move[0] <= 'h' &&
            move[1] >= '1' && move[1] <= '8' &&
            move[2] >= 'a' && move[2] <= 'h' &&
            move[3] >= '1' && move[3] <= '8');
    }

    vector<int> FilterValidMoves(int fromIndex, vector<int> possibleMoves) const {
        ChessPiece piece = board[fromIndex];

        // Filter out moves that leave the king in check
        vector<int> filteredMoves;
        for (int toTile : possibleMoves) {
            // Simulate the move on a temporary board
            ChessBoard tempBoard = *this;
            tempBoard.MakeMove(fromIndex, toTile);

            // Check if the king is in check after the move
            if (!tempBoard.isCheck(tempBoard, piece.color)) {
                filteredMoves.push_back(toTile);
            }
        }
        return filteredMoves;

    }

    bool isValidPawnMove(int fromIndex, int toIndex, vector<int> possibleMoves)  {
        ComputePawnMoves(fromIndex, possibleMoves);
        possibleMoves = FilterValidMoves(fromIndex, possibleMoves);
        for (int move : possibleMoves) {
            if (move == toIndex) {
                return true;
            }
        }
        return false;
    }

    bool isValidSlidingPieceMove(int fromIndex, int toIndex, vector<int> possibleMoves) const {
        ComputeSlidingPieceMoves(fromIndex, possibleMoves);
        possibleMoves = FilterValidMoves(fromIndex, possibleMoves);
        for (int move : possibleMoves) {
            if (move == toIndex) {
                return true;
            }
        }
        return false;
    }

    bool isValidKnightMove(int fromIndex, int toIndex, vector<int> possibleMoves) const {
        ComputeKnightMoves(fromIndex, possibleMoves);
        possibleMoves = FilterValidMoves(fromIndex, possibleMoves);
        for (int move : possibleMoves) {
            if (move == toIndex) {
                return true;
            }
        }
        return false;
    }

    bool isValidKingMove(int fromIndex, int toIndex, vector<int> possibleMoves)  {
        ComputeKingMoves(fromIndex, possibleMoves);
        AddCastlingMoves(fromIndex, possibleMoves);
        possibleMoves = FilterValidMoves(fromIndex, possibleMoves);
        for (int move : possibleMoves) {
            if (move == toIndex) {
                return true;
            }
        }
        return false;
    }

    int calculatePlayerScore(char playerColor) const {
        int totalScore = 0;
        for (int i = 0; i < Total_tiles; i++) {
            if (board[i].color == playerColor) {
                totalScore += getPieceScore(board[i]);
            }
        }
        return totalScore;
    }

    void initializeBoardFromFEN(const string& fen) {
        int row = 0, col = 0;

        for (char symbol : fen) {
            if (symbol == ' ') {
                break;  // Stop parsing after the board section
            }

            if (symbol == '/') {
                row++;
                col = 0;
            }
            else if (isdigit(symbol)) {
                col += (symbol - '0');
            }
            else {
                char pieceColor = isupper(symbol) ? 'W' : 'B';
                board[row * 8 + col] = ChessPiece((symbol), pieceColor);
                col++;
            }
        }

        // Set the current player based on FEN
        currentPlayerIsWhite = (fen.find("w") != string::npos);

        // ...code for additional FEN features
    }

    void initializeBoard() {
        // Set up the board with pieces in their initial positions

        // Black pieces
        board[0] = ChessPiece(tolower(ROOK), 'B');
        board[1] = ChessPiece(tolower(KNIGHT), 'B');
        board[2] = ChessPiece(tolower(BISHOP), 'B');
        board[3] = ChessPiece(tolower(QUEEN), 'B');
        board[4] = ChessPiece(tolower(KING), 'B');
        board[5] = ChessPiece(tolower(BISHOP), 'B');
        board[6] = ChessPiece(tolower(KNIGHT), 'B');
        board[7] = ChessPiece(tolower(ROOK), 'B');
        for (int i = 8; i <= 15; i++) {
            board[i] = ChessPiece(tolower(PAWN), 'B');
        }

        // Empty spaces in the middle
        for (int i = 16; i <= 47; i++) {
            board[i] = ChessPiece(EMPTY, EMPTY);

        }

        // White pieces
        for (int i = 48; i <= 55; i++) {
            board[i] = ChessPiece((PAWN), 'W');
        }
        board[56] = ChessPiece((ROOK), 'W');
        board[57] = ChessPiece((KNIGHT), 'W');
        board[58] = ChessPiece((BISHOP), 'W');
        board[59] = ChessPiece((QUEEN), 'W');
        board[60] = ChessPiece((KING), 'W');
        board[61] = ChessPiece((BISHOP), 'W');
        board[62] = ChessPiece((KNIGHT), 'W');
        board[63] = ChessPiece((ROOK), 'W');
    }

    void DisplayBoard() const {
        cout << "8 ";
        for (int i = 0; i < Total_tiles; i++) {
            // cout << (i/8) + 1  << " ";
            cout << "[" << board[i].type << "]";
            if ((i + 1) % 8 == 0 && (i + 1) < 64) {
                cout << endl;
                cout << 8 - (i + 1) / 8 << " ";
            }

            // cout << endl;
        }
        cout << endl;
        cout << "   a  b  c  d  e  f  g  h";
    }

    void MakeMove(int fromTile, int toTile) {
        board[fromTile].firstMove = false;
        board[toTile] = board[fromTile];
        board[fromTile] = ChessPiece(); // Empty the source square

    }

    void MakeCompleteMove(int fromTile, int toTile) {
        MakeMove(fromTile, toTile);

        // Check for pawn promotion
        if (toupper(board[toTile].type) == PAWN) {
            // White pawn promotion
            if (toTile >= 0 && toTile <= 7) {
                promotePawn(toTile);
            }
            // Black pawn promotion
            else if (toTile >= 56 && toTile <= 63) {
                promotePawn(toTile);
            }

        }

        currentPlayerIsWhite = !currentPlayerIsWhite;

        SlidingPieceMoveData.clear();
        PawnMoveData.clear();
        KnightMoveData.clear();
        KingMoveData.clear();

    }

    bool isCurrentPlayerWhite() {
        return currentPlayerIsWhite;
    }

    void DisplayScores() {
        int whiteScore = calculatePlayerScore('W');
        int blackScore = calculatePlayerScore('B');
        cout << endl << "White score: " << whiteScore << " | Black score: " << blackScore << endl << endl;
        SlidingPieceMoveData.clear();
    }

    void ComputeSlidingPieceMoves(int pieceIndex, vector<int>& possibleMoves) const {
        ChessPiece piece = board[pieceIndex];
        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        // Offsets for sliding directions: up, down, left, right, diagonals
        vector<int> offsets = { 8, -8, 1, -1, 7, -7, 9, -9 };

        if (toupper(piece.type) == ROOK || toupper(piece.type) == QUEEN) {
            for (int i = 0; i < 4; ++i) {
                int offset = offsets[i];
                int newIndex = pieceIndex + offset;
                while (newIndex >= 0 && newIndex < 64) {
                    // Check if the new index is within the bounds of the board
                    if ((offset == 1 || offset == -1) && newIndex / 8 != row)
                        break;  // If moving horizontally, ensure it stays on the same row
                    if (board[newIndex].type != EMPTY) {
                        if (board[newIndex].color != piece.color) {
                            // If the square contains an enemy piece, add it to possibleMoves and stop searching in this direction
                            possibleMoves.push_back(newIndex);
                        }
                        break; // Stop searching in this direction if we encounter a piece
                    }
                    possibleMoves.push_back(newIndex);
                    newIndex += offset;
                }
            }
        }

        if (toupper(piece.type) == BISHOP || toupper(piece.type) == QUEEN) {
            for (int i = 4; i < 8; ++i) {
                int offset = offsets[i];
                int newIndex = pieceIndex + offset;
                while (newIndex >= 0 && newIndex < 64) {
                    // Check if the new index is within the bounds of the board
                    int newRow = newIndex / 8;
                    int newCol = newIndex % 8;
                    if (abs(newRow - row) != abs(newCol - col))
                        break;  // If not moving diagonally, break
                    if (board[newIndex].type != EMPTY) {
                        if (board[newIndex].color != piece.color) {
                            // If the square contains an enemy piece, add it to possibleMoves and stop searching in this direction
                            possibleMoves.push_back(newIndex);
                        }
                        break; // Stop searching in this direction if we encounter a piece
                    }
                    possibleMoves.push_back(newIndex);
                    newIndex += offset;
                }
            }
        }
    }

    void ComputeKnightMoves(int pieceIndex, vector<int>& possibleMoves) const {
        ChessPiece piece = board[pieceIndex];
        // Offsets for knight's moves
        vector<int> rowOffsets = { -2, -1, 1, 2, 2, 1, -1, -2 };
        vector<int> colOffsets = { 1, 2, 2, 1, -1, -2, -2, -1 };

        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        if (toupper(piece.type) == KNIGHT) {
            for (int i = 0; i < 8; ++i) {
                int newRow = row + rowOffsets[i];
                int newCol = col + colOffsets[i];

                // Check if the new position is within the board bounds
                if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                    int newIndex = newRow * 8 + newCol;
                    // Check if the square is empty or contains an opponent's piece
                    if (board[newIndex].type == EMPTY || board[newIndex].color != piece.color) {
                        possibleMoves.push_back(newIndex);
                    }
                }
            }
        }
    }

    // void ComputePawnMoves(int pieceIndex, vector<int>& possibleMoves) const {
    //     ChessPiece piece = board[pieceIndex];
    //     int row = pieceIndex / 8;
    //     int col = pieceIndex % 8;

    //     // Determine the direction of pawn movement based on its color
    //     int direction = (piece.color == 'W') ? -1 : 1;

    //     // Forward movement
    //     int forwardOne = pieceIndex + direction * 8;
    //     if (forwardOne >= 0 && forwardOne < 64 && board[forwardOne].type == EMPTY) {
    //         possibleMoves.push_back(forwardOne);
    //         // Check if the pawn is in its starting position and can move two squares forward
    //         if ((row == 6 && piece.color == 'W') || (row == 1 && piece.color == 'B')) {
    //             int forwardTwo = forwardOne + direction * 8;
    //             if (board[forwardTwo].type == EMPTY) {
    //                 possibleMoves.push_back(forwardTwo);
    //             }
    //         }
    //     }

    //     // Capture moves
    //     vector<int> captureOffsets = { direction * 7, direction * 9 };
    //     for (int offset : captureOffsets) {
    //         int targetIndex = pieceIndex + offset;
    //         int targetRow = targetIndex / 8;
    //         int targetCol = targetIndex % 8;
    //         if (targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8) {
    //             if (board[targetIndex].type != EMPTY && board[targetIndex].color != piece.color) {
    //                 possibleMoves.push_back(targetIndex);
    //             }
    //         }
    //     }
    // }

void ComputePawnMoves(int pieceIndex, vector<int>& possibleMoves)  {
    ChessPiece piece = board[pieceIndex];
    int row = pieceIndex / 8;
    int col = pieceIndex % 8;

    // Determine the direction of pawn movement based on its color
    int direction = (piece.color == 'W') ? -1 : 1;

    // Forward movement
    int forwardOne = pieceIndex + direction * 8;
    if (forwardOne >= 0 && forwardOne < 64 && board[forwardOne].type == EMPTY) {
        possibleMoves.push_back(forwardOne);
        // Check if the pawn is in its starting position and can move two squares forward
        if ((row == 6 && piece.color == 'W') || (row == 1 && piece.color == 'B')) {
            int forwardTwo = forwardOne + direction * 8;
            if (board[forwardTwo].type == EMPTY) {
                possibleMoves.push_back(forwardTwo);
                board[forwardTwo].PawnMovedTwoSpaces = true; // Set the flag when pawn moves two squares
            }
        }
    }

    // Capture moves
    vector<int> captureOffsets = { direction * 7, direction * 9 };
    for (int offset : captureOffsets) {
        int targetIndex = pieceIndex + offset;
        int targetRow = targetIndex / 8;
        int targetCol = targetIndex % 8;
        if (targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8) {
            // Check for en passant capture
            if (board[targetIndex].type == EMPTY && targetCol != col) {
                int enPassantRow = (piece.color == 'W') ? 6 : 3; // Row where the en passant capture can occur
                if (row == enPassantRow && board[targetIndex - (8*direction)].PawnMovedTwoSpaces) { // Check if the pawn moved two spaces
                    cout << endl << endl << targetIndex - (8*direction) << endl << endl;
                    int prevPawnIndex = targetIndex - direction * 8; // Calculate the index of the opponent's pawn in the previous move
                    if (prevPawnIndex >= 0 && prevPawnIndex < 64 && board[prevPawnIndex].type == PAWN && board[prevPawnIndex].firstMove) {
                        possibleMoves.push_back(targetIndex);
                    }
                }
            }
            // Normal capture
            else if (board[targetIndex].type != EMPTY && board[targetIndex].color != piece.color) {
                possibleMoves.push_back(targetIndex);
            }
        }
    }
}




    void ComputeKingMoves(int pieceIndex, vector<int>& possibleMoves) const {
        ChessPiece piece = board[pieceIndex];
        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        // Offsets for king's moves
        vector<int> rowOffsets = { -1, -1, -1, 0, 0, 1, 1, 1 };
        vector<int> colOffsets = { -1, 0, 1, -1, 1, -1, 0, 1 };

        // Check all 8 directions
        for (int i = 0; i < 8; ++i) {
            int newRow = row + rowOffsets[i];
            int newCol = col + colOffsets[i];

            // Check if the new position is within the board bounds
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                int newIndex = newRow * 8 + newCol;
                // Check if the square is empty or contains an opponent's piece
                if ((board[newIndex].type == EMPTY || board[newIndex].color != piece.color)) {
                        possibleMoves.push_back(newIndex);

                }
            }
        }
    }

    void AddCastlingMoves(int pieceIndex, vector<int>& possibleMoves)  {
    ChessPiece piece = board[pieceIndex];
    int row = pieceIndex / 8;
    int col = pieceIndex % 8;

    // Check if it's a king and hasn't moved yet
    if (piece.type == KING && piece.firstMove) {
        // Check if the king is not in check and the squares between the king and rooks are empty
        if (!IsSquareAttacked(pieceIndex, piece.color) && 
            !IsSquareAttacked(pieceIndex + 1, piece.color) && 
            !IsSquareAttacked(pieceIndex + 2, piece.color)) {
            // Kingside castling
            if (board[pieceIndex + 3].type == ROOK && board[pieceIndex + 3].firstMove) {
                possibleMoves.push_back(pieceIndex + 2); // Add kingside castling move
            }
        }
        // Check if the king is not in check and the squares between the king and rooks are empty
        if (!IsSquareAttacked(pieceIndex, piece.color) && 
            !IsSquareAttacked(pieceIndex - 1, piece.color) && 
            !IsSquareAttacked(pieceIndex - 2, piece.color) &&
            !IsSquareAttacked(pieceIndex - 3, piece.color)) {
            // Queenside castling
            if (board[pieceIndex - 4].type == ROOK && board[pieceIndex - 4].firstMove) {
                possibleMoves.push_back(pieceIndex - 2); // Add queenside castling move
            }
        }
    }
}

bool IsSquareAttacked(int pieceIndex, char color){
    ChessBoard temp = *this;
    temp.board[pieceIndex] = ChessPiece((color = 'W')? toupper(KING):tolower(KING) , color);
    if(isCheck(temp, color)){
        return true;
    }
    return false;


}

    void NotAValidMove(ChessBoard CurrentBoard, char playercolor, char piecetype) {
        if (isCheck(CurrentBoard, playercolor)) {
            cout << "King is in Check. ";
        }
        cout << "Invalid move for " << piecetype << endl;
        

    }

    bool isCheck(const ChessBoard& chessboard, char playerColor)  {
        // Find the king's position
        int kingIndex = -1;
        for (int i = 0; i < Total_tiles; i++) {
            if (toupper(chessboard.board[i].type) == KING && chessboard.board[i].color == playerColor) {
                kingIndex = i;
                break;
            }
        }

        if (kingIndex == -1) {
            cout << "\nError Code 404: King not found. \n\n";
            exit(404);
            return false;
        }

        // Iterate through opponent's pieces and see if any can attack the king
        char opponentColor = (playerColor == 'W') ? 'B' : 'W';
        vector<int> possibleMoves;
        for (int i = 0; i < Total_tiles; i++) {
            if (chessboard.board[i].color == opponentColor) {
                if (toupper(chessboard.board[i].type) == ROOK || toupper(chessboard.board[i].type) == BISHOP || toupper(chessboard.board[i].type) == QUEEN) {
                    ComputeSlidingPieceMoves(i, possibleMoves); // Example function, change as needed
                }
                else if (toupper(chessboard.board[i].type) == PAWN) {
                    ComputePawnMoves(i, possibleMoves);
                }
                else if (toupper(chessboard.board[i].type) == KNIGHT) {
                    ComputeKnightMoves(i, possibleMoves);
                }            
                else if (toupper(chessboard.board[i].type) == KING) {
                    ComputeKingMoves(i, possibleMoves);
                }
                


                // Check if any of the possible moves include the king's position
                for (int move : possibleMoves) {
                    if (move == kingIndex) {

                        return true; // King is in check
                    }
                }
            }
        }
        return false; // King is not in check
    }

    bool isCheckmate(ChessBoard& chessboard, char playerColor)  {
        if (!isCheck(chessboard, playerColor)) {
            return false; // King is not in check, so it can't be checkmate
        }

        vector<int> possibleMoves = GetAllPossibleMoves(playerColor);
        if (possibleMoves.size() > 0) {
            return false; // King can escape from check, so it's not checkmate
        }

        return true; // King is in checkmate
    }

    bool isValidMove(const string& move, int& fromTile, int& toTile)  {
        if (!isValidMoveFormat(move)) {
            cout << "Invalid move format. Please use the format 'e2e4'." << endl;
            return false;
        }
        // Extracting coordinates
        int fromCol = move[0] - 'a';
        int fromRow = 7 - (move[1] - '1');
        int toCol = move[2] - 'a';
        int toRow = 7 - (move[3] - '1');
        fromTile = fromRow * 8 + fromCol;
        toTile = toRow * 8 + toCol;
        ChessBoard CurrentBoard = *this;

        // // Validate the move coordinates
        if (fromTile < 0 || fromTile >= 64 || toTile < 0 || toTile >= 64) {
            cout << "Invalid move. Coordinates are out of bounds." << endl;
            return false;
        }

        // Check if the source square is empty
        if (board[fromTile].type == EMPTY) {
            cout << "Invalid move. Source square is empty." << endl;
            return false;
        }

        // Check if the piece being moved belongs to the current player
        char currentPlayerColor = (currentPlayerIsWhite ? 'W' : 'B');
        if (board[fromTile].color != currentPlayerColor) {
            cout << "Invalid move. You can only move your own pieces." << endl;
            return false;
        }

        //Check for Pawns
        if (toupper(board[fromTile].type) == PAWN) {
            if (isValidPawnMove(fromTile, toTile, PawnMoveData)) {
                return true;
            }
            else {
                NotAValidMove(CurrentBoard, currentPlayerColor, board[fromTile].type);
                return false;
            }
        }

        //Check for Queens, Bishops, Rooks
        if (board[fromTile].isSlidingPiece) {
            if (isValidSlidingPieceMove(fromTile, toTile, SlidingPieceMoveData)) {
                return true;
            }
            else {
                NotAValidMove(CurrentBoard, currentPlayerColor, board[fromTile].type);
                return false;
            }
        }

        // Check for Knights
        if (toupper(board[fromTile].type) == KNIGHT) {
            if (isValidKnightMove(fromTile, toTile, KnightMoveData)) {
                return true;
            }
            else {
                NotAValidMove(CurrentBoard, currentPlayerColor, board[fromTile].type);
                return false;
            }
        }

        // Check for Kings
        if (toupper(board[fromTile].type) == KING) {
            if (isValidKingMove(fromTile, toTile, KingMoveData)) {
                return true;
            }
            else {
                NotAValidMove(CurrentBoard, currentPlayerColor, board[fromTile].type);
                return false;
            }
        }

        // Add more custom validation logic based on the rules of your chess game

        return true;
    }

    vector<int> GetAllPossibleMoves(char playerColor)  {
        vector<int> possibleMoves;

        for (int i = 0; i < Total_tiles; i++) {
            if (board[i].color == playerColor) {
                vector<int> moves;

                // Compute moves based on the piece type
                if (toupper(board[i].type) == ROOK || toupper(board[i].type) == BISHOP || toupper(board[i].type) == QUEEN) {
                    ComputeSlidingPieceMoves(i, moves);
                }
                else if (toupper(board[i].type) == PAWN) {
                    ComputePawnMoves(i, moves);
                }
                else if (toupper(board[i].type) == KNIGHT) {
                    ComputeKnightMoves(i, moves);
                }
                else if (toupper(board[i].type) == KING) {
                    ComputeKingMoves(i, moves);
                }
                moves = FilterValidMoves(i, moves);
                for (auto move : moves) {
                    possibleMoves.push_back(move);
                }
            }
        }
    
        return possibleMoves;
    }

    vector<string> GetAllPossibleMovesInChessNotation(char playerColor) {
        vector<string> possibleMoves;

        for (int i = 0; i < Total_tiles; i++) {
            if (board[i].color == playerColor) {
                vector<int> moves;

                // Compute moves based on the piece type
                if (toupper(board[i].type) == ROOK || toupper(board[i].type) == BISHOP || toupper(board[i].type) == QUEEN) {
                    ComputeSlidingPieceMoves(i, moves);
                }
                else if (toupper(board[i].type) == PAWN) {
                    ComputePawnMoves(i, moves);
                }
                else if (toupper(board[i].type) == KNIGHT) {
                    ComputeKnightMoves(i, moves);
                }
                else if (toupper(board[i].type) == KING) {
                    ComputeKingMoves(i, moves);
                }

                // Check if the move is legal (does not leave the king in check)
                for (int toTile : moves) {
                    // Simulate the move
                    ChessBoard tempBoard = *this;
                    tempBoard.MakeMove(i, toTile);

                    // Check if the king is in check after the move
                    if (!tempBoard.isCheck(tempBoard, playerColor)) {
                        // If not in check, add the move to possible moves
                        possibleMoves.push_back(ConvertToChessNotation(i, toTile));
                    }
                }
            }
        }

        return possibleMoves;
    }

    string ConvertToChessNotation(int fromTile, int toTile) {
        string chessNotation = "";

        // Convert the 'fromTile' index to chess notation
        char fromCol = 'a' + (fromTile % 8);
        int fromRow = 8 - (fromTile / 8);
        chessNotation += fromCol;
        chessNotation += to_string(fromRow);

        // Add the 'toTile' index to chess notation
        char toCol = 'a' + (toTile % 8);
        int toRow = 8 - (toTile / 8);
        chessNotation += toCol;
        chessNotation += to_string(toRow);

        return chessNotation;
    }

};

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


     string initialFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
     // string initialFEN = "K7/7r/6q1/8/8/8/8/2K5 b KQkq - 0 1";
    //   string initialFEN = "K1k5/8/8/8/8/8/8/8 b KQkq - 0 1";

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
