#include<iostream>
using namespace std;


class Board{
public:
    void DisplayBoard(){
        for(int file = 0; file <8;file++){
            for(int rank=0;rank<8;rank++){
                cout << "[ ]";
            }
            cout << endl;
        }
    }

};


int main(){
    Board board;
    board.DisplayBoard();
}