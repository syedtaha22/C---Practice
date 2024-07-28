#include<iostream>
using namespace std;

class Board{
    int board[8][8];
public:
    Board(){
        for (int row = 0; row < 8; row++) {
            for (int coloumn = 0; coloumn < 8; coloumn++) {
                board[row][coloumn] = coloumn;
            }
            cout << endl;
        }
    }
    void displayBoard() const {
        for (int row = 0; row < 8; row++) {
            cout << 8 - row << " ";
            for (int coloumn = 0; coloumn < 8; coloumn++) {
                cout << "[" << row << board[row][coloumn] << "]";
            }
            cout << endl;
        }
        cout << "    a   b   c   d   e   f   g   h";
}
};




int main(){
    Board b1;
    b1.displayBoard();
    cout << endl << endl;




    return 0;
}