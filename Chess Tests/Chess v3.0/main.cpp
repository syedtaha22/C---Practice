#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include "raylib.h"
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <random>
#include <fstream>

using namespace std;
using namespace chrono;

Sound ChessPiecePlaced;
Sound ChessPieceCaptured;
Sound KingChecked;
Sound GameStarts;
Sound GameEnds;

string ChessPiecePlacedFile = "Resources\\Audio\\ChessPiecePlaced.wav";
string ChessPieceCapturedFile = "Resources\\Audio\\ChessPieceCaptured.wav";
string KingCheckedFile = "Resources\\Audio\\KingChecked.wav";
string GameStartsFile = "Resources\\Audio\\GameStart.wav";
string GameEndsFile = "Resources\\Audio\\GameEnd.wav";
string fontFile = "Resources\\Fonts\\Roengineo\\Roengineo-Regular.ttf";
string programIcon = "Resources\\BlackPieces\\bn.png";

Font myFont; // Global variable, initialize at beginning.

const int screenWidth = 1000;
const int screenHeight = 520;
const int boardSize = 8;
const int tileSize = 65;
const int MaxHistorySize = 100;

Color lightSquare = {238, 238, 210, 255};
Color darkSquare = {118, 150, 86, 255};
Color Transparent = {0, 0, 0, 0};
Color Background = {49, 46, 43, 255};
// Color MoveHighlight = { 187, 204, 68, 255};
Color MoveHighlight = Fade({255, 255, 0, 200}, 0.5);
Color MovesForPieceHighLight = Fade({255, 0, 0, 200}, 0.5);

int offsets;

const char EMPTY = ' ';
const char PAWN = 'P';
const char ROOK = 'R';
const char KNIGHT = 'N';
const char BISHOP = 'B';
const char QUEEN = 'Q';
const char KING = 'K';

const int Total_tiles = 64;
enum class PromotionPiece
{
    QUEEN,
    ROOK,
    KNIGHT,
    BISHOP
};

class ReadWrite
{

    virtual int readEloFromFile() = 0;
    virtual void saveEloToFile() const = 0;
};

void DrawTextWithCustomFont(const char *text, float posX, float posY, int fontSize, Color color)
{
    Vector2 Position = {posX, posY};
    DrawTextEx(myFont, text, Position, fontSize, 1, color);
}

class ChessPiece
{
public:
    char type;
    char color;
    bool firstMove;
    bool isSlidingPiece;
    Texture2D texture;
    Rectangle rectangle;
    bool isDragged;
    bool canCastle;

    ChessPiece(char pieceType = EMPTY, char pieceColor = EMPTY, bool isFirstMove = true)
        : type(pieceType), color(pieceColor), firstMove(true)
    {
        isSlidingPiece = (toupper(type) == ROOK) || (toupper(type) == BISHOP) || (toupper(type) == QUEEN);

        AssignTextures();
        rectangle.width = tileSize;
        rectangle.height = tileSize;
        isDragged = false;
    }

    void AssignTextures()
    {
        string fileName = "Resources\\";
        fileName += (color == 'W') ? "WhitePieces\\" : "BlackPieces\\";

        fileName += tolower(color);
        if (type != EMPTY)
        {
            fileName += tolower(type);
            fileName += ".png";
            texture = LoadTexture(fileName.c_str());
        }
    }

    void DestroyTextures()
    {
        if (type != EMPTY)
        {
            cout << "Unloaded Texture for: " << color << " " << type << endl;
            UnloadTexture(texture);
        }
    }
};

class ChessBoard
{
private:
    vector<int> SlidingPieceMoveData;
    vector<int> PawnMoveData;
    vector<int> KnightMoveData;
    vector<int> KingMoveData;

    vector<int> MovesForSelectedPiece;
    vector<int> OpponentMoves;

    bool currentPlayerIsWhite;
    bool DoCastle;
    pair<int, int> MoveIndices;
    // Scores for each piece type
    const int pawnScore = 1;
    const int rookScore = 5;
    const int knightScore = 3;
    const int bishopScore = 3;
    const int queenScore = 9;
    const int kingScore = 100;

public:
    ChessPiece board[Total_tiles];
    vector<string> moveHistory;

    ChessBoard()
    {
        currentPlayerIsWhite = true;
        for (auto chesspiece : board)
        {
            chesspiece = ChessPiece(EMPTY, EMPTY);
            DoCastle = false;
            MoveIndices = {-1, -1};
        }
    }

    int getPieceScore(const ChessPiece &piece) const
    {
        switch (toupper(piece.type))
        {
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
            return 0; // Unknown piece type
        }
    }

    void AddMoveToHistory(string move)
    {
        moveHistory.push_back(move);
    }

    void promotePawn(int toTile)
    {
        bool PawnPromoted = false;
        // cout << "\nPawn promotion! Choose a piece (Q, R, N, B): ";

        ChessPiece promotedPiece;
        //
        // if (IsKeyPressed(KEY_Q)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? toupper(QUEEN) : tolower(QUEEN), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Queen" << endl;
        //    PawnPromoted = false;
        //    //break;
        //}
        // else if (IsKeyPressed(KEY_R)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? toupper(ROOK) : tolower(ROOK), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Rook" << endl;
        //    //break;
        //}
        // else if (IsKeyPressed(KEY_N)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? toupper(KNIGHT) : tolower(KNIGHT), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Knight" << endl;
        //    //break;
        //}
        // else if (IsKeyPressed(KEY_B)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? toupper(BISHOP) : tolower(BISHOP), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Bishop" << endl;
        //    //break;
        //}
        // else {
        //    cout << "Invalid choice. Defaulting to Queen promotion." << endl;
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? toupper(QUEEN) : tolower(QUEEN), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Queen" << endl;
        //    //break;
        //}

        promotedPiece = ChessPiece(currentPlayerIsWhite ? toupper(QUEEN) : tolower(QUEEN), currentPlayerIsWhite ? 'W' : 'B');
        // cout << "Piece Promoted to Queen" << endl;
        // board[toTile].DestroyTextures();
        //  Update the board with the promoted piece
        board[toTile] = promotedPiece;
        SetPiecePositions();
    }

    vector<int> FilterValidMoves(int fromIndex, vector<int> possibleMoves) const
    {
        ChessPiece piece = board[fromIndex];

        // Filter out moves that leave the king in check
        vector<int> filteredMoves;
        for (int toTile : possibleMoves)
        {
            // Simulate the move on a temporary board
            ChessBoard tempBoard = *this;
            tempBoard.MakeMove(fromIndex, toTile);

            // Check if the king is in check after the move
            if (!tempBoard.isCheck(tempBoard, piece.color, "Board: Filter Valid Moves"))
            {
                filteredMoves.push_back(toTile);
            }
        }
        return filteredMoves;
    }

    int calculatePlayerScore(char playerColor) const
    {
        int totalScore = 0;
        for (int i = 0; i < Total_tiles; i++)
        {
            if (board[i].color == playerColor)
            {
                totalScore += getPieceScore(board[i]);
            }
        }
        return totalScore;
    }

    void initializeBoardFromFEN(const string &fen)
    {
        moveHistory.clear();
        MoveIndices = {-1, -1};
        int row = 0, col = 0;

        for (char symbol : fen)
        {
            if (symbol == ' ')
            {
                break; // Stop parsing after the board section
            }

            if (symbol == '/')
            {
                row++;
                col = 0;
            }
            else if (isdigit(symbol))
            {
                // Fill empty squares
                int numEmptySquares = symbol - '0';
                for (int i = 0; i < numEmptySquares; i++)
                {
                    board[row * 8 + col] = ChessPiece(EMPTY, EMPTY);
                    col++;
                }
            }
            else
            {
                char pieceColor = isupper(symbol) ? 'W' : 'B';
                board[row * 8 + col] = ChessPiece(symbol, pieceColor);
                col++;
            }
        }

        // Set the current player based on FEN
        currentPlayerIsWhite = (fen.find("w") != string::npos);
        SetPiecePositions();
        // ...code for additional FEN features
    }

    // Function to generate and save FEN string to file

    void saveMatchHistoryToFile()
    {
        // Open file for writing (in append mode)
        ofstream file("MatchHistory.txt", ios::app);
        if (!file.is_open())
        {
            cerr << "Error: Unable to open file for writing." << endl;
            return;
        }

        // Generate FEN string from current board state
        string fen;
        for (int row = 0; row < 8; row++)
        {
            int emptyCount = 0;
            for (int col = 0; col < 8; col++)
            {
                int index = row * 8 + col;
                if (board[index].type == EMPTY)
                {
                    emptyCount++;
                }
                else
                {
                    if (emptyCount > 0)
                    {
                        fen += to_string(emptyCount);
                        emptyCount = 0;
                    }
                    fen += board[index].type;
                }
            }
            if (emptyCount > 0)
            {
                fen += to_string(emptyCount);
            }
            if (row < 7)
            {
                fen += '/';
            }
        }
        fen += (currentPlayerIsWhite ? " w " : " b ");
        // Add additional FEN features if needed

        // Write FEN string to file
        file << fen << endl; // Append newline after each FEN string
        // Close file
        file.close();

        cout << "Saved FEN string" << endl;
    }

    void InitializeDefaultBoard()
    {

        currentPlayerIsWhite = true;
        // Set up the board with pieces in their initial positions
        string DefaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        initializeBoardFromFEN(DefaultFEN);
    }

    void DisplayBoard() const
    {
        cout << "8 ";
        for (int i = 0; i < Total_tiles; i++)
        {
            // cout << (i/8) + 1  << " ";
            cout << "[" << board[i].type << "]";
            if ((i + 1) % 8 == 0 && (i + 1) < Total_tiles)
            {
                cout << endl;
                cout << 8 - (i + 1) / 8 << " ";
            }

            // cout << endl;
        }
        cout << endl;
        cout << "   a  b  c  d  e  f  g  h";
    }

    pair<int, int> PieceCoordinates(int pieceIndex)
    {
        if (pieceIndex < 0 || pieceIndex >= Total_tiles)
        {

            return make_pair(-1, -1);
        }
        int rank = pieceIndex / boardSize;
        int file = pieceIndex % boardSize;
        return make_pair(rank, file);
    }

    void SetPiecePositions()
    {
        // Set initial positions of the pieces
        for (int index = 0; index < Total_tiles; index++)
        {
            pair<int, int> PieceCoords = PieceCoordinates(index);
            board[index].rectangle.x = (PieceCoords.second) * tileSize;
            board[index].rectangle.y = (PieceCoords.first) * tileSize;
        }
    }

    void DrawBoard()
    {
        for (int index = 0; index < Total_tiles; index++)
        {

            pair<int, int> PieceCoords = PieceCoordinates(index);

            Color squareColor = (PieceCoords.first + PieceCoords.second) % 2 == 0 ? lightSquare : darkSquare;

            int LocationX = PieceCoords.second * tileSize;
            int LocationY = PieceCoords.first * tileSize;

            DrawRectangle(LocationX, LocationY, tileSize, tileSize, squareColor);

            if (MoveIndices.first == index || MoveIndices.second == index)
            {
                DrawRectangle(LocationX, LocationY, tileSize, tileSize, MoveHighlight);
            }

            for (auto move : MovesForSelectedPiece)
            {
                if (move == index)
                {
                    DrawRectangle(LocationX, LocationY, tileSize, tileSize, MovesForPieceHighLight);
                }
            }
        }
    }

    void DrawChessPiece()
    {
        for (int index = 0; index < Total_tiles; index++)
        {
            DrawTexture(board[index].texture, static_cast<int>(board[index].rectangle.x), static_cast<int>(board[index].rectangle.y), WHITE);
        }
    }

    int getTileIndex(float x, float y, int tileSize)
    {
        // Calculate the file (column) index
        int file = static_cast<int>(x / tileSize);
        // Calculate the rank (row) index
        int rank = static_cast<int>(y / tileSize);
        // Calculate the index in the array
        int index = rank * 8 + file;
        return index;
    }

    void UpdateChessPiece(ChessPiece &piece, int InitialIndex)
    {
        if (piece.type != EMPTY)
        {

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), piece.rectangle))
            {
                piece.isDragged = true;
                MovesForSelectedPiece = GetAllPossibleMovesForPiece(piece.type, InitialIndex);
            }
            if (piece.isDragged)
            {
                piece.rectangle.x = GetMouseX() - piece.rectangle.width / 2;
                piece.rectangle.y = GetMouseY() - piece.rectangle.height / 2;
                if (piece.rectangle.x < 0)
                    piece.rectangle.x = 0;
                if (piece.rectangle.y < 0)
                    piece.rectangle.y = 0;
                if (piece.rectangle.x + piece.rectangle.width > GetScreenWidth())
                    piece.rectangle.x = GetScreenWidth() - piece.rectangle.width;
                if (piece.rectangle.y + piece.rectangle.height > GetScreenHeight())
                    piece.rectangle.y = GetScreenHeight() - piece.rectangle.height;

                float tileX = round(piece.rectangle.x / tileSize) * tileSize;
                float tileY = round(piece.rectangle.y / tileSize) * tileSize;

                int FinalIndex = getTileIndex(tileX, tileY, tileSize);
                string move = ConvertToChessNotation(InitialIndex, FinalIndex);

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    if (isValidMove(FinalIndex))
                    {
                        piece.isDragged = false;

                        piece.rectangle.x = tileX;
                        piece.rectangle.y = tileY;
                        MakeCompleteMove(InitialIndex, FinalIndex, move);

                        cout << "White Plays: " << move << "\n";
                        // WaitTime(0.5);
                    }
                    else
                    {
                        piece.isDragged = false;
                        int file = InitialIndex % 8;
                        int rank = InitialIndex / 8;
                        piece.rectangle.x = file * tileSize;
                        piece.rectangle.y = rank * tileSize;
                    }
                }
            }
        }
    }

    bool isValidMove(int index)
    {
        for (auto Move : MovesForSelectedPiece)
        {
            if (index == Move)
            {
                return true;
            }
        }
        return false;
    }

    void MakeMove(int fromTile, int toTile)
    {
        string move = ConvertToChessNotation(fromTile, toTile);

        // Check if it's a castling move and if castling is allowed
        if (IsCastlingMove(move, board[fromTile]))
        {

            if (toTile == 62 || toTile == 7)
            { // Kingside castling
                int rookFromTile = (fromTile == 60) ? 63 : 7;
                int rookToTile = (fromTile == 60) ? 61 : 5;
                board[rookToTile] = board[rookFromTile];
                board[rookFromTile] = ChessPiece();
            }
            else if (toTile == 58 || toTile == 2)
            { // Queenside castling
                int rookFromTile = (fromTile == 60) ? 56 : 0;
                int rookToTile = (fromTile == 60) ? 59 : 3;
                board[rookToTile] = board[rookFromTile]; // Move the rook
                board[rookFromTile] = ChessPiece();      // Empty the original rook square
            }
        }

        board[fromTile].firstMove = false;
        board[toTile] = board[fromTile];
        board[fromTile] = ChessPiece(); // Empty the source square
    }

    bool IsCastlingMove(string move, ChessPiece pieceMoved)
    {
        return (pieceMoved.type == KING) && (move == "e1g1" || move == "e1c1" || "e8g8" || move == "e8c8");
    }

    void MakeCompleteMove(int fromTile, int toTile, string move)
    {
        MakeMove(fromTile, toTile);

        // Check for pawn promotion
        if (toupper(board[toTile].type) == PAWN)
        {
            // White pawn promotion
            if (toTile >= 0 && toTile <= 7)
            {
                promotePawn(toTile);
            }
            // Black pawn promotion
            else if (toTile >= 56 && toTile <= 63)
            {
                cout << "Test";
                promotePawn(toTile);
            }
        }

        if (moveHistory.size() >= MaxHistorySize)
        {
            moveHistory.clear();
        }

        AddMoveToHistory(move);
        MoveIndices = {fromTile, toTile};

        SetPiecePositions();
        // PlaySound(ChessPiecePlaced);
        currentPlayerIsWhite = !currentPlayerIsWhite;
        PlayChessSound();
        // DisplayScores();
        SlidingPieceMoveData.clear();
        PawnMoveData.clear();
        KnightMoveData.clear();
        KingMoveData.clear();
        MovesForSelectedPiece.clear();
    }

    void PlayChessSound()
    {
        ChessBoard temp = *this;
        char PlayerColor = (temp.isCurrentPlayerWhite()) ? 'W' : 'B';
        if (isCheck(temp, PlayerColor, "board: Play Chess Sound"))
        {
            PlaySound(KingChecked);
        }
        else
        {
            PlaySound(ChessPiecePlaced);
        }
    }

    bool isCurrentPlayerWhite() const
    {
        return currentPlayerIsWhite;
    }

    void DisplayScores()
    {
        int whiteScore = calculatePlayerScore('W');
        int blackScore = calculatePlayerScore('B');
        cout << endl
             << "White score: " << whiteScore << " | Black score: " << blackScore << endl
             << endl;
    }

    void ComputeSlidingPieceMoves(int pieceIndex, vector<int> &possibleMoves) const
    {
        ChessPiece piece = board[pieceIndex];
        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        // Offsets for sliding directions: up, down, left, right, diagonals
        vector<int> offsets = {8, -8, 1, -1, 7, -7, 9, -9};

        if (toupper(piece.type) == ROOK || toupper(piece.type) == QUEEN)
        {
            for (int i = 0; i < 4; ++i)
            {
                int offset = offsets[i];
                int newIndex = pieceIndex + offset;
                while (newIndex >= 0 && newIndex < Total_tiles)
                {
                    // Check if the new index is within the bounds of the board
                    if ((offset == 1 || offset == -1) && newIndex / 8 != row)
                        break; // If moving horizontally, ensure it stays on the same row
                    if (board[newIndex].type != EMPTY)
                    {
                        if (board[newIndex].color != piece.color)
                        {
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

        if (toupper(piece.type) == BISHOP || toupper(piece.type) == QUEEN)
        {
            for (int i = 4; i < 8; ++i)
            {
                int offset = offsets[i];
                int newIndex = pieceIndex + offset;
                while (newIndex >= 0 && newIndex < Total_tiles)
                {
                    // Check if the new index is within the bounds of the board
                    int newRow = newIndex / 8;
                    int newCol = newIndex % 8;
                    if (abs(newRow - row) != abs(newCol - col))
                        break; // If not moving diagonally, break
                    if (board[newIndex].type != EMPTY)
                    {
                        if (board[newIndex].color != piece.color)
                        {
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

    void ComputeKnightMoves(int pieceIndex, vector<int> &possibleMoves) const
    {
        ChessPiece piece = board[pieceIndex];
        // Offsets for knight's moves
        vector<int> rowOffsets = {-2, -1, 1, 2, 2, 1, -1, -2};
        vector<int> colOffsets = {1, 2, 2, 1, -1, -2, -2, -1};

        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        if (toupper(piece.type) == KNIGHT)
        {
            for (int i = 0; i < 8; ++i)
            {
                int newRow = row + rowOffsets[i];
                int newCol = col + colOffsets[i];

                // Check if the new position is within the board bounds
                if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
                {
                    int newIndex = newRow * 8 + newCol;
                    // Check if the square is empty or contains an opponent's piece
                    if (board[newIndex].type == EMPTY || board[newIndex].color != piece.color)
                    {
                        possibleMoves.push_back(newIndex);
                    }
                }
            }
        }
    }

    void ComputePawnMoves(int pieceIndex, vector<int> &possibleMoves) const
    {
        ChessPiece piece = board[pieceIndex];
        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        // Determine the direction of pawn movement based on its color
        int direction = (piece.color == 'W') ? -1 : 1;

        // Forward movement
        int forwardOne = pieceIndex + direction * 8;
        if (forwardOne >= 0 && forwardOne < Total_tiles && board[forwardOne].type == EMPTY)
        {
            possibleMoves.push_back(forwardOne);
            // Check if the pawn is in its starting position and can move two squares forward
            if ((row == 6 && piece.color == 'W') || (row == 1 && piece.color == 'B'))
            {
                int forwardTwo = forwardOne + direction * 8;
                if (board[forwardTwo].type == EMPTY)
                {
                    possibleMoves.push_back(forwardTwo);
                }
            }
        }

        // Capture moves
        vector<int> captureOffsets = {direction * 7, direction * 9};
        for (int offset : captureOffsets)
        {
            int targetIndex = pieceIndex + offset;
            int targetRow = targetIndex / 8;
            int targetCol = targetIndex % 8;
            int pieceCol = pieceIndex % 8;
            // Ensure target column is within bounds and not wrapping around the board
            if (targetCol >= 0 && targetCol < 8 && abs(targetCol - pieceCol) <= 1)
            {
                // Check if the target square is occupied by an opponent's piece
                if (board[targetIndex].type != EMPTY && board[targetIndex].color != piece.color)
                {
                    possibleMoves.push_back(targetIndex);
                }
            }
        }
    }

    void ComputeKingMoves(int KingIndex, vector<int> &possibleMoves) const
    {
        ChessPiece King = board[KingIndex];
        int row = KingIndex / 8;
        int col = KingIndex % 8;

        // Offsets for king's moves
        vector<int> rowOffsets = {-1, -1, -1, 0, 0, 1, 1, 1};
        vector<int> colOffsets = {-1, 0, 1, -1, 1, -1, 0, 1};

        // Check all 8 directions
        for (int i = 0; i < 8; ++i)
        {
            int newRow = row + rowOffsets[i];
            int newCol = col + colOffsets[i];

            // Check if the new position is within the board bounds
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
            {
                int newIndex = newRow * 8 + newCol;
                // Check if the square is empty or contains an opponent's piece
                if ((board[newIndex].type == EMPTY || board[newIndex].color != King.color))
                {
                    possibleMoves.push_back(newIndex);
                }
            }
        }

        // // Check for castling moves
        // if (King.firstMove || !IsTileUnderAttack(KingIndex)) {
        //     // Check kingside castling
        //     if (canCastleKingSide(KingIndex)) {
        //         possibleMoves.push_back(KingIndex + 2);
        //     }
        //     // Check queenside castling
        //     if (canCastleQueenide(KingIndex)) {
        //         possibleMoves.push_back(KingIndex - 2);
        //     }
        // }
    }

    bool canCastleKingSide(int KingIndex) const
    {
        int RookIndex = KingIndex + 3;
        if (board[RookIndex].type != ROOK && !(board[RookIndex].firstMove))
        {
            return false;
        }

        for (int i = KingIndex + 1; i < RookIndex; i++)
        {
            if (board[i].type != EMPTY || IsTileUnderAttack(i))
            {
                return false;
            }
        }

        return true;
    }

    bool canCastleQueenide(int KingIndex) const
    {

        int RookIndex = KingIndex - 4;
        if (board[RookIndex].type != ROOK && !(board[RookIndex].firstMove))
        {
            return false;
        }

        for (int i = KingIndex - 1; i > RookIndex; i--)
        {
            if (board[i].type != EMPTY || IsTileUnderAttack(i))
            {
                return false;
            }
        }

        return true;
    }

    bool isCheck(const ChessBoard &chessboard, char playerColor, string calledby) const
    {
        // Find the king's position
        int kingIndex = -100;
        for (int i = 0; i < Total_tiles; i++)
        {
            if (toupper(chessboard.board[i].type) == KING && chessboard.board[i].color == playerColor)
            {
                kingIndex = i;
                break;
            }
        }

        if (kingIndex == -100)
        {
            cout << "\nError Code 404: King not found. \n\n";
            cout << calledby << "\n\n";
            exit(404);
            return false;
        }

        // Iterate through opponent's pieces and see if any can attack the king
        char opponentColor = (playerColor == 'W') ? 'B' : 'W';
        vector<int> possibleMoves;
        for (int i = 0; i < Total_tiles; i++)
        {
            if (chessboard.board[i].color == opponentColor)
            {
                if (toupper(chessboard.board[i].type) == ROOK || toupper(chessboard.board[i].type) == BISHOP || toupper(chessboard.board[i].type) == QUEEN)
                {
                    ComputeSlidingPieceMoves(i, possibleMoves); // Example function, change as needed
                }
                else if (toupper(chessboard.board[i].type) == PAWN)
                {
                    ComputePawnMoves(i, possibleMoves);
                }
                else if (toupper(chessboard.board[i].type) == KNIGHT)
                {
                    ComputeKnightMoves(i, possibleMoves);
                }
                else if (toupper(chessboard.board[i].type) == KING)
                {
                    ComputeKingMoves(i, possibleMoves);
                }

                // Check if any of the possible moves include the king's position
                for (int move : possibleMoves)
                {
                    if (move == kingIndex)
                    {
                        // PlaySound(KingChecked);
                        return true; // King is in check
                    }
                }
            }
        }
        return false; // King is not in check
    }

    bool isCheckmate(ChessBoard &chessboard, char playerColor) const
    {
        if (!isCheck(chessboard, playerColor, "board: Is Check Mate"))
        {
            return false; // King is not in check, so it can't be checkmate
        }

        vector<int> possibleMoves = GetAllPossibleMoves(playerColor);
        if (possibleMoves.size() > 0)
        {
            return false; // King can escape from check, so it's not checkmate
        }

        return true; // King is in checkmate
    }

    vector<int> GetAllPossibleMoves(char playerColor) const
    {

        vector<int> possibleMoves;

        for (int i = 0; i < Total_tiles; i++)
        {
            if (board[i].color == playerColor)
            {
                vector<int> moves;

                // Compute moves based on the piece type
                if (toupper(board[i].type) == ROOK || toupper(board[i].type) == BISHOP || toupper(board[i].type) == QUEEN)
                {
                    ComputeSlidingPieceMoves(i, moves);
                }
                else if (toupper(board[i].type) == PAWN)
                {
                    ComputePawnMoves(i, moves);
                }
                else if (toupper(board[i].type) == KNIGHT)
                {
                    ComputeKnightMoves(i, moves);
                }
                else if (toupper(board[i].type) == KING)
                {
                    ComputeKingMoves(i, moves);
                }
                moves = FilterValidMoves(i, moves);
                for (auto move : moves)
                {
                    possibleMoves.push_back(move);
                }
            }
        }

        return possibleMoves;
    }

    vector<string> GetAllPossibleMovesInChessNotation(char playerColor) const
    {
        vector<string> possibleMoves;

        for (int i = 0; i < Total_tiles; i++)
        {
            if (board[i].color == playerColor)
            {
                vector<int> moves;

                // Compute moves based on the piece type
                if (toupper(board[i].type) == ROOK || toupper(board[i].type) == BISHOP || toupper(board[i].type) == QUEEN)
                {
                    ComputeSlidingPieceMoves(i, moves);
                }
                else if (toupper(board[i].type) == PAWN)
                {
                    ComputePawnMoves(i, moves);
                }
                else if (toupper(board[i].type) == KNIGHT)
                {
                    ComputeKnightMoves(i, moves);
                }
                else if (toupper(board[i].type) == KING)
                {
                    ComputeKingMoves(i, moves);
                }

                // Check if the move is legal (does not leave the king in check)
                for (int toTile : moves)
                {
                    // Simulate the move
                    ChessBoard tempBoard = *this;
                    tempBoard.MakeMove(i, toTile);

                    // Check if the king is in check after the move
                    if (!tempBoard.isCheck(tempBoard, playerColor, "board: Get all possible moves in chess notation"))
                    {
                        // If not in check, add the move to possible moves
                        possibleMoves.push_back(ConvertToChessNotation(i, toTile));
                    }
                }
            }
        }

        return possibleMoves;
    }

    string ConvertToChessNotation(int fromTile, int toTile) const
    {
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

    int getAttacksOnSquare(int squareIndex, char opponentColor) const
    {
        int attacks = 0;
        vector<int> opponentMoves = GetAllPossibleMoves(opponentColor);
        for (int i = 0; i < opponentMoves.size(); ++i)
        {
            if (opponentMoves[i] == squareIndex)
            {
                attacks++;
            }
        }
        return attacks;
    }

    bool IsTileUnderAttack(int squareIndex) const
    {

        for (int i = 0; i < OpponentMoves.size(); i++)
        {
            if (OpponentMoves[i] == squareIndex)
            {
                return true;
            }
        }
        return false;
    }

    void DisplayMoves()
    {
        for (auto move : OpponentMoves)
        {
            cout << move << endl;
        }
    }

    vector<int> GetAllPossibleMovesForPiece(char type, int index) const
    {
        vector<int> moves;

        // Compute moves based on the piece type
        if (toupper(type) == ROOK || toupper(type) == BISHOP || toupper(type) == QUEEN)
        {
            ComputeSlidingPieceMoves(index, moves);
        }
        else if (toupper(type) == PAWN)
        {
            ComputePawnMoves(index, moves);
        }
        else if (toupper(type) == KNIGHT)
        {
            ComputeKnightMoves(index, moves);
        }
        else if (toupper(type) == KING)
        {
            ComputeKingMoves(index, moves);
        }

        moves = FilterValidMoves(index, moves);
        return moves;
    }

    void DestroyBoard()
    {
        for (int i = 0; i < Total_tiles; i++)
        {
            board[i].DestroyTextures();
        }
    }

    ChessPiece GetPieceAtPosition(int index) const
    {
        return board[index];
    }

    vector<string> GetAllCaptureMovesInChessNotation(char color) const
    {
        vector<string> captureMoves;

        for (int fromTile = 0; fromTile < Total_tiles; fromTile++)
        {
            ChessPiece piece = board[fromTile];
            if (piece.color == color)
            {
                for (int toTile = 0; toTile < Total_tiles; toTile++)
                {
                    if (fromTile != toTile && isValidCaptureMove(fromTile, toTile))
                    {
                        string move = ConvertToChessNotation(fromTile, toTile);
                        captureMoves.push_back(move);
                    }
                }
            }
        }

        return captureMoves;
    }

    bool isValidCaptureMove(int fromTile, int toTile) const
    {
        ChessPiece fromPiece = board[fromTile];
        ChessPiece toPiece = board[toTile];

        // Check if there is a piece at the 'from' tile and it is of opposite color to the piece at the 'to' tile
        return fromPiece.type != EMPTY && toPiece.type != EMPTY && fromPiece.color != toPiece.color;
    }

    pair<int, int> convertChessNotationToIndices(const string &move) const
    {
        int fromCol = move[0] - 'a';
        int fromRow = 7 - (move[1] - '1');
        int toCol = move[2] - 'a';
        int toRow = 7 - (move[3] - '1');
        int fromTile = fromRow * 8 + fromCol;
        int toTile = toRow * 8 + toCol;
        return make_pair(fromTile, toTile);
    }

    void ComputeOpponentMoves()
    {
        OpponentMoves = GetAllPossibleMoves(isCurrentPlayerWhite() ? 'B' : 'W');
    }
};

class ChessEngine : public ReadWrite
{
    char color;
    const int MAX_DEPTH = 3; // Maximum depth for the Minimax algorithm

    // Piece-square tables for each piece type
    // Higher values indicate a more advantageous position for the piece
    const int PawnPST[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5, 5, 10, 25, 25, 10, 5, 5,
        0, 0, 0, 20, 20, 0, 0, 0,
        5, -5, -10, 0, 0, -10, -5, 5,
        5, 10, 10, -20, -20, 10, 10, 5,
        0, 0, 0, 0, 0, 0, 0, 0};
    const int RooksPST[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        5, 10, 10, 10, 10, 10, 10, 5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        0, 0, 0, 5, 5, 0, 0, 0};
    const int KnightsPST[64] = {
        -50,
        -40,
        -30,
        -30,
        -30,
        -30,
        -40,
        -50,
        -40,
        -20,
        0,
        0,
        0,
        0,
        -20,
        -40,
        -30,
        0,
        10,
        15,
        15,
        10,
        0,
        -30,
        -30,
        5,
        15,
        20,
        20,
        15,
        5,
        -30,
        -30,
        0,
        15,
        20,
        20,
        15,
        0,
        -30,
        -30,
        5,
        10,
        15,
        15,
        10,
        5,
        -30,
        -40,
        -20,
        0,
        5,
        5,
        0,
        -20,
        -40,
        -50,
        -40,
        -30,
        -30,
        -30,
        -30,
        -40,
        -50,
    };
    const int BishopsPST[64] = {
        -20,
        -10,
        -10,
        -10,
        -10,
        -10,
        -10,
        -20,
        -10,
        0,
        0,
        0,
        0,
        0,
        0,
        -10,
        -10,
        0,
        5,
        10,
        10,
        5,
        0,
        -10,
        -10,
        5,
        5,
        10,
        10,
        5,
        5,
        -10,
        -10,
        0,
        10,
        10,
        10,
        10,
        0,
        -10,
        -10,
        10,
        10,
        10,
        10,
        10,
        10,
        -10,
        -10,
        5,
        0,
        0,
        0,
        0,
        5,
        -10,
        -20,
        -10,
        -10,
        -10,
        -10,
        -10,
        -10,
        -20,
    };
    const int QueensPST[64] = {
        -20, -10, -10, -5, -5, -10, -10, -20,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -10, 0, 5, 5, 5, 5, 0, -10,
        -5, 0, 5, 5, 5, 5, 0, -5,
        0, 0, 5, 5, 5, 5, 0, -5,
        -10, 5, 5, 5, 5, 5, 0, -10,
        -10, 0, 5, 0, 0, 0, 0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20};
    const int KingPST[64] = {
        -80, -70, -70, -70, -70, -70, -70, -80,
        -60, -60, -60, -60, -60, -60, -60, -60,
        -40, -50, -50, -60, -60, -50, -50, -40,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        20, 20, -5, -5, -5, -5, 20, 20,
        20, 30, 10, 0, 0, 10, 30, 20};

public:
    int engineEloRating = 500;
    int NumberofMovesLookedAhead;
    int TimeTakenForSearch;

    ChessEngine(char pieceColor) : color(pieceColor), NumberofMovesLookedAhead(0)
    {
        engineEloRating = readEloFromFile();
        NumberofMovesLookedAhead = 0;
        TimeTakenForSearch = 0;
    }
    void shuffleMoves(vector<string> &possibleMoves)
    {
        unsigned seed = system_clock::now().time_since_epoch().count();
        shuffle(possibleMoves.begin(), possibleMoves.end(), default_random_engine(seed));
    }

    string GenerateMove(const ChessBoard &board)
    {
        int bestScore = numeric_limits<int>::min();
        string bestMove;
        auto start = high_resolution_clock::now();
        NumberofMovesLookedAhead = 0;

        vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(color);

        shuffleMoves(possibleMoves);

        for (const string &move : possibleMoves)
        {
            ChessBoard tempBoard = board; // Make a copy of the board to simulate moves
            int fromTile, toTile;
            // tempBoard.isValidMove(move, fromTile, toTile);
            pair<int, int> Indices = convertChessNotationToIndices(move);

            // Simulate the move
            tempBoard.MakeMove(Indices.first, Indices.second);

            // Calculate the score using Minimax with alpha-beta pruning
            int score = Minimax(tempBoard, MAX_DEPTH - 1, numeric_limits<int>::min(), numeric_limits<int>::max(), false, color, NumberofMovesLookedAhead);

            // Update the best move if needed
            if (score > bestScore)
            {
                bestScore = score;
                bestMove = move;
            }
        }

        possibleMoves.clear();
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(end - start);
        TimeTakenForSearch = duration.count();
        return bestMove;
    }

    int Minimax(ChessBoard &board, int depth, int alpha, int beta, bool maximizingPlayer, char color, int &MovesLookedAhead)
    {
        MovesLookedAhead++;

        // Base case: reached maximum depth or terminal state (checkmate)
        if (depth == 0 || board.isCheckmate(board, color))
        {
            if (board.isCheckmate(board, color))
            {
                return (maximizingPlayer ? -INFINITY : INFINITY); // Checkmate score
            }
            return Evaluate(board, color);
        }

        // Maximizing player's turn
        if (maximizingPlayer)
        {
            int maxScore = numeric_limits<int>::min();
            vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(color);
            shuffleMoves(possibleMoves);

            for (const string &move : possibleMoves)
            {
                ChessBoard tempBoard = board; // Make a copy of the board to simulate moves
                pair<int, int> Indices = convertChessNotationToIndices(move);
                tempBoard.MakeMove(Indices.first, Indices.second); // Make the move directly without isValidMove()

                // Recursive call for the opponent with a reduced depth
                int score = Minimax(tempBoard, depth - 1, alpha, beta, false, color, MovesLookedAhead);

                // Update alpha and maxScore
                maxScore = max(maxScore, score);
                alpha = max(alpha, score);

                // Alpha-beta pruning
                if (beta <= alpha)
                {
                    break;
                }
            }
            possibleMoves.clear();
            return maxScore;
        }
        // Minimizing player's turn
        else
        {
            int minScore = numeric_limits<int>::max();
            vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation((color == 'W' ? 'B' : 'W'));
            shuffleMoves(possibleMoves);

            for (const string &move : possibleMoves)
            {
                ChessBoard tempBoard = board; // Make a copy of the board to simulate moves
                pair<int, int> Indices = convertChessNotationToIndices(move);
                tempBoard.MakeMove(Indices.first, Indices.second); // Make the move directly without isValidMove()

                // Recursive call for the opponent with a reduced depth
                int score = Minimax(tempBoard, depth - 1, alpha, beta, true, color, MovesLookedAhead);

                // Update beta and minScore
                minScore = min(minScore, score);
                beta = min(beta, score);

                // Alpha-beta pruning
                if (beta <= alpha)
                {
                    break;
                }
            }
            possibleMoves.clear();
            return minScore;
        }
    }

    int Evaluate(const ChessBoard &chessboard, char currentPlayerColor) const
    {
        unordered_map<char, int> pieceValues;
        // Piece values for evaluation
        if (color == 'W')
        {
            pieceValues = {
                {'P', 100}, {'N', 320}, {'B', 330}, {'R', 500}, {'Q', 900}, {'K', 1000}, {'p', -100}, {'n', -320}, {'b', -330}, {'r', -500}, {'q', -900}, {'k', -1000}};
        }
        else
        {
            pieceValues = {
                {'P', -100}, {'N', -320}, {'B', -330}, {'R', -500}, {'Q', -900}, {'K', -1000}, {'p', 100}, {'n', 320}, {'b', 330}, {'r', 500}, {'q', 900}, {'k', 1000}};
        }

        // Evaluate material advantage
        int SelfMaterial = 0, OpponentMeterial = 0;
        for (int i = 0; i < Total_tiles; ++i)
        {
            ChessPiece piece = chessboard.board[i];
            if (piece.type != EMPTY)
            {
                if (piece.color == color)
                {
                    SelfMaterial += pieceValues[piece.type];
                }
                else
                {
                    OpponentMeterial += pieceValues[piece.type];
                }
            }
        }
        int materialAdvantage = (currentPlayerColor == 'W') ? (SelfMaterial - OpponentMeterial) : (OpponentMeterial - SelfMaterial);

        // Evaluate positional advantage using PSTs
        int positionalAdvantage = 0;
        for (int i = 0; i < Total_tiles; ++i)
        {
            ChessPiece piece = chessboard.board[i];
            if (piece.type != EMPTY)
            {
                int pieceValue = getPSTValue(piece, i, currentPlayerColor);
                positionalAdvantage += pieceValue;
            }
        }

        // Total evaluation is a combination of material, positional, and safety advantages
        return materialAdvantage + positionalAdvantage;
    }

    int getPSTValue(ChessPiece piece, int squareIndex, char currentPlayerColor) const
    {
        const int *piecePST = nullptr;

        switch (toupper(piece.type))
        {
        case PAWN:
            piecePST = (currentPlayerColor == 'B') ? InvertTable(PawnPST) : PawnPST;
            break;
        case KNIGHT:
            piecePST = (currentPlayerColor == 'B') ? InvertTable(KnightsPST) : KnightsPST;
            break;
        case BISHOP:
            piecePST = (currentPlayerColor == 'B') ? InvertTable(BishopsPST) : BishopsPST;
            break;
        case ROOK:
            piecePST = (currentPlayerColor == 'B') ? InvertTable(RooksPST) : RooksPST;
            break;
        case QUEEN:
            piecePST = (currentPlayerColor == 'B') ? InvertTable(QueensPST) : QueensPST;
            break;
        case KING:
            piecePST = (currentPlayerColor == 'B') ? InvertTable(KingPST) : KingPST;
            break;
        }

        // Ensure that the piece type index is within bounds
        if (piecePST != nullptr)
        {
            return piecePST[squareIndex];
        }
        else
        {
            // Handle unknown piece types or other errors
            return 0; // Or handle the error in an appropriate way
        }
    }

    pair<int, int> convertChessNotationToIndices(const string &move) const
    {
        int fromCol = move[0] - 'a';
        int fromRow = 7 - (move[1] - '1');
        int toCol = move[2] - 'a';
        int toRow = 7 - (move[3] - '1');
        int fromTile = fromRow * 8 + fromCol;
        int toTile = toRow * 8 + toCol;
        return make_pair(fromTile, toTile);
    }

    void PlayMove(const string &move, ChessBoard &board)
    {

        pair<int, int> Indices = convertChessNotationToIndices(move);

        // string player = (color == 'W') ? "White" : "Black";
        // cout << player << " plays: " << move << endl;
        board.MakeCompleteMove(Indices.first, Indices.second, move);
    }

    int *InvertTable(const int *originalArray) const
    {
        int *invertedArray = new int[Total_tiles]; // Create an array of size 64

        for (int i = 0; i < Total_tiles; i++)
        {
            invertedArray[i] = originalArray[63 - i];
        }

        return invertedArray;
    }

    int readEloFromFile()
    {
        std::ifstream inputFile;
        std::string filename = std::string(1, color) + "ELO.txt";
        inputFile.open(filename);

        int elo = engineEloRating; // Default value in case the file cannot be opened

        if (inputFile.is_open())
        {
            inputFile >> elo;
            inputFile.close();
            std::cout << "Elo rating read from " << filename << ": " << elo << std::endl;
        }
        else
        {
            std::cerr << "Unable to open file " << filename << " for reading. Using default Elo rating." << std::endl;
        }

        return elo;
    }

    void saveEloToFile() const
    {
        ofstream outputFile;
        string filename = string(1, color) + "ELO.txt";
        outputFile.open(filename);

        int defaultElo = 500;

        if (outputFile.is_open())
        {

            outputFile << engineEloRating << endl;
            outputFile.close();
            cout << "Elo rating saved to " << filename << endl;
        }
        else
        {
            cerr << "Unable to open file " << filename << " for writing. Creating a new file with default Elo rating." << endl;
            outputFile.open(filename);
            if (outputFile.is_open())
            {
                outputFile << defaultElo << std::endl;
                outputFile.close();
                cout << "New file created with default Elo rating: " << defaultElo << endl;
            }
            else
            {
                cerr << "Unable to create file " << filename << " for writing." << endl;
            }
        }
    }

    void Reset()
    {
        TimeTakenForSearch = 0;
        NumberofMovesLookedAhead = 0;
    }

    // Function to check if a move results in a check
    bool CheckAfterMove(const string &move, const ChessBoard &board, char color) const
    {
        ChessBoard tempBoard = board; // Make a copy of the board to simulate the move
        pair<int, int> indices = convertChessNotationToIndices(move);
        tempBoard.MakeMove(indices.first, indices.second); // Make the move directly without isValidMove()

        // Check if the move puts the opponent's king in check
        return tempBoard.isCheck(tempBoard, (color == 'W' ? 'B' : 'W'), "Engine: Check after move");
    }

    bool IsCaptureMove(const string &move, const ChessBoard board) const
    {
        pair<int, int> indices = convertChessNotationToIndices(move);
        ChessPiece targetPiece = board.GetPieceAtPosition(indices.second);

        // Check if the target piece is an opponent's piece
        return targetPiece.color != board.GetPieceAtPosition(indices.first).color && targetPiece.color != EMPTY;
    }

    // Function to sort moves considering checks and captures first
    void SortMoves(vector<string> &moves, const ChessBoard &board, char color) const
    {
        // Define custom comparator to sort moves
        auto customComparator = [&](const string &move1, const string &move2)
        {
            // Check if move1 gives check
            bool check1 = CheckAfterMove(move1, board, color);

            // Check if move2 gives check
            bool check2 = CheckAfterMove(move2, board, color);

            // If one move gives check and the other doesn't, prioritize the move that gives check
            if (check1 != check2)
                return check1;

            // If engineh moves give check or engineh moves don't give check, prioritize captures
            bool capture1 = IsCaptureMove(move1, board);
            bool capture2 = IsCaptureMove(move2, board);

            return capture1 > capture2; // Sorting captures before non-captures
        };

        // Sort moves using the custom comparator
        sort(moves.begin(), moves.end(), customComparator);
    }
};

class User : public ReadWrite
{
public:
    string Username;
    int ELO = 500;

    User(string name) : Username(name)
    {
        ELO = readEloFromFile();
    }

    int readEloFromFile()
    {
        cout << "TEst" << endl;
        ifstream inputFile;
        string filename = Username + "ELO.txt";
        inputFile.open(filename);

        int elo = ELO; // Default value in case the file cannot be opened

        if (inputFile.is_open())
        {
            inputFile >> elo;
            inputFile.close();
            cout << "Elo rating read from " << filename << ": " << elo << endl;
        }
        else
        {
            cerr << "Unable to open file " << filename << " for reading. Using default Elo rating." << endl;
        }

        return elo;
    }

    void saveEloToFile() const
    {
        ofstream outputFile;
        string filename = Username + "ELO.txt";
        outputFile.open(filename);

        int defaultElo = 500;

        if (outputFile.is_open())
        {

            outputFile << ELO << endl;
            outputFile.close();
            cout << "Elo rating saved to " << filename << endl;
        }
        else
        {
            cerr << "Unable to open file " << filename << " for writing. Creating a new file with default Elo rating." << endl;
            outputFile.open(filename);
            if (outputFile.is_open())
            {
                outputFile << defaultElo << std::endl;
                outputFile.close();
                cout << "New file created with default Elo rating: " << defaultElo << endl;
            }
            else
            {
                cerr << "Unable to create file " << filename << " for writing." << endl;
            }
        }
    }
};

class BoardStats
{
private:
    const int TextSize = 40;
    const int fontSize = 20;
    const int textX = 550;
    const int textY = 20;
    const int BoardDimensions = tileSize * boardSize;

public:
    bool ShowMoveHistory;
    bool CalculateELO;
    char winner;

    BoardStats()
    {
        ShowMoveHistory = false;
        winner = ' ';
        CalculateELO = true;
    }

    void DisplayMessage(string message)
    {
        int TextPositionX = BoardDimensions + (screenWidth - BoardDimensions) / 2;                                                                        // Centers text on X-axis of Remaining Area
        int TextPositionY = screenHeight / 2;                                                                                                             // Centers text on Y-axis
        DrawTextWithCustomFont(message.c_str(), TextPositionX - MeasureText(message.c_str(), TextSize) / 2, TextPositionY - TextSize / 2, TextSize, RED); // Balance Based on TextSize
    }

    bool GameIsEnded(ChessBoard &board)
    {
        char playerColor = board.isCurrentPlayerWhite() ? 'W' : 'B';

        string EndMessage;

        if (board.isCheck(board, playerColor, "stats: Game is Ended: Winner"))
        {
            if (board.isCheckmate(board, playerColor))
            {
                string EndMessage = "Winner is ";
                EndMessage += (board.isCurrentPlayerWhite() ? "Black" : "White");
                winner = board.isCurrentPlayerWhite() ? 'B' : 'W';
                DisplayMessage(EndMessage);
                // PlaySound(GameEnds);
                return true;
            }
        }
        if (!board.isCheck(board, playerColor, "stats: Game is Ended: stalemate") && board.GetAllPossibleMoves(playerColor).size() == 0)
        {
            string EndMessage = "---Stalemate---";
            DisplayMessage(EndMessage);
            return true;
        }
        if (board.calculatePlayerScore('W') <= 100 && board.calculatePlayerScore('B') <= 100)
        {
            string EndMessage = "---Draw---";
            DisplayMessage(EndMessage);
            return true;
        }

        return false;
    }

    void DisplayStats(ChessBoard chessboard, ChessEngine engine, User player)
    {

        int LookAheads = engine.NumberofMovesLookedAhead;
        int TimeTaken = engine.TimeTakenForSearch;

        int whiteScore = chessboard.calculatePlayerScore('W');
        int blackScore = chessboard.calculatePlayerScore('B');

        string LastMovePlayed = "";
        string BlackMessage = "Saw " + to_string(LookAheads) + " futures in " + to_string(TimeTaken) + " seconds";

        string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";

        if (chessboard.moveHistory.size() != 0)
        {
            LastMovePlayed = chessboard.moveHistory.back();
        }

        // Display if Black Is in check
        // Display Black Score
        // Display Engine message
        // Display Last Move
        // Display Current Player
        // Display white Score
        // Display if White Is in Check

        // Black
        if (chessboard.isCheck(chessboard, 'B', "Stats: DisplayStats: black"))
        {
            DrawTextWithCustomFont("Black King is in Check", textX, (tileSize * 1) - fontSize, fontSize, RED);
        }
        else
        {
            DrawTextWithCustomFont("Black King is not in Check", textX, (tileSize * 1) - fontSize, fontSize, BLACK);
        }

        DrawTextWithCustomFont(("Black ELO: " + to_string(engine.engineEloRating)).c_str(), textX, (tileSize * 2 - fontSize) - fontSize, fontSize, BLACK);
        DrawTextWithCustomFont(("Black Player Score: " + to_string(blackScore)).c_str(), textX, (tileSize * 3 - fontSize) - fontSize, fontSize, BLACK);
        DrawTextWithCustomFont(BlackMessage.c_str(), textX, (tileSize * 2 + fontSize) - (fontSize - 0), fontSize - 4, BLACK);

        // Current Player
        DrawTextWithCustomFont(("Current Player: " + currentPlayer).c_str(), textX, screenHeight / 2, fontSize, (currentPlayer == "White") ? WHITE : BLACK);
        DrawTextWithCustomFont(LastMovePlayed.c_str(), textX, (tileSize * 5 + fontSize) - fontSize, fontSize, RED);

        // White

        DrawTextWithCustomFont(("White Player Score: " + to_string(whiteScore)).c_str(), textX, (tileSize * 5 + fontSize) + fontSize, fontSize, WHITE);
        DrawTextWithCustomFont(("White ELO: " + to_string(player.ELO)).c_str(), textX, (tileSize * 6 + fontSize) + fontSize, fontSize, WHITE);

        if (chessboard.isCheck(chessboard, 'W', "Stats: DisplayStats: white"))
        {
            DrawTextWithCustomFont("White King is in Check", textX, (tileSize * 7) + fontSize, fontSize, RED);
        }
        else
        {
            DrawTextWithCustomFont("White King is not in Check", textX, (tileSize * 7) + fontSize, fontSize, WHITE);
        }
    }

    void DisplayStats(ChessBoard chessboard, ChessEngine Black, ChessEngine White)
    {

        int LookAheadsBlack = Black.NumberofMovesLookedAhead;
        int TimeTakenBlack = Black.TimeTakenForSearch;
        int LookAheadsWhite = White.NumberofMovesLookedAhead;
        int TimeTakenWhite = White.TimeTakenForSearch;
        int whiteScore = chessboard.calculatePlayerScore('W');
        int blackScore = chessboard.calculatePlayerScore('B');

        string LastMovePlayed = "";
        string BlackMessage = "Saw " + to_string(LookAheadsBlack) + " futures in " + to_string(TimeTakenBlack) + " milliseconds";
        string WhiteMessage = "Saw " + to_string(LookAheadsWhite) + " futures in " + to_string(TimeTakenWhite) + " milliseconds";
        string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";

        if (chessboard.moveHistory.size() != 0)
        {
            LastMovePlayed = chessboard.moveHistory.back();
        }

        // Display if Black Is in check
        // Display Black Score
        // Display Engine message
        // Display Last Move
        // Display Current Player
        // Display white Score
        // Display if White Is in Check

        // Black
        if (chessboard.isCheck(chessboard, 'B', "Stats: DisplayStats: black: 2"))
        {
            DrawTextWithCustomFont("Black King is in Check", textX, (tileSize * 1) - fontSize, fontSize, RED);
        }
        else
        {
            DrawTextWithCustomFont("Black King is not in Check", textX, (tileSize * 1) - fontSize, fontSize, BLACK);
        }

        DrawTextWithCustomFont(("Black ELO: " + to_string(Black.engineEloRating)).c_str(), textX, (tileSize * 2 - fontSize) - fontSize, fontSize, BLACK);
        DrawTextWithCustomFont(("Black Player Score: " + to_string(blackScore)).c_str(), textX, (tileSize * 3 - fontSize) - fontSize, fontSize, BLACK);
        DrawTextWithCustomFont(BlackMessage.c_str(), textX, (tileSize * 2 + fontSize) - (fontSize - 0), fontSize - 4, BLACK);

        // Current Player
        DrawTextWithCustomFont(("Current Player: " + currentPlayer).c_str(), textX, screenHeight / 2, fontSize, (currentPlayer == "White") ? WHITE : BLACK);
        DrawTextWithCustomFont(LastMovePlayed.c_str(), textX, (tileSize * 5 + fontSize) - fontSize, fontSize, RED);

        // White
        DrawTextWithCustomFont(WhiteMessage.c_str(), textX, (tileSize * 7 + fontSize) - (fontSize - 0), fontSize - 4, WHITE);
        DrawTextWithCustomFont(("White Player Score: " + to_string(whiteScore)).c_str(), textX, (tileSize * 5 + fontSize) + fontSize, fontSize, WHITE);
        DrawTextWithCustomFont(("White ELO: " + to_string(White.engineEloRating)).c_str(), textX, (tileSize * 6 + fontSize) + fontSize, fontSize, WHITE);

        if (chessboard.isCheck(chessboard, 'W', "Stats: DisplayStats: black: 1"))
        {
            DrawTextWithCustomFont("White King is in Check", textX, (tileSize * 7) + fontSize, fontSize, RED);
        }
        else
        {
            DrawTextWithCustomFont("White King is not in Check", textX, (tileSize * 7) + fontSize, fontSize, WHITE);
        }
    }

    void DisplayMoveHistory(ChessBoard chessboard)
    {
        string Moves = "";
        for (int index = 0; index < chessboard.moveHistory.size(); index++)
        {
            Moves += chessboard.moveHistory[index];
            Moves += ", ";
            if (index % 6 == 0 && index != 0)
            {
                Moves += "\n\n";
            }
        }
        DrawTextWithCustomFont(Moves.c_str(), textX, textY, fontSize, RED);
    }

    // Function to calculate the expected score of the engine against a given opponent
    double calculateExpectedScore(double engineRating, double opponentRating)
    {
        return 1.0 / (1 + pow(10, (opponentRating - engineRating) / 400));
    }

    // Function to update the engine's Elo rating based on the outcome of a game
    int updateEloRating(int engineRating, int opponentRating, bool engineWon)
    {
        double expectedScore = calculateExpectedScore(engineRating, opponentRating);
        double actualScore = engineWon ? 1.0 : 0.0;
        double kFactor = 42; // Adjust the K-factor as needed

        // Update the Elo rating based on the outcome
        return engineRating + kFactor * (actualScore - expectedScore);
    }

    void Reset()
    {
        ShowMoveHistory = false;
        winner = ' ';
        CalculateELO = true;
    }
};

class GameModes
{
    ChessBoard chessBoard;
    BoardStats statistics;
    ChessEngine Black;
    ChessEngine White;
    User Player;

public:
    void SinglePlayerMode()
    {
    }
};

pair<float, float> CalculateTextPosition(Rectangle rect, string textToShow)
{
    float textX = rect.x + (rect.width - MeasureText(textToShow.c_str(), 20)) / 2;
    float textY = rect.y + (rect.height - 20) / 2;
    return make_pair(textX, textY);
}

Rectangle CalculateTextBounds(string text, int fontSize, float x, float y)
{

    float textWidth = MeasureText(text.c_str(), fontSize) - fontSize;
    float textHeight = fontSize + 5; // Assuming single line text

    // Define the Rectangle based on text dimensions and position
    return {x, y, textWidth, textHeight};
}

void DrawButton(string text, float PosX, float PosY)
{
    int fontSize = 25;
    Rectangle button = CalculateTextBounds(text, fontSize, PosX, PosY);
    DrawRectangle(button.x, button.y, button.width, button.height, GRAY);
    DrawTextWithCustomFont(text.c_str(), button.x, button.y, fontSize, BLACK);
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Chess");
    InitAudioDevice();
    ChessBoard chessboard;
    BoardStats GameStats;
    ChessEngine Black('B');
    User Taha("Taha");

    string FENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";

    ChessPiecePlaced = LoadSound(ChessPiecePlacedFile.c_str());
    ChessPieceCaptured = LoadSound(ChessPieceCapturedFile.c_str());
    KingChecked = LoadSound(KingCheckedFile.c_str());
    GameStarts = LoadSound(GameStartsFile.c_str());
    GameEnds = LoadSound(GameEndsFile.c_str());

    Image icon = LoadImage(programIcon.c_str());

    SetWindowIcon(icon);

    myFont = LoadFont(fontFile.c_str());
    string move;

    bool DoneOnce = false;
    SetTargetFPS(60);
    bool gameStarted = false;

    Rectangle StartButton = {screenWidth / 2 - 100, screenHeight / 2 - 50, 200, 75};
    pair<float, float> StartButtonLocation = CalculateTextPosition(StartButton, "Start Game");

    // Main menu loop
    while (!WindowShouldClose())
    {
        // Draw
        BeginDrawing();
        ClearBackground(Background);
        // Check if the game has started
        if (gameStarted)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                PlaySound(KingChecked);
            }

            if (!DoneOnce)
            {
                PlaySound(GameStarts);
                chessboard.initializeBoardFromFEN(FENString);

                DoneOnce = true;
            }

            if (!GameStats.GameIsEnded(chessboard))
            {
                vector<string> PossibleMoves = chessboard.GetAllPossibleMovesInChessNotation(chessboard.isCurrentPlayerWhite() ? 'W' : 'B');
                chessboard.ComputeOpponentMoves();
                if (chessboard.isCurrentPlayerWhite())
                {

                    for (int i = 0; i < Total_tiles; i++)
                    {

                        chessboard.UpdateChessPiece(chessboard.board[i], i);
                        // chessboard.SetPiecePositions();
                    }
                }
                else
                {
                    move = Black.GenerateMove(chessboard);
                    Black.PlayMove(move, chessboard);
                    // chessboard.SetPiecePositions();
                }

                if (IsKeyPressed(KEY_H))
                {
                    GameStats.ShowMoveHistory = !GameStats.ShowMoveHistory;
                }

                if (GameStats.ShowMoveHistory)
                {
                    GameStats.DisplayMoveHistory(chessboard);
                }
                else
                {
                    GameStats.DisplayStats(chessboard, Black, Taha);
                }

                chessboard.DrawBoard();
                chessboard.DrawChessPiece();
            }
            else
            {
                chessboard.DrawBoard();
                chessboard.DrawChessPiece();
                if (GameStats.CalculateELO)
                {

                    int oldBlackELO = Black.engineEloRating;
                    Black.engineEloRating = GameStats.updateEloRating(Black.engineEloRating, Taha.ELO, (GameStats.winner == 'B'));
                    Taha.ELO = GameStats.updateEloRating(Taha.ELO, oldBlackELO, (GameStats.winner == 'W'));
                    Taha.saveEloToFile();
                    Black.saveEloToFile();
                    chessboard.saveMatchHistoryToFile();
                    GameStats.CalculateELO = false;
                }
                if (IsKeyPressed(KEY_R))
                {
                    DoneOnce = false;
                    GameStats.Reset();
                    Black.Reset();
                    chessboard.DestroyBoard();
                }
                if (IsKeyPressed(KEY_M))
                {
                    DoneOnce = false;
                    GameStats.Reset();
                    Black.Reset();
                    chessboard.DestroyBoard();
                    gameStarted = false;
                }
            }
        }
        else
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), StartButton))
                {
                    gameStarted = true;
                }
            }
            DrawRectangle(StartButton.x, StartButton.y, StartButton.width, StartButton.height, GRAY);
            DrawTextWithCustomFont("Start Game", StartButtonLocation.first, StartButtonLocation.second, 25, BLACK);
        }
        EndDrawing();
    }
    chessboard.DestroyBoard();
    UnloadSound(ChessPiecePlaced);
    UnloadSound(ChessPieceCaptured);
    UnloadSound(KingChecked);
    UnloadSound(GameEnds);
    UnloadSound(GameStarts);
    UnloadFont(myFont);
    UnloadImage(icon);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}