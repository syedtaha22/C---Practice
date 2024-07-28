#include <iostream>
using namespace std;

class Piece
{
public:
    const int EMPTY = 0;
    const int KING = 1;
    const int PAWN = 2;
    const int KNIGHT = 3;
    const int BISHOP = 4;
    const int ROOK = 5;
    const int QUEEN = 6;

    const int White = 8;
    const int Black = 16;
};

void DisplayBoard(const int *board)
{
    cout << "8 ";
    for (int i = 0; i < 64; i++)
    {
        // cout << (i/8) + 1  << " ";
        cout << "[" << board[i] << "]";
        if ((i + 1) % 8 == 0 && (i + 1) < 64)
        {
            cout << endl;
            cout << 8 - (i + 1) / 8 << " ";
        }

        // cout << endl;
    }
    cout << endl;
    cout << "   a  b  c  d  e  f  g  h";
}

int main()
{

    Piece piece;

    int Color[2] = {piece.White, piece.Black};
    int Type[6] = {piece.KING, piece.PAWN, piece.KNIGHT, piece.BISHOP, piece.ROOK, piece.QUEEN};

    for (int colorIndex = 0; colorIndex < 2; colorIndex++)
    {
        for (int typeIndex = 0; typeIndex < 6; typeIndex++)
        {
            int result = Color[colorIndex] | Type[typeIndex];
            cout << result << endl;
        }
    }

    int Squares[64];

    Squares[0] = piece.ROOK | piece.Black;
    Squares[1] = piece.KNIGHT | piece.Black;
    Squares[2] = piece.BISHOP | piece.Black;
    Squares[3] = piece.QUEEN | piece.Black;
    Squares[4] = piece.KING | piece.Black;
    Squares[5] = piece.BISHOP | piece.Black;
    Squares[6] = piece.KNIGHT | piece.Black;
    Squares[7] = piece.ROOK | piece.Black;
    for (int i = 8; i <= 15; i++)
    {
        Squares[i] = piece.PAWN | piece.Black;
    }

    // Empty spaces in the middle
    for (int i = 16; i <= 47; i++)
    {
        Squares[i] = piece.EMPTY | piece.EMPTY;
    }

    // White pieces
    for (int i = 48; i <= 55; i++)
    {
        Squares[i] = piece.PAWN | piece.White;
    }

    Squares[56] = piece.ROOK | piece.White;
    Squares[57] = piece.KNIGHT | piece.White;
    Squares[58] = piece.BISHOP | piece.White;
    Squares[59] = piece.QUEEN | piece.White;
    Squares[60] = piece.KING | piece.White;
    Squares[61] = piece.BISHOP | piece.White;
    Squares[62] = piece.KNIGHT | piece.White;
    Squares[63] = piece.ROOK | piece.White;

    // DisplayBoard(Squares);
}
