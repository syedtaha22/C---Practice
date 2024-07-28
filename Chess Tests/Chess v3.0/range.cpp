#include<iostream>
using namespace std;
class Displaychess{
	char board[63];
public:
	Displaychess() {
		for (int i = 0; i < (63); i++) {
			switch (i) {
			case 0:
				board[i] = 'R';  // black rook
				break;
			case 1:
				board[i] = 'B'; // black bishop
				break;
			case 2:
				board[i] = 'N'; //black knight
				break;
			case 3:
				board[i] = 'Q'; //black queen
				break;
			case 4:
				board[i] = 'K'; //black king
				break;
			case 5:
				board[i] = 'N'; //  black knight
				break;
			case 6:
				board[i] = 'B'; // black bishop
				break;
			case 7:
				board[i] = 'R'; // black rook 
				break;
            case 8 ... 15:
                board[i] = 'P'; //black pawn 
                break;
            case 48 ... 55:
                board[i] = 'p'; // white pawn5
                break;
            case 56:
                board [i] = 'r';
                break;
            case 57:
                board [i] = 'b';
                break;            
            case 58:
                board [i] = 'n';
                break;
            case 59:
                board [i] = 'k';
                break; 
            case 60:
                board [i] = 'q';
                break;
            case 61:
                board [i] = 'n';
                break;
            case 62:
                board[i] = 'b';
                break;
            case 63:
                board[i] = 'r';
                break;
            default:
                board[i] = ' ';     
                break;                     
            }
		}
    }
	void displayboard() {
		for (int i = 1; i < 65; i++) {
			cout << "[" << board[i - 1] << "]";
			if (i % 8 == 0 && i > 0) {
				cout << endl;
			}
		}
	}
};
int main() {
	Displaychess chessboard;
	chessboard.displayboard();
	return 0;
}