#include<iostream>
using namespace std;



const int EMPTY = 0;
const int KING = 1;
const int PAWN = 2;
const int KNIGHT = 3;
const int BISHOP = 4;
const int ROOK = 5;
const int QUEEN = 6;

const int White = 8;
const int Black = 16;

class ChessPiece{
public:
    int type;
    int color;

    int PieceCode;

    //Piece Type Codes:
    //White King   = 9
    //White Pawn   = 10
    //White Knigh  = 11
    //White Bishop = 12
    //White Rook   = 13
    //White Queen  = 14   
     
    //Black King   = 17
    //Black Pawn   = 18
    //Black Knigh  = 19
    //Black Bishop = 20
    //Black Rook   = 21
    //Black Queen  = 22


    ChessPiece(int pieceType = EMPTY, int pieceColor = EMPTY)
     : type(pieceType), color(pieceColor){

        PieceCode = (color << 3) | type;
        if(type != EMPTY){
            cout << color << " " << type << ": " << PieceCode << endl;

        }
        
    }

};

int main(){

    ChessPiece WhiteRook = ChessPiece(ROOK, White); // 8 6: 13

    ChessPiece board[64];

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
}