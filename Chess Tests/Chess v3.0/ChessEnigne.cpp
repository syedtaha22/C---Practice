#include<iostream>
#include <chrono>
#include<vector>
#include<string>
#include<unordered_map>
#include<algorithm>
#include <cmath>
#include <random> 
#include<fstream>
#include <thread>
#include<sstream>
#include<iomanip>
#include<limits>
//#include<atomic>
// #define RAYGUI_IMPLEMENTATION
// #include "raygui.h"
 #include "raylib.h"






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
string fontFile = "Resources\\Fonts\\Roboto\\Roboto-Regular.ttf";
string programIcon = "Resources\\BlackPieces\\bn.png";


Font myFont;    // Global variable, initialize at beginning.

const int screenWidth = 1500;
const int screenHeight = 800;
const int boardSize = 8;
const int tileSize = 80;
const int MaxHistorySize = 100;
const int ReverseOffset = 7;


const float fontSize = 50;

int BoardOffsetX = 800;
int BoardOffsetY = 80;

int Segments = 20;

float Roundedness = 0.02f;
float buttonWidth = 200;
float buttonHeight = 70;
float buttonCenterX = buttonWidth / 2;
float buttonCenterY = buttonHeight / 2;
float InfoBoxX = 60;
float InfoBoxY = 80;
float InfoBoxWidth = 610;
float InfoBoxHeight = 640;

bool DoneOnceperMove = false;

Rectangle PawnPromotionDialogue;

Color lightSquare = { 238, 238, 210, 255 };
Color darkSquare = { 118, 150, 86, 255 };
Color Transparent = { 0,0,0,0 };
Color Background = { 49, 46, 43, 255 };
Color Translucent = { 0,0,0,64 };
//Color Background = { 0,0,0,0 };

//Color MoveHighlight = { 187, 204, 68, 255};
Color MoveHighlight = Fade({ 255, 255,0, 200 }, 0.5);
Color MovesForPieceHighLight = Fade({ 255,0,0,200 }, 0.5);
Color PreMoveHighlight = Fade({ 0,0,255,200 }, 0.5);
Color NextMoveHighlight = Fade({ 0,255,0,200 }, 0.5);

bool gameStarted = false;
bool isMultiplayerGame = false; // Multiplayer
bool isVersusAI = false; // AI game mode

int offsets;





const int EMPTY = 0;
const int KING = 1;
const int PAWN = 2;
const int KNIGHT = 3;
const int BISHOP = 4;
const int ROOK = 5;
const int QUEEN = 6;

const int White = 8;
const int Black = 16;
const int TotalTiles = 64;


class PreMoves {

    static const int MAX_SIZE = 100;
    string arr[MAX_SIZE];
    int front, rear;

public:
    PreMoves() {
        front = -1;
        rear = -1;
    }

    void add(string value) {
        if (isFull()) {
            cout << "Queue is Full, Can't Enqueue more elements" << endl;
            return;
        }

        if (front == -1) front++;
        rear++;
        arr[rear] = value;
        cout << "Element added to Queue: " << arr[rear] << endl;

    }

    void remove() {
        if (isEmpty()) {
            cout << "Queue is Empty, Can't Dequeue elements" << endl;
            return;
        }

        //Check if element is last element of the Queue
        else if (front >= rear) {
            cout << arr[front] << " has been removed from the Queue" << endl;
            front = -1;
            rear = -1;
        }
        else {
            cout << arr[front] << " has been removed from the Queue" << endl;
            front++;
        }
    }

    string peek() const {
        if (isEmpty()) {
            cout << "String Queue is empty. Cannot peek." << endl;
            return "EMPTY";
        }
        return arr[front];
    }

    bool isEmpty() const {
        return front == -1;
    }

    bool isFull() const {
        return (front == 0 && rear == MAX_SIZE - 1);
    }

    void display() const {
        cout << "Elements are: ";
        for (int i = front; i <= rear; i++) {
            cout << arr[i] << " ";

        }
        cout << endl;
    }

    void clearQueue() {
        *this = PreMoves();
    }
};

class PreMoveIndices {

    static const int MAX_SIZE = 100;
    int arr[MAX_SIZE] = { -1 };
    int front, rear;

public:
    PreMoveIndices() {

        front = -1;
        rear = -1;
    }


    void add(int value) {
        if (isFull()) {
            cout << "Queue is Full, Can't Enqueue more elements" << endl;
            return;
        }

        if (front == -1) front++;
        rear++;
        arr[rear] = value;
        cout << "Element added to Queue: " << arr[rear] << endl;

    }

    void remove() {
        if (isEmpty()) {
            cout << "Queue is Empty, Can't Dequeue elements" << endl;
            return;
        }

        //Check if element is last element of the Queue
        else if (front >= rear) {
            cout << arr[front] << " has been removed from the Queue" << endl;
            front = -1;
            rear = -1;
        }
        else {
            cout << arr[front] << " has been removed from the Queue" << endl;
            front++;
        }
    }

    int peek() const {
        if (isEmpty()) {
            cout << "Integer Queue is empty. Cannot peek." << endl;
            return -1;
        }
        return arr[front];
    }

    bool isEmpty() const {
        return front == -1;
    }

    bool isFull() const {
        return (front == 0 && rear == MAX_SIZE - 1);
    }

    void display() const {
        cout << "Elements are: ";
        for (int i = front; i <= rear; i++) {
            cout << arr[i] << " ";

        }
        cout << endl;
    }


    bool CompareIndex(const int& index) const {
        for (int i = front; i <= rear; i++) {
            if (index == arr[i]) {
                return true;
            }

        }
        return false;

    }

    void clearQueue() {
        *this = PreMoveIndices();
    }
};

class ReadWrite {

    virtual int readEloFromFile() = 0;
    virtual void saveEloToFile() const = 0;
};

class ChessPiece {

public:
    int type;
    int color;
    bool firstMove;
    bool isSlidingPiece;
    Texture texture;
    Rectangle rectangle;
    bool isDragged;
    bool canCastleQueenSide;
    bool canCastleKingSide;
    int pieceCode;

    ChessPiece(int pieceType = EMPTY, int pieceColor = EMPTY, bool isFirstMove = true) {
        type = pieceType;
        color = pieceColor;

        pieceCode = color | type;
        firstMove = true;
        isSlidingPiece = (type == ROOK) || (type == BISHOP) || (type == QUEEN);
        canCastleQueenSide = false;
        canCastleKingSide = false;

        AssignTextures();
        rectangle.width = tileSize;
        rectangle.height = tileSize;
        isDragged = false;
    }

    void AssignTextures() {
        if (type != EMPTY) {
            string fileName = "Resources\\";
            fileName += (color == White) ? "WhitePieces\\" : "BlackPieces\\";
            fileName += std::to_string(pieceCode);
            fileName += ".png";

            std::cout << fileName << std::endl;
            
            texture = LoadTexture(fileName.c_str());
        }
    }

    void DestroyTextures() {
        if (type != EMPTY) {
            cout << "Unloaded Texture for: " << color << " " << type << endl;
            UnloadTexture(texture);
        }
    }
};

class ChessBoard {
private:
    vector<pair<int, int>> MovesForSelectedPiece;
    vector<pair<int, int>> OpponentMoves;


    bool DoCastle;
    bool PieceIsCaptured;




    pair<int, int> MoveIndices;

    const int pawnScore = 1;
    const int rookScore = 5;
    const int knightScore = 3;
    const int bishopScore = 3;
    const int queenScore = 9;
    const int kingScore = 100;

public:
    //bool isPawnPromotion;
    pair<bool, int> PawnPromotion;
    bool currentPlayerIsWhite;
    bool isBoardReversed;
    ChessPiece board[TotalTiles];
    vector<string> moveHistory;
    PreMoves preMoves;
    PreMoveIndices InitialIndexes;
    PreMoveIndices FinalIndexes;



    ChessBoard() {

        currentPlayerIsWhite = true;
        DoCastle = false;
        PieceIsCaptured = false;
        MoveIndices = { -1, -1 };
        //isPawnPromotion = false;
        PawnPromotion.first = false;
        PawnPromotion.second = -1;
        isBoardReversed = false;
    }

    int getPieceScore(const ChessPiece& piece) const {
        switch (piece.type) {
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

    void AddMoveToHistory(string move) {
        moveHistory.push_back(move);
    }

    void promotePawn(int toTile) {
        bool PawnPromoted = false;
        //cout << "\nPawn promotion! Choose a piece (Q, R, N, B): ";
        ChessPiece promotedPiece;
        //
        //if (IsKeyPressed(KEY_Q)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? wdw(QUEEN) : wdw(QUEEN), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Queen" << endl;
        //    PawnPromoted = false;
        //    //break;
        //}
        //else if (IsKeyPressed(KEY_R)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? wdw(ROOK) : wdw(ROOK), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Rook" << endl;
        //    //break;
        //}
        //else if (IsKeyPressed(KEY_N)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? wdw(KNIGHT) : wdw(KNIGHT), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Knight" << endl;
        //    //break;
        //}
        //else if (IsKeyPressed(KEY_B)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? wdw(BISHOP) : wdw(BISHOP), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Bishop" << endl;
        //    //break;
        //}
        //else {
        //    cout << "Invalid choice. Defaulting to Queen promotion." << endl;
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? wdw(QUEEN) : wdw(QUEEN), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Queen" << endl;
        //    //break;
        //}
        promotedPiece = ChessPiece(QUEEN, currentPlayerIsWhite ? White : Black);
        //cout << "Piece Promoted to Queen" << endl;
        //board[toTile].DestroyTextures();
        // Update the board with the promoted piece
        board[toTile] = promotedPiece;
        SetPiecePositions();
    }

    //void PromotePawn(char piecetype) {
    //    cout << "Promoted pawn to: " << (currentPlayerIsWhite ? wdw(piecetype) : wdw(piecetype)) << endl;
    //    PawnPromotion.first = false;
    //    ChessPiece promotedPiece = ChessPiece(currentPlayerIsWhite ? wdw(piecetype) : wdw(piecetype), currentPlayerIsWhite ? 'W' : 'B');
    //    cout << "Piece Promoted to Queen" << endl;
    //    board[PawnPromotion.second].DestroyTextures();
    //    //Update the board with the promoted piece
    //    board[PawnPromotion.second] = promotedPiece;
    //    SetPiecePositions();
    //    currentPlayerIsWhite = !currentPlayerIsWhite;
    //}


    vector<pair <int, int>> FilterValidMoves(vector<pair <int, int>> possibleMoves) const {
        //cout << possibleMoves.size() << endl;
        ChessPiece piece = board[possibleMoves[0].first]; //As Initial index Will be Same for all pairs

        // Filter out moves that leave the king in check
        vector<pair <int, int>> filteredMoves;
        for (pair <int, int> move : possibleMoves) {
            // Simulate the move on a temporary board
            ChessBoard tempBoard = *this;
            tempBoard.MakeMove(move.first, move.second);

            // Check if the king is in check after the move
            if (!tempBoard.isCheck(tempBoard, piece.color, "Board: Filter Valid Moves T")) {
                filteredMoves.push_back(move);
            }
        }
        return filteredMoves;

    }

    int calculatePlayerScore(int playerColor) const {
        int totalScore = 0;
        for (int i = 0; i < TotalTiles; i++) {
            if (board[i].color == playerColor) {
                totalScore += getPieceScore(board[i]);
            }
        }
        return totalScore;
    }

    void initializeBoardFromFEN(const string& fen) {
        moveHistory.clear();
        MoveIndices = { -1,-1 };
        int row = 0, col = 0;
        // Parse the FEN string
        for (char symbol : fen) {
            if (symbol == ' ') {
                break;  // Stop parsing after the board section
            }
            if (symbol == '/') {
                row++;
                col = 0;
            }
            else if (isdigit(symbol)) {
                // Fill empty squares
                int numEmptySquares = symbol - '0';
                for (int i = 0; i < numEmptySquares; i++) {
                    board[row * 8 + col] = EMPTY;
                    col++;
                }
            }
            else {
                // Set piece based on FEN character
                int pieceType = EMPTY;
                int pieceColor = (isupper(symbol) ? White : Black);
                switch (toupper(symbol)) {
                case 'K':
                    pieceType = KING;
                    break;
                case 'P':
                    pieceType = PAWN;
                    break;
                case 'N':
                    pieceType = KNIGHT;
                    break;
                case 'B':
                    pieceType = BISHOP;
                    break;
                case 'R':
                    pieceType = ROOK;
                    break;
                case 'Q':
                    pieceType = QUEEN;
                    break;
                default:
                    break;
                }
                board[row * 8 + col] = ChessPiece(pieceType, pieceColor);
                col++;
            }
        }
        // Set the current player based on FEN
        currentPlayerIsWhite = (fen.find("w") != string::npos);
        int WhiteKingIndex = GetKingIndex(White);
        int BlackKingIndex = GetKingIndex(Black);
        size_t castlingRightsPos = fen.find_last_of(" ");
        if (castlingRightsPos != string::npos && castlingRightsPos + 1 < fen.length()) {
            string castlingRights = fen.substr(castlingRightsPos + 1);
            for (char c : castlingRights) {
                if (c == 'K') {
                    board[WhiteKingIndex].canCastleKingSide = true;
                }
                else if (c == 'Q') {
                    board[WhiteKingIndex].canCastleQueenSide = true;
                }
                else if (c == 'k') {
                    board[BlackKingIndex].canCastleKingSide = true;
                }
                else if (c == 'q') {
                    board[BlackKingIndex].canCastleQueenSide = true;
                }
            }
        }

        // Set piece positions and additional FEN features if needed
        SetPiecePositions();
        // ...code for additional FEN features
    }

    void saveMatchHistoryToFile()const {
        // Open file for writing (in append mode)
        ofstream file("MatchHistory.txt", ios::app);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file for writing." << endl;
            return;
        }

        // Generate FEN string from current board state
        string fen;
        for (int row = 0; row < 8; row++) {
            int emptyCount = 0;
            for (int col = 0; col < 8; col++) {
                int index = row * 8 + col;
                if (board[index].type == EMPTY) {
                    emptyCount++;
                }
                else {
                    if (emptyCount > 0) {
                        fen += to_string(emptyCount);
                        emptyCount = 0;
                    }
                    fen += board[index].type;
                }
            }
            if (emptyCount > 0) {
                fen += to_string(emptyCount);
            }
            if (row < 7) {
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


    void initializeBoard() {
        // Set up the board with pieces in their initial positions

        // Black pieces
        board[0] = ChessPiece(ROOK, Black);
        board[1] = ChessPiece(KNIGHT, Black);
        board[2] = ChessPiece(BISHOP, Black);
        board[3] = ChessPiece(QUEEN, Black);
        board[4] = ChessPiece(KING, Black);
        board[5] = ChessPiece(BISHOP, Black);
        board[6] = ChessPiece(KNIGHT, Black);
        board[7] = ChessPiece(ROOK, Black);
        for (int i = 8; i <= 15; i++) {
            board[i] = ChessPiece(PAWN, Black);
        }

        // Empty spaces in the middle
        for (int i = 16; i <= 47; i++) {
            board[i] = ChessPiece(EMPTY, EMPTY);

        }

        // White pieces
        for (int i = 48; i <= 55; i++) {
            board[i] = ChessPiece(PAWN, White);
        }
        board[56] = ChessPiece(ROOK, White);
        board[57] = ChessPiece(KNIGHT, White);
        board[58] = ChessPiece(BISHOP, White);
        board[59] = ChessPiece(QUEEN, White);
        board[60] = ChessPiece(KING, White);
        board[61] = ChessPiece(BISHOP, White);
        board[62] = ChessPiece(KNIGHT, White);
        board[63] = ChessPiece(ROOK, White);
        SetPiecePositions();
    }


    void DisplayBoard() const {
        cout << "8 ";
        for (int i = 0; i < TotalTiles; i++) {
            // cout << (i/8) + 1  << " ";
            cout << "[" << board[i].type << "]";
            if ((i + 1) % 8 == 0 && (i + 1) < TotalTiles) {
                cout << endl;
                cout << 8 - (i + 1) / 8 << " ";
            }

            // cout << endl;
        }
        cout << endl;
        cout << "   a  b  c  d  e  f  g  h";
    }

    pair<int, int> PieceCoordinates(int pieceIndex) {
        if (pieceIndex < 0 || pieceIndex >= TotalTiles) {

            return make_pair(-1, -1);
        }
        int rank = pieceIndex / boardSize;
        int file = pieceIndex % boardSize;
        return make_pair(rank, file);
    }

    void SetPiecePositions() {
        // Set initial positions of the pieces
        for (int index = 0; index < TotalTiles; index++) {
            pair<int, int>  PieceCoords = PieceCoordinates(index);
            board[index].rectangle.x = BoardOffsetX + static_cast<float>((abs((isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize);
            board[index].rectangle.y = BoardOffsetY + static_cast<float>((abs((isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize);
        }
    }

    void DrawBoard() {
        for (int index = 0; index < TotalTiles; index++) {


            pair<int, int> PieceCoords = PieceCoordinates(index);


            Color squareColor = (PieceCoords.first + PieceCoords.second) % 2 == 0 ? lightSquare : darkSquare;

            int LocationX = BoardOffsetX + (abs((isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize;
            int LocationY = BoardOffsetY + (abs((isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize;

            DrawRectangle(LocationX, LocationY, tileSize, tileSize, squareColor);

            if (!FinalIndexes.isEmpty() && FinalIndexes.peek() == index) {
                DrawRectangle(LocationX, LocationY, tileSize, tileSize, MovesForPieceHighLight);

            }
            else if (FinalIndexes.CompareIndex(index) && FinalIndexes.peek() != index) {
                DrawRectangle(LocationX, LocationY, tileSize, tileSize, MovesForPieceHighLight);

            }


            //Highlight Last Move
            if (MoveIndices.first == index || MoveIndices.second == index) {
                DrawRectangle(LocationX, LocationY, tileSize, tileSize, MoveHighlight);
            }
            //Show Moves for Piece
            for (const auto& move : MovesForSelectedPiece) {
                if (move.second == index) {
                    float Diameter = 26;
                    float Radius = Diameter / 2;
                    float Offset = Diameter + Radius;

                    Rectangle CorrectMoves = { static_cast<float>(LocationX) + Offset, static_cast<float>(LocationY) + Offset, Diameter, Diameter };
                    DrawCircle(LocationX + static_cast<int>(Offset), LocationY + static_cast<int>(Offset), Radius, Translucent);
                }
            }
        }
    }

    void DrawChessPiece() {
        for (int index = 0; index < TotalTiles; index++) {
            int PositionX = static_cast<int>(board[index].rectangle.x);
            int PositionY = static_cast<int>(board[index].rectangle.y);
            DrawTexture(board[index].texture, PositionX, PositionY, WHITE);
        }
    }

    int GetKingIndex(int playercolor) const {
        for (int index = 0; index < TotalTiles; index++) {
            if (board[index].type == KING && board[index].color == playercolor) {
                return index;
            }
        }

        cout << "\n\nGetKingIndex: KING not Found\n\n";
        exit(404);

    }

    int getTileIndex(float x, float y, int tileSize) {
        // Calculate the file (column) index
        int file = static_cast<int>(x / tileSize);
        // Calculate the rank (row) index
        int rank = static_cast<int>(y / tileSize);
        // Calculate the index in the array
        //cout << rank << " " << file << endl;
        int index = rank * 8 + file;
        return index;
    }

    void DisplayMoves(vector<int> moves)const {
        for (auto move : moves) {
            cout << move << " ";
        }
        cout << endl;
    }

    void UpdateChessPiece(ChessPiece& piece, int InitialIndex) {
        if (piece.type != EMPTY) {

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), piece.rectangle)) {
                piece.isDragged = true;
                MovesForSelectedPiece = GetAllPossibleMovesForPiece(piece.type, InitialIndex);
                //DisplayMoves(MovesForSelectedPiece);
                //cout << InitialIndex << endl;

            }
            if (piece.isDragged) {
                piece.rectangle.x = GetMouseX() - piece.rectangle.width / 2;
                piece.rectangle.y = GetMouseY() - piece.rectangle.height / 2;
                if (piece.rectangle.x < 0) piece.rectangle.x = 0;
                if (piece.rectangle.y < 0) piece.rectangle.y = 0;
                if (piece.rectangle.x + piece.rectangle.width > GetScreenWidth()) piece.rectangle.x = GetScreenWidth() - piece.rectangle.width;
                if (piece.rectangle.y + piece.rectangle.height > GetScreenHeight()) piece.rectangle.y = GetScreenHeight() - piece.rectangle.height;






                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    float tileX = round((piece.rectangle.x - BoardOffsetX) / tileSize) * tileSize;
                    float tileY = round((piece.rectangle.y - BoardOffsetY) / tileSize) * tileSize;
                    int FinalIndex = abs((isBoardReversed * (TotalTiles - 1)) - getTileIndex(tileX, tileY, tileSize));
                    InitialIndex = abs((isBoardReversed * ReverseOffset) - InitialIndex); //adjust for Reversals
                    pair<int, int> Move = make_pair(InitialIndex, FinalIndex);
                    


                    //cout << move << endl;
                    //cout << InitialIndex << " " << FinalIndex << endl;


                    //cout << tileX << " " << tileY << endl;
                    if (isValidMove(Move)) {
                        piece.isDragged = false;
                        piece.rectangle.x = tileX;
                        piece.rectangle.y = tileY;

                        
                        MakeCompleteMove(Move);

                        //WaitTime(0.5);

                    }
                    else {
                        piece.isDragged = false;
                        int file = abs((isBoardReversed * ReverseOffset) - (InitialIndex % 8));
                        int rank = abs((isBoardReversed * ReverseOffset) - (InitialIndex / 8));
                        piece.rectangle.x = BoardOffsetX + static_cast<float>(file * tileSize);
                        piece.rectangle.y = BoardOffsetY + static_cast<float>(rank * tileSize);

                    }

                }
            }
        }
    }

    //void HandlePremoves(ChessPiece& piece, int InitialIndex) {
    //    if (piece.type != EMPTY) {
    //        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), piece.rectangle)) {
    //            piece.isDragged = true;
    //            MovesForSelectedPiece = GetAllPossibleMovesForPiece(piece.type, InitialIndex);
    //            DisplayMoves(MovesForSelectedPiece);
    //        }
    //        if (piece.isDragged) {
    //            piece.rectangle.x = GetMouseX() - piece.rectangle.width / 2;
    //            piece.rectangle.y = GetMouseY() - piece.rectangle.height / 2;
    //            if (piece.rectangle.x < 0) piece.rectangle.x = 0;
    //            if (piece.rectangle.y < 0) piece.rectangle.y = 0;
    //            if (piece.rectangle.x + piece.rectangle.width > GetScreenWidth()) piece.rectangle.x = GetScreenWidth() - piece.rectangle.width;
    //            if (piece.rectangle.y + piece.rectangle.height > GetScreenHeight()) piece.rectangle.y = GetScreenHeight() - piece.rectangle.height;
    //            float tileX = round((piece.rectangle.x - BoardOffsetX) / tileSize) * tileSize;
    //            float tileY = round((piece.rectangle.y - BoardOffsetY) / tileSize) * tileSize;
    //            int FinalIndex = getTileIndex(tileX, tileY, tileSize);
    //            string move = ConvertToChessNotation(InitialIndex, FinalIndex);
    //            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    //                if (isValidMove(FinalIndex)) {
    //                    piece.isDragged = false;
    //                    piece.rectangle.x = tileX;
    //                    piece.rectangle.y = tileY;
    //                    AddMoveToQueue(move, InitialIndex, FinalIndex);
    //                    MakeMove(InitialIndex, FinalIndex);
    //                    SetPiecePositions();
    //                }
    //                else {
    //                    piece.isDragged = false;
    //                    int file = InitialIndex % 8;
    //                    int rank = InitialIndex / 8;
    //                    piece.rectangle.x = BoardOffsetX + static_cast<float>(file * tileSize);
    //                    piece.rectangle.y = BoardOffsetY + static_cast<float>(rank * tileSize);
    //                }
    //            }
    //        }
    //    }
    //}

    void MakeTemporaryMove() {

    }

    bool isValidMove(pair<int, int> move) {
        for (const auto& Move : MovesForSelectedPiece) {
            if (move == Move) {
                return true;
            }
        }
        return false;
    }

    void MakeMove(int fromTile, int toTile) {
        string move = ConvertToChessNotation(make_pair(fromTile, toTile));



        // Check if it's a castling move and if castling is allowed
        if (IsCastlingMove(move, board[fromTile])) {



            if (fromTile == 62 || fromTile == 7) { // Kingside castling 
                int rookFromTile = (fromTile == 60) ? 63 : 7;
                int rookToTile = (fromTile == 60) ? 61 : 5;
                board[rookFromTile].firstMove = false;
                board[rookToTile] = board[rookFromTile];
                board[rookFromTile] = ChessPiece();


            }
            else if (fromTile == 58 || fromTile == 2) { // Queenside castling 
                int rookFromTile = (fromTile == 60) ? 56 : 0;
                int rookToTile = (fromTile == 60) ? 59 : 3;
                board[rookFromTile].firstMove = false;
                board[rookToTile] = board[rookFromTile]; // Move the rook
                board[rookFromTile] = ChessPiece(); // Empty the original rook square
            }
        }


        board[fromTile].firstMove = false;
        board[toTile] = board[fromTile];
        board[fromTile] = ChessPiece(); // Empty the source square
        MovesForSelectedPiece.clear();
        //if(isMultiplayerGame) ReverseBoard();
    }

    bool IsCastlingMove(string move, ChessPiece pieceMoved) {

        return (pieceMoved.type == KING) && (move == "e1g1" || move == "e1c1" || move == "e8g8" || move == "e8c8");
    }

    //void MakeCompleteMove(int fromTile, int toTile, string move) {
    //    PieceIsCaptured = isValidCaptureMove(fromTile, toTile);
    //    MakeMove(fromTile, toTile);
    //    // Check for pawn promotion
    //    if (wdw(board[toTile].type) == PAWN) {
    //        // White pawn promotion
    //        if (toTile >= 0 && toTile <= 7) {
    //            PawnPromotionDialogue.x = board[toTile].rectangle.x;
    //            PawnPromotionDialogue.y = board[toTile].rectangle.y;
    //            cout << board[toTile].rectangle.x << " " << PawnPromotionDialogue.y << endl;
    //            PawnPromotionDialogue.width = tileSize;
    //            PawnPromotionDialogue.height = tileSize;
    //            currentPlayerIsWhite = !currentPlayerIsWhite;
    //            PawnPromotion.first = true;
    //            PawnPromotion.second = toTile;
    //            //promotePawn(toTile);
    //        }
    //        // Black pawn promotion
    //        else if (toTile >= 56 && toTile <= 63) {
    //            cout << "Test";
    //            currentPlayerIsWhite = !currentPlayerIsWhite;
    //            PawnPromotion.first = true;
    //            PawnPromotion.second = toTile;
    //            //promotePawn(toTile);
    //        }
    //    }
    //    SetPiecePositions();
    //    if (moveHistory.size() >= MaxHistorySize) {
    //        moveHistory.clear();
    //    }
    //    AddMoveToHistory(move);
    //    MoveIndices = { fromTile, toTile };
    //    currentPlayerIsWhite = !currentPlayerIsWhite;
    //    PlayChessSound();
    //    //DisplayScores();
    //    SlidingPieceMoveData.clear();
    //    PawnMoveData.clear();
    //    KnightMoveData.clear();
    //    KingMoveData.clear();
    //    MovesForSelectedPiece.clear();
    //}

    void MakeCompleteMove(pair<int, int> Move) {
        string move = ConvertToChessNotation(Move);
        PieceIsCaptured = isValidCaptureMove(Move);
        MakeMove(Move.first,Move.second);

        // Check for pawn promotion
        if (board[Move.second].type == PAWN) {
            // White pawn promotion
            if (Move.second >= 0 && Move.second <= 7) {
                promotePawn(Move.second);
            }
            // Black pawn promotion
            else if (Move.second >= 56 && Move.second <= 63) {
                cout << "Test";
                promotePawn(Move.second);
            }

        }


        cout << (currentPlayerIsWhite ? "White" : "Black") <<  " Plays : " << move << "\n";

        if (moveHistory.size() >= MaxHistorySize) {
            moveHistory.clear();
        }

        AddMoveToHistory(move);
        MoveIndices = Move;

       

        SetPiecePositions();

        currentPlayerIsWhite = !currentPlayerIsWhite;
        PlayChessSound();
        MovesForSelectedPiece.clear();


    }

    void ReverseBoard() {
        isBoardReversed = !isBoardReversed;
    }

    void PlayChessSound() const {
        ChessBoard temp = *this;
        char PlayerColor = (temp.isCurrentPlayerWhite()) ? White : Black;
        if (isCheck(temp, PlayerColor, "board: Play Chess Sound")) {
            PlaySound(KingChecked);
        }
        else if (PieceIsCaptured) {
            PlaySound(ChessPieceCaptured);
        }
        else {
            PlaySound(ChessPiecePlaced);
        }


    }

    bool isCurrentPlayerWhite() const {
        return currentPlayerIsWhite;
    }

    void DisplayScores() const {
        int whiteScore = calculatePlayerScore('W');
        int blackScore = calculatePlayerScore('B');
        cout << endl << "White score: " << whiteScore << " | Black score: " << blackScore << endl << endl;

    }

    void ComputeSlidingPieceMoves(int pieceIndex, vector<pair <int, int>>& possibleMoves) const {
        ChessPiece piece = board[pieceIndex];
        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        // Offsets for sliding directions: up, down, left, right, diagonals
        vector<int> offsets = { 8, -8, 1, -1, 7, -7, 9, -9 };

        if (piece.type == ROOK || piece.type == QUEEN) {
            for (int i = 0; i < 4; ++i) {
                int offset = offsets[i];
                int newIndex = pieceIndex + offset;
                while (newIndex >= 0 && newIndex < TotalTiles) {
                    // Check if the new index is within the bounds of the board
                    if ((offset == 1 || offset == -1) && newIndex / 8 != row)
                        break;  // If moving horizontally, ensure it stays on the same row
                    if (board[newIndex].type != EMPTY) {
                        if (board[newIndex].color != piece.color) {
                            // If the square contains an enemy piece, add it to possibleMoves and stop searching in this direction

                            possibleMoves.push_back(make_pair(pieceIndex, newIndex));
                        }
                        break; // Stop searching in this direction if we encounter a piece
                    }
                    possibleMoves.push_back(make_pair(pieceIndex, newIndex));
                    newIndex += offset;
                }
            }
        }

        if (piece.type == BISHOP || piece.type == QUEEN) {
            for (int i = 4; i < 8; ++i) {
                int offset = offsets[i];
                int newIndex = pieceIndex + offset;
                while (newIndex >= 0 && newIndex < TotalTiles) {
                    // Check if the new index is within the bounds of the board
                    int newRow = newIndex / 8;
                    int newCol = newIndex % 8;
                    if (abs(newRow - row) != abs(newCol - col))
                        break;  // If not moving diagonally, break
                    if (board[newIndex].type != EMPTY) {
                        if (board[newIndex].color != piece.color) {
                            // If the square contains an enemy piece, add it to possibleMoves and stop searching in this direction
                            possibleMoves.push_back(make_pair(pieceIndex, newIndex));
                        }
                        break; // Stop searching in this direction if we encounter a piece
                    }
                    possibleMoves.push_back(make_pair(pieceIndex, newIndex));
                    newIndex += offset;
                }
            }
        }
    }

    void ComputeKnightMoves(int pieceIndex, vector<pair <int, int>>& possibleMoves) const {
        ChessPiece piece = board[pieceIndex];
        // Offsets for knight's moves
        vector<int> rowOffsets = { -2, -1, 1, 2, 2, 1, -1, -2 };
        vector<int> colOffsets = { 1, 2, 2, 1, -1, -2, -2, -1 };

        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        if (piece.type == KNIGHT) {
            for (int i = 0; i < 8; ++i) {
                int newRow = row + rowOffsets[i];
                int newCol = col + colOffsets[i];

                // Check if the new position is within the board bounds
                if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                    int newIndex = newRow * 8 + newCol;
                    // Check if the square is empty or contains an opponent's piece
                    if (board[newIndex].type == EMPTY || board[newIndex].color != piece.color) {
                        possibleMoves.push_back(make_pair(pieceIndex, newIndex));
                    }
                }
            }
        }
    }

    void ComputePawnMoves(int pieceIndex, vector<pair <int, int>>& possibleMoves) const {
        ChessPiece piece = board[pieceIndex];
        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        // Determine the direction of pawn movement based on its color
        int direction = (piece.color == White) ? -1 : 1;

        // Forward movement
        int forwardOne = pieceIndex + direction * 8;
        if (forwardOne >= 0 && forwardOne < TotalTiles && board[forwardOne].type == EMPTY) {
            possibleMoves.push_back(make_pair(pieceIndex, forwardOne));
            // Check if the pawn is in its starting position and can move two squares forward
            if ((row == 6 && piece.color == White) || (row == 1 && piece.color == Black)) {
                int forwardTwo = forwardOne + direction * 8;
                if (board[forwardTwo].type == EMPTY) {
                    possibleMoves.push_back(make_pair(pieceIndex, forwardTwo));
                }
            }
        }


        // Capture moves
        vector<int> captureOffsets = { direction * 7, direction * 9 };
        for (int offset : captureOffsets) {
            int targetIndex = pieceIndex + offset;
            int targetRow = targetIndex / 8;
            int targetCol = targetIndex % 8;
            int pieceCol = pieceIndex % 8;
            // Ensure target column is within bounds and not wrapping around the board
            if (targetCol >= 0 && targetCol < 8 && abs(targetCol - pieceCol) <= 1) {
                // Check if the target square is occupied by an opponent's piece
                if (board[targetIndex].type != EMPTY && board[targetIndex].color != piece.color) {
                    possibleMoves.push_back(make_pair(pieceIndex, targetIndex));
                }
            }
        }

    }

    void ComputeKingMoves(int KingIndex, vector<pair <int, int>>& possibleMoves) const {
        ChessPiece King = board[KingIndex];
        int row = KingIndex / 8;
        int col = KingIndex % 8;

        // Offsets for king's moves
        vector<int> rowOffsets = { -1, -1, -1, 0, 0, 1, 1, 1 };
        vector<int> colOffsets = { -1, 0, 1, -1, 1, -1, 0, 1 };

        // Check all 8 directions
        for (int i = 0; i < 8; i++) {
            int newRow = row + rowOffsets[i];
            int newCol = col + colOffsets[i];

            // Check if the new position is within the board bounds
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                int newIndex = newRow * 8 + newCol;
                // Check if the square is empty or contains an opponent's piece
                if ((board[newIndex].type == EMPTY || board[newIndex].color != King.color)) {
                    possibleMoves.push_back(make_pair(KingIndex, newIndex));
                }
            }
        }

        if ((King.color == Black && KingIndex == 4) || (King.color == White && KingIndex == 60)) {
            
            // Check for castling moves
            if (King.firstMove && !IsTileUnderAttack(KingIndex)) {
                // Check kingside castling
                if (canCastleKingSide(KingIndex)) {
                    possibleMoves.push_back(make_pair(KingIndex, KingIndex + 2));
                }
                // Check queenside castling
                if (canCastleQueenide(KingIndex)) {
                    cout << KingIndex << endl;
                    possibleMoves.push_back(make_pair(KingIndex, KingIndex - 2));
                }
            }

        }


    }

    bool canCastleKingSide(int KingIndex) const {
        if (!board[KingIndex].canCastleKingSide) {
            return false;

        }
        int RookIndex = KingIndex + 3;
        if (board[RookIndex].type != ROOK && !(board[RookIndex].firstMove)) {
            return false;
        }

        for (int i = KingIndex + 1; i < RookIndex; i++) {
            if (board[i].type != EMPTY || IsTileUnderAttack(i)) {
                return false;
            }
        }

        return true;
    }

    bool canCastleQueenide(int KingIndex) const {
        if (!board[KingIndex].canCastleQueenSide) {
            return false;

        }

        int RookIndex = KingIndex - 4;
        if (board[RookIndex].type != ROOK && !(board[RookIndex].firstMove)) {
            return false;
        }

        for (int i = KingIndex - 1; i > RookIndex; i--) {
            if (board[i].type != EMPTY || IsTileUnderAttack(i)) {
                return false;
            }
        }

        return true;
    }

    bool isCheck(const ChessBoard chessboard, int playerColor, string calledby) const {
        // Find the king's position
        int kingIndex = -100;
        for (int i = 0; i < TotalTiles; i++) {
            if (chessboard.board[i].type == KING && chessboard.board[i].color == playerColor) {
                kingIndex = i;
                break;
            }
        }

        if (kingIndex == -100) {
            cout << kingIndex;
            cout << "\nError Code 404: King not found. \n\n";
            cout << calledby << "\n\n";
            //exit(404);
            return false;
        }

        // Iterate through opponent's pieces and see if any can attack the king
        int opponentColor = (playerColor == White) ? Black : White;

        for (int i = 0; i < TotalTiles; i++) {
            vector<pair<int, int>> possibleMoves;
            if (chessboard.board[i].color == opponentColor) {
                if (chessboard.board[i].isSlidingPiece) {
                    ComputeSlidingPieceMoves(i, possibleMoves);
                }
                else if (chessboard.board[i].type == PAWN) {
                    ComputePawnMoves(i, possibleMoves);
                }
                else if (chessboard.board[i].type == KNIGHT) {
                    ComputeKnightMoves(i, possibleMoves);
                }
                else if (chessboard.board[i].type == KING) {
                    ComputeKingMoves(i, possibleMoves);
                }



                // Check if any of the possible moves include the king's position
                for (const auto& move : possibleMoves) {
                    if (move.second == kingIndex) {
                        possibleMoves.clear();
                        return true; // King is in check
                    }
                }
                possibleMoves.clear();
            }
        }

        return false; // King is not in check
    }

    bool isCheckmate(ChessBoard& chessboard, char playerColor) const {

        vector<pair<int, int>> possibleMoves = GetAllPossibleMovePairs(playerColor);
        if (isCheck(chessboard, playerColor, "board: Is Check Mate") && possibleMoves.size() == 0) {
            return true; //No Valid Move, CheckMate
        }

        return false; // King is either not in check, or can escape(moves > 0)
    }

    string ConvertToChessNotation(pair<int, int> move) const {

        string chessNotation = "";


        // Convert the 'fromTile' index to chess notation
        char fromCol = 'a' + (move.first % 8);
        int fromRow = 8 - (move.first / 8);
        chessNotation += fromCol;
        chessNotation += to_string(fromRow);

        // Add the 'toTile' index to chess notation
        char toCol = 'a' + (move.second % 8);
        int toRow = 8 - (move.second / 8);
        chessNotation += toCol;
        chessNotation += to_string(toRow);


        return chessNotation;
    }

    int getAttacksOnSquare(int squareIndex, int opponentColor) const {
        int attacks = 0;
        vector<pair<int, int>> opponentMoves = GetAllPossibleMovePairs(opponentColor);
        for (int i = 0; i < opponentMoves.size(); ++i) {
            if (opponentMoves[i].second == squareIndex) {
                attacks++;
            }
        }
        return attacks;
    }

    bool IsTileUnderAttack(int& squareIndex) const {
        if (OpponentMoves.size() != 0) {
            for (int i = 0; i < OpponentMoves.size(); i++) {
                if (OpponentMoves[i].first == squareIndex) {
                    return true;
                }
            }
        }

        return false;
    }

    void DisplayOpponentMoves() {
        for (auto& move : OpponentMoves) {
            cout << move.first << " " << move.second << endl;
        }

    }

    vector<pair<int, int>> GetAllPossibleMovesForPiece(int type, int index) const {
        vector<pair<int, int>> moves;

        // Compute moves based on the piece type
        if (type == ROOK || type == BISHOP || type == QUEEN) {
            ComputeSlidingPieceMoves(index, moves);
        }
        else if (type == PAWN) {
            ComputePawnMoves(index, moves);
        }
        else if (type == KNIGHT) {
            ComputeKnightMoves(index, moves);
        }
        else if (type == KING) {

            ComputeKingMoves(index, moves);
        }


        moves = FilterValidMoves(moves);
        return moves;
    }

    void DestroyBoard() {
        for (int i = 0; i < TotalTiles; i++) {
            board[i].DestroyTextures();
        }

    }

    ChessPiece GetPieceAtPosition(int index) const {
        return board[index];
    }

    vector<pair<int, int>> GetAllCaptureMovesInChessNotation(int color) const {
        vector<pair<int, int>> captureMoves;

        for (int fromTile = 0; fromTile < TotalTiles; fromTile++) {
            ChessPiece piece = board[fromTile];
            if (piece.color == color && piece.type != EMPTY) {
                for (int toTile = 0; toTile < TotalTiles; toTile++) {
                    ChessPiece OpponentPiece = board[toTile];
                    if (fromTile != toTile && OpponentPiece.type != EMPTY && OpponentPiece.color != color) {
                        captureMoves.push_back(make_pair(fromTile, toTile));
                    }
                }
            }
        }

        return captureMoves;
    }


    bool isValidCaptureMove(pair<int, int> move) const {
        ChessPiece fromPiece = board[move.first];
        ChessPiece toPiece = board[move.second];

        return fromPiece.type != EMPTY && toPiece.type != EMPTY && fromPiece.color != toPiece.color;
    }

    pair<int, int> convertChessNotationToIndices(const string& move) const {
        int fromCol = move[0] - 'a';
        int fromRow = 7 - (move[1] - '1');
        int toCol = move[2] - 'a';
        int toRow = 7 - (move[3] - '1');
        int fromTile = fromRow * 8 + fromCol;
        int toTile = toRow * 8 + toCol;
        return make_pair(fromTile, toTile);
    }

    void ComputeOpponentMoves() {
        OpponentMoves = GetAllPossibleMovePairs(isCurrentPlayerWhite() ? White : Black);
    }

    bool isValidMoveFormat(const string& move) const {
        return (move.length() == 4 &&
            move[0] >= 'a' && move[0] <= 'h' &&
            move[1] >= '1' && move[1] <= '8' &&
            move[2] >= 'a' && move[2] <= 'h' &&
            move[3] >= '1' && move[3] <= '8');
    }

    void AddMoveToQueue(const string& move, const int& initial, const int& final) {
        preMoves.add(move);
        InitialIndexes.add(initial);
        FinalIndexes.add(final);

        //cout << "\n Added " << move << " to queue\n";
    }

    //void MakePreMove() {
    //    MakeCompleteMove(InitialIndexes.peek(), FinalIndexes.peek(), preMoves.peek());
    //    InitialIndexes.remove();
    //    FinalIndexes.remove();
    //    preMoves.remove();
    //}

    void clearPreMoveData() {
        preMoves.clearQueue();
        InitialIndexes.clearQueue();
        FinalIndexes.clearQueue();
    }

    vector<pair <int, int>> GetAllPossibleMovePairs(int PlayerColor) const{
        vector<pair <int, int>> possibleMoves;

        for (int i = 0; i < TotalTiles; i++) {
            if (board[i].color == PlayerColor) {
                vector<pair <int, int>> moves;

                // Compute moves based on the piece type
                if (board[i].isSlidingPiece) {
                    ComputeSlidingPieceMoves(i, moves);
                }
                else if (board[i].type == PAWN) {
                    ComputePawnMoves(i, moves);
                }
                else if (board[i].type == KNIGHT) {
                    ComputeKnightMoves(i, moves);
                }
                else if (board[i].type == KING) {
                    ComputeKingMoves(i, moves);

                }
                //Can't filter moves if no moves to filter
                if (moves.size() != 0) {
                    moves = FilterValidMoves(moves);
                    for (auto& move : moves) {
                        possibleMoves.push_back(move);
                    }
                }

            }
        }

        //cout << "\nThis is Working\n";
        return possibleMoves;
    }

    vector<pair <int, int>> GetAllPossibleMovePairs() const {
        int PlayerColor = (isCurrentPlayerWhite()) ? White : Black;
        vector<pair <int, int>> possibleMoves;

        for (int i = 0; i < TotalTiles; i++) {
            if (board[i].color == PlayerColor) {
                vector<pair <int, int>> moves;

                // Compute moves based on the piece type
                if (board[i].isSlidingPiece) {
                    ComputeSlidingPieceMoves(i, moves);
                }
                else if (board[i].type == PAWN) {
                    ComputePawnMoves(i, moves);
                }
                else if (board[i].type == KNIGHT) {
                    ComputeKnightMoves(i, moves);
                }
                else if (board[i].type == KING) {
                    ComputeKingMoves(i, moves);

                }
                //Can't filter moves if no moves to filter
                if (moves.size() != 0) {
                    moves = FilterValidMoves(moves);
                    for (auto& move : moves) {
                        possibleMoves.push_back(move);
                    }
                }

            }
        }

        //cout << "\nThis is Working\n";
        return possibleMoves;

    }

    vector<string> GetAllPossibleMovesInChessNotation(int PlayerColor) const{
        vector < pair<int, int>> PossibleMoves = GetAllPossibleMovePairs(PlayerColor);
        vector<string> moves;
        for (const auto& move : PossibleMoves) {
            moves.push_back(ConvertToChessNotation(move));

        }
        return moves;

    }

};

// Define a structure for a transposition table entry
class TranspositionTables {

    uint64_t zobristKeys[64][12]; // 64 squares, 12 piece types

    unordered_map<uint64_t, pair<float, int>> transpositionTable; // <hash, <score, depth>>


public:
    double SizeOfTranspostionTable;

    TranspositionTables() {
        SizeOfTranspostionTable = 0.0;

        initZobristKeys();
    }

    void initZobristKeys() {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);

        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 12; ++j) {
                zobristKeys[i][j] = dis(gen);
               
            }
        }
    }

    // Compute hash value for the current board position
    uint64_t computeHash(const ChessBoard& board) const {
        uint64_t hash = 0;
        for (int i = 0; i < 64; ++i) {
            ChessPiece piece = board.board[i];
            if (piece.type != EMPTY) {
                int index = (piece.color == White) ? (piece.type -1) : (5 + piece.type);
                hash ^= zobristKeys[i][index];
            }
        }
        return hash;
    }


    // Search in transposition table
    pair<float, int> lookupTranspositionTable(uint64_t hash) const {
        auto it = transpositionTable.find(hash);
        if (it != transpositionTable.end()) {
            return it->second;
        }
        return { -INFINITY, -1 }; // Not found
    }

    // Store in transposition table
    void storeTranspositionTable(uint64_t hash, float score, int depth) {
        transpositionTable[hash] = { score, depth };
        ComputeSizeOfTranspositionTable();
    }

    void ComputeSizeOfTranspositionTable() {

        size_t pairSize = sizeof(pair<const uint64_t, pair<float, int>>);                        // Size of each key-value pair in bytes
        size_t numPairs = transpositionTable.size();                                             // Total number of pairs in the map
        size_t totalPairSize = pairSize * numPairs;                                              // Total size of pairs in bytes
        size_t overhead = sizeof(transpositionTable);                                            // Overhead of the unordered_map itself
        SizeOfTranspostionTable = static_cast<double>(totalPairSize + overhead) / (1024 * 1024); // Convert bytes to megabytes


    }

    bool isValuePresent(uint64_t hash) {

        for (const auto& key : transpositionTable) {
            if (key.first == hash) {
                return true;
            }
        }
        return false;
    }

    //~TranspositionTables(){
    //    for (int i = 0; i < 64; ++i) {
    //        delete[] zobristKeys[i];
    //    }
    //    delete[] zobristKeys;

    //}

};




class ChessEngine : public ReadWrite {
    int PlayingAs;
    int Depth = 2; // Maximum depth for the Minimax algorithm
    TranspositionTables transpostionTable;


    bool terminateSearch; // Atomic flag variable to terminate the search
    bool startSearch; // Atomic flag variable to start the search


    // Piece-square tables for each piece type
    // Higher values indicate a more advantageous position for the piece

    const int PawnPST[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
         5,  5, 10, 25, 25, 10,  5,  5,
         0,  0,  0, 20, 20,  0,  0,  0,
         5, -5,-10,  0,  0,-10, -5,  5,
         5, 10, 10,-20,-20, 10, 10,  5,
         0,  0,  0,  0,  0,  0,  0,  0
    };
    const int RooksPST[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
         5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
         0,  0,  0,  5,  5,  0,  0,  0
    };
    const int KnightsPST[64] = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    };
    const int BishopsPST[64] = {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    };
    const int QueensPST[64] = {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
         -5,  0,  5,  5,  5,  5,  0, -5,
          0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    };
    const int KingPST[64] = {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
         20, 20,  0,  0,  0,  0, 20, 20,
         20, 30, 10,  0,  0, 10, 30, 20
    };
    const int KingEndgamePST[64]{
        -50,-40,-30,-20,-20,-30,-40,-50,
        -30,-20,-10,  0,  0,-10,-20,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-30,  0,  0,  0,  0,-30,-30,
        -50,-30,-30,-30,-30,-30,-30,-50
    };



public:
    int engineEloRating = 500;
    int NumberofMovesLookedAhead;
    __int64 TimeTakenForSearch;
    int NumberOfBranchesPruned;



    ChessEngine(int EngineColor) : PlayingAs(EngineColor), NumberofMovesLookedAhead(0) {

        engineEloRating = readEloFromFile();
        NumberofMovesLookedAhead = 0;
        TimeTakenForSearch = 0;
        terminateSearch = false;
        startSearch = false;
        NumberOfBranchesPruned = 0;
    }


    void MakeCompleteMove(pair<int, int> move, ChessBoard& chessboard) const {

        chessboard.MakeMove(move.first,move.second);
        // Check for pawn promotion
        if (chessboard.board[move.second].type == PAWN) {
            // White pawn promotion
            if (move.second >= 0 && move.second <= 7) {
                chessboard.promotePawn(move.second);
            }
            // Black pawn promotion
            else if (move.second >= 56 && move.second <= 63) {
                cout << "Test";
                chessboard.promotePawn(move.second);
            }
        }
        chessboard.currentPlayerIsWhite = !chessboard.currentPlayerIsWhite;


    }

    void shuffleMoves(vector<pair<int, int>>& possibleMoves) {
        unsigned seed = static_cast<unsigned>(system_clock::now().time_since_epoch().count());
        shuffle(possibleMoves.begin(), possibleMoves.end(), default_random_engine(seed));

    }

    void DisplayMoves(vector<string> moves) {
        cout << endl;
        for (string move : moves) {
            cout << move << " ";
        }
        cout << endl;
    }

    //string GenerateMove(const ChessBoard& board) {
    //    float bestScore = -INFINITY;
    //    string bestMove;
    //    auto start = high_resolution_clock::now();
    //    NumberofMovesLookedAhead = 0;
    //    // Adjusting depth based on the number of possible moves
    //    int depth = Depth;
    //    vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(PlayingAs);
    //    if (possibleMoves.size() > 20) {
    //        depth--; // Reduce depth if there are many possible moves
    //    }
    //    auto end_time = high_resolution_clock::now() + seconds(5); // Set time limit of 5 seconds
    //    // Iterative deepening loop
    //    for (int d = 1; d <= Depth; ++d) {
    //        for (const string& move : possibleMoves) {
    //            ChessBoard tempBoard = board;
    //            pair<int, int> Indices = convertChessNotationToIndices(move);
    //            MakeCompleteMove(Indices.first, Indices.second, tempBoard);
    //            // Perform minimax search with the current depth
    //            float score = Minimax(tempBoard, d, -INFINITY, INFINITY, false, PlayingAs);
    //            // Update the best move if needed
    //            if (score > bestScore) {
    //                bestScore = score;
    //                bestMove = move;
    //            }
    //            // Check if time limit is reached
    //            if (high_resolution_clock::now() >= end_time) {
    //                break;
    //            }
    //        }
    //        // Check if time limit is reached
    //        if (high_resolution_clock::now() >= end_time) {
    //            break;
    //        }
    //    }
    //    possibleMoves.clear();
    //    auto end = high_resolution_clock::now();
    //    auto duration = duration_cast<seconds>(end - start);
    //    TimeTakenForSearch = duration.count();
    //    return bestMove;
    //}

    //pair<int, int> GenerateMove(const ChessBoard& board) {
    //    int bestScore = INT_MIN;
    //    pair<int,int> bestMove;
    //    auto start = high_resolution_clock::now();
    //    NumberofMovesLookedAhead = 0;
    //    vector<pair<int,int>> possibleMoves = board.GetAllPossibleMovePairs(PlayingAs);
    //    shuffleMoves(possibleMoves);
    //    
    //    auto current_time = high_resolution_clock::now(); // Get the current time
    //    for (const pair<int, int>& move : possibleMoves) {
    //        if (terminateSearch.load()) {
    //            break; 
    //        }
    //        ChessBoard tempBoard = board;
    //        // Simulate the move
    //        MakeCompleteMove(move, tempBoard);
    //        // Calculate the score using Minimax with alpha-beta pruning
    //        int score = Search(tempBoard, Depth, INT_MIN, INT_MAX, false, PlayingAs);
    //        // Update the best move if needed
    //        if (score >= bestScore) {
    //            bestScore = score;
    //            bestMove = move;
    //        }
    //        auto end = high_resolution_clock::now();
    //        auto duration = duration_cast<seconds>(end - start);
    //        TimeTakenForSearch = duration.count(); // Update the time taken for search
    //    }
    //    possibleMoves.clear();
    //    return bestMove;
    //}

    //string GenerateMove(const ChessBoard& board) {
    //    float bestScore = -INFINITY;
    //    string bestMove;
    //    auto start = high_resolution_clock::now();
    //    NumberofMovesLookedAhead = 0;
    //    vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(PlayingAs);
    //    shuffleMoves(possibleMoves);
    //    //DisplayMoves(possibleMoves);
    //    for (const string& move : possibleMoves) {
    //        ChessBoard tempBoard = board;
    //        //tempBoard.isValidMove(move, fromTile, toTile);
    //        pair<int, int> Indices = convertChessNotationToIndices(move);
    //        // Simulate the move
    //        MakeCompleteMove(Indices.first, Indices.second, tempBoard);
    //        // Calculate the score using Minimax with alpha-beta pruning
    //        float score = Search(tempBoard, Depth, -INFINITY, INFINITY, false, PlayingAs);
    //        //cout << "\n\n\n" << score;
    //        // Update the best move if needed
    //        if (score >= bestScore) {
    //            bestScore = score;
    //            bestMove = move;
    //        }
    //        auto end = high_resolution_clock::now();
    //        auto duration = duration_cast<seconds>(end - start);
    //        TimeTakenForSearch = duration.count();
    //    }
    //    possibleMoves.clear();
    //    return bestMove;
    //}

    void StartSearch() {

        startSearch = true;
    }

    void StopSearching() {
        startSearch = false; // Set the stop flag to stop the search

    }

    void TerminateSearch() {
        terminateSearch = true;
    }

    //float Search(ChessBoard& board, int depth, int alpha, int beta, bool maximizingPlayer, int color) {
    //    NumberofMovesLookedAhead++;
    //    
    //    uint64_t hash = transpostionTable.computeHash(board);
    //    
    //    if (transpostionTable.isValuePresent(hash)) {
    //        auto stored = transpostionTable.lookupTranspositionTable(hash);
    //        if (stored.second >= depth) {
    //            cout << hash << endl;
    //            return stored.first; // Return the stored evaluation if it's at least as deep as the current search depth
    //        }
    //    }
    //    // Base case: reached maximum depth or terminal state (checkmate)
    //    if (depth == 0) {
    //        return Evaluate(board, color);
    //    }
    //    if (board.isCheckmate(board, color)) {
    //        return (maximizingPlayer ? INT_MIN : INT_MAX); // Checkmate score
    //    }
    //    // Maximizing player's turn
    //    if (maximizingPlayer) {
    //        int maxScore = INT_MIN;
    //        vector<pair<int, int>> possibleMoves = board.GetAllPossibleMovePairs(color);
    //        //shuffleMoves(possibleMoves);
    //        SortMoves(possibleMoves, board, color);
    //        for (const pair<int, int>& move : possibleMoves) {
    //            ChessBoard tempBoard = board;
    //            //pair<int, int> Indices = convertChessNotationToIndices(move);
    //            MakeCompleteMove(move, tempBoard);
    //            int score = Search(tempBoard, depth - 1, alpha, beta, false, color);
    //            maxScore = max(maxScore, score);
    //            alpha = max(alpha, score);
    //            // Store the evaluation in the transposition table
    //            transpostionTable.storeTranspositionTable(hash, maxScore, depth);
    //            // Alpha-beta pruning
    //            if (beta <= alpha) {
    //                NumberOfBranchesPruned++;
    //                break;
    //            }
    //        }
    //        possibleMoves.clear();
    //        return maxScore;
    //    }
    //    // Minimizing player's turn
    //    else {
    //        int minScore = INT_MAX;
    //        vector<pair<int, int>> possibleMoves = board.GetAllPossibleMovePairs((PlayingAs == Black) ? White : Black);
    //        //shuffleMoves(possibleMoves);
    //        SortMoves(possibleMoves, board, (PlayingAs == Black) ? White : Black);
    //        for (const pair<int,int>& move : possibleMoves) {
    //            ChessBoard tempBoard = board;
    //            //pair<int, int> Indices = convertChessNotationToIndices(move);
    //            MakeCompleteMove(move, tempBoard);
    //            int score = Search(tempBoard, depth - 1, alpha, beta, true, color);
    //            minScore = min(minScore, score);
    //            beta = min(beta, score);
    //            // Store the evaluation in the transposition table
    //            transpostionTable.storeTranspositionTable(hash, minScore, depth);
    //            // Alpha-beta pruning
    //            if (beta <= alpha) {
    //                NumberOfBranchesPruned++;
    //                break;
    //            }
    //        }
    //        possibleMoves.clear();
    //        return minScore;
    //    }
    //}

    //int Search(int depth, int alpha, int beta, ChessBoard& board) {
    //    NumberofMovesLookedAhead++;
    //    if (depth == 0) {
    //        return Evaluate(board);
    //    }
    //    if (board.isCheckmate(board, board.isCurrentPlayerWhite() ? White : Black)) {
    //        return INT_MIN;
    //          
    //    }
    //    vector<pair<int, int>> moves = board.GetAllPossibleMovePairs();
    //    for (const auto& move : moves) {
    //        ChessBoard temp = board;
    //        MakeCompleteMove(move, temp);
    //        int evaluation = -Search(depth - 1, -beta, -alpha, temp);
    //        //cout << evaluation << endl;
    //        if (evaluation >= beta) {
    //            NumberOfBranchesPruned++;
    //            return beta;
    //        }
    //        alpha = max(alpha, evaluation);
    //    }
    //    return alpha;
    //}

    // int Evaluate(ChessBoard& board) {
    //     int WhiteEval = board.calculatePlayerScore(White) * 100;
    //     int BlackEval = board.calculatePlayerScore(Black) * 100;
    //     int evaluation = WhiteEval - BlackEval;
    //     int perspective = (board.isCurrentPlayerWhite()) ? 1 : -1;
    //     return evaluation * perspective;
    // }

    //float Minimax(ChessBoard& board, int depth, float alpha, float beta, bool maximizingPlayer, int color) {
    //    NumberofMovesLookedAhead++;
    //    // Base case: reached maximum depth or terminal state (checkmate)
    //    if (depth == 0) {
    //        return Evaluate(board, color);
    //    }
    //    if (board.isCheckmate(board, color)) {
    //        return (maximizingPlayer ? -INFINITY : INFINITY); // Checkmate score
    //    }
    //    // Maximizing player's turn
    //    if (maximizingPlayer) {
    //        float maxScore = -INFINITY;
    //        vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(color);
    //        shuffleMoves(possibleMoves);
    //        //DisplayMoves(possibleMoves);
    //        for (const string& move : possibleMoves) {
    //            ChessBoard tempBoard = board; // Make a copy of the board to simulate moves
    //            pair<int, int> Indices = convertChessNotationToIndices(move);
    //            //tempBoard.MakeMove(Indices.first, Indices.second); // Make the move directly without isValidMove()
    //            MakeCompleteMove(Indices.first, Indices.second, tempBoard);
    //            // Recursive call for the opponent with a reduced depth
    //            float score = Minimax(tempBoard, depth - 1, alpha, beta, false, color);
    //            // Update alpha and maxScore
    //            maxScore = max(maxScore, score);
    //            alpha = max(alpha, score);
    //            // Alpha-beta pruning
    //            if (beta <= alpha) {
    //                break;
    //            }
    //        }
    //        possibleMoves.clear();
    //        return maxScore;
    //    }
    //    // Minimizing player's turn
    //    else {
    //        float minScore = INFINITY;
    //        vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation((PlayingAs == Black) ? White : Black);
    //        shuffleMoves(possibleMoves);
    //       // DisplayMoves(possibleMoves);
    //        for (const string& move : possibleMoves) {
    //            ChessBoard tempBoard = board; // Make a copy of the board to simulate moves
    //            pair<int, int> Indices = convertChessNotationToIndices(move);
    //            MakeCompleteMove(Indices.first, Indices.second, tempBoard);
    //            //tempBoard.MakeMove(Indices.first, Indices.second); // Make the move directly without isValidMove()
    //            // Recursive call for the opponent with a reduced depth
    //            float score = Minimax(tempBoard, depth - 1, alpha, beta, true, color);
    //            // Update beta and minScore
    //            minScore = min(minScore, score);
    //            beta = min(beta, score);
    //            // Alpha-beta pruning
    //            if (beta <= alpha) {
    //                break;
    //            }
    //        }
    //        possibleMoves.clear();
    //        return minScore;
    //    }
    //}

    void SortMoves(vector<pair<int, int>>& moves, const ChessBoard& board, char color) const {
        // Define custom comparator to sort moves
        auto customComparator = [&](const pair<int, int>& move1, const pair<int, int>& move2)
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


    void GenerateCapturesAndQuietMoves(const ChessBoard& board, vector<pair<int, int>>& captures, vector<pair<int, int>>& quietMoves, int color) const {
        vector<pair<int, int>> allMoves = board.GetAllPossibleMovePairs(color);
        for (const auto& move : allMoves) {
            if (IsCaptureMove(move, board)) {
                captures.push_back(move);
            }
            else {
                quietMoves.push_back(move);
            }
        }
    }

    int Evaluate(const ChessBoard& chessboard, int currentPlayerColor) {
       unordered_map<int, int> pieceValues = {
           {PAWN, 100}, {KNIGHT, 320}, {BISHOP, 330}, {ROOK, 500}, {QUEEN, 900}, {KING, 1000}
       };
       // Evaluate material advantage
       
       int selfMaterial = chessboard.calculatePlayerScore(currentPlayerColor) * 100;
       int opponentMaterial = chessboard.calculatePlayerScore((currentPlayerColor == White) ? White : Black) * 100;
       int materialAdvantage = (currentPlayerColor == White) ? (selfMaterial - opponentMaterial) : (opponentMaterial - selfMaterial);
       // Evaluate positional advantage using PSTs
       int positionalAdvantage = 0;
       bool isEndgame = (selfMaterial < 120 && opponentMaterial < 120); // Check if it's an endgame
       for (int i = 0; i < TotalTiles; ++i) {
           ChessPiece piece = chessboard.board[i];
           if (piece.type != EMPTY) {
               int pieceValue = getPSTValue(piece, i, currentPlayerColor);
               positionalAdvantage += pieceValue;
           }
           // Adjust king's positional advantage during endgames
           if (isEndgame) {
               adjustEndgamePositionalAdvantage(chessboard, currentPlayerColor, positionalAdvantage);
           }
           // Evaluate pawn promotion during endgames
           if (piece.type == PAWN && isEndgame) {
               // Check if the pawn can be promoted
               if ((piece.color == White && i >= 48) || (piece.color == Black && i <= 15)) {
                   // Evaluate the potential gain from pawn promotion
                   positionalAdvantage += (currentPlayerColor == PlayingAs ? 900 : -900); // Adjust as needed
               }
           }
       }
       // Evaluate moves and captures
       float moveAdvantage = 0, captureAdvantage = 0;
       vector<pair<int, int>> possibleMoves = chessboard.GetAllPossibleMovePairs(currentPlayerColor);
       for (const auto& move : possibleMoves) {
          if (IsCaptureMove(move, chessboard)) {
              captureAdvantage += (currentPlayerColor == PlayingAs ? 50 : -50); // Adjust as needed
          }
          else {
              moveAdvantage += (currentPlayerColor == PlayingAs ? 10 : -10); // Adjust as needed
          }
       }
       // Evaluate moves and captures
       float OpponentMoveAdvantage = 0, OpponentCaptureAdvantage = 0;
       possibleMoves = chessboard.GetAllPossibleMovePairs(currentPlayerColor == 'W' ? 'B':'W');
       for (const auto& move : possibleMoves) {
          if (IsCaptureMove(move, chessboard)) {
              OpponentMoveAdvantage += (currentPlayerColor == PlayingAs ? -50 : +50); // Adjust as needed
          }
          else {
              OpponentCaptureAdvantage += (currentPlayerColor == PlayingAs ? -10 : +10); // Adjust as needed
          }
       }
       // Total evaluation is a combination of material, positional, move, and capture advantages
       return materialAdvantage + positionalAdvantage + moveAdvantage + captureAdvantage + OpponentMoveAdvantage + OpponentCaptureAdvantage;
    }


    void adjustEndgamePositionalAdvantage(const ChessBoard& chessboard, int currentPlayerColor, int& positionalAdvantage) const {


        int kingSquareIndex = chessboard.GetKingIndex(currentPlayerColor);
        int opponentKingSquareIndex = chessboard.GetKingIndex((currentPlayerColor == White) ? Black : White);


        int kingRow = kingSquareIndex / 8;
        int kingCol = kingSquareIndex % 8;
        int opponentKingRow = opponentKingSquareIndex / 8;
        int opponentKingCol = opponentKingSquareIndex % 8;

        // Encourage forcing opponent's king to the corner
        int cornerDist = min(min(opponentKingRow, 7 - opponentKingRow), min(opponentKingCol, 7 - opponentKingCol));
        positionalAdvantage += (currentPlayerColor == PlayingAs ? 10 : -10) * cornerDist;

        // Encourage bringing own king closer to the opponent's king
        int kingDist = abs(kingRow - opponentKingRow) + abs(kingCol - opponentKingCol);
        positionalAdvantage += (currentPlayerColor == PlayingAs ? -5 : 5) * kingDist;
    }

    //int getPSTValue(ChessPiece piece, int squareIndex, int currentPlayerColor) {
    //    const int* piecePST = nullptr;
    //    int pieceType = piece.type;
    //    if (pieceType == PAWN) {
    //        piecePST = (currentPlayerColor == Black) ? InvertTable(PawnPST) : PawnPST;
    //    }
    //    else if (pieceType == KNIGHT) {
    //        piecePST = (currentPlayerColor == Black) ? InvertTable(KnightsPST) : KnightsPST;
    //    }
    //    else if (pieceType == BISHOP) {
    //        piecePST = (currentPlayerColor == Black) ? InvertTable(BishopsPST) : BishopsPST;
    //    }
    //    else if (pieceType == ROOK) {
    //        piecePST = (currentPlayerColor == Black) ? InvertTable(RooksPST) : RooksPST;
    //    }
    //    else if (pieceType == QUEEN) {
    //        piecePST = (currentPlayerColor == Black) ? InvertTable(QueensPST) : QueensPST;
    //    }
    //    else if (pieceType == KING) {
    //        piecePST = (currentPlayerColor == Black) ? InvertTable(KingPST) : KingPST;
    //    }
    //    if (piecePST != nullptr) {
    //        return piecePST[squareIndex];
    //    }
    //    else {
    //        return 0;
    //    }
    //}

    pair<int, int> convertChessNotationToIndices(const string& move) const {
        int fromCol = move[0] - 'a';
        int fromRow = 7 - (move[1] - '1');
        int toCol = move[2] - 'a';
        int toRow = 7 - (move[3] - '1');
        int fromTile = fromRow * 8 + fromCol;
        int toTile = toRow * 8 + toCol;
        return make_pair(fromTile, toTile);
    }

    void PlayMove(const pair<int, int>& move, ChessBoard& board) {

        //pair<int, int> Indices = convertChessNotationToIndices(move);
        //string Move = board.ConvertToChessNotation(move.first, move.second);
        //string player = (color == 'W') ? "White" : "Black";
        //cout << player << " plays: " << move << endl;
        board.MakeCompleteMove(move);
        //if (board.PawnPromotion.first) {
        //    board.PromotePawn(QUEEN);
        //}
        startSearch = false;
    }

    int* InvertTable(const int* originalArray) {

        int* invertedArray = new int[TotalTiles]; // Create an array of size 64

        for (int i = 0; i < TotalTiles; i++) {
            invertedArray[i] = originalArray[63 - i];
        }

        return invertedArray;
    }

    int readEloFromFile() {
        ifstream inputFile;
        string filename = string(1, PlayingAs) + "ELO.txt";
        inputFile.open(filename);

        int elo = engineEloRating; // Default value in case the file cannot be opened

        if (inputFile.is_open()) {
            inputFile >> elo;
            inputFile.close();
            cout << "Elo rating read from " << filename << ": " << elo << endl;
        }
        else {
            cerr << "Unable to open file " << filename << " for reading. Using default Elo rating." << endl;
        }

        return elo;
    }

    void DisplayTable(const int* table) const {
        cout << "\n\n\n";
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                cout << table[i * 8 + j] << " ";
            }
            cout << endl;
        }
        cout << "\n\n\n";
    }

    void saveEloToFile() const {
        ofstream outputFile;
        string filename = string(1, PlayingAs) + "ELO.txt";
        outputFile.open(filename);

        int defaultElo = 500;

        if (outputFile.is_open()) {

            outputFile << engineEloRating << endl;
            outputFile.close();
            cout << "Elo rating saved to " << filename << endl;
        }
        else {
            cerr << "Unable to open file " << filename << " for writing. Creating a new file with default Elo rating." << endl;
            outputFile.open(filename);
            if (outputFile.is_open()) {
                outputFile << defaultElo << endl;
                outputFile.close();
                cout << "New file created with default Elo rating: " << defaultElo << endl;
            }
            else {
                cerr << "Unable to create file " << filename << " for writing." << endl;
            }
        }
    }

    void Reset() {
        TimeTakenForSearch = 0;
        NumberofMovesLookedAhead = 0;
        terminateSearch = false;
        startSearch = false;
        NumberOfBranchesPruned = 0;
    }

    bool CheckAfterMove(const pair<int, int>& move, const ChessBoard& board, char color) const {
        ChessBoard tempBoard = board;

        MakeCompleteMove(move, tempBoard);


        return tempBoard.isCheck(tempBoard, (color == White ? Black : White), "Engine: Check after move");
    }

    bool IsCaptureMove(const pair<int, int>& move, const ChessBoard board) const {
        pair<int, int> indices;
        ChessPiece targetPiece = board.GetPieceAtPosition(indices.second);

        return targetPiece.color != board.GetPieceAtPosition(indices.first).color && targetPiece.color != EMPTY;
    }

    double getSizeOfTranspositionTable() const {
        return transpostionTable.SizeOfTranspostionTable;
    }

    bool isSearchStarted() { return startSearch; }

    void PauseSearch() {
        terminateSearch = !terminateSearch;
    }

    pair<int, int> GenerateMove(const ChessBoard& board)
    {
        int bestScore = numeric_limits<int>::min();
        pair<int, int> bestMove;
        auto start = high_resolution_clock::now();
        NumberofMovesLookedAhead = 0;

        vector<pair<int, int>> possibleMoves = board.GetAllPossibleMovePairs(PlayingAs);

        shuffleMoves(possibleMoves);

        for (const pair<int, int>& move : possibleMoves)
        {
            ChessBoard tempBoard = board; // Make a copy of the board to simulate moves


            // Simulate the move
            tempBoard.MakeMove(move.first,move.second);

            // Calculate the score using Minimax with alpha-beta pruning
            int score = Minimax(tempBoard, Depth, numeric_limits<int>::min(), numeric_limits<int>::max(), false, PlayingAs);

            // Update the best move if needed
            if (score >= bestScore)
            {
                bestScore = score;
                bestMove = move;
            }

            auto end = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(end - start);
            TimeTakenForSearch = duration.count();
        }

        possibleMoves.clear();

        return bestMove;
    }

    int Minimax(ChessBoard& board, int depth, int alpha, int beta, bool maximizingPlayer, int color)
    {
        NumberofMovesLookedAhead++;

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
        if (maximizingPlayer) {
            int maxScore = numeric_limits<int>::min();
            vector<pair<int, int>> possibleMoves = board.GetAllPossibleMovePairs(color);
            shuffleMoves(possibleMoves);

            for (const pair<int, int>& move : possibleMoves)
            {
                ChessBoard tempBoard = board; // Make a copy of the board to simulate moves
                //pair<int, int> Indices = convertChessNotationToIndices(move);
                tempBoard.MakeMove(move.first,move.second); // Make the move directly without isValidMove()

                // Recursive call for the opponent with a reduced depth
                int score = Minimax(tempBoard, depth - 1, alpha, beta, false, color);

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
        else {
            int minScore = numeric_limits<int>::max();
            vector<pair<int, int>> possibleMoves = board.GetAllPossibleMovePairs((color == White ? Black : White));
            shuffleMoves(possibleMoves);

            for (const pair<int, int>& move : possibleMoves)
            {
                ChessBoard tempBoard = board; // Make a copy of the board to simulate moves
                //pair<int, int> Indices = convertChessNotationToIndices(move);
                tempBoard.MakeMove(move.first,move.second); // Make the move directly without isValidMove()

                // Recursive call for the opponent with a reduced depth
                int score = Minimax(tempBoard, depth - 1, alpha, beta, true, color);

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

    // int Evaluate(const ChessBoard& chessboard, char currentPlayerColor) {
    //     unordered_map<char, int> pieceValues;
    //     // Piece values for evaluation
    //     if (PlayingAs == White)
    //     {
    //         pieceValues = {
    //             {PAWN | White, 100}, {BISHOP | White, 320}, {KNIGHT | White, 330}, {ROOK | White, 500}, {QUEEN | White, 900}, {KING | White, 1000},
    //             {PAWN | Black, -100}, {BISHOP | Black, -320}, {KNIGHT | Black, -330}, {ROOK | Black, -500}, {QUEEN | Black, -900}, {KING | Black, -1000 } };
    //     }
    //     else
    //     {
    //         pieceValues = {
    //             {PAWN | White, -100}, {BISHOP | White, -320}, {KNIGHT | White, -330}, {ROOK | White, -500}, {QUEEN | White, -900}, {KING | White, -1000 },
    //             {PAWN | Black, 100}, {BISHOP | Black, 320}, {KNIGHT | Black, 330}, {ROOK | Black, 500}, {QUEEN | Black, 900}, {KING | Black, 1000 } };
    //     }

    //     // Evaluate material advantage
    //     int SelfMaterial = 0, OpponentMeterial = 0;
    //     for (int i = 0; i < TotalTiles; ++i)
    //     {
    //         ChessPiece piece = chessboard.board[i];
    //         if (piece.type != EMPTY)
    //         {
    //             if (piece.color == PlayingAs)
    //             {
    //                 SelfMaterial += pieceValues[piece.type | piece.color];
    //             }
    //             else
    //             {
    //                 OpponentMeterial += pieceValues[piece.type | piece.color];
    //             }
    //         }
    //     }
    //     int materialAdvantage = (currentPlayerColor == White) ? (SelfMaterial - OpponentMeterial) : (OpponentMeterial - SelfMaterial);

    //     // Evaluate positional advantage using PSTs
    //     int positionalAdvantage = 0;
    //     for (int i = 0; i < TotalTiles; ++i)
    //     {
    //         ChessPiece piece = chessboard.board[i];
    //         if (piece.type != EMPTY)
    //         {
    //             int pieceValue = getPSTValue(piece, i, currentPlayerColor);
    //             positionalAdvantage += pieceValue;
    //         }
    //     }

    //     // Total evaluation is a combination of material, positional, and safety advantages
    //     return materialAdvantage + positionalAdvantage;
    // }

    int getPSTValue(ChessPiece piece, int squareIndex, char currentPlayerColor) {
        const int* piecePST = nullptr;

        switch (piece.type)
        {
        case PAWN:
            piecePST = (currentPlayerColor == Black) ? InvertTable(PawnPST) : PawnPST;
            break;
        case KNIGHT:
            piecePST = (currentPlayerColor == Black) ? InvertTable(KnightsPST) : KnightsPST;
            break;
        case BISHOP:
            piecePST = (currentPlayerColor == Black) ? InvertTable(BishopsPST) : BishopsPST;
            break;
        case ROOK:
            piecePST = (currentPlayerColor == Black) ? InvertTable(RooksPST) : RooksPST;
            break;
        case QUEEN:
            piecePST = (currentPlayerColor == Black) ? InvertTable(QueensPST) : QueensPST;
            break;
        case KING:
            piecePST = (currentPlayerColor == Black) ? InvertTable(KingPST) : KingPST;
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
};

class User : public ReadWrite {

public:
    string Username;
    int ELO = 500;

    User(string name) : Username(name) {
        ELO = readEloFromFile();
    }

    int readEloFromFile() {
        cout << "TEst" << endl;
        ifstream inputFile;
        string filename = Username + "ELO.txt";
        inputFile.open(filename);
        int elo = ELO; // Default value in case the file cannot be opened
        if (inputFile.is_open()) {
            inputFile >> elo;
            inputFile.close();
            cout << "Elo rating read from " << filename << ": " << elo << endl;
        }
        else {
            cerr << "Unable to open file " << filename << " for reading. Using default Elo rating." << endl;
        }

        return elo;
    }

    void saveEloToFile() const {
        ofstream outputFile;
        string filename = Username + "ELO.txt";
        outputFile.open(filename);
        int defaultElo = 500;
        if (outputFile.is_open()) {

            outputFile << ELO << endl;
            outputFile.close();
            cout << "Elo rating saved to " << filename << endl;
        }
        else {
            cerr << "Unable to open file " << filename << " for writing. Creating a new file with default Elo rating." << endl;
            outputFile.open(filename);
            if (outputFile.is_open()) {
                outputFile << defaultElo << endl;
                outputFile.close();
                cout << "New file created with default Elo rating: " << defaultElo << endl;
            }
            else {
                cerr << "Unable to create file " << filename << " for writing." << endl;
            }
        }
    }


};

class BoardStats {
private:
    const float TextSize = 50;

    const float textX = (InfoBoxWidth / 2) + InfoBoxX;
    const float textY = 100;
    const int BoardDimensions = tileSize * boardSize;
    const Color messageColor = WHITE;
    const Color AlertColor = RED;
    const char* BlackInCheck = "Black's King is in Check";
    const char* WhiteInCheck = "White's King is in Check";
    const char* ELO = "ELO: ";
    const char* Black_ = "Black";
    const char* White_ = "White";



public:
    bool ShowMoveHistory;
    bool CalculateELO;
    int winner;

    BoardStats() {
        ShowMoveHistory = false;
        winner = ' ';
        CalculateELO = true;

    }

    void DisplayMessage(string message) {
        float TextPositionX = static_cast<float>(800 / 2); // Centers text on X-axis of Remaining Area
        float TextPositionY = static_cast<float>(screenHeight) / 2; // Centers text on Y-axis
        TextPositionX = TextPositionX - static_cast<float>(MeasureText(message.c_str(), static_cast<int>(TextSize))) / 2;
        TextPositionY = TextPositionY - TextSize / 2;
        DrawTextWithCustomFont(message.c_str(), TextPositionX, TextPositionY, TextSize, RED); //Balance Based on TextSize
    }

    bool GameIsEnded(ChessBoard& board) {
        int playerColor = board.isCurrentPlayerWhite() ? White : Black;

        string EndMessage;

        if (board.isCheck(board, playerColor, "stats: Game is Ended: Winner")) {
            if (board.isCheckmate(board, playerColor)) {
                string EndMessage = "Winner is ";
                EndMessage += (board.isCurrentPlayerWhite() ? Black_ : White_);
                winner = board.isCurrentPlayerWhite() ? Black : White;
                DisplayMessage(EndMessage);
                //PlaySound(GameEnds);
                return true;
            }
        }
        if (!board.isCheck(board, playerColor, "stats: Game is Ended: stalemate") && board.GetAllPossibleMovePairs(playerColor).size() == 0) {
            string EndMessage = "---Stalemate---";
            DisplayMessage(EndMessage);
            return true;


        }
        if (board.calculatePlayerScore(White) <= 100 && board.calculatePlayerScore(Black) <= 100) {
            string EndMessage = "---Draw---";
            DisplayMessage(EndMessage);
            return true;
        }

        return false;

    }

    string CloseInBrackets(string stringClose) {
        string temp = "(";
        temp += stringClose;
        temp += ")";
        return temp;

    }

    void DisplayStats(ChessBoard& chessboard, ChessEngine& engine, User& player) {


        string LastMovePlayed = "";
        string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";



        if (chessboard.moveHistory.size() != 0) {
            LastMovePlayed = (chessboard.moveHistory.back());
        }


        if (chessboard.isCheck(chessboard, Black, "Stats: DisplayStats: black")) {
            DrawTextWithCustomFont(Black_, textX - (TextCenter(Black_, fontSize).x/2), textY - (TextCenter(Black_, fontSize).y / 2) + 20, fontSize, AlertColor);
        }
        else {
            DrawTextWithCustomFont(Black_, textX - (TextCenter(Black_, fontSize).x / 2), textY - (TextCenter(Black_, fontSize).y / 2) + 20, fontSize, messageColor);
        }




        MovesAndHistory(LastMovePlayed, chessboard);

        if (chessboard.isCheck(chessboard, White, "Stats: DisplayStats: white")) {
            DrawTextWithCustomFont(White_, textX - (TextCenter(White_, fontSize).x / 2), screenHeight - 146 - fontSize + 30, fontSize, AlertColor);
        }
        else {
            DrawTextWithCustomFont(White_, textX - (TextCenter(White_, fontSize).x / 2), screenHeight - 146 - fontSize + 30, fontSize, messageColor);
        }

        // (isVersusAI) {
        int LookAheads = engine.NumberofMovesLookedAhead;
        long long TimeTaken = engine.TimeTakenForSearch;
        int BranchesPruned = engine.NumberOfBranchesPruned;
        double SizeOfTable = engine.getSizeOfTranspositionTable();
        string WhiteELO = CloseInBrackets(ELO + to_string(player.ELO));
        string BLackELO = CloseInBrackets(ELO + to_string(engine.engineEloRating));
        string BlackMessage = "Saw " + to_string(LookAheads) + " futures in " + to_string(TimeTaken) + "s";
        string PruningMessage = "Pruned " + to_string(BranchesPruned) + " Branches";
        string TableSizeMessage = "Size: " + to_string(SizeOfTable);
        DrawTextWithCustomFont(BlackMessage.c_str(), textX - (TextCenter(BlackMessage.c_str(), fontSize - 30).x / 2), textY + 70, fontSize - 30, messageColor);
        DrawTextWithCustomFont(TableSizeMessage.c_str(), textX - (TextCenter(TableSizeMessage.c_str(), fontSize - 30).x / 2), textY + 90, fontSize - 30, messageColor);
        DrawTextWithCustomFont(PruningMessage.c_str(), textX - (TextCenter(PruningMessage.c_str(), fontSize - 30).x / 2), textY + 110, fontSize - 30, messageColor);
        
        DrawTextWithCustomFont(WhiteELO.c_str(), textX - (TextCenter(WhiteELO.c_str(), fontSize - 30).x / 2), screenHeight - 146 + 30, fontSize - 30, messageColor);
        DrawTextWithCustomFont(BLackELO.c_str(), textX - (TextCenter(BLackELO.c_str(), fontSize - 30).x / 2), textY + 50, fontSize - 30, messageColor);
        //}



        if (!chessboard.preMoves.isEmpty()) {
            string NextMoveToPlay = "Next in Queue: " + chessboard.preMoves.peek();
            DrawTextWithCustomFont(NextMoveToPlay.c_str(), textX - 20, screenHeight - 166 - fontSize + 30, fontSize - 30, messageColor);
        }




    }

    void MovesAndHistory(string LastMovePlayed, ChessBoard& chessboard) {
        if (ShowMoveHistory) {
            DisplayMoveHistory(chessboard);

        }
        else {
            DrawTextWithCustomFont(LastMovePlayed.c_str(), textX + LastMovePlayed.length() + 10, screenHeight / 2 - 42, fontSize - 10, AlertColor);
        }

    }

    //void DisplayStats(ChessBoard chessboard, ChessEngine Black, ChessEngine White) {
    //    int LookAheadsBlack = Black.NumberofMovesLookedAhead;
    //    auto TimeTakenBlack = Black.TimeTakenForSearch;
    //    int LookAheadsWhite = White.NumberofMovesLookedAhead;
    //    auto TimeTakenWhite = White.TimeTakenForSearch;
    //    int whiteScore = chessboard.calculatePlayerScore('W');
    //    int blackScore = chessboard.calculatePlayerScore('B');
    //    string LastMovePlayed = "";
    //    string BlackMessage = "Saw " + to_string(LookAheadsBlack) + " futures in " + to_string(TimeTakenBlack) + " milliseconds";
    //    string WhiteMessage = "Saw " + to_string(LookAheadsWhite) + " futures in " + to_string(TimeTakenWhite) + " milliseconds";
    //    string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";
    //    if (chessboard.moveHistory.size() != 0) {
    //        LastMovePlayed = chessboard.moveHistory.back();
    //    }
    //    //Display if Black Is in check
    //    //Display Black Score
    //    //Display Engine message
    //    //Display Last Move
    //    //Display Current Player
    //    //Display white Score
    //    //Display if White Is in Check
    //    //Black
    //    if (chessboard.isCheck(chessboard, 'B', "Stats: DisplayStats: black: 2")) {
    //        DrawTextWithCustomFont("Black King is in Check", textX, (tileSize * 1) - fontSize, fontSize, RED);
    //    }
    //    else {
    //        DrawTextWithCustomFont("Black King is not in Check", textX, (tileSize * 1) - fontSize, fontSize, BLACK);
    //    }
    //    DrawTextWithCustomFont(("Black ELO: " + to_string(Black.engineEloRating)).c_str(), textX, (tileSize * 2 - fontSize) - fontSize, fontSize, BLACK);
    //    DrawTextWithCustomFont(("Black Player Score: " + to_string(blackScore)).c_str(), textX, (tileSize * 3 - fontSize) - fontSize, fontSize, BLACK);
    //    DrawTextWithCustomFont(BlackMessage.c_str(), textX, (tileSize * 2 + fontSize) - (fontSize - 0), fontSize - 4, BLACK);
    //    //Current Player
    //    DrawTextWithCustomFont(("Current Player: " + currentPlayer).c_str(), textX, static_cast<float>(screenHeight) / 2, fontSize, (currentPlayer == "White") ? WHITE : BLACK);
    //    DrawTextWithCustomFont(LastMovePlayed.c_str(), textX, (tileSize * 5 + fontSize) - fontSize, fontSize, RED);
    //    //White
    //    DrawTextWithCustomFont(WhiteMessage.c_str(), textX, (tileSize * 7 + fontSize) - (fontSize - 0), fontSize - 4, WHITE);
    //    DrawTextWithCustomFont(("White Player Score: " + to_string(whiteScore)).c_str(), textX, (tileSize * 5 + fontSize) + fontSize, fontSize, WHITE);
    //    DrawTextWithCustomFont(("White ELO: " + to_string(White.engineEloRating)).c_str(), textX, (tileSize * 6 + fontSize) + fontSize, fontSize, WHITE);
    //    if (chessboard.isCheck(chessboard, 'W', "Stats: DisplayStats: black: 1")) {
    //        DrawTextWithCustomFont("White King is in Check", textX, (tileSize * 7) + fontSize, fontSize, RED);
    //    }
    //    else {
    //        DrawTextWithCustomFont("White King is not in Check", textX, (tileSize * 7) + fontSize, fontSize, WHITE);
    //    }
    //}

    void DisplayMoveHistory(ChessBoard chessboard) {

        string Moves = "";
        for (int index = 0; index < chessboard.moveHistory.size(); index++) {
            Moves += chessboard.moveHistory[index];
            Moves += ", ";
            if ((index + 1) % 6 == 0) {
                Moves += "\n\n";
            }
        }
        DrawTextWithCustomFont(Moves.c_str(), 150, textY + 90, fontSize - 20, RED);



    }

    double calculateExpectedScore(double engineRating, double opponentRating) {
        return 1.0 / (1 + pow(10, (opponentRating - engineRating) / 400));
    }

    int updateEloRating(int engineRating, int opponentRating, bool engineWon) {
        double expectedScore = calculateExpectedScore(engineRating, opponentRating);
        double actualScore = engineWon ? 1.0 : 0.0;
        double kFactor = 42; // Adjust the K-factor as needed

        // Update the Elo rating based on the outcome
        return static_cast<int>(engineRating + kFactor * (actualScore - expectedScore));
    }

    void Reset() {
        ShowMoveHistory = false;
        winner = ' ';
        CalculateELO = true;
    }

    void DrawTextWithCustomFont(const char* text, float posX, float posY, float fontSize, Color color) {
        Vector2 Position = { posX, posY };
        DrawTextEx(myFont, text, Position, fontSize, 1.0, color);
    }

    Vector2 TextCenter(const char* text, float fontSize) {
        return MeasureTextEx(myFont, text, fontSize, 1.0);
    }


    void DrawEvaluationColumn(ChessBoard& chessboard, ChessEngine& engine) {
        // Calculate evaluation scores for both players
        float whiteScore = Evaluate(chessboard, White, engine);
        float blackScore = Evaluate(chessboard, White, engine);

        // Calculate proportions of white and black color for the column
        float totalScore = whiteScore + blackScore;
        float whiteProportion = (totalScore != 0) ? (whiteScore / totalScore) : 0;
        float blackProportion = (totalScore != 0) ? (blackScore / totalScore) : 0;
        ostringstream White;
        ostringstream Black;
        White << fixed << setprecision(1) << whiteProportion;
        Black << fixed << setprecision(1) << blackProportion;
        string WhiteProp = White.str(); //White Proportions
        string BlackProp = Black.str(); //Black Proportions





        Rectangle columnRect = { 763,80,25,640 }; // { x, y, width, height };


        // Draw the evaluation column with appropriate colors
        DrawRectangleRec(columnRect, WHITE); // Fill with white color
        Rectangle blackPart = { columnRect.x, columnRect.y, columnRect.width, columnRect.height * blackProportion };
        Rectangle whitePart = { columnRect.x, columnRect.y + blackPart.height, columnRect.width, columnRect.height * whiteProportion };
        DrawRectangleRec(whitePart, WHITE); // Fill with white color
        DrawRectangleRec(blackPart, BLACK); // Fill with black color
        DrawTextWithCustomFont(WhiteProp.c_str(), 765, 703, 15, BLACK);
        DrawTextWithCustomFont(BlackProp.c_str(), 765, 85, 15, WHITE);


    }


    float Evaluate(const ChessBoard& chessboard, int currentPlayerColor, ChessEngine& engine) const {
        // Piece values
        unordered_map<int, float> pieceValues = {
            {PAWN, 100}, {KNIGHT, 320}, {BISHOP, 330}, {ROOK, 500}, {QUEEN, 900}, {KING, 1000}
        };

        // Evaluate material advantage
        float selfMaterial = 0, opponentMaterial = 0;
        for (int i = 0; i < TotalTiles; ++i) {
            ChessPiece piece = chessboard.board[i];
            if (piece.type != EMPTY) {
                if (piece.color == currentPlayerColor) {
                    selfMaterial += pieceValues[piece.type];
                }
                else {
                    opponentMaterial += pieceValues[piece.type];
                }
            }
        }
        float materialAdvantage = selfMaterial - opponentMaterial;

        // Evaluate positional advantage using PSTs
        float positionalAdvantage = 0;
        for (int i = 0; i < TotalTiles; ++i) {
            ChessPiece piece = chessboard.board[i];
            if (piece.type != EMPTY) {
                int pieceValue = engine.getPSTValue(piece, i, currentPlayerColor);
                positionalAdvantage += pieceValue;
            }
        }

        // Evaluate move options and capture moves
        vector<pair<int, int>> moves = chessboard.GetAllPossibleMovePairs(currentPlayerColor);
        int moveOptions = static_cast<int>(moves.size());
        int captureMoves = 0;
        for (const auto& move : moves) {
            if (engine.IsCaptureMove(move, chessboard)) {
                captureMoves++;
            }
        }

        // Total evaluation is a combination of material, positional, move options, and capture moves
        float evaluation = abs(materialAdvantage + positionalAdvantage);
        evaluation += moveOptions * 10; // Adjust weight as needed
        evaluation += captureMoves * 20; // Adjust weight as needed

        return evaluation;
    }


};

class GameModes {
    ChessBoard chessBoard;
    BoardStats statistics;
    ChessEngine Black;
    ChessEngine White;
    User Player;

public:
    bool GameStartedWithAI;
    bool MultiplayerGameStarted;


    void SinglePlayerMode() {


    }
};

void CalculateAIMove(ChessEngine& engine, ChessBoard& board) {

    while (true) {
        if (engine.isSearchStarted() && !board.isCurrentPlayerWhite()) {
            pair<int,int> move;
            board.ComputeOpponentMoves();
            cout << "Running...\n";
            move = engine.GenerateMove(board);
            if (move == make_pair(0,0)) {
                engine.PlayMove(move, board);

            }
            engine.StopSearching();

        }

    }

}


bool isGameEnded;

int main() {
    InitWindow(screenWidth, screenHeight, "Chess");
    InitAudioDevice();
    ChessBoard chessboard;
    BoardStats GameStats;
    ChessEngine Gemini(Black);
    User Taha("Taha");

    string FENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    //string FENString = "2k5/ppp5/8/8/8/8/2P1P1PP/4K3 w KQkq";
    //string FENString = "8/p5k1/8/8/8/8/7P/7K w KQkq";
    // string FENString = "3rr3/3k4/8/8/3K4/8/8/8 w KQkq";
    //string FENString = "4kbnr/8/8/8/8/8/8/5K1R w k";
    //string FENString = "6Q1/8/8/8/8/8/K5R1/7k w KQkq";
    //chessboard.initializeBoard();
    chessboard.initializeBoardFromFEN(FENString);

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
    bool StartGame = false;


    Rectangle InfoBox = { InfoBoxX, InfoBoxY, InfoBoxWidth, InfoBoxHeight };
    Rectangle StartButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 3) - buttonCenterY, 200, 70 };
    Rectangle MultiplayerButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 2) - buttonCenterY, 200, 70 };
    Rectangle FENButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 1.5f) - buttonCenterY, 200, 70 };

    thread aiThread(CalculateAIMove, ref(Gemini), ref(chessboard));
    aiThread.detach(); // Detach the thread to let it run independentl

    // Main menu loop
    while (!WindowShouldClose()) {
        // Draw
        BeginDrawing();
        ClearBackground(Background);
        DrawRectangleRounded(InfoBox, Roundedness, Segments, Translucent);
        // Check if the game has started
        if (gameStarted) {

            if (IsKeyPressed(KEY_SPACE)) {
                PlaySound(KingChecked);
            }

            if (!DoneOnce) {
                PlaySound(GameStarts);
                chessboard.initializeBoardFromFEN(FENString);
                //chessboard.initializeBoard();


                DoneOnce = true;
            }


            if (!GameStats.GameIsEnded(chessboard)) {
                vector<pair<int,int>> PossibleMoves = chessboard.GetAllPossibleMovePairs(chessboard.isCurrentPlayerWhite() ? 'W' : 'B');

                if (chessboard.isCurrentPlayerWhite()) {
                    

                    for (int i = 0; i < TotalTiles; i++) {
                        if (chessboard.board[i].color == White) {
                            chessboard.UpdateChessPiece(chessboard.board[i], i);
                        }
                    }
                }
                else {
                    
                    chessboard.ComputeOpponentMoves();
                    Gemini.StartSearch();
                }
                

                if (IsKeyPressed(KEY_H)) {
                    GameStats.ShowMoveHistory = !GameStats.ShowMoveHistory;
                }
                if (IsKeyPressed(KEY_P)) {
                    Gemini.PauseSearch();
                    PlaySound(KingChecked);

                }
                if (IsKeyPressed(KEY_R)) {
                    Gemini.TerminateSearch();
                    DoneOnce = false;
                    GameStats.Reset();
                    Gemini.Reset();
                    chessboard.DestroyBoard();
                }

                if (GameStats.ShowMoveHistory) {
                    GameStats.DisplayMoveHistory(chessboard);
                }
                else {
                    GameStats.DisplayStats(chessboard, Gemini, Taha);
                }

                chessboard.DrawBoard();
                chessboard.DrawChessPiece();

            }

            else {

                Gemini.TerminateSearch();
                chessboard.DrawBoard();
                chessboard.DrawChessPiece();
                if (GameStats.CalculateELO) {

                    int oldBlackELO = Gemini.engineEloRating;
                    Gemini.engineEloRating = GameStats.updateEloRating(Gemini.engineEloRating, Taha.ELO, (GameStats.winner == Black));
                    Taha.ELO = GameStats.updateEloRating(Taha.ELO, oldBlackELO, (GameStats.winner == White));
                    Taha.saveEloToFile();
                    Gemini.saveEloToFile();
                    chessboard.saveMatchHistoryToFile();
                    GameStats.CalculateELO = false;

                }
                if (IsKeyPressed(KEY_R)) {
                    Gemini.TerminateSearch();
                    DoneOnce = false;
                    GameStats.Reset();
                    Gemini.Reset();
                    chessboard.DestroyBoard();
                }
                if (IsKeyPressed(KEY_M)) {
                    DoneOnce = false;
                    GameStats.Reset();
                    Gemini.Reset();
                    chessboard.DestroyBoard();
                    gameStarted = false;
                }
            }

        }

        else {
            if (!StartGame) {
                gameStarted = true;
            }
            chessboard.DrawBoard();
            chessboard.DrawChessPiece();

            //StartGame = GuiButton(StartButton, "Start");


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



