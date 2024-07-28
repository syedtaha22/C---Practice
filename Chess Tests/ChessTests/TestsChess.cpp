#include<iostream>
#include<string>
#include "raylib.h"
using namespace std;

const char EMPTY = ' ';
const char PAWN = 'P';
const char ROOK = 'R';
const char KNIGHT = 'N';
const char BISHOP = 'B';
const char QUEEN = 'Q';
const char KING = 'K';

enum class PromotionPiece { QUEEN, ROOK, KNIGHT, BISHOP };

class ChessPiece {
public:
    char type;
    char color;
    bool firstMove; // Added flag for the first move
    ChessPiece(char pieceType = EMPTY, char pieceColor = EMPTY, bool isFirstMove = true)
        : type(pieceType), color(pieceColor), firstMove(true) {}
};


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

public:

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
                board[row][col] = ChessPiece((symbol), pieceColor);
                col++;
            }
        }

        // Set the current player based on FEN
        currentPlayerIsWhite = (fen.find("w") != string::npos);

        // ...code for additional FEN features
    }


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

    int calculatePlayerScore(char playerColor) const {
        int totalScore = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j].color == playerColor) {
                    totalScore += getPieceScore(board[i][j]);
                }
            }
        }
        return totalScore;
    }

    void promotePawn(int toRow, int toCol) {
        cout << "Pawn promotion! Choose a piece (Q, R, N, B): ";
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
        board[toRow][toCol] = promotedPiece;
    }

    bool isCurrentPlayerWhite() {
        return currentPlayerIsWhite;
    }

    void initializeBoard() {
        // Set up the board with pieces in their initial positions

        // Black pieces
        board[0][0] = ChessPiece(tolower(ROOK), 'B');
        board[0][1] = ChessPiece(tolower(KNIGHT), 'B');
        board[0][2] = ChessPiece(tolower(BISHOP), 'B');
        board[0][3] = ChessPiece(tolower(QUEEN), 'B');
        board[0][4] = ChessPiece(tolower(KING), 'B');
        board[0][5] = ChessPiece(tolower(BISHOP), 'B');
        board[0][6] = ChessPiece(tolower(KNIGHT), 'B');
        board[0][7] = ChessPiece(tolower(ROOK), 'B');
        for (int i = 0; i < 8; i++) {
            board[1][i] = ChessPiece(tolower(PAWN), 'B');
        }

        // Empty spaces in the middle
        for (int i = 2; i < 6; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = ChessPiece(EMPTY, EMPTY);
            }
        }

        // White pieces
        for (int i = 0; i < 8; i++) {
            board[6][i] = ChessPiece((PAWN), 'W');
        }
        board[7][0] = ChessPiece((ROOK), 'W');
        board[7][1] = ChessPiece((KNIGHT), 'W');
        board[7][2] = ChessPiece((BISHOP), 'W');
        board[7][3] = ChessPiece((QUEEN), 'W');
        board[7][4] = ChessPiece((KING), 'W');
        board[7][5] = ChessPiece((BISHOP), 'W');
        board[7][6] = ChessPiece((KNIGHT), 'W');
        board[7][7] = ChessPiece((ROOK), 'W');
    }

    void displayBoard() const {
        for (int i = 0; i < 8; i++) {
            cout << 8 - i << " ";
            for (int j = 0; j < 8; j++) {
                cout << "[" << board[i][j].type << "]";
            }
            cout << endl;
        }
        cout << "   a  b  c  d  e  f  g  h";
    }

    void makeMove(int fromRow, int fromCol, int toRow, int toCol) {
        board[fromRow][fromCol].firstMove = false;
        board[toRow][toCol] = board[fromRow][fromCol];
        board[fromRow][fromCol] = ChessPiece(); // Empty the source square

        // Check for pawn promotion
        if (toupper(board[toRow][toCol].type) == PAWN) {
            // White pawn promotion
            if (toRow == 0) {
                promotePawn(toRow, toCol);
            }
            // Black pawn promotion
            else if (toRow == 7) {
                promotePawn(toRow, toCol);
            }

        }

        currentPlayerIsWhite = !currentPlayerIsWhite;

        int whiteScore = calculatePlayerScore('W');
        int blackScore = calculatePlayerScore('B');
        cout << "White score: " << whiteScore << " | Black score: " << blackScore << endl;

    }

    bool isValidMoveFormat(const string& move) const {
        return (move.length() == 4 &&
            move[0] >= 'a' && move[0] <= 'h' &&
            move[1] >= '1' && move[1] <= '8' &&
            move[2] >= 'a' && move[2] <= 'h' &&
            move[3] >= '1' && move[3] <= '8');
    }

    bool isValidPawnMove(const ChessPiece& piece, int fromRow, int fromCol, int toRow, int toCol, const ChessPiece& targetPiece) const {
        int direction = (piece.color == 'W') ? 1 : -1; // Direction depends on pawn color

        if (piece.firstMove) {
            if ((fromRow - toRow) == 2 * direction && toCol == fromCol && targetPiece.type == EMPTY) {
                return true;
            }
        }
        if ((fromRow - toRow) == direction && toCol == fromCol && targetPiece.type == EMPTY) {
            return true;
        }
        // Pawn capture
        if ((fromRow - toRow) == direction && abs(toCol - fromCol) == 1 && targetPiece.type != EMPTY && targetPiece.color != piece.color) {
            return true;
        }
        return false;
    }

    bool isValidBishopMove(int fromRow, int fromCol, int toRow, int toCol) const {
        // Check if the move is along a diagonal
        if (abs(fromRow - toRow) != abs(fromCol - toCol)) {
            cout << "Invalid Bishop Move" << endl;
            return false;
        }

        // Check if there are no pieces blocking the path
        int rowStep = (toRow > fromRow) ? 1 : -1;
        int colStep = (toCol > fromCol) ? 1 : -1;

        int currentRow = fromRow + rowStep;
        int currentCol = fromCol + colStep;

        while (currentRow != toRow && currentCol != toCol) {
            if (board[currentRow][currentCol].type != EMPTY) {
                cout << "There is a piece in the path" << endl;
                return false;
            }
            currentRow += rowStep;
            currentCol += colStep;
        }

        // Check if the destination square is either empty or has an opponent's piece
        if (board[toRow][toCol].type == EMPTY || board[toRow][toCol].color != board[fromRow][fromCol].color) {
            return true;
        }
        cout << "Invalid Bishop Move" << endl;
        return false;
    }

    bool isValidRookMove(int fromRow, int fromCol, int toRow, int toCol) const {
        // Check if the move is along a row or column
        if (fromRow != toRow && fromCol != toCol) {
            cout << "Invalid Rook Move" << endl;
            return false;
        }

        // Check if there are no pieces blocking the path
        if (fromRow == toRow) {
            int colStep = (toCol > fromCol) ? 1 : -1;
            int currentCol = fromCol + colStep;

            while (currentCol != toCol) {
                if (board[fromRow][currentCol].type != EMPTY) {
                    cout << "There is a piece in the path" << endl;
                    return false;
                }
                currentCol += colStep;
            }
        }
        else {
            int rowStep = (toRow > fromRow) ? 1 : -1;
            int currentRow = fromRow + rowStep;

            while (currentRow != toRow) {
                if (board[currentRow][fromCol].type != EMPTY) {
                    cout << "There is a piece in the path" << endl;
                    return false;
                }
                currentRow += rowStep;
            }
        }

        // Check if the destination square is either empty or has an opponent's piece
        if (board[toRow][toCol].type == EMPTY || board[toRow][toCol].color != board[fromRow][fromCol].color) {
            return true;
        }
        cout << "Invalid Rook Move" << endl;
        return false;
    }

    bool isValidKnightMove(int fromRow, int fromCol, int toRow, int toCol) const {
        int rowDiff = abs(toRow - fromRow);
        int colDiff = abs(toCol - fromCol);

        // Check if the move is an "L" shape (2 squares in one direction, 1 square in the other)
        if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
            // Knights can jump over other pieces, so no need to check for pieces in the path
            return true;
        }

        cout << "Invalid Knight Move" << endl;
        return false;
    }

    bool isValidQueenMove(int fromRow, int fromCol, int toRow, int toCol) const {
        // Check if the move is along a row, column, or diagonal
        if (fromRow != toRow && fromCol != toCol && abs(fromRow - toRow) != abs(fromCol - toCol)) {
            cout << "Invalid Queen Move" << endl;
            return false;
        }

        // Check if there are no pieces blocking the path
        if (fromRow == toRow) {
            int colStep = (toCol > fromCol) ? 1 : -1;
            int currentCol = fromCol + colStep;

            while (currentCol != toCol) {
                if (board[fromRow][currentCol].type != EMPTY) {
                    cout << "There is a piece in the path" << endl;
                    return false;
                }
                currentCol += colStep;
            }
        }
        else if (fromCol == toCol) {
            int rowStep = (toRow > fromRow) ? 1 : -1;
            int currentRow = fromRow + rowStep;

            while (currentRow != toRow) {
                if (board[currentRow][fromCol].type != EMPTY) {
                    cout << "There is a piece in the path" << endl;
                    return false;
                }
                currentRow += rowStep;
            }
        }
        else {
            int rowStep = (toRow > fromRow) ? 1 : -1;
            int colStep = (toCol > fromCol) ? 1 : -1;

            int currentRow = fromRow + rowStep;
            int currentCol = fromCol + colStep;

            while (currentRow != toRow && currentCol != toCol) {
                if (board[currentRow][currentCol].type != EMPTY) {
                    cout << "There is a piece in the path" << endl;
                    return false;
                }
                currentRow += rowStep;
                currentCol += colStep;
            }
        }

        // Check if the destination square is either empty or has an opponent's piece
        if (board[toRow][toCol].type == EMPTY || board[toRow][toCol].color != board[fromRow][fromCol].color) {
            return true;
        }

        cout << "Invalid Queen Move" << endl;
        return false;
    }

    bool isValidKingMove(int fromRow, int fromCol, int toRow, int toCol) const {
        // Check if the move is within one square in any direction
        if (abs(fromRow - toRow) <= 1 && abs(fromCol - toCol) <= 1) {
            // Check if the destination square is either empty or has an opponent's piece
            if (board[toRow][toCol].type == EMPTY || board[toRow][toCol].color != board[fromRow][fromCol].color) {
                return true;
            }
        }

        cout << "Invalid King Move" << endl;
        return false;
    }

    bool isValidMove(const string& move, int& fromRow, int& fromCol, int& toRow, int& toCol) const {
        if (!isValidMoveFormat(move)) {
            cout << "Invalid move format. Please use the format 'e2e4'." << endl;
            return false;
        }
        // Extracting coordinates
        fromCol = move[0] - 'a';
        fromRow = 7 - (move[1] - '1');
        toCol = move[2] - 'a';
        toRow = 7 - (move[3] - '1');


        // Validate the move coordinates
        if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 || toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
            cout << "Invalid move. Coordinates are out of bounds." << endl;
            return false;
        }

        // Check if the source square is empty
        if (board[fromRow][fromCol].type == EMPTY) {
            cout << "Invalid move. Source square is empty." << endl;
            return false;
        }

        // Check if the piece being moved belongs to the current player
        char currentPlayerColor = (currentPlayerIsWhite ? 'W' : 'B');
        if (board[fromRow][fromCol].color != currentPlayerColor) {
            cout << "Invalid move. You can only move your own pieces." << endl;
            return false;
        }

        //Check for Pawns
        if (toupper(board[fromRow][fromCol].type) == PAWN) {
            if (isValidPawnMove(board[fromRow][fromCol], fromRow, fromCol, toRow, toCol, board[toRow][toCol])) {
                return true;
            }
            else {
                cout << "Invalid Pawn Move" << endl;
                return false;
            }
        }
        //Check for Bishops
        if (toupper(board[fromRow][fromCol].type) == BISHOP) {
            if (isValidBishopMove(fromRow, fromCol, toRow, toCol)) {
                return true;
            }
            else {
                return false;
            }
        }
        //Check for Rooks
        if (toupper(board[fromRow][fromCol].type) == ROOK) {
            if (isValidRookMove(fromRow, fromCol, toRow, toCol)) {
                return true;
            }
            else {
                return false;
            }
        }

        // Check for Knights
        if (toupper(board[fromRow][fromCol].type) == KNIGHT) {
            if (isValidKnightMove(fromRow, fromCol, toRow, toCol)) {
                return true;
            }
            else {
                return false;
            }
        }
        //Check for Queens
        if (toupper(board[fromRow][fromCol].type) == QUEEN) {
            if (isValidQueenMove(fromRow, fromCol, toRow, toCol)) {
                return true;
            }
            else {
                return false;
            }
        }
        // Check for Kings
        if (toupper(board[fromRow][fromCol].type) == KING) {
            if (isValidKingMove(fromRow, fromCol, toRow, toCol)) {
                return true;
            }
            else {
                return false;
            }
        }




        // Add more custom validation logic based on the rules of your chess game

        return true;
    }
};



//void GameOptions(ChessBoard& board) {
//
//    int option;
//    cout << "---------------------Game Options---------------------" << endl << endl;
//    cout << "Select An Option: " << endl;
//    cout << "1) Load Default Game" << endl;
//    cout << "1) Load Game from FEN string" << endl;
//    cout << "3) Help" << endl;
//    cout << "Enter your Option: ";
//    cin >> option;
//    if (option == 1) {
//        string DefaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
//        board.initializeBoardFromFEN(DefaultFEN);
//    }
//    else if (option == 2) {
//        string FenString;
//        cout << "Enter FEN string: ";
//        cin >> FenString;
//        board.initializeBoardFromFEN(FenString);
//    }
//    else if (option == 3) {
//        Help();
//    }
//    else {
//        //By defualt
//        string DefaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
//        board.initializeBoardFromFEN(DefaultFEN);
//    }
//}

int main() {
    ChessBoard chessboard;
    //GameOptions(chessboard);
     string initialFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


     //string initialFEN = "8/8/8/4p1Kq/2k1P3/8/8/8 b - - 0 1";

     // Initialize the chessboard using FEN
     chessboard.initializeBoardFromFEN(initialFEN);

    // Display the board
    chessboard.displayBoard();


    int fromRow, fromCol, toRow, toCol;

    while (true) {
        cout << "\n\n" << (chessboard.isCurrentPlayerWhite() ? "White" : "Black") << "'s turn." << endl;
        // Get move from the user
        string move;
        do {
            cout << "Enter your move (e.g., e2e4): ";
            cin >> move;
        } while (!chessboard.isValidMove(move, fromRow, fromCol, toRow, toCol));

        // Make the move
        chessboard.makeMove(fromRow, fromCol, toRow, toCol);

        cout << endl;
        // Display the updated board
        chessboard.displayBoard();
    }

    return 0;
}